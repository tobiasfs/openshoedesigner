///////////////////////////////////////////////////////////////////////////////
// Name               : OpenGLCanvas.cpp
// Purpose            : Class providing wxWidgets with an OpenGL canvas with extra functions
// Thread Safe        : Yes
// Platform dependent : Yes
// Compiler Options   : -lopengl32 -lglu
// Author             : Tobias Schaefer
// Created            : 13.09.2009
// Copyright          : (C) 2009 Tobias Schaefer <tobiassch@users.sourceforge.net>
// Licence            : GNU General Public License version 3.0 (GPLv3)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
///////////////////////////////////////////////////////////////////////////////

#include "OpenGLCanvas.h"

#include "../StdInclude.h"

#include <cfloat>
#ifdef USE_6DOFCONTROLLER
#include "../system/controller/Control3D.h"
#endif

#include <wx/dcclient.h>

#include <iostream>
#include <sstream>
#include <stdexcept>

#include "OpenGL.h"

#include "GL/glu.h"

#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif
#include <cmath>

static int wx_gl_attribs[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE,
		24, 0 };

OpenGLCanvas::OpenGLCanvas(wxWindow *parent, wxWindowID id, const wxPoint &pos,
		const wxSize &size, long style, const wxString &name) :
		wxGLCanvas(parent, id, wx_gl_attribs, pos, size,
				style | wxFULL_REPAINT_ON_RESIZE, name), Light0(GL_LIGHT0) {

	m_gllist = 0;
	w = h = 500;
	x = y = 250;
	unitAtOrigin = 1000;

	turntableX = 0;
	turntableY = M_PI / 2;
	scale = 5.0;
	stereoMode = Stereo3D::Off;
	eyeDistance = 0.1;
	focalDistance = 1.0;
	backgroundGrayLevel = 102;
	rightEyeR = 0;
	rightEyeG = 77;
	rightEyeB = 102;
	leftEyeR = 179;
	leftEyeG = 0;
	leftEyeB = 0;

	rotationMode = Rotation::Interwoven;

	this->Connect(wxEVT_PAINT, wxPaintEventHandler(OpenGLCanvas::OnPaint),
			nullptr, this);
	this->Connect(wxEVT_ENTER_WINDOW,
			wxMouseEventHandler(OpenGLCanvas::OnEnterWindow), nullptr, this);
	this->Connect(wxEVT_MOTION, wxMouseEventHandler(OpenGLCanvas::OnMouseEvent),
			nullptr, this);
	this->Connect(wxEVT_MOUSEWHEEL,
			wxMouseEventHandler(OpenGLCanvas::OnMouseEvent), nullptr, this);
	this->Connect(wxEVT_RIGHT_DOWN,
			wxMouseEventHandler(OpenGLCanvas::OnMouseEvent), nullptr, this);
	this->Connect(wxEVT_MIDDLE_DOWN,
			wxMouseEventHandler(OpenGLCanvas::OnMouseEvent), nullptr, this);
	this->Connect(wxEVT_RIGHT_DCLICK,
			wxMouseEventHandler(OpenGLCanvas::OnMouseEvent), nullptr, this);

#ifdef USE_6DOFCONTROLLER
	timer.SetOwner(this);
	this->Connect(wxEVT_TIMER, wxTimerEventHandler(OpenGLCanvas::OnTimer),
			nullptr, this);
	timer.Start(100);
#endif
}

OpenGLCanvas::~OpenGLCanvas() {
#ifdef USE_6DOFCONTROLLER
	this->Disconnect(wxEVT_TIMER, wxTimerEventHandler(OpenGLCanvas::OnTimer),
			nullptr, this);
#endif
	this->Disconnect(wxEVT_RIGHT_DCLICK,
			wxMouseEventHandler(OpenGLCanvas::OnMouseEvent), nullptr, this);
	this->Disconnect(wxEVT_MIDDLE_DOWN,
			wxMouseEventHandler(OpenGLCanvas::OnMouseEvent), nullptr, this);
	this->Disconnect(wxEVT_RIGHT_DOWN,
			wxMouseEventHandler(OpenGLCanvas::OnMouseEvent), nullptr, this);
	this->Disconnect(wxEVT_MOUSEWHEEL,
			wxMouseEventHandler(OpenGLCanvas::OnMouseEvent), nullptr, this);
	this->Disconnect(wxEVT_MOTION,
			wxMouseEventHandler(OpenGLCanvas::OnMouseEvent), nullptr, this);
	this->Disconnect(wxEVT_ENTER_WINDOW,
			wxMouseEventHandler(OpenGLCanvas::OnEnterWindow), nullptr, this);
	this->Disconnect(wxEVT_PAINT, wxPaintEventHandler(OpenGLCanvas::OnPaint),
			nullptr, this);

	if (context != nullptr)
		delete context;
}

