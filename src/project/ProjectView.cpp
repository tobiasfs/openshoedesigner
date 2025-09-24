///////////////////////////////////////////////////////////////////////////////
// Name               : ProjectView.cpp
// Purpose            :
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 28.01.2017
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

#include "ProjectView.h"

#include <cstdio>

#include "../3D/OpenGLMaterial.h"
#include "../gui/FrameMain.h"
#include "../gui/FrameParent.h"
#include "../main.h"

#include "../3D/Surface.h"
#include "../3D/TestGrid.h"

IMPLEMENT_DYNAMIC_CLASS(ProjectView, wxView)

ProjectView::ProjectView() :
		wxView() {
}

ProjectView::~ProjectView() {
	DEBUGOUT << "ProjectView: Destructor called...\n";
}

bool ProjectView::OnCreate(wxDocument *doc, long flags) {
	DEBUGOUT << "ProjectView::OnCreate(...) called...\n";

	if (!wxView::OnCreate(doc, flags)) {
		DEBUGOUT << "ProjectView::OnCreate(...) failed.\n";
		return false;
	}

	wxFrame *frame = wxGetApp().CreateChildFrame(this);
	wxASSERT(frame == GetFrame());

	BackgroundImage temp;
	temp.LoadFile(_T("references/bones/Skel_right.jpg"), wxBITMAP_TYPE_JPEG);
	background.push_back(temp);

//	Project* project = wxStaticCast(this->GetDocument(), Project);
//	foot = &(project->footL);

	frame->Show();
	DEBUGOUT << "ProjectView::OnCreate(...) successful.\n";
	return true;
}

