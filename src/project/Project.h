///////////////////////////////////////////////////////////////////////////////
// Name               : Project.h
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

#ifndef PROJECT_H
#define PROJECT_H

/*!\class Project
 * \ingroup project
 * \code #include "Project.h"\endcode
 * \brief Project container
 *
 * Data part of the Data View Model.
 *
 * Dataflow upon update:
 *
 * - The Shoe gets parameters about the Foot (size, width, form, ...).
 * - The Shoe updates its parameters (angles, heights)
 * - The data of the Shoe is passed back to the Foot to update the position of the Bone%s.
 * - The form of the Foot is calculated, this is not the form of the Last. Albeit it is quite close.
 *
 * Here it runs off into various generators.
 *
 * - Basic generator
 * - Generator for classic construction
 * - Generator for cemented shoes
 * - Generator for injection molded shoes
 * - Dutch clog generator
 *
 */

#include <iostream>
#include <memory>
#include <stddef.h>
#include <wx/docview.h>
#include <wx/object.h>
#include <wx/platform.h>
#include <wx/setup.h>
#include <wx/string.h>
#include <wx/thread.h>

#include "../3D/OrientedMatrix.h"
#include "../3D/PointCloud.h"
#include "../3D/Polygon3.h"
#include "CoordinateSystem.h"

#include "Builder.h"
#include "Configuration.h"
#include "design/Design.h"
#include "foot/FootModel.h"
#include "FootMeasurements.h"
#include "object/Insole.h"
#include "object/LastModel.h"
#include "ParameterEvaluator.h"

class WorkerThread;

#if wxUSE_STD_IOSTREAM
typedef std::istream DocumentIstream;
typedef std::ostream DocumentOstream;
#else // !wxUSE_STD_IOSTREAM
typedef wxInputStream DocumentIstream;
typedef wxOutputStream DocumentOstream;
#endif // wxUSE_STD_IOSTREAM/!wxUSE_STD_IOSTREAM

class Project: public wxDocument {
	friend class WorkerThread;
public:

public:
	Project();
	virtual ~Project();

	DocumentOstream& SaveObject(DocumentOstream &ostream);
	DocumentIstream& LoadObject(DocumentIstream &istream);

	void SaveFootModel(wxString fileName);
	void SaveLast(wxString fileName, bool left, bool right);
	void SaveSkin(wxString fileName, bool left, bool right);

	void Register();
	void CheckNeeded();
	void Update();

	void StopAllThreads(); //!< Call from OnClose; the event loop has to be running.

private:
	void OnCalculationDone(wxThreadEvent &event);
	void OnRefreshViews(wxThreadEvent &event);

public:

	Configuration config;
	FootMeasurements footL;
	FootMeasurements footR;
	std::shared_ptr<Design> design;

	ParameterEvaluator evaluator;
	Builder builder;

	// Last to generate
	std::shared_ptr<LastModel> lastL;
	std::shared_ptr<LastModel> lastR;

	// Shoe
	std::shared_ptr<ObjectGeometry> heelL;
	std::shared_ptr<ObjectGeometry> heelR;

	std::shared_ptr<Insole> insoleFlatL;
	std::shared_ptr<Insole> insoleFlatR;
	std::shared_ptr<Insole> insoleL;
	std::shared_ptr<Insole> insoleR;

	// Pattern for the upper of the shoe
	std::shared_ptr<CoordinateSystem> csL;
	std::shared_ptr<CoordinateSystem> csR;

//	FootModel footL;
//	FootModel footR;

//	LastModel lastModelL;
//	LastModel lastModelR;

private:
	bool useMultiThreading = false;
	WorkerThread *thread0;
	WorkerThread *thread1;

	wxCriticalSection CS;
	wxCriticalSection CSLeft;
	wxCriticalSection CSRight;

DECLARE_DYNAMIC_CLASS(Project)
	;DECLARE_NO_COPY_CLASS(Project)
	;

};

#endif /* PROJECT_H */
