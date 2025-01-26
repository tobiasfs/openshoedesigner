///////////////////////////////////////////////////////////////////////////////
// Name               : FrameCalculator.cpp
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
#include "FrameCalculator.h"

#include "SemaphoreTryLocker.h"

#include <regex>

static const std::regex midiPattern("[^a-zA-Z]*midi\\D*(\\d+).*",
		std::regex_constants::icase);

FrameCalculator::FrameCalculator(wxWindow *parent) :
		GUIFrameCalculator(parent), loopGuard(1, 1) {

	// Configure Scintilla text displays
	{
		m_textCtrlCode->SetLexer(wxSTC_LEX_CPP);
		m_textCtrlCode->SetWrapMode(wxSTC_WRAP_WORD);
		m_textCtrlCode->SetWrapVisualFlags(wxSTC_WRAPVISUALFLAG_END);

		wxFont font(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL,
				wxFONTWEIGHT_NORMAL);
		for (int n = wxSTC_C_DEFAULT; n <= wxSTC_STYLE_LASTPREDEFINED + 5; ++n)
			m_textCtrlCode->StyleSetFont(n, font);
		for (int n = wxSTC_C_DEFAULT; n <= wxSTC_STYLE_LASTPREDEFINED + 5; ++n)
			m_textCtrlAsm->StyleSetFont(n, font);
		for (int n = wxSTC_C_DEFAULT; n <= wxSTC_STYLE_LASTPREDEFINED + 5; ++n)
			m_textCtrlStack->StyleSetFont(n, font);

		m_textCtrlCode->StyleSetForeground(wxSTC_C_NUMBER, wxColour("BROWN"));

//	m_textCtrlCode->SetMarginType(1, wxSTC_MARGIN_FOLD);
//	m_textCtrlCode->SetMarginMask(1, wxSTC_MASK_FOLDERS);
//	m_textCtrlCode->SetMarginWidth(1, 16);
//	m_textCtrlCode->SetProperty(wxT("fold"), wxT("1"));

		m_textCtrlCode->SetKeyWords(0,
				"break continue do else false for if return switch true while ");
		m_textCtrlCode->SetKeyWords(1,
				"abs acos asin atan atan2 cbrt ceil cos exp exp2 floow limit log log10 log2 max min mod pow round sin sqrt tan");
		m_textCtrlCode->StyleSetForeground(wxSTC_C_WORD, wxColour("BLUE"));
		m_textCtrlCode->StyleSetBold(wxSTC_C_WORD, true);
		m_textCtrlCode->StyleSetForeground(wxSTC_C_WORD2, wxColour("CYAN"));
		m_textCtrlCode->StyleSetBold(wxSTC_C_WORD2, true);

		m_textCtrlCode->StyleSetForeground(wxSTC_C_COMMENT, wxColour("GREY"));
		m_textCtrlCode->StyleSetItalic(wxSTC_C_COMMENT, true);
		m_textCtrlCode->StyleSetForeground(wxSTC_C_COMMENTLINE,
				wxColour("GREY"));
		m_textCtrlCode->StyleSetItalic(wxSTC_C_COMMENTLINE, true);
		m_textCtrlCode->StyleSetForeground(wxSTC_C_COMMENTDOC,
				wxColour("GREY"));
		m_textCtrlCode->StyleSetItalic(wxSTC_C_COMMENTDOC, true);
		m_textCtrlCode->StyleSetForeground(wxSTC_C_COMMENTLINEDOC,
				wxColour("GREY"));
		m_textCtrlCode->StyleSetItalic(wxSTC_C_COMMENTLINEDOC, true);

//	m_textCtrlCode->AutoCompSetFillUps(true);
//	m_textCtrlCode->AutoCompSetAutoHide(true);
//	m_textCtrlCode->AutoCompSetMaxHeight(10);
//	m_textCtrlCode->CallTipSetBackground(wxColour("LIGHT GREY"));
//	m_textCtrlCode->CallTipSetForeground(wxColour("BLACK"));

		m_textCtrlCode->StyleSetForeground(styleCodeError, wxColor("BLACK"));
		m_textCtrlCode->StyleSetBackground(styleCodeError, wxColor("ORANGE"));

		m_textCtrlAsm->StyleSetForeground(styleInstructionHighlight,
		wxColor("YELLOW"));
		m_textCtrlAsm->StyleSetBackground(styleInstructionHighlight,
		wxColor("GREY"));

	}
	// Initial code
	m_textCtrlCode->SetText(
			"// A torus\n\nd = abs(sqrt(x^2+y^2)-0.5);\nreturn sqrt(d^2+z^2)-0.2;\n");
//	m_textCtrlCode->SetText("d = abs(sqrt(x^2+y^2)-0.5);\nreturn sqrt(d^2+z^2+sqrt(abs(x))/20)-0.2;\n");

// Always present set of input variables
	parser.vm.heap.Set("x", 0);
	parser.vm.heap.Set("y", 0);
	parser.vm.heap.Set("z", 0);

	// Initial Parsing of the code
	parser.vm.maxSteps = 10000;
	CodeParse();
	CodeRun();

	// Update of the min-max values for x, y, and z on all pages.
	UpdateRanges();

	values1D = std::make_shared<Matrix>();
	values2D = std::make_shared<Matrix>();
	values3D = std::make_shared<Volume>();

	m_canvasGraph->values = values1D;

	UpdateGraphs();

	Refresh();

	Bind(wxEVT_STC_MODIFIED, &FrameCalculator::OnStcModified, this);

	timer.SetOwner(this);
	this->Connect(wxEVT_TIMER, wxTimerEventHandler(FrameCalculator::OnTimer),
			nullptr, this);
	timer.Start(100);
	Bind(wxEVT_THREAD, &FrameCalculator::OnThreadUpdate, this);
}

