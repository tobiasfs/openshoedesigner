///////////////////////////////////////////////////////////////////////////////
// Name               : ProjectView.h
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

#ifndef PROJECTVIEW_H
#define PROJECTVIEW_H
/*!\class ProjectView
 * \brief Project view options
 *
 * This class contains viewing options for displaying the data of the project. Thus different windows can
 * use the same project data and only display a different aspect.
 */

#include <stddef.h>
#include <wx/docview.h>
#include <wx/object.h>
#include <memory>
#include <vector>

#include "../3D/BackgroundImage.h"

class FootMeasurements;

class ProjectView: public wxView {
public:
	enum class Side : size_t {
		Both = 0, Left = 10, Right = 20
	};
	enum class Display : int {
		Shoe = 0, ///< The 3D view of the shoe is shown on the left
		Insole = 1,
		Analytics = 2,
		Pattern = 3
	};
	ProjectView();
	virtual ~ProjectView();

	void Paint(bool usePicking) const;
	void PaintBackground(bool showBehind = true) const;

	std::vector<BackgroundImage> background;

	Display display = Display::Shoe;
	Side active;

	bool showLeft;
	bool showRight;

	bool showFootScan;
	bool showBones;
	bool showSkin;
	bool showLeg;

	bool showLastScan;
	bool showLast;
	bool showInsole;
	bool showHeel;
	bool showUpper;
	bool showCutaway;
	bool showFloor;
	bool showCoordinateSystem;
	bool showBackground;

	const FootMeasurements* GetActiveFootMeasurements(void) const;

	virtual bool OnCreate(wxDocument *doc, long flags) override;
	virtual void OnDraw(wxDC *dc) override;
	virtual void OnUpdate(wxView *sender, wxObject *hint = nullptr) override;
	virtual void OnUpdate3D();
	virtual bool OnClose(bool deleteWindow = true) override;

private:
	void PaintLast() const;
	void PaintSole() const;
	void PaintUpper() const;
	void PaintCutaway() const;
	void PaintFloor() const;

DECLARE_DYNAMIC_CLASS(ProjectView)
	;
};

#endif /* PROJECTVIEW_H */
