///////////////////////////////////////////////////////////////////////////////
// Name               : FacadeTextField.h
// Purpose            : Updating of InputTextfields and associated UnitTextfields
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 09.07.2022
// Copyright          : (C) 2022 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#ifndef GUI_FACADETEXTFIELD_H
#define GUI_FACADETEXTFIELD_H

/*!\class FacadeTextField
 * \brief Facade to Handle the updating of textfields in the input dialogs.
 *
 * This takes into account, that the Units associated with the textfield may change
 * and may point to a totally different unit.
 *
 * For example: A textfield is of unit "cm". If the variable is output, the SI base variable
 * is recalculated to "cm"s, converted into a string and displayed.
 *
 * If a text is input it is checked, if it has no unit in the text. Then the unit of
 * "cm" is also assumed for this text.
 *
 * If on the other hand a unit is given, this unit overrides the unit of the textfield.
 *
 * If an erroreous, unparsable text is entered, the background is changed to red and
 * the errortext is displayed in the hover-text.
 * (The original hover-text is stored upon creation of this object.)
 */

#include "../math/Unit.h"
#include "../math/MathParser.h"

#include <wx/stattext.h>
#include <wx/textctrl.h>

#include <cstdio>
#include <sstream>

template<class T>
class FacadeTextField {
public:
	FacadeTextField() = default;
	FacadeTextField(const FacadeTextField&) = delete;
	FacadeTextField& operator=(const FacadeTextField&) = delete;

	void Set(wxTextCtrl *textctrl, wxStaticText *statictextunit = nullptr);
	void NoUnit();

	void UpdateUnitText() const;
	void UpdateUnitText(const Unit &unit) const;
	void UpdateUnitText(const Unit &unit);

	void ToTextCtrl(T &variable, const Unit &unit);
	void ToTextCtrl(T &variable);
	void ToTextCtrl(const T &variable, const Unit &unit) const;
	void ToTextCtrl(const T &variable) const;

	T ToVariable();
	T ToVariable(const Unit &unit);
	T ToVariable(const wxString &str);
	T ToVariable(const wxString &str, const Unit &unit);

private:
	Unit unit;
	bool isUnitSet = false;
	bool hasError = false;
	wxTextCtrl *textctrl = nullptr;
	wxStaticText *statictextunit = nullptr;
	T oldvalue = 0.0;
	wxString oldhovertext;
};

template<class T>
inline void FacadeTextField<T>::Set(wxTextCtrl *textctrl,
		wxStaticText *statictextunit) {
	this->textctrl = textctrl;
	this->oldhovertext = textctrl->GetToolTipText();
	this->statictextunit = statictextunit;
}

template<class T>
inline void FacadeTextField<T>::NoUnit() {
	unit = Unit();
	isUnitSet = true;
}

template<class T>
inline void FacadeTextField<T>::UpdateUnitText() const {
	if (!isUnitSet)
		throw std::logic_error(
				"The unit was not set before calling UpdateUnitText.");
	if (statictextunit == nullptr) {
		std::stringstream err;
		err << "In FacadeTextField<T>::UpdateUnitText() const for ID "
				<< std::to_string(textctrl->GetId())
				<< " there is no wxStaticText for the unit (" << unit.ToString()
				<< ").\n";
		throw std::runtime_error(err.str());
	} else {
		statictextunit->SetLabel(wxString(unit.GetOtherName()));
	}
}

template<class T>
inline void FacadeTextField<T>::UpdateUnitText(const Unit &unit) const {
	if (statictextunit == nullptr) {
		std::stringstream err;
		err
				<< "In FacadeTextField<T>::UpdateUnitText(const Unit &unit) const for ID "
				<< std::to_string(textctrl->GetId())
				<< " there is no wxStaticText for the unit (" << unit.ToString()
				<< ").\n";
		throw std::runtime_error(err.str());

	} else {
		statictextunit->SetLabel(wxString(unit.GetOtherName()));
	}
}

