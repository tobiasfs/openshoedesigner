///////////////////////////////////////////////////////////////////////////////
// Name               : CanvasTestStitch.h
// Purpose            : 
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 11.01.2025
// Copyright          : (C) 2025 Tobias Schaefer <tobiassch@users.sourceforge.net>
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
#ifndef SRC_GUI_CANVASTESTSTITCH_H_
#define SRC_GUI_CANVASTESTSTITCH_H_

/** \class CanvasTestStitch
 * 	\code #include "CanvasTestStitch.h"\endcode
 * 	\ingroup gui
 *  \brief Canvas displaying the stiched up patches.
 *
 * Text
 */

#include "../3D/OpenGLCanvas.h"

class CanvasTestStitch: public OpenGLCanvas {
public:
	CanvasTestStitch(wxWindow *parent, wxWindowID id = wxID_ANY,
			const wxPoint &pos = wxDefaultPosition, const wxSize &size =
					wxDefaultSize, long style = 0,
			const wxString &name = _T("CanvasTestStitch"));
	virtual ~CanvasTestStitch();

protected:
	void Render() override;
};

#endif /* SRC_GUI_CANVASTESTSTITCH_H_ */