#ifdef USE_6DOFCONTROLLER
void OpenGLCanvas::SetController(Control3D &control) {
	this->control = &control;
}
#endif

OpenGLCanvas::Context::Context(wxGLCanvas *canvas) :
		wxGLContext(canvas) {
	SetCurrent(*canvas);

#ifdef USE_GLAD
#ifndef __APPLE__

#if defined(_WIN32) || defined(__WIN32__)
#ifndef WIN32_LEAN_AND_MEAN
	// Reduce a bit header VC++ compilation time
#define WIN32_LEAN_AND_MEAN 1
#define LE_ME_ISDEF
#endif
	/*
	 APIENTRY is defined in oglpfuncs.h as well as by windows.h. Undefine
	 it to prevent a macro redefinition warning.
	 */
#undef APIENTRY
#include <windows.h> //For wglGetProcAddress
#ifdef LE_ME_ISDEF
#undef WIN32_LEAN_AND_MEAN
#undef LE_ME_ISDEF
#endif

	auto success = gladLoadGLLoader((GLADloadproc) wglGetProcAddress);

#else // Linux
	auto success = gladLoaderLoadGL();
#endif
	if (!success) {
		std::ostringstream out;
		out << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << " - ";
		out << "Failed to initialize GLAD.";
		throw std::runtime_error(out.str());
	}
#endif
#endif

	DEBUGOUT << "GL_VERSION: ";
	// If this line fails, glad was probably not initialized correctly.
	DEBUGOUT << glGetString(GL_VERSION);
	DEBUGOUT << '\n';
}

void OpenGLCanvas::OnEnterWindow(wxMouseEvent&WXUNUSED(event)) {
	SetFocus();
}

#ifdef USE_6DOFCONTROLLER
void OpenGLCanvas::OnTimer(wxTimerEvent &event) {
	if (control == nullptr)
		return;

	control->Pump();
	if (control->IsIdle())
		return;

	const float resRot = 2000;
	const float resMov = 5 * unitAtOrigin;

	rotmat = AffineTransformMatrix::RotationInterwoven(
			(float) control->GetAxis(3) / resRot,
			(float) control->GetAxis(4) / resRot,
			(float) control->GetAxis(5) / resRot) * rotmat;
	rotmat.TranslateGlobal((float) control->GetAxis(0) / resMov,
			(float) control->GetAxis(1) / resMov,
			(float) control->GetAxis(2) / resMov);

	//rotmat.RotateXY(1,0,1);
	if (control->GetButton(0)) {
		rotmat.SetIdentity();
	}
	this->Refresh();
}
#endif

void OpenGLCanvas::OnMouseEvent(wxMouseEvent &event) {
	if (event.ButtonDown(wxMOUSE_BTN_RIGHT)
			|| event.ButtonDown(wxMOUSE_BTN_MIDDLE)) {
		x = event.m_x;
		y = event.m_y;
	}
	if (event.ButtonDClick(wxMOUSE_BTN_RIGHT)) {
		rotmat = AffineTransformMatrix::Identity();
		turntableX = 0;
		turntableY = M_PI / 2;
		x = event.m_x;
		y = event.m_y;
		this->Refresh();
	}

	if (event.Dragging() && event.RightIsDown()) {
		switch (rotationMode) {
		case Rotation::Trackball: {
			const double r = (double) ((w < h) ? w : h) / 2.2;
			rotmat = AffineTransformMatrix::RotationTrackball(
					(double) (x - w / 2), (double) (h / 2 - y),
					(double) (event.m_x - w / 2), (double) (h / 2 - event.m_y),
					r) * rotmat;
			break;
		}
		case Rotation::Interwoven: {
			rotmat = AffineTransformMatrix::RotationXY(event.m_x - x,
					event.m_y - y, 2.0) * rotmat;
			break;
		}
		case Rotation::Turntable: {
			Vector3 temp = rotmat.GetOrigin();
			rotmat = AffineTransformMatrix::RotationAroundVector(
					Vector3(1, 0, 0), -M_PI / 2);
			turntableX += (double) (event.m_x - x) / 100;
			turntableY += (double) (event.m_y - y) / 100;

			rotmat = rotmat
					* AffineTransformMatrix::RotationAroundVector(
							Vector3(0, 1, 0), turntableX);

			rotmat = AffineTransformMatrix::RotationAroundVector(
					Vector3(1, 0, 0), turntableY) * rotmat;
			rotmat.SetOrigin(temp);

			break;
		}
		}
		x = event.m_x;
		y = event.m_y;

		this->Refresh();
	}

	if (event.Dragging() && event.MiddleIsDown()) {
		float movement = 1.0;
		if (unitAtOrigin > 1.0)
			movement = unitAtOrigin;
		const float dx = (float) (event.m_x - x) / movement;
		const float dy = (float) (event.m_y - y) / movement;
		if (rotationMode == Rotation::Turntable)
			rotmat.TranslateGlobal(dx, -dy, 0);
		else
			rotmat.TranslateGlobal(dx, -dy, 0);
		x = event.m_x;
		y = event.m_y;

		this->Refresh();
	} else {
		const int x = event.GetWheelRotation();
		if (x != 0) {
			scale *= exp(-((float) x) / 1000.0);
			this->Refresh();
		}
	}

	if (event.Moving() || event.Dragging())
		event.Skip();
}

