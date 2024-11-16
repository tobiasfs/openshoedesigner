///////////////////////////////////////////////////////////////////////////////
// Name               : CollectionUnits.cpp
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 18.02.2019
// Copyright          : (C) 2019 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#include "CollectionUnits.h"

#include <algorithm>

#include <cmath>

CollectionUnits::CollectionUnits() {

	// Setup available units

	unitsOfTime[0] = Unit("s", 1, Unit::Base::s, 1);
	unitsOfTime[1] = Unit("min", 60, Unit::Base::s, 1);
	unitsOfTime[2] = Unit("h", 3600, Unit::Base::s, 1);
	unitsOfTime[3] = Unit("d", 86400, Unit::Base::s, 1);

	unitsOfLength[0] = Unit("um", 1e-6, Unit::Base::m, 1);
	unitsOfLength[1] = Unit("mil", 25.4e-6, Unit::Base::m, 1);
	unitsOfLength[2] = Unit("mm", 1e-3, Unit::Base::m, 1);
	unitsOfLength[3] = Unit("cm", 1e-2, Unit::Base::m, 1);
	unitsOfLength[4] = Unit("in", 25.4e-3, Unit::Base::m, 1);
	unitsOfLength[5] = Unit("ft", 12 * 25.4e-3, Unit::Base::m, 1);
	unitsOfLength[6] = Unit("m", 1, Unit::Base::m, 1);
	
	unitsOfAngle[0] = Unit("rad", 1, "rad");
	unitsOfAngle[1] = Unit("deg", M_PI / 180.0, "rad");
	unitsOfAngle[2] = Unit("gon", M_PI / 200.0, "rad");

	Time = unitsOfTime[1];
	Distance = unitsOfLength[3];
	SmallDistance = unitsOfLength[2];
	Tolerance = unitsOfLength[0];
	Angle = unitsOfAngle[1];
	Percentage = Unit("%", (double) 0.01, "-");
}

bool CollectionUnits::Load(wxConfig* config) {
	wxASSERT(config!=NULL);
	if (config == NULL)
		return false;

	wxString temp;
	{
		config->Read(_T("UnitTime"), &temp, _T("min"));
		auto i = std::find(unitsOfTime.begin(), unitsOfTime.end(),
				temp.ToStdString());
		if (i != unitsOfTime.end())
			Time = *i;
	}
	{
		config->Read(_T("UnitDistance"), &temp, _T("cm"));
		auto i = std::find(unitsOfLength.begin(), unitsOfLength.end(),
				temp.ToStdString());
		if (i != unitsOfLength.end())
			Distance = *i;
	}
	{
		config->Read(_T("UnitSmallDistance"), &temp, _T("cm"));
		auto i = std::find(unitsOfLength.begin(), unitsOfLength.end(),
				temp.ToStdString());
		if (i != unitsOfLength.end())
			SmallDistance = *i;
	}
	{
		config->Read(_T("UnitTolerance"), &temp, _T("um"));
		auto i = std::find(unitsOfLength.begin(), unitsOfLength.end(),
				temp.ToStdString());
		if (i != unitsOfLength.end())
			Tolerance = *i;
	}
	{
		config->Read(_T("UnitAngle"), &temp, _T("deg"));
		auto i = std::find(unitsOfAngle.begin(), unitsOfAngle.end(),
				temp.ToStdString());
		if (i != unitsOfAngle.end())
			Angle = *i;
	}
	// unitsOfPercentage is not saved in config file.
	return true;
}

bool CollectionUnits::Save(wxConfig* config) {
	wxASSERT(config!=NULL);
	if (config == NULL)
		return false;

	config->Write(_T("UnitTime"), wxString(Time.GetOtherName()));
	config->Write(_T("UnitDistance"), wxString(Distance.GetOtherName()));
	config->Write(_T("UnitSmallDistance"),
			wxString(SmallDistance.GetOtherName()));
	config->Write(_T("UnitTolerance"), wxString(Tolerance.GetOtherName()));
	config->Write(_T("UnitAngle"), wxString(Angle.GetOtherName()));
	// unitsOfPercentage is not loaded from config file.
	return true;
}
