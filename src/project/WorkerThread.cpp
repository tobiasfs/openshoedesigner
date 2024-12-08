///////////////////////////////////////////////////////////////////////////////
// Name               : WorkerThread.cpp
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 05.10.2015
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

#include "WorkerThread.h"
#include "Project.h"

#include "../Config.h"

wxDEFINE_EVENT(wxEVT_COMMAND_THREAD_COMPLETED, wxThreadEvent);
wxDEFINE_EVENT(wxEVT_COMMAND_THREAD_UPDATE, wxThreadEvent);

WorkerThread::WorkerThread(Project * project, size_t threadNr)
		: wxThread(wxTHREAD_DETACHED), threadNr(threadNr)
{
	this->project = project;
}

WorkerThread::~WorkerThread()
{
	if(project != nullptr){
		wxCriticalSectionLocker enter(project->CS);
		switch(threadNr){
		case 0:
			project->thread0 = nullptr;
			break;
		case 1:
			project->thread1 = nullptr;
			break;
		}
	}
}

wxThread::ExitCode WorkerThread::Entry()
{
	if(threadNr >= 2) return (wxThread::ExitCode) 1;

	if(TestDestroy()) return (wxThread::ExitCode) 2;
	bool flag = true;
	while(flag && !TestDestroy()){
//		if(threadNr == 0) flag = project->UpdateLeft();
//		if(threadNr == 1) flag = project->UpdateRight();
		wxQueueEvent(project, new wxThreadEvent(wxEVT_COMMAND_THREAD_UPDATE));
	}
	wxQueueEvent(project, new wxThreadEvent(wxEVT_COMMAND_THREAD_COMPLETED));
	DEBUGOUT << "Thread" << threadNr << " exit.\n";
	return (wxThread::ExitCode) 0;
}
