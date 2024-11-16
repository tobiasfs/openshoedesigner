///////////////////////////////////////////////////////////////////////////////
// Name               : main.h
// Purpose            : Main entry point
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 11.08.2015
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

#ifndef MAIN_H
#define MAIN_H

#include "StdInclude.h"
#include "gui/FrameMain.h"

#include <wx/app.h>
#include <wx/intl.h>
#include <wx/config.h>

class openshoedesigner: public wxApp {
public:
	openshoedesigner();
	virtual ~openshoedesigner();

public:
//	FrameMain* frame;

protected:
	wxString loadOnStartup;
	wxLocale locale;
	wxConfig *config;

public:
	void OnAbout(wxCommandEvent&);
	virtual void OnInitCmdLine(wxCmdLineParser &parser);
	virtual bool OnCmdLineParsed(wxCmdLineParser &parser);
	virtual bool OnInit();
	virtual bool OnExceptionInMainLoop();
	virtual void OnUnhandledException();
	virtual int OnExit();
	wxFrame* CreateChildFrame(wxView *view, ProjectView::FrameType frametype);

wxDECLARE_NO_COPY_CLASS(openshoedesigner);wxDECLARE_EVENT_TABLE();
};
DECLARE_APP(openshoedesigner)

#endif /* MAIN_H */
