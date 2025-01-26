///////////////////////////////////////////////////////////////////////////////
// Name               : FrameCalculator.h
// Purpose            : 
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 19.01.2025
// Copyright          : (C) 2025 Tobias Schaefer <tobiassch@users.sourceforge.net>
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
#ifndef SRC_GUI_FRAMECALCULATOR_H_
#define SRC_GUI_FRAMECALCULATOR_H_

/** \class FrameCalculator
 * 	\code #include "FrameCalculator.h"\endcode
 * 	\ingroup GroupName
 *  \brief Description
 *
 * Text
 */

#include "gui.h"

#include "../3D/Geometry.h"
#include "../3D/Volume.h"
#include "../math/DependentVector.h"
#include "../math/MathParser.h"
#include "../system/MidiPort.h"
#include "../system/StopWatch.h"

#include <wx/thread.h>
#include <wx/timer.h>
#include <memory>

class FrameCalculator: public GUIFrameCalculator, public wxThreadHelper {
public:
	FrameCalculator(wxWindow *parent);
	virtual ~FrameCalculator();

	bool CodeParse();
	void CodeRun();

	MathParser::Value xmin = -1.0;
	MathParser::Value xmax = 1.0;
	MathParser::Value ymin = -1.0;
	MathParser::Value ymax = 1.0;
	MathParser::Value zmin = -1.0;
	MathParser::Value zmax = 1.0;

	std::shared_ptr<MidiDevice> mididevice;

protected:
	virtual void OnButton(wxCommandEvent &event) override;
	virtual void OnCheckBox(wxCommandEvent &event) override;
	virtual void OnKillFocus(wxFocusEvent &event) override;
	virtual void OnNotebookPageChanging( wxNotebookEvent &event) override;
	virtual void OnNotebookPageChanged( wxNotebookEvent &event) override;
	virtual void OnPropertyGridChanging(wxPropertyGridEvent &event) override;
	virtual void OnPropertyGridChanged(wxPropertyGridEvent &event) override;
	virtual void OnStcModified(wxStyledTextEvent &event);
	virtual void OnTextEnter(wxCommandEvent &event) override;

    void OnClose(wxCloseEvent& evt);
    void OnThreadUpdate(wxThreadEvent& evt);
	virtual void OnTimer(wxTimerEvent &event);
	virtual wxThread::ExitCode Entry();
	wxCriticalSection m_dataCS;

	bool SetRangeValue(const std::string &textNew, MathParser::Value &value,
			wxTextCtrl *textCtrl);
	void UpdateRanges();
	void UpdateStack();
	void UpdateVariables();
	bool UpdateInstructions();
	void UpdateInstructionPointer();

	void UpdateGraphs();

	bool MidiIn();
	void MidiOut();

private:
	wxTimer timer;
	wxSemaphore loopGuard;

	size_t lastInstructionPointer = (size_t) -1;

	bool shouldUpdateGraphs = true;
//	wxCriticalSection criticalSection;
	wxMutex isUpdatingGraphs;

	MathParser parser;

	const int styleCodeError = wxSTC_STYLE_LASTPREDEFINED + 1;
	const int styleInstructionHighlight = 24;

	StopWatch sw;

	uint8_t updateDimensions = 0;
	MathParser::VM updaterVm;
	std::shared_ptr<Matrix> values1D;
	std::shared_ptr<Matrix> values2D;
	std::shared_ptr<Volume> values3D;

};

#endif /* SRC_GUI_FRAMECALCULATOR_H_ */
