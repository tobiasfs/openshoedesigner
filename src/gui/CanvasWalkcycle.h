///////////////////////////////////////////////////////////////////////////////
// Name               : CanvasWalkcycle.h
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

#ifndef CANVASWALKCYCLE_H
#define CANVASWALKCYCLE_H

/*!\class CanvasWalkcycle
 * \brief ...
 *
 * ...
 */

#include "../3D/AffineTransformMatrix.h"
#include "../project/ParameterFormula.h"

#include <wx/panel.h>
#include <wx/timer.h>

#include <mutex>
#include <thread>

class CanvasWalkcycle: public wxPanel {
public:
	CanvasWalkcycle(wxWindow *parent, wxWindowID id = wxID_ANY,
			const wxPoint &pos = wxDefaultPosition, const wxSize &size =
					wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER,
			const wxString &name = wxPanelNameStr);
	virtual ~CanvasWalkcycle();

	std::shared_ptr<ParameterFormula> speed; ///< Walking/Running speed in m/s.
	Unit unit;
	int multiplier = 5;
private:
	wxTimer timer;
	double t = 0.0;
	double dt = 1.0 / 30.0;
	double cycle = 0.0;

	AffineTransformMatrix projection;
	AffineTransformMatrix view;

	AffineTransformMatrix s;
	AffineTransformMatrix sRev;
	AffineTransformMatrix g;
	AffineTransformMatrix gRev;

	std::mutex cyclelock;

	void PaintGrid(wxDC &dc);

protected:
	void OnPaint(wxPaintEvent &event);
	void OnSize(wxSizeEvent &event);
	void OnTimer(wxTimerEvent &event);
	void OnMotion(wxMouseEvent &event);
	void OnLeftDown(wxMouseEvent &event);
};

#endif /* CANVASWALKCYCLE_H */