FrameCalculator::~FrameCalculator() {
	Unbind(wxEVT_THREAD, &FrameCalculator::OnThreadUpdate, this);
	this->Disconnect(wxEVT_TIMER, wxTimerEventHandler(FrameCalculator::OnTimer),
			nullptr, this);
	Unbind(wxEVT_STC_MODIFIED, &FrameCalculator::OnStcModified, this);
}

void FrameCalculator::OnClose(wxCloseEvent &evt) {
	if (GetThread() && GetThread()->IsRunning())
		GetThread()->Wait();
	Destroy();
}

wxThread::ExitCode FrameCalculator::Entry() {
	// https://docs.wxwidgets.org/latest/classwx_thread_helper.html

	// The width of the matrix is at first only the number of element on the
	// stack + the number of dimensions. There might be execution paths in the
	// vm that return a different number of results. The matrix will be extended
	// dynamically later on, if needed.

	switch (updateDimensions) {
	case 1: {
		if (!values1D || updaterVm.instructions.empty()) {
			wxQueueEvent(GetEventHandler(), new wxThreadEvent());
			return (wxThread::ExitCode) 0;
		}
		const int Nx = 300;
		size_t width = 1 + updaterVm.stack.size();
		*values1D = Matrix::Zeros(Nx, width);
		const size_t idxX = updaterVm.heap.GetIndex("x");
		Polynomial mapX = Polynomial::ByValue(0, xmin.ToDouble(), Nx - 1,
				xmax.ToDouble());
		for (size_t nx = 0; nx < Nx; ++nx) {
			updaterVm.Reset();
			const double x = mapX((double) nx);
			values1D->Insert(x, nx, 0);
			updaterVm.heap[idxX]() = x;
			try {
				updaterVm.Run();
				if ((updaterVm.stack.size() + 1) > width) {
					width = updaterVm.stack.size() + 1;
					values1D->SetSize(Nx, width);
				}
				for (size_t m = 0; m < updaterVm.stack.size(); ++m)
					values1D->Insert(updaterVm.stack[m].ToDouble(), nx, m + 1);
			} catch (const std::exception &ex) {
			}
		}
	}
		break;
	case 2: {
		if (!values2D || updaterVm.instructions.empty()) {
			wxQueueEvent(GetEventHandler(), new wxThreadEvent());
			return (wxThread::ExitCode) 0;
		}
		const int Nx = 100;
		const int Ny = 100;
		size_t width = 2 + updaterVm.stack.size();
		*values2D = Matrix::Zeros(Nx * Ny, width);
		const size_t idxX = updaterVm.heap.GetIndex("x");
		const size_t idxY = updaterVm.heap.GetIndex("y");
		Polynomial mapX = Polynomial::ByValue(0, xmin.ToDouble(), Nx - 1,
				xmax.ToDouble());
		Polynomial mapY = Polynomial::ByValue(0, ymin.ToDouble(), Ny - 1,
				ymax.ToDouble());
		for (size_t ny = 0; ny < Ny; ++ny) {
			for (size_t nx = 0; nx < Nx; ++nx) {
				updaterVm.Reset();
				const double x = mapX((double) nx);
				const double y = mapY((double) ny);
				values2D->Insert(x, ny * Nx + nx, 0);
				values2D->Insert(y, ny * Nx + nx, 1);
				updaterVm.heap[idxX]() = x;
				updaterVm.heap[idxY]() = y;
				try {
					updaterVm.Run();
					if ((updaterVm.stack.size() + 1) > width) {
						width = updaterVm.stack.size() + 2;
						values2D->SetSize(Nx, width);
					}
					for (size_t m = 0; m < updaterVm.stack.size(); ++m)
						values2D->Insert(updaterVm.stack[m].ToDouble(),
								ny * Nx + nx, m + 2);
				} catch (const std::exception &ex) {
				}
			}
			//	GetThread()->TestDestroy();

		}
	}
		break;
	case 3: {

		if (!values3D || updaterVm.instructions.empty()) {
			wxQueueEvent(GetEventHandler(), new wxThreadEvent());
			return (wxThread::ExitCode) 0;
		}

		const int Nx = 40;
		const int Ny = 40;
		const int Nz = 40;
		const size_t idxX = updaterVm.heap.GetIndex("x");
		const size_t idxY = updaterVm.heap.GetIndex("y");
		const size_t idxZ = updaterVm.heap.GetIndex("z");
		Polynomial mapX = Polynomial::ByValue(0, xmin.ToDouble(), Nx - 1,
				xmax.ToDouble());
		Polynomial mapY = Polynomial::ByValue(0, ymin.ToDouble(), Ny - 1,
				ymax.ToDouble());
		Polynomial mapZ = Polynomial::ByValue(0, zmin.ToDouble(), Nz - 1,
				zmax.ToDouble());

		values3D->SetSize(Nx, Ny, Nz, 1);

		for (size_t nz = 0; nz < Nz; ++nz) {
			for (size_t ny = 0; ny < Ny; ++ny) {
				for (size_t nx = 0; nx < Nx; ++nx) {
					updaterVm.Reset();
					const double x = mapX((double) nx);
					const double y = mapY((double) ny);
					const double z = mapZ((double) nz);
					updaterVm.heap[idxX]() = x;
					updaterVm.heap[idxY]() = y;
					updaterVm.heap[idxZ]() = z;
					try {
						updaterVm.Run();
						values3D->Insert(updaterVm.stack.front().ToDouble(), nx,
								ny, nz);
					} catch (const std::exception &ex) {
					}
				}
			}
		}
	}
		break;
	default:
		break;
	}

	wxQueueEvent(GetEventHandler(), new wxThreadEvent());

	return (wxThread::ExitCode) 0;
}

