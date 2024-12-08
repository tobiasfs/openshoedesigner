///////////////////////////////////////////////////////////////////////////////
// Name               : main.cpp
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

#include "main.h"

#include "gui/FrameParent.h"
#include "gui/FrameMain.h"

#include "languages.h"
#include <wx/aboutdlg.h>

#include <wx/docview.h>
#include <wx/cmdline.h>
#include <cstdlib>

#include "math/Dependencies.h"
#include "math/DependentVector.h"

IMPLEMENT_APP(openshoedesigner)

wxBEGIN_EVENT_TABLE(openshoedesigner, wxApp) EVT_MENU(wxID_ABOUT, openshoedesigner::OnAbout)
wxEND_EVENT_TABLE()

void openshoedesigner::OnAbout(wxCommandEvent&) {
	wxAboutDialogInfo aboutInfo;

	aboutInfo.SetName(_T("OpenShoeDesigner"));
	aboutInfo.SetVersion(_T("0.1"));
	aboutInfo.SetDescription(_("Design software for Lasts and Shoes"));
	aboutInfo.SetCopyright(_T("(C) 2018-2023"));
	aboutInfo.SetWebSite(
			_T("https://sourceforge.net/projects/openshoedesigner/"));
	aboutInfo.SetLicence(
			_T(
					"GNU General Public License version 3.0 (GPLv3)\n\nThis program comes with ABSOLUTELY NO WARRANTY.\nThis is free software, and you are welcome to redistribute it under certain conditions.\n\nYou should have received a copy of the GNU General Public License along with this program.\nIf not, see http://www.gnu.org/licenses/."));
	aboutInfo.AddDeveloper(_T("Tobias Schäfer"));

	wxAboutBox(aboutInfo);
}

openshoedesigner::openshoedesigner() {

//	DependentVector D;
//	D.PushBack(-1, 0);
//	D.PushBack(0, 2);
//	D.PushBack(3, 4);
//	D.PushBack(5, 1);
//	D.PushBack(7, 3);

//	std::cout << D.IatX(-1) << "\n";
//	std::cout << D.IatX(0) << "\n";
//	std::cout << D.IatX(1) << "\n";
//	std::cout << D.IatX(2) << "\n";
//	std::cout << D.IatX(3) << "\n";
//	std::cout << D.IatX(4) << "\n";
//	std::cout << D.IatX(5) << "\n";

//	std::cout << D.YatX(-1) << "\n";
//	std::cout << D.YatX(0) << "\n";
//	std::cout << D.YatX(1) << "\n";
//	std::cout << D.YatX(4) << "\n";

//	std::cout << D.IatY(1.5,DependentVector::Direction::first_fallingbelow) << "\n";
//	std::cout << D.IatY(1.5,DependentVector::Direction::last_fallingbelow) << "\n";
//	std::cout << D.IatY(1.5,DependentVector::Direction::first_risingabove) << "\n";
//	std::cout << D.IatY(1.5,DependentVector::Direction::last_risingabove) << "\n";

//	std::cout << D.XatY(0.1) << "\n";
//	std::cout << D.XatY(29) << "\n";
//	std::cout << D.XatY(31) << "\n";

	Dependencies D;
	D.SetSize(8, 3);
//
// [1,2,3,4]
//	double h[] = {1, -1, 0, 0, 1, 0, 1, -1, 1, 0, 2, 0, 1, 0, 3, 0, -1, 1, 0, 0,
//			0, 1, -1, 1, 0, 1, 0, 2, 0, 1, 0, 3};
//
// [1,2,3,5]
//	double h[] = {1, 0, 0, 0, 1, 0, 1, -1, 1, 0, 2, 0, 1, 0, 3, 0, -1, 1, 0, 0,
//			0, 1, -1, 1, 0, 1, 0, 2, 0, 1, 0, 3};
// [1,2,3]
//	double h[] = {1, -2, 0, 0, 1, -2, 1, -1, 1, -2, 2, 0, 1, -2, 3, 0, -1, 2, 0,
//			0, 0, 0, -1, 1, 0, 0, 0, 2, 0, 0, 0, 3};
//
//	double h[] = { 0.25,1,-0.25,-1,0,0,0,0      ,0,0,0.5,1,-0.5,-1,0,0,   0,0,0,0,0.75,1,-0.75,-1};
//
//	D.Insert(h, D.Numel());
//	D.Transpose();
//
//	std::cout << D << '\n';
//
//	D.Calculate();
//
//	std::cout << D << '\n';
//	return;

	config = new wxConfig(_T("openshoedesigner"));

	unsigned int selectedLanguage = wxLocale::GetSystemLanguage();
	if (selectedLanguage == wxLANGUAGE_UNKNOWN)
		selectedLanguage = wxLANGUAGE_DEFAULT;

	// Read language from config.
	wxString str;
	if (config->Read(_T("Language"), &str)) {
		unsigned int i;
		for (i = 0; i < WXSIZEOF(langNames); i++)
			if (str.CmpNoCase(langNames[i]) == 0) {
				selectedLanguage = langIds[i];
			}
	}

	if (!locale.Init(selectedLanguage, wxLOCALE_DONT_LOAD_DEFAULT)) {
		wxLogWarning
		(_T("This language is not supported by the system."));
		return;
	}

	locale.AddCatalogLookupPathPrefix(_T("i18n"));
	bool catalogLoaded = locale.AddCatalog(_T("openshoedesigner"));
	if (!catalogLoaded) {
		wxString temp;
		temp =
		_T("The translation catalog for ") + locale.GetCanonicalName() +
		_T(" was not loaded !");
		wxLogWarning
		(temp);
	}
	locale.AddCatalog("wxstd");

}

