///////////////////////////////////////////////////////////////////////////////
// Name               : Project.cpp
// Purpose            : Project data
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 29.01.2016
// Copyright          : (C) 2016 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#include "Project.h"

#include "../3D/FileOBJ.h"
#include "../3D/FileSTL.h"
#include "../math/MathParser.h"
#include "WorkerThread.h"

#include <wx/log.h>
#include <wx/txtstrm.h>
#if wxUSE_STD_IOSTREAM
#include <wx/wfstream.h>
#else
#include <wx/ioswrap.h>
#endif

#include <cstdio>
#include <float.h>

#include "../gui/gui.h"
#include "../gui/IDs.h"

#include "../3D/OpenGL.h"

IMPLEMENT_DYNAMIC_CLASS(Project, wxDocument)

Project::Project() :
		wxDocument() {

	Register(); // Register all ParameterFormulas with the ParameterEvaluator.

//	vol.SetSize(4, 4, 4, 0.1);
//	vol.SetOrigin(Vector3(-0.15, -0.15, -0.15));

//	vol.SetSize(2, 2, 2, 0.1);
//	vol.SetOrigin(Vector3(-0.05, -0.05, -0.05));

//	for(size_t n = 0; n < vol.Numel(); n++)
//		vol[n] = 0.0;
//	vol.CalcSurface();

//	footScan.InitExample();

	thread0 = nullptr;
	thread1 = nullptr;

	insoleL = std::make_shared<Insole>();
	insoleR = std::make_shared<Insole>();

	lastL = std::make_shared<LastModel>();
	lastR = std::make_shared<LastModel>();

	heelL = std::make_shared<ObjectGeometry>();
	heelR = std::make_shared<ObjectGeometry>();

	builder.Setup(*this);

	Bind(wxEVT_COMMAND_THREAD_COMPLETED, &Project::OnCalculationDone, this);
	Bind(wxEVT_COMMAND_THREAD_UPDATE, &Project::OnRefreshViews, this);
	DEBUGOUT << "Project: Constructor called.\n";
}

Project::~Project() {
	Unbind(wxEVT_COMMAND_THREAD_UPDATE, &Project::OnRefreshViews, this);
	Unbind(wxEVT_COMMAND_THREAD_COMPLETED, &Project::OnCalculationDone, this);
	{
		wxCriticalSectionLocker locker(CSLeft);
		if (thread0 != nullptr)
			thread0->Delete();
	}
	{
		wxCriticalSectionLocker locker(CSRight);
		if (thread1 != nullptr)
			thread1->Delete();
	}
	DEBUGOUT << "Project: Destructor called.\n";
}

/*


 heightfield = footL.skin.SurfaceField();
 OrientedMatrix temp = heightfield.XRay(Volume::Method::MinValue);
 bow.Clear();
 for (unsigned int i = 0; i < temp.Numel(); i++) {
 if (temp[i] < DBL_MAX) {
 bow.InsertPoint(i * temp.dx + temp.origin.x, 0,
 temp[i] + temp.origin.z);
 }
 }
 xray = footL.skin.XRay(Volume::Method::MeanValue);
 bow = footL.GetCenterline();
 //	bow.elements[0] = lastvol.GetSurface(bow.elements[1],
 //			(bow.elements[0] - bow.elements[1]) * 2);
 //	size_t M = bow.elements.GetCount();
 //	bow.elements[M - 1] = lastvol.GetSurface(bow.elements[M - 2],
 //			(bow.elements[M - 1] - bow.elements[M - 2]) * 2);
 //
 bow.Resample(50);
 bow.Filter(20);

 return true;
 }*/