void FrameCalculator::OnThreadUpdate(wxThreadEvent &evt) {
//	sw.Stop();
//	std::cout << __FUNCTION__ << " (" << sw.GetSecondsWall() << ")\n";

	if (updateDimensions == 2) {
		m_canvasXY->geometries.resize(1);
		Geometry &geo = m_canvasXY->geometries[0];
		geo.Clear();
		const size_t rows = values2D->Size(0);
		const size_t plots = values2D->Size(1) - 2;
		//TODO Transfer these values somehow.
		const int Nx = 100;
		const int Ny = 100;
		if (plots > 0) {
			for (size_t m = 0; m < plots; ++m) {
				std::vector<Vector3> verts(rows);
				for (size_t n = 0; n < rows; ++n) {
					verts[n].x = values2D->operator()(n, 0);
					verts[n].y = values2D->operator()(n, 1);
					verts[n].z = values2D->operator()(n, m + 2);
				}
				const size_t offset = geo.VertexCount();
				geo.AddVertex(verts);
				for (size_t nx = 1; nx < Nx; ++nx)
					for (size_t ny = 1; ny < Ny; ++ny) {
						const size_t idx0 = nx + ny * Nx + offset;
						const size_t idx1 = idx0 - 1;
						const size_t idx2 = idx1 - Nx;
						const size_t idx3 = idx2 + 1;
						geo.AddQuad(idx0, idx1, idx2, idx3);
					}
			}
			geo.CalculateNormals();
		}
	}
	try {
		if (updateDimensions == 3) {
			values3D->CalcSurface();
			m_canvasXYZ->geometries.resize(1);
			Geometry &geo = m_canvasXYZ->geometries[0];
			geo.Clear();
			geo = values3D->geometry;
			geo.FlipInsideOutside();
			// Rescale and center generated surface.
			const int Nx = 40;
			const int Ny = 40;
			const int Nz = 40;
			AffineTransformMatrix m;
			m.ScaleGlobal(
					(xmax.ToDouble() - xmin.ToDouble()) / (double) (Nx - 1),
					(ymax.ToDouble() - ymin.ToDouble()) / (double) (Ny - 1),
					(zmax.ToDouble() - zmin.ToDouble()) / (double) (Nz - 1));
			m.TranslateGlobal(xmin.ToDouble(), ymin.ToDouble(),
					zmin.ToDouble());
			geo.Transform(m);

			if(!geo.SelfCheckPassed(false))
			{
				std::cout << "Problem.\n";
			}



			geo.Join();
			geo.CalculateNormals();
			geo.smooth = true;
		}
	} catch (std::exception &ex) {
		std::cout << ex.what() << "\n";
	}
	isUpdatingGraphs.Unlock();

	m_canvasGraph->Refresh();
	m_canvasXY->Refresh();
	m_canvasXYZ->Refresh();

	UpdateGraphs();
//	wxCriticalSectionLocker lock(m_dataCS);
}

