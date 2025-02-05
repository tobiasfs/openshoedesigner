///////////////////////////////////////////////////////////////////////////////
// Name               : CanvasGraph.h
// Purpose            :
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 30.10.2015
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

#ifndef PANELGRAPH_H
#define PANELGRAPH_H

/*!\class CanvasGraph
 * \brief Simple 2D-graph for DependentVector%s
 *
 * Plots the content of a DependentVetor in a 2D graph.
 */

#include "../3D/AffineTransformMatrix.h"
#include "../math/Matrix.h"
#include "../math/Polynomial.h"
#include "../math/Unit.h"

#include <memory>
#include <wx/panel.h>

class CanvasGraph: public wxPanel {
public:
	CanvasGraph(wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint &pos =
			wxDefaultPosition, const wxSize &size = wxDefaultSize,
			long style = wxTAB_TRAVERSAL | wxNO_BORDER, const wxString &name =
					wxPanelNameStr);
	virtual ~CanvasGraph();

protected:
	void OnPaint(wxPaintEvent &event);
	void OnSize(wxSizeEvent &event);
	void OnMouseEvent(wxMouseEvent &event);

private:
	class Axis {
	public:
		Axis() = default;
		Axis(const Unit &unit_);
		void Set(double vmin, double vmax, size_t tickCount);
		void Paint(wxDC &dc, const AffineTransformMatrix &s) const;

		double dv = 1.0;
		int divs = 1;
		int n0 = -1;
		int n1 = 1;
		Unit unit;
	};

public:

	Axis axisX; ///< Unit for gridlines (e.g. "1 cm", "1 in", "5 mm", ...)
	Axis axisY; ///< Unit for gridlines (e.g. "1 cm", "1 in", "5 mm", ...)
	int multiplier = 5; ///< Plot a darker line every x units.

	std::shared_ptr<const Matrix> values;

protected:
	AffineTransformMatrix projection;
	AffineTransformMatrix view;

	AffineTransformMatrix s; ///< Mapping from SI-units to screen pixels.
	AffineTransformMatrix sRev;

private:
	int m_x = 0;
	int m_y = 0;
};

#endif /* PANELGRAPH_H */