void OpenGLCanvas::OnPaint(wxPaintEvent&WXUNUSED(event)) {

	wxPaintDC(this); // Set the clipping for this area

	if (!IsShown())
		return;

	//#ifndef __WXMOTIF__
	//	if(!GetContext()) return;
	//#endif

	if (context == nullptr)
		context = new Context(this);
	context->SetCurrent(*this); // Link OpenGL to this area

	// Set GL viewport (not called by wxGLCanvas::OnSize on all platforms...)
	GetClientSize(&w, &h);
	glViewport(0, 0, (GLint) w, (GLint) h);

	// Setup OpenGL state machine

	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Light0.SetAmbient(0.2, 0.2, 0.2);
	Light0.SetDiffuse(0.6, 0.6, 0.6);
	Light0.SetSpecular(0.95, 0.95, 0.95);
	Light0.SetPosition(1, 0.4, 1);
	Light0.moveWithCamera = true;

#ifdef GL_VERSION_1_2
	// Use RESCALE_NORMAL in OpenGL 1.2 or higher
	glEnable(GL_RESCALE_NORMAL);
#else
	// Fallback for OpenGL 1.1
	glEnable(GL_NORMALIZE);
#endif
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glEnable(GL_COLOR_MATERIAL);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDrawBuffer(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_ONE, GL_ZERO); // disable alpha blending
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);

	{ // Render background
		if (stereoMode == Stereo3D::Anaglyph) {
			glColor3ub(backgroundGrayLevel, backgroundGrayLevel,
					backgroundGrayLevel);
			glDisable(GL_COLOR_MATERIAL);
		}

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		AffineTransformMatrix::Orthogonal(0, 1, 0, 1, -1, 1).GLMultMatrix();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		// Reset materials
		GLfloat params[4] = { 0, 0, 0, 1 };
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, params);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, params);
		glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 0);

		if (stereoMode == Stereo3D::Anaglyph) {
			glColor3ub(backgroundGrayLevel, backgroundGrayLevel,
					backgroundGrayLevel);
		}

		glNormal3b(0, 0, 1);
		glBegin(GL_QUADS);
		if (stereoMode != Stereo3D::Anaglyph)
			glColor3ub(50, 50, 50);
		glVertex2i(1, 1);
		glVertex2i(0, 1);
		if (stereoMode != Stereo3D::Anaglyph)
			glColor3ub(100, 100, 255);
		glVertex2i(0, 0);
		glVertex2i(1, 0);
		glEnd();

		// Clear the depth buffer bit again. Now the rest of the
		// image is always painted in front of the background.
		glClear( GL_DEPTH_BUFFER_BIT);
	}

	const GLdouble aspect = (GLdouble) w / (GLdouble) h; // Calculate perspective

	projection = AffineTransformMatrix::Perspective(M_PI_4, aspect, 0.01, 10.0);
