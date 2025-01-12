///////////////////////////////////////////////////////////////////////////////
// Name               : CanvasInsole.h
// Purpose            : 
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 25.12.2024
// Copyright          : (C) 2024 Tobias Schaefer <tobiassch@users.sourceforge.net>
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
#ifndef SRC_GUI_CANVASINSOLE_H_
#define SRC_GUI_CANVASINSOLE_H_

/** \class CanvasInsole
 * 	\code #include "PanelInsole.h"\endcode
 * 	\ingroup GUI
 *  \brief Panel displaying the insole
 *
 * 2D Panel displaying the insole with measurements.
 */

#include "../project/object/Insole.h"
#include "CanvasMeasurementGrid.h"

#include <memory>
#include <wx/dcclient.h>

class CanvasInsole: public CanvasMeasurementGrid {
public:
	CanvasInsole(wxWindow *parent,
            wxWindowID id = wxID_ANY,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxTAB_TRAVERSAL | wxNO_BORDER,
            const wxString& name = wxPanelNameStr);
	virtual ~CanvasInsole();

protected:
	void OnPaint(wxPaintEvent &event);
	void OnSize(wxSizeEvent &event);

public:
	std::shared_ptr<Insole> insoleL;
	double dL = 0.1;
	std::shared_ptr<Insole> insoleR;
	double dR = 0.1;

private:
	void PaintInsole(wxDC &dc, const AffineTransformMatrix &m,
			const std::shared_ptr<Insole> &insole);
};

#endif /* SRC_GUI_CANVASINSOLE_H_ */
