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
 *
 *
 *
 */

#include "../3D/AffineTransformMatrix.h"
#include "../math/Unit.h"
#include <wx/panel.h>

class CanvasMeasurementGrid: public wxPanel {
public:
	CanvasMeasurementGrid(wxWindow *parent, wxWindowID id = wxID_ANY,
			const wxPoint &pos = wxDefaultPosition, const wxSize &size =
					wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER,
			const wxString &name = wxPanelNameStr);
	virtual ~CanvasMeasurementGrid();

protected:
	void OnSize(wxSizeEvent &event);
	void OnMouseEvent(wxMouseEvent &event);
	void OnPaint(wxPaintEvent &event);

	void PaintGrid(wxDC &dc);

protected:

public:

	Unit unit; ///< Unit for gridlines (e.g. "1 cm", "1 in", "5 mm", ...)
	int multiplier = 5; ///< Plot a darker line every x units.

protected:
	AffineTransformMatrix projection;
	AffineTransformMatrix view;
	AffineTransformMatrix camera; ///< Mapping into the main XY coordinate system


private:
	AffineTransformMatrix gridInv; ///< Reverse for displaying the grid

	int m_x = 0;
	int m_y = 0;

};

#endif /* SRC_GUI_CANVASMEASUREMENTGRID_H_ */