//	projection = AffineTransformMatrix::Orthogonal(-aspect * 0.4142,
//			aspect * 0.4142, -0.4142, 0.4142, 0.4142, -0.4142);

	camera = AffineTransformMatrix::Translation(0, 0, -focalDistance)
			* AffineTransformMatrix::Scaling(scale) * rotmat;
	camera_position = camera.Inverse();
	model.SetIdentity();

	glEnable(GL_LIGHTING);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	projection.GLMultMatrix();
	glMatrixMode(GL_MODELVIEW);

	glCullFace(GL_BACK);
	glLoadIdentity();

	Light0.Update(false);

	if (stereoMode == Stereo3D::Anaglyph) {
		glColorMask((leftEyeR == 0) ? GL_FALSE : GL_TRUE,
				(leftEyeG == 0) ? GL_FALSE : GL_TRUE,
				(leftEyeB == 0) ? GL_FALSE : GL_TRUE, GL_TRUE);
		glEnable(GL_COLOR_MATERIAL);
		glColor3ub(leftEyeR, leftEyeG, leftEyeB);
		glDisable(GL_COLOR_MATERIAL);
	}
	if (stereoMode == Stereo3D::Shutter) {
		glDrawBuffer(GL_BACK_LEFT);
	}
	if (stereoMode != Stereo3D::Off) {
		glRotatef(atan(eyeDistance / 2 / focalDistance) * 180.0 / M_PI, 0, 1,
				0);
		glTranslatef(eyeDistance / 2, 0, 0);
	}

	camera.GLMultMatrix();

	{
		// Determine unit length at origin
		// Note that the reference implementation makes some assumptions on
		// the form of the projection matrix, that do not work with all
		// projection matrices.
		// https://www.khronos.org/opengl/wiki/GluProject_and_gluUnProject_code
		//
		// The code below works with perspective and orthogonal matrices.

		GLint vp[4];
		GLdouble mv[16];
		GLdouble pr[16];
		glGetDoublev( GL_MODELVIEW_MATRIX, mv);
		glGetDoublev( GL_PROJECTION_MATRIX, pr);
		glGetIntegerv( GL_VIEWPORT, vp);

		double sc = std::sqrt(mv[0] * mv[0] + mv[1] * mv[1] + mv[2] * mv[2]);
		double T1 = 2.0 * (pr[11] * mv[14] + mv[15] * pr[15]);
		double T2 = mv[3] * pr[15];
		double T3 = pr[8] * mv[14] + pr[4] * mv[13] - pr[0] * mv[12];
		double T4 = (pr[0] * mv[15] - mv[3] * T3) * pr[15];
		T4 += pr[11] * (mv[3] * pr[12] + pr[0]) * mv[14];
		unitAtOrigin = sc * 2.0 * vp[2] * T4 / ((T1 - T2) * (T1 + T2));
	}

	Light0.Enable();
	Light0.Update(true);

	if (m_gllist == 0)
		m_gllist = glGenLists(1); // Make one (1) empty display list.
	glNewList(m_gllist, GL_COMPILE_AND_EXECUTE);

	Render();

	glEndList();

	if (stereoMode != Stereo3D::Off) {
		glCullFace(GL_BACK);
		glLoadIdentity();
	}

	if (stereoMode == Stereo3D::Anaglyph) {
		glColorMask((rightEyeR == 0) ? GL_FALSE : GL_TRUE,
				(rightEyeG == 0) ? GL_FALSE : GL_TRUE,
				(rightEyeB == 0) ? GL_FALSE : GL_TRUE, GL_TRUE);
		glEnable(GL_COLOR_MATERIAL);
		glColor3ub(rightEyeR, rightEyeG, rightEyeB);
		glDisable(GL_COLOR_MATERIAL);
	}
	if (stereoMode == Stereo3D::Shutter) {
		glDrawBuffer(GL_BACK_RIGHT);
	}

	if (stereoMode != Stereo3D::Off) {
		glClear(GL_DEPTH_BUFFER_BIT);
		glRotatef(-atan(eyeDistance / 2 / focalDistance) * 180.0 / M_PI, 0, 1,
				0);
		glTranslatef(-eyeDistance / 2, 0, 0);

		camera.GLMultMatrix();

		Light0.Update(true);

		// Render();
		glCallList(m_gllist);

		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	}

	glFlush();
	SwapBuffers();

}