void FrameCalculator::OnNotebookPageChanging(wxNotebookEvent &event) {
	UpdateRanges();
}

void FrameCalculator::OnNotebookPageChanged(wxNotebookEvent &event) {
	shouldUpdateGraphs = true;
	UpdateGraphs();
}

void FrameCalculator::OnTextEnter(wxCommandEvent &event) {
	SemaphoreTryLocker lock(loopGuard);
	if (!lock.IsOK())
		return;

	bool refresh = false;
	std::string textNew = event.GetString().ToStdString();
	wxTextCtrl *textCtrl = wxStaticCast(event.GetEventObject(), wxTextCtrl);

	switch (event.GetId()) {
	case ID_XMIN:
		refresh |= SetRangeValue(textNew, xmin, textCtrl);
		break;
	case ID_XMAX:
		refresh |= SetRangeValue(textNew, xmax, textCtrl);
		break;
	case ID_YMIN:
		refresh |= SetRangeValue(textNew, ymin, textCtrl);
		break;
	case ID_YMAX:
		refresh |= SetRangeValue(textNew, ymax, textCtrl);
		break;
	case ID_ZMIN:
		refresh |= SetRangeValue(textNew, zmin, textCtrl);
		break;
	case ID_ZMAX:
		refresh |= SetRangeValue(textNew, zmax, textCtrl);
		break;

	default:
		DEBUGOUT << "Line " << __LINE__ << ": " << __FUNCTION__ << "( "
				<< event.GetId() << " ) not implemented.\n";
		event.Skip();
	}

	if (refresh) {
		lock.UnLock();
		shouldUpdateGraphs = true;
		UpdateGraphs();
	}

}

