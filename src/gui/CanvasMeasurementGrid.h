///////////////////////////////////////////////////////////////////////////////
// Name               : PanelMeasurementGrid.h
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
#ifndef SRC_GUI_CANVASMEASUREMENTGRID_H_
#define SRC_GUI_CANVASMEASUREMENTGRID_H_

/** \class PanelMeasurementGrid
 * 	\code #include "PanelMeasurementGrid.h"\endcode
 * 	\ingroup GUI
 *  \brief Panel displaying a 2D grid for measurements.
 *
 * 2D Panel for displaying 2D layouts. Additional functions for inserting
 * measurements between points.
 */

#include "../math/Unit.h"
#include <wx/panel.h>

class CanvasMeasurementGrid: public wxPanel {
public:
	CanvasMeasurementGrid(wxWindow *parent, wxWindowID id = wxID_ANY,
			const wxPoint &pos = wxDefaultPosition, const wxSize &size =
					wxDefaultSize, long style =
			wxTAB_TRAVERSAL);
	virtual ~CanvasMeasurementGrid();

	double minX = -0.1;
	double maxX = 0.1;
	double minY = -0.2;
	double maxY = 0.2;

	int multiplier = 5;
	Unit unit;

protected:
	void OnPaint(wxPaintEvent &event);
	void OnSize(wxSizeEvent &event);
	void PaintGrid(wxDC &dc);
};

#endif /* SRC_GUI_CANVASMEASUREMENTGRID_H_ */