//	if (lastModelL.IsModified()) {
//		lastModelL.Modify(false);
//		insoleL.Mirror(false);
//		lastModelL.UpdateForm(insoleL, measL);
//		lastModelL.Mirror();
//		insoleL.Mirror(true);
////		csL.Update(measL, insoleL, lastModelL);
//		return true;
//	}
//	std::cout << "Update L - done\n";
//	return false;
//}
//bool Project::UpdateRight() {
//	wxCriticalSectionLocker locker(CSRight);
//	if (insoleR.IsModified()) {
//		insoleR.Modify(false);
//		insoleR.Construct(shoe, measR);
//		insoleR.Shape(shoe, fmax(-legLengthDifference->ToDouble(), 0));
//		lastModelR.Modify(true);
//		footR.ModifyPosition(true);
//		return true;
//	}
//	if (footR.IsModifiedForm()) {
//		footR.ModifyForm(false);
////		footR.UpdateForm(measR);
//		footR.ModifyPosition(true);
//		return true;
//	}/*
//	 if (footR.IsModifiedPosition()) {
//	 footR.ModifyPosition(false);
//	 footR.UpdatePosition(shoe, fmax(-legLengthDifference.ToDouble(), 0),
//	 measR.angleMixing.ToDouble());
//	 footR.ModifySkin(true);
//	 return true;
//	 }
//	 if (footR.IsModifiedSkin()) {
//	 footR.ModifySkin(false);
//	 footR.CalculateSkin();
//	 return true;
//	 }*/
//	if (lastModelR.IsModified()) {
//		lastModelR.Modify(false);
//		lastModelR.UpdateForm(insoleR, measR);
////		csR.Update(measR, insoleR, lastModelR);
//		return true;
//	}
//
//	std::cout << "Update R - done\n";
//	return false;
//}
void Project::Register() {
	evaluator.Clear();
	evaluator.SetGroup();
	config.Register(evaluator);
	evaluator.SetGroup((size_t) ProjectView::Side::Left);
	footL.Register(evaluator);
	evaluator.SetGroup((size_t) ProjectView::Side::Right);
	footR.Register(evaluator);
}

void Project::CheckNeeded() {
	const auto &v = GetViews();
	for (auto &ob : v) {
		const ProjectView *projectview = wxStaticCast(ob, ProjectView);

		if (projectview->display == ProjectView::Display::Shoe) {
			if (projectview->active == ProjectView::Side::Left
					|| projectview->active == ProjectView::Side::Both) {
				lastL->MarkNeeded(true);
				heelL->MarkNeeded(true);
			}
			if (projectview->active == ProjectView::Side::Right
					|| projectview->active == ProjectView::Side::Both) {
				lastR->MarkNeeded(true);
				heelR->MarkNeeded(true);
			}
		}
		if (projectview->display == ProjectView::Display::Insole) {
			if (projectview->active == ProjectView::Side::Left
					|| projectview->active == ProjectView::Side::Both) {
				insoleL->MarkNeeded(true);
			}
			if (projectview->active == ProjectView::Side::Right
					|| projectview->active == ProjectView::Side::Both) {
				insoleR->MarkNeeded(true);
			}
		}
	}
}

void Project::Update() {
	if (config.IsModified() || footL.IsModified() || footR.IsModified())
		Modify(true);

	try {
		evaluator.Update();
		evaluator.Calculate();
	} catch (std::exception &ex) {
		std::cerr << "Error while updating: " << ex.what() << '\n';
//		return;
	}

	builder.Setup(*this);

	CheckNeeded();

	builder.Update(*this);

	if (!builder.error.empty())
		std::cerr << builder.error << "\n";

	config.Modify(false);
	footL.Modify(false);
	footR.Modify(false);

//	if (useMultiThreading)
//		std::cout << "Starting threads ...";
//	if (measR.IsModified() || legLengthDifference->IsModified()
//			|| shoe.IsModified() || lastModelR.IsModified()) {
//
//		if (measR.IsModified())
//			footR.ModifyForm(true);
//		measR.Modify(false);
//		insoleR.Modify(true);
//
//		if (measR.IsModified())
//			footR.ModifyForm(true);
//
//		if (thread1 == nullptr) {
//			if (useMultiThreading) {
//				thread1 = new WorkerThread(this, 1);
//				if (thread1->Run() != wxTHREAD_NO_ERROR) {
//					wxLogError
//					("Can't create the thread1!");
//					delete thread1;
//					thread1 = nullptr;
//				}
//			} else {
//				while (UpdateRight())
//					;
//			}
//		}
//
//	}
//	if (measL.IsModified() || legLengthDifference->IsModified()
//			|| shoe.IsModified() || lastModelL.IsModified()) {
//		if (measL.IsModified())
//			footL.ModifyForm(true);
//		//measL->Modify(false);
//		legLengthDifference->Modify(false);
//		//shoe->Modify(false);
//		insoleL.Modify(true);
//
//		if (thread0 == nullptr) {
//			if (useMultiThreading) {
//				thread0 = new WorkerThread(this, 0);
//				if (thread0->Run() != wxTHREAD_NO_ERROR) {
//					wxLogError
//					("Can't create the thread0!");
//					delete thread0;
//					thread0 = nullptr;
//				}
//			} else {
//				while (UpdateLeft())
//					;
//			}
//		}
//	}
//	if (useMultiThreading)
//		std::cout << " done.\n";
	UpdateAllViews();
}