void FrameCalculator::OnKillFocus(wxFocusEvent &event) {
	SemaphoreTryLocker lock(loopGuard);
	if (!lock.IsOK())
		return;

	bool refresh = false;
	wxTextCtrl *textCtrl = wxStaticCast(event.GetEventObject(), wxTextCtrl);
	std::string textNew = textCtrl->GetValue().ToStdString();

	switch (event.GetId()) {
	case ID_XMIN:
		refresh |= SetRangeValue(textNew, xmin, textCtrl);
		break;
	case ID_XMAX:
		refresh |= SetRangeValue(textNew, xmax, textCtrl);
		break;
	case ID_YMIN:
		refresh |= SetRangeValue(textNew, ymin, textCtrl);
		break;
	case ID_YMAX:
		refresh |= SetRangeValue(textNew, ymax, textCtrl);
		break;
	case ID_ZMIN:
		refresh |= SetRangeValue(textNew, zmin, textCtrl);
		break;
	case ID_ZMAX:
		refresh |= SetRangeValue(textNew, zmax, textCtrl);
		break;

	default:
		DEBUGOUT << "Line " << __LINE__ << ": " << __FUNCTION__ << "( "
				<< event.GetId() << " ) not implemented.\n";
		event.Skip();
	}

	if (refresh) {
		lock.UnLock();
		shouldUpdateGraphs = true;
		UpdateGraphs();
	}
}

void FrameCalculator::OnPropertyGridChanging(wxPropertyGridEvent &event) {
	SemaphoreTryLocker lock(loopGuard);
	if (!lock.IsOK())
		return;

	const std::string &label =
			wxStaticCast(event.GetProperty(),wxStringProperty)->GetLabel().ToStdString();
	const std::string &value = event.GetValue().GetString().ToStdString();

	MathParser mp;
	try {
		mp.ParseExpression(value);
		mp.vm.Run();
		if (!mp.vm.stack.empty()) {
			const MathParser::Value &newVal = mp.vm.stack.front();
			std::cout << "PropertyName= " << label << ", PropertyValue= "
					<< event.GetPropertyValue().GetString().ToStdString()
					<< ", Value: " << value << ", newVal= " << newVal.ToString()
					<< "\n";
			parser.vm.heap.Set(label, newVal);
			shouldUpdateGraphs = true;
		}
	} catch (const std::exception &ex) {
		event.Veto();
		std::cout << "Vetoed.\n";
	}
}

void FrameCalculator::OnPropertyGridChanged(wxPropertyGridEvent &event) {
	SemaphoreTryLocker lock(loopGuard);
	if (!lock.IsOK())
		return;
	if (m_checkBoxAutorun->GetValue()) {
		lock.UnLock();
		CodeRun();
		UpdateGraphs();
	}
}

void FrameCalculator::OnStcModified(wxStyledTextEvent &event) {
	SemaphoreTryLocker lock(loopGuard);
	if (!lock.IsOK()) {
		event.Skip();
		return;
	}
	if (event.GetId() != ID_CODE) {
		event.Skip();
		return;
	}

	lock.UnLock();
	bool modified = CodeParse();
	if (modified) {
		UpdateVariables();
		UpdateStack();
		UpdateInstructions();
		UpdateInstructionPointer();
		UpdateGraphs();
	}
}

void FrameCalculator::OnTimer(wxTimerEvent &event) {
#ifdef USE_PORTMIDI
	if (mididevice && mididevice->Poll()) {
		MidiIn();
		CodeRun();
		UpdateGraphs();
	}
#endif
}

