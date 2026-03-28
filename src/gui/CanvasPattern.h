///////////////////////////////////////////////////////////////////////////////
// Name               : CanvasPattern.h
// Purpose            :
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 01.11.2017
// Copyright          : (C) 2017 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#ifndef CANVASPATTERN_H
#define CANVASPATTERN_H

/*!\class CanvasPattern
 * \brief Panel displaying the elements of the flattened pattern
 *
 * ...
 */

#include "CanvasMeasurementGrid.h"
#include "../project/Design.h"

#include <memory>
#include <wx/panel.h>

class CanvasPattern: public CanvasMeasurementGrid {
public:
	explicit CanvasPattern(wxWindow *parent, wxWindowID id = wxID_ANY,
			const wxPoint &pos = wxDefaultPosition, const wxSize &size =
					wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER,
			const wxString &name = wxPanelNameStr);
	virtual ~CanvasPattern();

protected:
	void OnPaint(wxPaintEvent &event) override;
//	void OnSize(wxSizeEvent &event) override;
//
//	void OnMotion(wxMouseEvent &event) override;
//	void OnLeftDown(wxMouseEvent &event) override;

public:
	std::shared_ptr<Design> design;

};

#endif /* CANVASPATTERN_H */