template<class T>
inline void FacadeTextField<T>::UpdateUnitText(const Unit &unit) {
	this->unit = unit;
	isUnitSet = true;
	if (statictextunit == nullptr) {
		std::stringstream err;
		err << "In FacadeTextField<T>::UpdateUnitText(const Unit &unit) for ID "
				<< std::to_string(textctrl->GetId())
				<< " there is no wxStaticText for the unit (" << unit.ToString()
				<< ").\n";
		throw std::runtime_error(err.str());

	} else {
		statictextunit->SetLabel(wxString(unit.GetOtherName()));
	}
}

template<class T>
inline void FacadeTextField<T>::ToTextCtrl(T &variable, const Unit &unit) {
	if (statictextunit == nullptr && !unit.NoUnit()) {
		std::stringstream err;
		err
				<< "In FacadeTextField<T>::ToTextCtrl(T& variable, const Unit& unit) for ID "
				<< std::to_string(textctrl->GetId())
				<< " there is no wxStaticText for the unit (" << unit.ToString()
				<< ").\n";
		throw std::runtime_error(err.str());

	} else {
		UpdateUnitText(unit);
	}
	ToTextCtrl(variable);
}

template<class T>
inline void FacadeTextField<T>::ToTextCtrl(T &variable) {
	if (!isUnitSet)
		throw std::logic_error(
				"The unit was not set before calling ToTextCtrl(T& variable) in "
						+ std::to_string(textctrl->GetId()));
	if (hasError)
		return;

	if (std::is_same<T, size_t>::value) {
		textctrl->SetValue(wxString::Format(_T("%zd"), variable));
	} else {
		oldvalue = variable;
		textctrl->SetValue(wxString(unit.TextFromSI(variable)));
	}
}

template<class T>
inline void FacadeTextField<T>::ToTextCtrl(const T &variable,
		const Unit &unit) const {

	UpdateUnitText(unit);

	ToTextCtrl(variable);
}

template<class T>
inline void FacadeTextField<T>::ToTextCtrl(const T &variable) const {
	if (!isUnitSet)
		throw std::logic_error(
				"The unit was not set before calling ToTextCtrl(const T& variable) const in "
						+ std::to_string(textctrl->GetId()));
	if (hasError)
		return;
	if (std::is_same<T, size_t>::value) {
		textctrl->SetValue(wxString::Format(_T("%zd"), variable));
	} else {
		textctrl->SetValue(wxString(unit.TextFromSI((double) variable)));
	}
}

template<class T>
inline T FacadeTextField<T>::ToVariable() {
	if (!isUnitSet && !std::is_same<T, size_t>::value)
		throw std::logic_error(
				"The unit was not set before calling ToVariable() in "
						+ std::to_string(textctrl->GetId()));
	return this->ToVariable(unit);
}

template<class T>
inline T FacadeTextField<T>::ToVariable(const Unit &unit) {
	return this->ToVariable(textctrl->GetValue(), unit);
}

template<class T>
inline T FacadeTextField<T>::ToVariable(const wxString &str) {
	if (!isUnitSet && !std::is_same<T, size_t>::value)
		throw std::logic_error(
				"The unit was not set before calling ToVariable(str) in "
						+ std::to_string(textctrl->GetId()));
	return ToVariable(str, unit);
}

template<class T>
inline T FacadeTextField<T>::ToVariable(const wxString &str, const Unit &unit) {
	MathParser m;
	try {
		m.ParseExpression(str.ToStdString());
		MathParser::VM &vm = m.vm;
		vm.Reset();
		vm.Run();

		if (vm.stack.empty())
			throw std::runtime_error(
					"The entered string did not return a value.");

		double result = vm.stack.front().ToDouble();
		Unit resultunit = vm.stack.front().GetUnit();
		if (resultunit.NoUnit()) {
			result = unit.ToSI(result);
		} else {
			if (resultunit != unit)
				throw std::runtime_error(
						"The unit of the expression is not of the type "
								+ unit.ToString() + ".");
		}

		textctrl->SetToolTip(oldhovertext);
		textctrl->SetBackgroundColour(wxNullColour);
		hasError = false;
		oldvalue = (T) result;
	} catch (std::exception &ex) {
		textctrl->SetToolTip(wxString(ex.what()));
		textctrl->SetBackgroundColour(*wxRED);
		hasError = true;
	}

	return oldvalue;
}

#endif /* GUI_FACADETEXTFIELD_H */