void FrameCalculator::OnButton(wxCommandEvent &event) {

	switch (event.GetId()) {
	case ID_RUN: {
		if (parser.vm.HasRun())
			parser.vm.Reset();
		parser.vm.Run();
	}
		break;
	case ID_STEPASM: {
		parser.vm.StepOpCode();
	}
		break;
	case ID_STEPINSTRUCTION: {
		parser.vm.StepExpression();
	}
		break;
	case ID_STOP: {
		parser.vm.Reset();
	}
		break;
	default:
		DEBUGOUT << "Line " << __LINE__ << ": " << __FUNCTION__ << "( "
				<< event.GetId() << " ) not implemented.\n";
		event.Skip();
		break;
	}
	UpdateVariables();
	UpdateStack();
	UpdateInstructionPointer();
	MidiOut();
}

void FrameCalculator::OnCheckBox(wxCommandEvent &event) {
	SemaphoreTryLocker lock(loopGuard);
	if (!lock.IsOK())
		return;
	switch (event.GetId()) {
	case ID_AUTORUN:
		CodeRun();
		break;
	default:
		DEBUGOUT << "Line " << __LINE__ << ": " << __FUNCTION__ << "( "
				<< event.GetId() << " ) not implemented.\n";
		event.Skip();
	}
}

bool FrameCalculator::CodeParse() {
	SemaphoreTryLocker lock(loopGuard);
	if (!lock.IsOK())
		return false;

	std::string code = m_textCtrlCode->GetText().ToStdString();

	{
		MathParser::Value tempx = parser.vm.heap.Get("x");
		MathParser::Value tempy = parser.vm.heap.Get("y");
		MathParser::Value tempz = parser.vm.heap.Get("z");
		parser.vm.heap.clear();
		parser.vm.heap.Set("x", tempx);
		parser.vm.heap.Set("y", tempy);
		parser.vm.heap.Set("z", tempz);
	}

	try {
		parser.ParseCode(code);
		parser.vm.Reset();
		if (m_checkBoxAutorun->GetValue()) {
			if (mididevice)
				mididevice->Poll();
			MidiIn();
			parser.vm.Run();
			MidiOut();
		}
		m_textCtrlCode->AnnotationSetVisible(false);
	} catch (const MathParser::Exception &ex) {
		m_textCtrlCode->AnnotationClearAll();
		int row = ex.row - 1;
		int col = ex.col - 1;
		if (row < 0)
			row = m_textCtrlCode->GetLineCount() - 1;
		std::string errorText;
		if (col < 0)
			errorText = ex.ToString();
		else
			errorText = std::string(col, ' ') + "^" + ex.what();
		m_textCtrlCode->AnnotationSetText(row, wxString(errorText));
		m_textCtrlCode->AnnotationSetStyle(row, styleCodeError);
		m_textCtrlCode->AnnotationSetVisible(true);
		return false;
	} catch (const std::exception &ex) {
		std::string errorText = ex.what();
		int row = m_textCtrlCode->GetLineCount() - 1;
		m_textCtrlCode->AnnotationSetText(row, wxString(errorText));
		m_textCtrlCode->AnnotationSetStyle(row, styleCodeError);
		m_textCtrlCode->AnnotationSetVisible(true);
		return false;
	}
	m_propertyGridVariables->Clear();
	shouldUpdateGraphs = true;
	return true;
}

void FrameCalculator::CodeRun() {
	const bool autorun = m_checkBoxAutorun->GetValue();
	if (autorun) {
		parser.vm.Reset();
		parser.vm.Run();

		UpdateVariables();
		UpdateStack();
		UpdateInstructionPointer();
		MidiOut();
	}
}