DocumentIstream& Project::LoadObject(DocumentIstream &istream) {
	wxDocument::LoadObject(istream);
#if wxUSE_STD_IOSTREAM
	DocumentIstream &stream = istream;
#else
	wxTextInputStream stream(istream);
#endif
	wxDocument::LoadObject(istream);
	try {
		JSON json = JSON::Load(stream);
		if (!json.IsObject())
			return istream;

		if (json.HasKey("configuration")) {
			JSON &conf = json["configuration"];
			config.FromJSON(conf);
		}
		if (json.HasKey("measurements")) {
			JSON &meas = json["measurements"];
			footL.FromJSON(meas);
			footR.FromJSON(meas);
		}
		if (json.HasKey("measurementsLeft")) {
			JSON &measL = json["measurementsLeft"];
			footL.FromJSON(measL);
		}
		if (json.HasKey("measurementsRight")) {
			JSON &measR = json["measurementsRight"];
			footR.FromJSON(measR);
		}
		stream.clear();
		Update();
		UpdateAllViews();
		Modify(false);
	} catch (const std::exception &ex) {
		std::cerr << "Error while loading project: " << ex.what() << "\n";
	}

//	DEBUGOUT << "stream.good() = " << stream.good() << "\n";
//	DEBUGOUT << "stream.bad() = " << stream.bad() << "\n";
//	DEBUGOUT << "stream.fail() = " << stream.fail() << "\n";
//	DEBUGOUT << "stream.eof() = " << stream.eof() << "\n";

	DEBUGOUT << __FUNCTION__;
	if (!stream)
		DEBUGOUT << ": failed.\n";
	else
		DEBUGOUT << ": successful.\n";
	return istream;
}

DocumentOstream& Project::SaveObject(DocumentOstream &ostream) {
	wxDocument::SaveObject(ostream);
#if wxUSE_STD_IOSTREAM
	DocumentOstream &stream = ostream;
#else
	wxTextOutputStream stream(ostream);
#endif
	wxDocument::SaveObject(ostream);

	JSON json;
	json.SetObject();
	JSON &conf = json["configuration"];
	config.ToJSON(conf);

	if (footL == footR) {
		JSON &meas = json["measurements"];
		footL.ToJSON(meas);
	} else {
		JSON &measL = json["measurementsLeft"];
		footL.ToJSON(measL);
		JSON &measR = json["measurementsRight"];
		footR.ToJSON(measR);
	}
	json.Save(stream);
	return ostream;
}

void Project::SaveFootModel(wxString fileName) {
	wxFileOutputStream output(fileName);
	wxTextOutputStream text(output);
//	return footL.SaveModel(&text);
}

void Project::SaveLast(wxString fileName, bool left, bool right) {
	FileOBJ obj(fileName.ToStdString());
//	if (left)
//		obj.Write(lastModelL.resized);
//	if (right)
//		obj.Write(lastModelR.resized);

//	measR.SaveJSON("test.json");

//	wxFFileOutputStream outStream(fileName);
//	FileSTL temp;
//	if(left) temp.WriteStream(outStream, lastModelL.resized);
//	if(right) temp.WriteStream(outStream, lastModelR.resized);

}

void Project::SaveSkin(wxString fileName, bool left, bool right) {
	FileSTL temp(fileName.ToStdString());
//	if (left)
//		temp.Write(footL.skin.geometry);
//	if (right)
//		temp.Write(footR.skin.geometry);
}

void Project::OnRefreshViews(wxThreadEvent &event) {
	UpdateAllViews();
}

void Project::OnCalculationDone(wxThreadEvent &event) {
	CS.Enter();

	CS.Leave();
	UpdateAllViews(); //TODO This has been done by OnRefreshViews.
}

void Project::StopAllThreads() {
	{
		wxCriticalSectionLocker enter(CS);
		if (thread0) {
			if (thread0->Delete() != wxTHREAD_NO_ERROR)
				wxLogError
				("Can't delete thread0!");
		}
		if (thread1) {
			if (thread1->Delete() != wxTHREAD_NO_ERROR)
				wxLogError
				("Can't delete thread1!");
		}
	}
	while (true) {
		{
			wxCriticalSectionLocker enter(CS);
			if (thread0 == nullptr && thread1 == nullptr)
				break;
		}
		wxThread::This()->Sleep(1);
	}
}