OpenGLCanvas::Arrow OpenGLCanvas::GetPosition(int x, int y) const {
	GLint vp[4];
	glGetIntegerv( GL_VIEWPORT, vp);

	// Note that gy is inverted.
	// Screen coordinates are X: left -> right and Y: top bottom
	// gx and gy are normalized coordinates:
	// gx left -> right, -1 .. 1, gy bottom -> top, -1 .. 1

	double gx = (2.0 * (double) (x - vp[0]) - (double) vp[2]) / (double) vp[2];
	double gy = 1.0 - 2.0 * ((double) vp[1] + y) / (double) vp[3];

	// Fricas code:
	// )set fortran optlevel 2
	// )set output fortran on
	// mpr:=matrix([[pr[0],pr[4],pr[8],pr[12]],[pr[1],pr[5],pr[9],pr[13]],[pr[2],pr[6],pr[10],pr[14]],[0,0,pr[11],pr[15]]]);
	// mmv:=matrix([[mv[0],mv[4],mv[8],mv[12]],[mv[1],mv[5],mv[9],mv[13]],[mv[2],mv[6],mv[10],mv[14]],[0,0,0,1]]);
	// P := mpr*mmv*matrix([[x],[z],[-y],[1]]) - matrix([[gx*h],[gy*h],[gz*h],[h]]);
	// solve([P(1,1),P(2,1),P(4,1)], [x,y,z])(1)

	// Note that y and z are exchanged in the code above to make everything
	// fit together from screen coordinates to OpenGL coordinates.

	// This describes a solution for x,y,z given gx, gy. The free variable h
	// represents the line into the view plane. The solution can be rewritten
	// as A*h + B

	// The code works with orthogonal and perspective projection matrices.

	const double T2 = camera[10];
	const double T3 = camera[6];
	const double T4 = camera[5] * T2;
	const double T5 = T3 * camera[9];
	const double T6 = camera[4] * T2;
	const double T7 = T3 * camera[8];
	const double T8 = projection[11];
	const double T9 = T4 - T5;
	const double T10 = projection[4];
	const double T11 = T6 - T7;
	const double T12 = projection[0];
	const double T13 = -T4 + T5;
	const double T14 = projection[5];
	const double T15 = -T6 + T7;
	const double T16 = projection[1];
	const double T17 = camera[4];
	const double T18 = camera[9];
	const double T19 = camera[5];
	const double T20 = camera[8];
	const double T21 = T17 * T18;
	const double T22 = T19 * T20;
	const double T23 = T13 * T10 + T15 * T12;
	const double T24 = projection[9];
	const double T25 = T9 * T14 + T11 * T16;
	const double T26 = projection[8];
	const double T27 = -T21 + T22;
	const double T28 = T21 - T22;
	const double T29 = T9 * T10 + T11 * T12;
	const double T30 = T13 * T14 + T15 * T16;
	const double T31 = camera[14];
	const double T32 = camera[13];
	const double T33 = camera[12];
	const double T34 = camera[0];
	const double T35 = camera[1];
	const double T36 = camera[2];
	const double T37 = T34 * T19;
	const double T38 = T35 * T17;
	const double T39 = T34 * T3;
	const double T40 = T36 * T17;
	const double T41 = T35 * T3;
	const double T42 = T36 * T19;
	const double T43 = T41 - T42;
	const double T44 = T39 - T40;
	const double T45 = -T41 + T42;
	const double T46 = -T39 + T40;
	const double T47 = -T37 + T38;
	const double T48 = T37 - T38;
	const double T49 = T43 * T10 + T44 * T12;
	const double T50 = T45 * T14 + T46 * T16;
	const double T51 = projection[15];
	const double T52 = T45 * T10 + T46 * T12;
	const double T53 = projection[13];
	const double T54 = T43 * T14 + T44 * T16;
	const double T55 = projection[12];
	const double T56 = ((T48 * T2 + T46 * T18 + T43 * T20) * T12 * T14
			+ (T47 * T2 + T44 * T18 + T45 * T20) * T16 * T10) * T8;
	if (fabs(T56) < FLT_EPSILON) {
		std::ostringstream err;
		err << __FILE__ << ":" << __LINE__ << "::" << __FUNCTION__ << " - ";
		err << " The projection matrix or the object matrix are defect.";
		err << " An inverse projection cannot be calculated.";
		throw std::logic_error(err.str());
	}
	const double T57 = T35 * T2;
	const double T58 = T36 * T18;
	const double T59 = T34 * T2;
	const double T60 = T36 * T20;
	const double T61 = -T57 + T58;
	const double T62 = -T59 + T60;
	const double T63 = T57 - T58;
	const double T64 = T59 - T60;
	const double T65 = T34 * T18;
	const double T66 = T35 * T20;
	const double T67 = T63 * T10 + T64 * T12;
	const double T68 = T61 * T14 + T62 * T16;
	const double T69 = T65 - T66;
	const double T70 = -T65 + T66;
	const double T71 = T61 * T10 + T62 * T12;
	const double T72 = T63 * T14 + T64 * T16;

	Arrow ret;
	ret.normal.x = (T23 * T8 * gy + T25 * T8 * gx + T29 * T24 + T30 * T26
			+ T28 * T12 * T14 + T27 * T16 * T10)

	/ T56;
	ret.normal.y = (T49 * T8 * gy + T50 * T8 * gx + T52 * T24 + T54 * T26
			+ T47 * T12 * T14 + T48 * T16 * T10) / T56;
	ret.normal.z = (T67 * T8 * gy + T68 * T8 * gx + T71 * T24 + T72 * T26
			+ T70 * T12 * T14 + T69 * T16 * T10) / T56;
	ret.origin.x = ((T23 * T24 + T25 * T26 + T27 * T12 * T14 + T28 * T16 * T10)
			* T51 + T29 * T8 * T53 + T30 * T8 * T55
			+ ((T27 * T31 + T11 * T32 + T13 * T33) * T12 * T14
					+ (T28 * T31 + T15 * T32 + T9 * T33) * T16 * T10) * T8)
			/ T56;
	ret.origin.y = ((T49 * T24 + T50 * T26 + T48 * T12 * T14 + T47 * T16 * T10)
			* T51 + T52 * T8 * T53 + T54 * T8 * T55
			+ ((T48 * T31 + T46 * T32 + T43 * T33) * T12 * T14
					+ (T47 * T31 + T44 * T32 + T45 * T33) * T16 * T10) * T8)
			/ T56;
	ret.origin.z = ((T67 * T24 + T68 * T26 + T69 * T12 * T14 + T70 * T16 * T10)
			* T51 + T71 * T8 * T53 + T72 * T8 * T55
			+ ((T69 * T31 + T62 * T32 + T63 * T33) * T12 * T14
					+ (T70 * T31 + T64 * T32 + T61 * T33) * T16 * T10) * T8)
			/ T56;

	ret.normal.Normalize();

	return ret;
}

