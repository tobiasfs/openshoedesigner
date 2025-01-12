///////////////////////////////////////////////////////////////////////////////
// Name               : Canvas3D.cpp
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 11.08.2015
// Copyright          : (C) 2015 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

//#define GL_GLEXT_PROTOTYPES

#include "Canvas3D.h"

#include "../3D/OpenGLMaterial.h"
#include "../Config.h"
#include "../StdInclude.h"

#include <stdint.h>

#include "../3D/OpenGL.h"

//#ifdef __WXMAC__
//#include "OpenGL/glu.h"
////#include "OpenGL/gl.h"
//#else
////#include <GL/gl.h>
//#include <GL/glu.h>
//#endif

Canvas3D::Canvas3D(wxWindow *parent, wxWindowID id,
		const wxPoint &pos, const wxSize &size, long style,
		const wxString &name) :
		OpenGLCanvas(parent, id, pos, size, style, name)  {
	projectview = nullptr;
}



//void Canvas3D::ConnectMouseEvents()
//{
//	this->Connect(wxEVT_MOTION, wxMouseEventHandler(Canvas3D::OnMouseEvent),
//	nullptr, this);
//	this->Connect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(Canvas3D::OnMouseEvent),
//	nullptr, this);
//	this->Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(Canvas3D::OnMouseEvent),
//	nullptr, this);
//	this->Connect(wxEVT_RIGHT_DCLICK,
//			wxMouseEventHandler(Canvas3D::OnMouseEvent), nullptr, this);
//	this->Connect(wxEVT_MIDDLE_DOWN,
//			wxMouseEventHandler(Canvas3D::OnMouseEvent), nullptr, this);
//	this->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(Canvas3D::OnMouseEvent),
//	nullptr, this);
//	this->Connect(wxEVT_LEFT_DCLICK,
//			wxMouseEventHandler(Canvas3D::OnMouseEvent), nullptr, this);
//	this->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(Canvas3D::OnMouseEvent),
//	nullptr, this);
//}
//
//void Canvas3D::DisconnectMouseEvents()
//{
//	this->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(Canvas3D::OnMouseEvent),
//	nullptr, this);
//	this->Connect(wxEVT_LEFT_DCLICK,
//			wxMouseEventHandler(Canvas3D::OnMouseEvent), nullptr, this);
//	this->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(Canvas3D::OnMouseEvent),
//	nullptr, this);
//	this->Connect(wxEVT_MIDDLE_DOWN,
//			wxMouseEventHandler(Canvas3D::OnMouseEvent), nullptr, this);
//	this->Connect(wxEVT_RIGHT_DCLICK,
//			wxMouseEventHandler(Canvas3D::OnMouseEvent), nullptr, this);
//	this->Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(Canvas3D::OnMouseEvent),
//	nullptr, this);
//	this->Connect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(Canvas3D::OnMouseEvent),
//	nullptr, this);
//	this->Connect(wxEVT_MOTION, wxMouseEventHandler(Canvas3D::OnMouseEvent),
//	nullptr, this);
//}

void Canvas3D::SetProjectView(const ProjectView *projectview_) {
	projectview = projectview_;
}

void Canvas3D::Render() {
	if (projectview == nullptr)
		return;
	projectview->PaintBackground(true);

	glClear( GL_DEPTH_BUFFER_BIT);

	// XY in the plane and Z pointing upwards.
	glRotatef(-90, 1, 0, 0);

	if (projectview->showCoordinateSystem)
		PaintCorrdinateSystem();

	// Paint a 3D cursor on the surface of the last.
	{
		OpenGLMaterial matCursor(1, 1, 1, 0.9);
		matCursor.UseMaterial();
		glBegin(GL_LINES);
		Vector3 c = a + b;
		glVertex3d(a.x, a.y, a.z);
		glVertex3d(c.x, c.y, c.z);
		glEnd();
	}

	projectview->Paint(false);
}

void Canvas3D::RenderPick() {
	if (projectview == nullptr)
		return;
	glClear( GL_DEPTH_BUFFER_BIT);
	projectview->Paint(true);
}

void Canvas3D::PaintCorrdinateSystem() {
	OpenGLMaterial matX(0.8, 0.0, 0.0, 0.8);
	OpenGLMaterial matY(0.0, 0.8, 0.0, 0.8);
	OpenGLMaterial matZ(0.0, 0.0, 0.8, 0.8);
	const double len = 0.3;
	const double rad = 0.04;
	const uint_fast8_t N = 4;
	matX.UseMaterial();
	glBegin(GL_LINES);
	glNormal3f(-1, 0, 0);
	glVertex3f(-len, 0, 0);
	glNormal3f(1, 0, 0);
	glVertex3f(len, 0, 0);
	glEnd();
	matY.UseMaterial();
	glBegin(GL_LINES);
	glNormal3f(0, -1, 0);
	glVertex3f(0, -len, 0);
	glNormal3f(0, 1, 0);
	glVertex3f(0, len, 0);
	glEnd();
	matZ.UseMaterial();
	glBegin(GL_LINES);
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(0, 0, -len);
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(0, 0, len);
	glEnd();
	const double n0 = cos(M_PI_4);
	const double n1 = sin(M_PI_4);
	matX.UseMaterial();
	glBegin(GL_LINES);
	for (uint_fast8_t n = 0; n < N; n++) {
		const double a = 2 * M_PI / N * (double) (n);
		const double c = cos(a);
		const double s = sin(a);
		glNormal3d(n0, c * n1, s * n1);
		glVertex3d(len, 0, 0);
		glVertex3d(len - rad, rad * c, rad * s);
	}
	glEnd();
	matY.UseMaterial();
	glBegin(GL_LINES);
	for (uint_fast8_t n = 0; n < N; n++) {
		const double a = 2 * M_PI / N * (double) (n);
		const double c = cos(a);
		const double s = sin(a);
		glNormal3d(s * n1, n0, c * n1);
		glVertex3d(0, len, 0);
		glVertex3d(rad * s, len - rad, rad * c);
	}
	glEnd();
	matZ.UseMaterial();
	glBegin(GL_LINES);
	for (uint_fast8_t n = 0; n < N; n++) {
		const double a = 2 * M_PI / N * (double) (n);
		const double c = cos(a);
		const double s = sin(a);
		glNormal3d(c * n1, s * n1, n0);
		glVertex3d(0, 0, len);
		glVertex3d(rad * c, rad * s, len - rad);
	}
	glEnd();
}

//void Canvas3D::OnMouseEvent(wxMouseEvent& event)
//{
//	event.Skip();
//	if(projectview == nullptr) return;
//	Project* project = wxStaticCast(projectview->GetDocument(), Project);
//
//	if(event.Moving()){
//		int x = event.GetX();
//		int y = event.GetY();
//		OpenGLPick result;
//		this->OnPick(result, x, y);
//		if(result.HasHits()){
//			result.SortByNear();
//			if(result.Get(1) == 3){
//				double u = project->lastL.nurbs.GetU(result.Get(2));
//				double v = project->lastL.nurbs.GetV(result.Get(3));
//
//				a = project->lastL.nurbs.Position(u, v);
//				b = project->lastL.nurbs.Normal(u, v);
//				Refresh();
//			}
//
//		}
//
//	}
//	OpenGLCanvas::OnMouseEvent(event);
//}