void ProjectView::Paint(bool usePicking) const {

	Project *project = wxStaticCast(this->GetDocument(), Project);

	OpenGLMaterial matBones(OpenGLMaterial::Preset::Pearl);
	OpenGLMaterial matFoot(OpenGLMaterial::Preset::RedRubber, 0.3);
	OpenGLMaterial matLast(OpenGLMaterial::Preset::CyanPlastic);
	OpenGLMaterial matFloor(OpenGLMaterial::Preset::WhitePlastic);
	OpenGLMaterial matInsole(OpenGLMaterial::Preset::YellowPlastic, 0.3);

	matFoot.SetSimpleColor(0.8, 0.5, 0);
	matFloor.SetSimpleColor(0.9, 0.9, 0.9);

	OpenGLMaterial matLines;
	OpenGLMaterial matScan;
	matLines.SetSimpleColor(1, 1, 1, 0.6);
	matScan.SetSimpleColor(0.4, 0.9, 0.6);

	const bool shiftapart = (showLeft && showRight);

	// XY in the plane and Z pointing upwards.
	glRotatef(-90, 1, 0, 0);

	PaintBackground(true);
	glClear( GL_DEPTH_BUFFER_BIT);

	if (showOrigin)
		Canvas3D::PaintCorrdinateSystem();

	matBones.UseColor();

#ifdef DEBUG
	if (!usePicking) {
		OpenGLMaterial::EnableColors();
		matLines.UseColor(0.8);

		project->builder.Paint();
	}
#endif

	if (showLeft) {

		glPushMatrix();
		if (shiftapart)
			glTranslatef(0, project->footL.littleToeGirth->ToDouble() / M_PI,
					0);

		glLoadName(0); // Left

//		if (project->measurementsource
//				== Project::MeasurementSource::scanBased && showFootScan) {
//			glPushName(0);
//			matScan.UseColor(0.5);
//			glNormal3f(1, 0, 0);
//			project->footScan.Paint();
//			glPopName();
//		}

//		if (project->measurementsource
//				== Project::MeasurementSource::measurementBased && showBones) {
//			glPushName(1);
//			matBones.UseMaterial();
//			project->footL.PaintBones();
//			glPopName();
//		}

//		if (project->modeltype == Project::ModelType::boneBased && showSkin
//				&& !usePicking) {
//			glPushName(2);
//			matFoot.UseMaterial();
//			project->footL.PaintSkin();
//			glPopName();
//		}

//		if (project->modeltype == Project::ModelType::lastBased && showLast)
		{
			glPushName(3);

			matScan.UseMaterial();

//			if (project->lastNormalized)
//				project->lastNormalized->Paint();

			glPopName();
		}

		if (showLast) {
			glPushName(3);
			matLast.UseMaterial();
			project->lastL->Paint();
			glPopName();
		}

		if (showInsole) {
			glPushName(4);
//			matLines.UseMaterial();
//			project->bow.Paint();
			matInsole.UseMaterial();
			project->insoleL->Paint();
			glPopName();
		}

		if (showHeel) {
			glPushName(12);
			matInsole.UseMaterial();
			project->heelL->Paint();
			glPopName();
		}
		if (showUpper) {
			glPushName(13);
			OpenGLMaterial::EnableColors();
			PaintUpper();
			glPopName();
		}

		if (showCutaway) {
			OpenGLMaterial::EnableColors();
			glPushName(14);
			PaintCutaway();
			glPopName();
		}

		if (showCoordinateSystem) {
			glPushName(15);
			OpenGLMaterial::EnableColors();
			project->csL->Paint();
			glPopName();
		}

		glPopMatrix();
	}
	if (showRight) {
		glPushMatrix();
		if (shiftapart)
			glTranslatef(0, -project->footR.littleToeGirth->ToDouble() / M_PI,
					0);

		glLoadName(1); // Right

//		if (project->measurementsource
//				== Project::MeasurementSource::scanBased && showFootScan) {
//			glPushName(0);
//			matScan.UseColor(0.5);
//			glNormal3f(1, 0, 0);
//			project->footScan.Paint();
//			glPopName();
//		}

//		if (project->measurementsource
//				== Project::MeasurementSource::measurementBased && showBones) {
//			glPushName(1);
//			matBones.UseMaterial();
//			project->footR.PaintBones();
//			glPopName();
//		}

//		if (project->modeltype == Project::ModelType::boneBased && showSkin
//				&& !usePicking) {
//			glPushName(2);
//			matFoot.UseMaterial();
//			project->footR.PaintSkin();
//			glPopName();
//		}

//		if (project->modeltype == Project::ModelType::lastBased && showLast) {
//			glPushName(3);
//			matScan.UseMaterial();
//			project->lastModelR.Paint();
//			glPopName();
//		}

		if (showLast) {
			glPushName(3);
			matLast.UseMaterial();
			project->lastR->Paint();
			glPopName();
		}

		if (showInsole) {
			glPushName(4);
			matInsole.UseMaterial();
			project->insoleR->Paint();
			glPopName();
		}

		if (showHeel) {
			glPushName(12);
			matInsole.UseMaterial();
			project->heelR->Paint();
			glPopName();
		}
		if (showUpper) {
			glPushName(13);
			OpenGLMaterial::EnableColors();
			PaintUpper();
			glPopName();
		}

		if (showCutaway) {
			OpenGLMaterial::EnableColors();
			glPushName(14);
			PaintCutaway();
			glPopName();
		}

		if (showCoordinateSystem) {
			glPushName(15);
			OpenGLMaterial::EnableColors();
			project->csR->Paint();
			glPopName();
		}

		glPopMatrix();
	}

	if (!usePicking) {
		glLoadName(16);
		matFloor.UseMaterial();
		if (showFloor)
			PaintFloor();
		glLoadName(17);
		OpenGLMaterial::EnableColors();
		if (showBackground)
			PaintBackground(false);
	}
	OpenGLMaterial::EnableColors();
}

//void ProjectView::PaintLast() const {
//	Project* project = wxStaticCast(this->GetDocument(), Project);
//
//	project->lastL.Paint();
//	project->lastvol.PaintSurface();
//}

void ProjectView::PaintSole() const {
//	Project* project = wxStaticCast(this->GetDocument(), Project);
//
//	project->sole.Paint();
}