#ifdef USE_3DPICKING
void OpenGLCanvas::OnPick(OpenGLPick &result, int x, int y) {
	this->OnPick(result, wxPoint(x, y));
}

void OpenGLCanvas::OnPick(OpenGLPick &result, wxPoint pos) {
	result.SetHits(0);
	if (!IsShown())
		return;
	//TODO: Test if needed:
	//wxClientDC(this); // Set the clipping for this area

	//#ifndef __WXMOTIF__
	//	if(!GetContext()) return false;
	//#endif

	if (context == nullptr)
		context = new Context(this);
	context->SetCurrent(*this); // Link OpenGL to this area
//	if(!isInitialized){
//		InitGL(); // Init OpenGL once, but after SetCurrent
//		isInitialized = true;
//	}
	int w;
	int h;
	GetClientSize(&w, &h);
	glViewport(0, 0, (GLint) w, (GLint) h);

	glClear(GL_DEPTH_BUFFER_BIT);

	glSelectBuffer(result.GetBufferSize(), result.GetBuffer());
	glRenderMode(GL_SELECT);
	glInitNames();

#ifndef USE_GLAD
	glMatrixMode(GL_PROJECTION);
	glCullFace(GL_BACK);
	glLoadIdentity();

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	gluPickMatrix(pos.x, viewport[3] - pos.y, 10, 10, viewport);

	GLdouble ar = (GLdouble) w / (GLdouble) h; // Calculate perspective
	gluPerspective(45, ar, 0.01, 10);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

#endif

	camera.GLMultMatrix();

	RenderPick();
	glFlush();
	GLint hits = glRenderMode(GL_RENDER);
	GLenum err = glGetError();
	result.SetHits(0);
	if (err) {
		DEBUGOUT << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__
				<< " - Error: " << err;
		switch (err) {
		case GL_INVALID_VALUE:
			DEBUGOUT << " (GL_INVALID_VALUE)\n";
			break;
		case GL_INVALID_OPERATION:
			DEBUGOUT << " (GL_INVALID_OPERATION)\n";
			break;
		default:
			DEBUGOUT << '\n';
		}
		return;
	}
	if (hits == (GLint) 0xFFFFFFFF) {
		DEBUGOUT
				<< "OpenGLCanvas::OnPick - Buffer Overflow (increase buffer size).\n";
		return;
	}
	if (hits > 0)
		result.SetHits(hits);
}
#endif

void OpenGLCanvas::RenderPick() {
	this->Render();
}