openshoedesigner::~openshoedesigner() {
	DEBUGOUT << "wxApp: Destructor called\n";
	delete config; // config is written back on deletion of object
}

// The Commandline is parsed before OnInit is called.
void openshoedesigner::OnInitCmdLine(wxCmdLineParser &parser) {
	parser.AddParam(_("<filepath of document to open>"), wxCMD_LINE_VAL_STRING,
			wxCMD_LINE_PARAM_OPTIONAL);
//	parser.AddSwitch("t", "test", "Runs unit tests on some of the classes.");
	wxApp::OnInitCmdLine(parser);
}

bool openshoedesigner::OnCmdLineParsed(wxCmdLineParser &parser) {
	int count = parser.GetParamCount();
	if (count == 1) {
		wxString str = parser.GetParam(0);
		//		if(_DEBUGMODE) wxLogMessage(_T("cmd line param: ") + str);
		loadOnStartup = str;
	}
	return true;
}

bool openshoedesigner::OnInit() {
	if (!wxApp::OnInit())
		return false;
	::wxInitAllImageHandlers(); // Load all image handlers for reading in background images.

	SetAppName("openshoedesigner");
	SetAppDisplayName("Open Shoe Designer");

	wxDocManager *docManager = new wxDocManager;

	new wxDocTemplate(docManager, "Shoe Design", "*.dsn", "", "dsn", "Project",
			"Project View", CLASSINFO(Project), CLASSINFO(ProjectView));

#if defined( __WXMAC__ )  && wxOSX_USE_CARBON
	wxFileName::MacRegisterDefaultTypeAndCreator("dsn" , 'WXMB' , 'WXMA'); // ?
#endif

	docManager->FileHistoryLoad(*config);

	wxFrame *parent;
	parent = new FrameParent(docManager, config, nullptr, wxID_ANY,
			GetAppDisplayName());

	SetTopWindow(parent);
	parent->Show(false);

	try {
		Project *project;
		if (loadOnStartup.IsEmpty()) {
			project = (Project*) docManager->CreateDocument(wxEmptyString,
					wxDOC_NEW);
		} else {
			project = (Project*) docManager->CreateDocument(loadOnStartup,
					wxDOC_SILENT);
		}
	} catch (std::exception &exception) {
		std::cerr << "Exeption caught on first CreateDocument:\n"
				<< exception.what() << "\n";
	}

	return true;
}

bool openshoedesigner::OnExceptionInMainLoop() {
	std::string error;
	try {
		throw; // Rethrow the current exception.
	} catch (const std::exception &e) {
		error = e.what();
	} catch (...) {
		error = "unknown error.";
	}
	std::cerr << "Unexpected exception has occurred: " << error
			<< " The program will terminate.\n";
	return false;
}

void openshoedesigner::OnUnhandledException() {
	std::cerr << "Unhandled Exception.\n";
}

int openshoedesigner::OnExit() {
	wxDocManager *const docManager = wxDocManager::GetDocumentManager();
	docManager->FileHistorySave(*config);
	delete docManager;
	DEBUGOUT << "wxApp: Exiting Application\n";
	return wxApp::OnExit();
}

wxFrame* openshoedesigner::CreateChildFrame(wxView *view,
		ProjectView::FrameType frametype) {
	wxFrame *subframe;
	wxDocument *doc = view->GetDocument();

	switch (frametype) {
	case ProjectView::FrameType::mainframe:
		subframe = new FrameMain(doc, view, config,
				wxStaticCast(GetTopWindow(), wxDocParentFrame));
		break;

	default:
		throw(std::logic_error("CreateChildFrame - Unsupported windowtype."));
	}
	return subframe;
}
