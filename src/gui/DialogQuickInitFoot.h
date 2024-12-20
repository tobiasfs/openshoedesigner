///////////////////////////////////////////////////////////////////////////////
// Name               : DialogQuickInitFoot.h
// Purpose            : Dialog to initialize foot and last settings
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 01.11.2015
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

#ifndef DIALOGQUICKINITFOOT_H
#define DIALOGQUICKINITFOOT_H

/*!\class DialogQuickInitFoot
 * \brief Dialog to initialize foot and last settings
 *
 * Dialog asking for the shoesize to set up the basic foot and last parameter.
 */

#include "gui.h"
#include "../math/MathParser.h"

class DialogQuickInitFoot: public GUIDialogQuickInitFoot {
public:
	DialogQuickInitFoot(wxWindow *parent);
	virtual ~DialogQuickInitFoot() = default;

	virtual void OnTextEnter(wxCommandEvent &event) override;
	virtual void OnClose(wxCommandEvent &event) override;

	double length;
	double width;

private:
	void ParseText(std::string text);
};

#endif /* DIALOGQUICKINITFOOT_H */