bool FrameCalculator::SetRangeValue(const std::string &textNew,
		MathParser::Value &value, wxTextCtrl *textCtrl) {
	MathParser mp;
	bool refresh = false;
	try {
		mp.ParseExpression(textNew);
		mp.vm.Run();
		if (!mp.vm.stack.empty()) {
			const double vnew = mp.vm.stack[0].ToDouble();
			const double vold = value.ToDouble();
			if (std::fabs(vnew - vold) > DBL_EPSILON
					|| mp.vm.stack[0].GetUnit() != value.GetUnit()) {
				value = mp.vm.stack[0];
				textCtrl->SetValue(value.ToString());
				textCtrl->SetToolTip("");
				textCtrl->SetBackgroundColour(wxNullColour);
				refresh = true;
			}
		}
	} catch (const std::exception &ex) {
		textCtrl->SetToolTip(wxString(ex.what()));
		textCtrl->SetBackgroundColour(*wxRED);
	}
	return refresh;
}

void FrameCalculator::UpdateRanges() {
	m_textCtrlXMin->SetValue(xmin.ToString());
	m_textCtrlXMax->SetValue(xmax.ToString());
	m_textCtrlXMin2->SetValue(xmin.ToString());
	m_textCtrlXMax2->SetValue(xmax.ToString());
	m_textCtrlXMin3->SetValue(xmin.ToString());
	m_textCtrlXMax3->SetValue(xmax.ToString());

	m_textCtrlYMin2->SetValue(ymin.ToString());
	m_textCtrlYMax2->SetValue(ymax.ToString());
	m_textCtrlYMin3->SetValue(ymin.ToString());
	m_textCtrlYMax3->SetValue(ymax.ToString());

	m_textCtrlZMin3->SetValue(zmin.ToString());
	m_textCtrlZMax3->SetValue(zmax.ToString());
}

void FrameCalculator::UpdateVariables() {
	SemaphoreTryLocker lock(loopGuard);
	if (!lock.IsOK())
		return;

	wxPropertyGridIterator it = m_propertyGridVariables->GetIterator();
	bool alwaysAppend = false;
	for (size_t idx = 0; idx < parser.vm.heap.size(); ++idx) {
		const auto &var = parser.vm.heap[idx];
		const std::string &label = var.name;

		if (alwaysAppend || it.AtEnd()) {
			alwaysAppend = true;
			std::string name = std::string("var_") + std::to_string(idx);
			m_propertyGridVariables->Append(
					new wxStringProperty(label, name, var.ToString()));
		} else {
			wxStringProperty *p = wxStaticCast(*it, wxStringProperty);
			p->SetLabel(label);
			p->SetValueFromString(var.ToString());
			it++;
		}
	}
}

void FrameCalculator::UpdateStack() {
	SemaphoreTryLocker lock(loopGuard);
	if (!lock.IsOK())
		return;

	std::string stack;
	for (size_t n = 0; n < parser.vm.stack.size(); ++n) {
		if (n > 0)
			stack += "\n";
		stack += parser.vm.stack[n].ToString();
	}
	m_textCtrlStack->SetText(stack);
}

bool FrameCalculator::UpdateInstructions() {

	std::string instructions;
	for (size_t n = 0; n < parser.vm.instructions.size(); ++n) {
		if (n > 0)
			instructions += "\n";
		instructions += parser.vm.GetInstruction(n);
	}
	std::string oldInstructions = m_textCtrlAsm->GetText().ToStdString();
	if (instructions != oldInstructions) {
		m_textCtrlAsm->SetText(instructions);
		lastInstructionPointer = (size_t) -1;
		return true;
	}
	return false;
}

