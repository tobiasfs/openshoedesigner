///////////////////////////////////////////////////////////////////////////////
// Name               : OpenGLCanvas.h
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

#ifndef OPENGLCANVAS_H
#define OPENGLCANVAS_H

/*!\class OpenGLCanvas
 * \brief Extended wxGLCanvas
 * \ingroup OpenGL
 *
 * Adapter class to provide some basic 3D functions for OpenGL canvases.
 *
 * Provides features like:
 *  * Mouse movement
 *  * 6-DOF controller support
 *  * Object picking
 *  * Stereo 3D (Anaglyph- and Shutterglasses)
 *
 *
 *
 */

#include "../Config.h"

#ifdef USE_6DOFCONTROLLER
class Control3D;
#include <wx/timer.h>
#endif
#ifdef USE_3DPICKING
#include "OpenGLPick.h"
#endif

#include "OpenGLLight.h"
//#include "OpenGLShader.h"

#include "AffineTransformMatrix.h"

#include <wx/glcanvas.h>

class OpenGLCanvas: public wxGLCanvas {
public:
	enum struct Stereo3D {
		Off, //!< No stereo effect
		Anaglyph, //!< Stereoeffect using colored glasses
		Shutter //!< Stereoeffect using shutter-glasses
	};
	enum struct Rotation {
		Trackball, //!< Use trackball style mouse control.
		Interwoven, //!< Rotate around the object using interwoven mouse movements.
		Turntable //!< Rotate the object like a turntable. X rotates always around the Z axis, while Y rotates around the X axis.
	};

	OpenGLCanvas(wxWindow *parent, wxWindowID id = wxID_ANY,
			const wxPoint &pos = wxDefaultPosition, const wxSize &size =
					wxDefaultSize, long style = 0,
			const wxString &name = _T("OpenGLCanvas"));
	virtual ~OpenGLCanvas();

	class Context: public wxGLContext {
	public:
		Context() = delete;
		explicit Context(wxGLCanvas *canvas);
	};

public:
	/**\name Positioning in 3D space
	 *
	 * The position is split into
	 *
	 *\{
	 */

	AffineTransformMatrix projection;
	AffineTransformMatrix camera;
	AffineTransformMatrix camera_position;
	AffineTransformMatrix model;

	AffineTransformMatrix rotmat; ///< Rotation around current origin

	float scale; ///< Scale of the scene

	/**\}
	 */

	/**\name Support for stereo-3D rendering
	 *
	 *
	 *
	 * \{
	 */
	Stereo3D stereoMode;
	float eyeDistance;
	float focalDistance;
	unsigned char backgroundGrayLevel;
	unsigned char rightEyeR;
	unsigned char rightEyeG;
	unsigned char rightEyeB;
	unsigned char leftEyeR;
	unsigned char leftEyeG;
	unsigned char leftEyeB;

	/**\}
	 */

	Rotation rotationMode;

	OpenGLLight Light0; ///< Default light

	/**\brief Calculated unit length in pixel at the origin
	 *
	 * This variable contains the distance in X for the length of one
	 * unit in 3D space. This distance can be used to move the origin in 3D
	 * in unison with the mouse cursor.
	 *
	 * The calculation is updated in the beginning of each rendering of the
	 * scene.
	 */
	float unitAtOrigin;

protected:
	int x; //!< Startpoint for mouse dragging
	int y; //!< Startpoint for mouse dragging
	int w; //!< Width of viewport
	int h; //!< Height of viewport
	float turntableX;
	float turntableY;
	unsigned int m_gllist = 0;

//	OpenGLShader shadows;
//	GLuint depthBuffer;
//	GLuint depthMap;

private:
	Context *context = nullptr;

#ifdef USE_6DOFCONTROLLER
	Control3D *control = nullptr; //!< Link to 6DOF-controller
	wxTimer timer; //!< Timer for polling the controller
#endif

public:
	class Arrow {
	public:
		Vector3 normal;
		Vector3 origin;
	};
	Arrow GetPosition(int x, int y) const;

protected:
	virtual void Render() = 0;
	virtual void RenderPick();

	void OnPaint(wxPaintEvent&WXUNUSED(event));
	void OnEnterWindow(wxMouseEvent&WXUNUSED(event));

	virtual void OnMouseEvent(wxMouseEvent &event);

#ifdef USE_6DOFCONTROLLER
public:
	void SetController(Control3D &control);
#endif

#ifdef USE_3DPICKING
public:
	void OnPick(OpenGLPick &result, int x, int y);
	void OnPick(OpenGLPick &result, wxPoint pos);
private:
	void OnTimer(wxTimerEvent &event);
#endif

wxDECLARE_NO_COPY_CLASS(OpenGLCanvas);
};

#endif /* OPENGLCANVAS_H */