void ProjectView::PaintUpper() const {
	Project *project = wxStaticCast(this->GetDocument(), Project);
	glColor3f(0.1, 0.4, 0.0);
	for (const Geometry &geo : project->upperL->patches) {
		geo.Paint();
	}
}

void ProjectView::PaintCutaway() const {
	Project *project = wxStaticCast(this->GetDocument(), Project);
//	project->xray.Paint();
}

void ProjectView::PaintFloor() const {
	const float d = 0.5;
	const int N = 10;
	const float dd = d / (float) N;
	glNormal3f(0, 0, 1);
//	glColor3f(0.4, 0.4, 0.4);

	glBegin(GL_QUADS);

	for (int n = -N; n < N; n++) {
		for (int m = -N; m < N; m++) {

			glVertex3f((n - 1) * dd, (m - 1) * dd, 0);
			glVertex3f((n + 1) * dd, (m - 1) * dd, 0);
			glVertex3f((n + 1) * dd, (m + 1) * dd, 0);
			glVertex3f((n - 1) * dd, (m + 1) * dd, 0);

		}
	}

	glEnd();
}

void ProjectView::PaintBackground(bool showBehind) const {
	if (!showBackground)
		return;
	for (std::vector<BackgroundImage>::const_iterator image =
			background.begin(); image != background.end(); image++) {
		if (image->showBehindGeometry == showBehind)
			image->Paint();
	}
}

const FootMeasurements* ProjectView::GetActiveFootMeasurements(void) const {
	//FIXME This function returns a raw pointer.
	const Project *project = wxStaticCast(this->GetDocument(), Project);
	switch (active) {
	case Side::Both:
	case Side::Left:
		return &(project->footL);
	case Side::Right:
		return &(project->footR);
	}
	return nullptr;
}

void ProjectView::OnDraw(wxDC *dc) {
	DEBUGOUT << "ProjectView::OnDraw(...) called...\n";
}

void ProjectView::OnUpdate(wxView *sender, wxObject *hint) {
	wxView::OnUpdate(sender, hint);

	FrameMain *temp = wxStaticCast(GetFrame(), FrameMain);
	temp->TransferDataToWindow();
	temp->Refresh();
}

void ProjectView::OnUpdate3D() {
	FrameMain *frame = wxStaticCast(GetFrame(), FrameMain);
	FrameParent *parentframe = wxStaticCast(frame->GetParent(), FrameParent);
	parentframe->settingsStereo3D.WriteToCanvas(frame->m_canvas3D);
	frame->m_canvas3D->Refresh();
}

bool ProjectView::OnClose(bool deleteWindow) {

	DEBUGOUT << "ProjectView::OnClose( " << (deleteWindow ? "true" : "false")
			<< " ) called...\n";

	if (!wxView::OnClose(deleteWindow))
		return false;

	Activate(false);

	wxDocument *doc = GetDocument();
	wxDocManager *manager = doc->GetDocumentManager();
	wxList tempDocs = manager->GetDocuments();
	wxList tempViews = doc->GetViews();

	DEBUGOUT << "ProjectView::OnClose: " << tempDocs.GetCount() << " docs, "
			<< tempViews.GetCount() << " views\n";

//	GetDocument()->DeleteContents();
	wxWindow *frame = this->GetFrame();
	if (tempDocs.GetCount() <= 1 && tempViews.GetCount() <= 1
			&& frame != nullptr) {
		wxWindow *parent = frame->GetParent();
		DEBUGOUT << "ProjectView::OnClose: Posting wxID_EXIT to parent\n";
		wxCommandEvent event(wxEVT_COMMAND_MENU_SELECTED, wxID_EXIT);
		wxPostEvent(parent, event);
	}

	if (deleteWindow) {
		DEBUGOUT
				<< "ProjectView::OnClose: Request destruction of associated Frame\n";
		frame->Destroy();
		SetFrame(nullptr);
	}
	return true;
}