void FrameCalculator::UpdateInstructionPointer() {
	if (m_textCtrlAsm->GetValue().IsEmpty())
		return;

	if (lastInstructionPointer == parser.vm.GetInstructionPointer())
		return;

	if (lastInstructionPointer != (size_t) -1) {
		const int p0 = m_textCtrlAsm->PositionFromLine(lastInstructionPointer);
		const int p1 = m_textCtrlAsm->GetLineEndPosition(
				lastInstructionPointer);
		if (p0 < p1) {
			m_textCtrlAsm->StartStyling(p0, 0xffff);
			m_textCtrlAsm->SetStyling(p1 - p0, 0);
		}
	}
	lastInstructionPointer = parser.vm.GetInstructionPointer();
	{
		const int p0 = m_textCtrlAsm->PositionFromLine(lastInstructionPointer);
		const int p1 = m_textCtrlAsm->GetLineEndPosition(
				lastInstructionPointer);
		if (p0 < p1) {
			m_textCtrlAsm->StartStyling(p0, 0xffff);
			m_textCtrlAsm->SetStyling(p1 - p0, styleInstructionHighlight);
			m_textCtrlAsm->ShowPosition(p0);
		}
	}

	if (parser.vm.stepCount >= parser.vm.maxSteps)
		m_textCtrlMaxSteps->SetBackgroundColour(*wxRED);
	else
		m_textCtrlMaxSteps->SetBackgroundColour(wxNullColour);

}

void FrameCalculator::UpdateGraphs() {
	if (!shouldUpdateGraphs)
		return;
//	sw.Stop();
//	std::cout << __FUNCTION__ << " (" << sw.GetSecondsWall() << ") calling\n";

	if (isUpdatingGraphs.TryLock() != wxMUTEX_NO_ERROR)
		return;

	updateDimensions = m_notebook->GetSelection() + 1;
	if (updateDimensions < 1 || updateDimensions > 3) {
		isUpdatingGraphs.Unlock();
		return;
	}
	// Make a deep copy of the virtual machine.
	updaterVm = parser.vm;

	shouldUpdateGraphs = false;
//	sw.Stop();
//	std::cout << __FUNCTION__ << " (" << sw.GetSecondsWall() << ") starting\n";

	if (CreateThread(wxTHREAD_JOINABLE) != wxTHREAD_NO_ERROR) {
		wxLogError
		("Could not create the worker thread!");
		return;
	}
	if (GetThread()->Run() != wxTHREAD_NO_ERROR) {
		wxLogError
		("Could not run the worker thread!");
		return;
	}
}

bool FrameCalculator::MidiIn() {
	if (!mididevice)
		return false;

	bool modified = false;

	for (size_t n = 0; n < parser.vm.heap.size(); ++n) {
		const MathParser::VM::Variable &var = parser.vm.heap[n];
		if (!var.isinput)
			continue;

		const std::string &name = var.name;
		std::smatch match;
		if (!std::regex_match(name, match, midiPattern))
			continue;

		unsigned int midiChannel;
		try {
			// Convert the matched number to an unsigned int
			midiChannel = std::stoul(match[1].str());
		} catch (const std::exception &ex) {
			continue;
		}
		if (midiChannel >= 256)
			continue;

		double value = (double) mididevice->cc[midiChannel];
		double oldValue = var.ToDouble();
		if (std::fabs(value - oldValue) <= DBL_EPSILON)
			continue;
		parser.vm.heap.Set(name, value);
		shouldUpdateGraphs = true;
		modified = true;
	}
	return modified;
}

void FrameCalculator::MidiOut() {
	if (!mididevice)
		return;

	for (size_t n = 0; n < parser.vm.heap.size(); ++n) {
		const MathParser::VM::Variable &var = parser.vm.heap[n];
		if (!var.isoutput)
			continue;

		const std::string &name = var.name;
		std::smatch match;
		if (!std::regex_match(name, match, midiPattern))
			continue;

		unsigned int midiChannel;
		try {
			// Convert the matched number to an unsigned int
			midiChannel = std::stoul(match[1].str());
		} catch (const std::exception &ex) {
			continue;
		}

		if (midiChannel >= 256)
			continue;

		uint8_t value = (int) std::round(
				std::fmin(std::fmax(var.ToDouble(), 0.0), 127.0));
		mididevice->cc[midiChannel] = value;
	}
	mididevice->Poll();
}
