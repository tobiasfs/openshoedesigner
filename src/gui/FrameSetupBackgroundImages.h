///////////////////////////////////////////////////////////////////////////////
// Name               : FrameSetupBackgroundImages.h
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 18.02.2019
// Copyright          : (C) 2019 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#ifndef GUI_FRAMESETUPBACKGROUNDIMAGES_H
#define GUI_FRAMESETUPBACKGROUNDIMAGES_H

/*!\class FrameSetupBackgroundImages
 * \brief ...
 *
 * ...
 */

#include "gui.h"

class FrameSetupBackgroundImages: public GUIFrameSetupBackgroundImages {
public:
	FrameSetupBackgroundImages(wxWindow *parent, wxWindowID id = wxID_ANY,
			const wxString &title = _("Setup background images"),
			const wxPoint &pos = wxDefaultPosition,
			const wxSize &size = wxSize(582, 561),
			long style = wxDEFAULT_FRAME_STYLE | wxRESIZE_BORDER | wxSTAY_ON_TOP
					| wxTAB_TRAVERSAL);
	virtual ~FrameSetupBackgroundImages() = default;

	void OnButtonAdd(wxCommandEvent &event) override;
	void OnButtonRemove(wxCommandEvent &event) override;
	void OnTextEnter(wxCommandEvent &event) override;
	void OnSpin(wxSpinEvent &event) override;
	void OnRightDown(wxMouseEvent &event) override;
	void OnScroll(wxScrollEvent &event) override;

protected:
	virtual void OnCheckBox(wxCommandEvent &event);
};

#endif /* GUI_FRAMESETUPBACKGROUNDIMAGES_H */
