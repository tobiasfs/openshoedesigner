///////////////////////////////////////////////////////////////////////////////
// Name               : Unit.cpp
// Purpose            : Value with unit
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 07.06.2014
// Copyright          : (C) 2014 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#include "Unit.h"

#include <cfloat>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <numeric>
#include <sstream>

Unit::Unit(int s, int m, int kg, int A, int K, int mol, int cd) {
	powerunit[0] = s;
	powerunit[1] = m;
	powerunit[2] = kg;
	powerunit[3] = A;
	powerunit[4] = K;
	powerunit[5] = mol;
	powerunit[6] = cd;

	SIName = this->ToString();
	otherName = SIName;
}

Unit::Unit(const std::string &otherName_) :
		otherName(otherName_) {
	ParseString(otherName_);
	SIName = this->ToString();
}

Unit::Unit(const std::string &otherName_, const double factor_,
		const std::string &_SIName) :
		SIName(_SIName), otherName(otherName_), factor(factor_) {
}

double Unit::FromSI(const double value) const {
	return value / factor;
}

double Unit::ToSI(const double value) const {
	return value * factor;
}

std::string Unit::GetSIName() const {
	return SIName;
}

std::string Unit::GetOtherName() const {
	return otherName;
}

std::string Unit::TextFromSIWithUnit(const double value,
		int digitsAfterComma) const {
	std::stringstream buf;
	if (digitsAfterComma >= 0) {
		buf.setf(std::ios::fixed, std::ios::floatfield);
		buf.precision(digitsAfterComma);
	}
	buf << FromSI(value);
	if (!NoUnit()) {
		if (otherName.empty() && std::fabs(factor - 1.0) < DBL_EPSILON)
			buf << ' ' << ToString();
		else
			buf << ' ' << otherName;
	}
	return buf.str();
}

std::string Unit::TextFromSI(const double value, int digitsAfterComma) const {
	std::stringstream buf;
	if (digitsAfterComma >= 0) {
		buf.setf(std::ios::fixed, std::ios::floatfield);
		buf.precision(digitsAfterComma);
	}
	buf << FromSI(value);
	return buf.str();
}

uint_fast8_t Unit::Size() {
	return unitcount;
}

std::string Unit::GetName(Base idx) {
	switch (idx) {
	case Base::s:
		return "s";
	case Base::m:
		return "m";
	case Base::kg:
		return "kg";
	case Base::A:
		return "A";
	case Base::K:
		return "K";
	case Base::mol:
		return "mol";
	case Base::cd:
		return "cd";
	default:
		return "";
	}
}

int Unit::operator[](Base idx) const {
	return powerunit[(uint_fast8_t) idx];
}

std::string Unit::ToString(bool useNegativeExponents,
		bool modelicaStyle) const {
	std::stringstream buf;
	bool first = true;

	//FIXME Per volume units are rendered incorrectly e.g. "mol/ml" -> "lx*mol/(m*cd)"
	//FIXME The whole algorithm is unstable when working with L=log2(x);return 2^L; without any unit anywhere!

	std::array<int, derivedunitcount> f;
	f.fill(0);
	std::array<int, unitcount> temp;
	std::copy(powerunit.begin(), powerunit.end(), temp.begin());
	int bestv = derivedUnits[0].Diff(temp, 0);
	int bestf = 0;
	size_t pos = 0;
	do {
		for (size_t n = 0; n < derivedunitcount; ++n) {
			int cf = derivedUnits[n].Compare(temp);
			if (first && cf <= 0)
				continue;
			int d = derivedUnits[n].Diff(temp, cf);
			if (d < bestv) {
				pos = n;
				bestv = d;
				bestf = cf;
			}
		}
		if (bestf != 0) {
			f[pos] = bestf;
			for (size_t n = 0; n < unitcount; ++n)
				temp[n] -= derivedUnits[pos].exp[n] * bestf;
			bestv = derivedUnits[0].Diff(temp, 0);
		}
		first = false;
	} while (bestv != 0);

	first = true;
	int fractioncount = 0;
	for (size_t n = 0; n < derivedunitcount; ++n) {
		if (f[n] < 0)
			++fractioncount;
		if (f[n] <= 0 && !useNegativeExponents)
			continue;
		if (first)
			first = false;
		else if (modelicaStyle)
			buf << ".";
		else
			buf << '*';
		if (derivedUnits[n].name.compare("g") == 0)
			buf << "k"; // In this case it is always kg.
		buf << derivedUnits[n].name;
		if (f[n] > 1) {
			if (modelicaStyle)
				buf << f[n];
			else
				buf << '^' << f[n];
		}
	}
	if (fractioncount == 0 || useNegativeExponents)
		return buf.str();
	if (first)
		buf << '1';
	buf << '/';
	if (fractioncount > 1)
		buf << '(';
	first = true;
	for (size_t n = 0; n < derivedunitcount; ++n) {
		if (f[n] >= 0)
			continue;
		if (first)
			first = false;
		else if (modelicaStyle)
			buf << ".";
		else
			buf << '*';
		if (derivedUnits[n].name.compare("g") == 0)
			buf << "k"; // In this case it is always kg.
		buf << derivedUnits[n].name;
		if (f[n] < -1) {
			if (modelicaStyle)
				buf << -f[n];
			else
				buf << '^' << -f[n];
		}
	}
	if (fractioncount > 1)
		buf << ')';
	return buf.str();
}

std::ostream& operator<<(std::ostream &out, const Unit &unit) {
//	out << " [";
//	for (size_t n = 0; n < Unit::unitcount; ++n) {
//		if (n > 0)
//			out << ", ";
//		out << unit.powerunit[n];
//	}
//	out << "]";
	out << unit.ToString();
	out << ", [" << unit.ToString() << "]";
	if (fabs(unit.factor - 1.0) > DBL_EPSILON)
		out << "/" << unit.factor;
	out << " = [" << unit.otherName << "]";

	return out;
}

Unit& Unit::operator *=(const Unit &rhs) {

	for (size_t i = 0; i < Unit::unitcount; ++i)
		this->powerunit[i] += rhs.powerunit[i];
	this->otherName = "?";
	return *this;
}

const Unit Unit::operator *(const Unit &rhs) const {
	Unit lhs = *this;
	lhs *= rhs;
	return lhs;
}

Unit& Unit::operator /=(const Unit &rhs) {
	for (size_t i = 0; i < Unit::unitcount; ++i)
		this->powerunit[i] -= rhs.powerunit[i];
	this->otherName = "?";
	return *this;
}

const Unit Unit::operator /(const Unit &rhs) const {
	Unit lhs = *this;
	lhs /= rhs;
	return lhs;
}

bool Unit::operator==(const Unit &rhs) const {
	for (size_t i = 0; i < Unit::unitcount; ++i)
		if (this->powerunit[i] != rhs.powerunit[i])
			return false;
	return true;
}

bool Unit::operator!=(const Unit &rhs) const {
	return !(*this == rhs);
}

bool Unit::operator ==(const std::string &rhs) const {
	return otherName.compare(rhs) == 0;
}

bool Unit::operator !=(const std::string &rhs) const {
	return !(*this == rhs);
}

bool Unit::NoUnit() const {
	for (const auto &p : powerunit)
		if (p != 0)
			return false;
	return true;
}

void Unit::Power(const double exponent) {
// (e.g. Noise-power-density has the unit of V/sqrt(Hz) or sometimes A/sqrt(Hz)).

	if (fabs(exponent - 1.0) < 1e-9)
		return;

	this->otherName = "?";

	//TODO: Create a more reliable algorithm here.
	if (fabs(exponent) < 1.0) {
		double f = (double) 1.0 / exponent;
		double r = std::round(f);
		for (size_t i = 0; i < Unit::unitcount; ++i)
			this->powerunit[i] /= f;

	} else {
		for (size_t i = 0; i < Unit::unitcount; ++i)
			this->powerunit[i] *= exponent;
	}

}

int Unit::DerivedUnit::Compare(const std::array<int, unitcount> &rhs) const {
	int r0 = Diff(rhs, 0);
	int r1 = Diff(rhs, 1);
	if (r1 < r0) {
		int ret = 1;
		r0 = r1;
		r1 = Diff(rhs, 2);
		while (r1 < r0) {
			++ret;
			r0 = r1;
			r1 = Diff(rhs, ret + 1);
		}
		return ret;
	}
	r1 = Diff(rhs, -1);
	if (r1 < r0) {
		int ret = -1;
		r0 = r1;
		r1 = Diff(rhs, ret - 1);
		while (r1 < r0) {
			--ret;
			r0 = r1;
			r1 = Diff(rhs, ret - 1);
		}
		return ret;
	}
	return 0;
}

int Unit::DerivedUnit::Diff(const std::array<int, unitcount> &rhs,
		int factor) const {
	int r = 0;
	for (uint_fast8_t n = 0; n < unitcount; ++n)
		r += std::abs(exp[n] * factor - rhs[n]);
	return r;
}

void Unit::ParseString(const std::string &txt) {

//	std::cout << "\n[";
//	size_t n = 0;
//	for (auto const & v : actionTable) {
//		std::cout << v << ',';
//		++n;
//		if ((n % 256) == 0)
//			std::cout << "...\n";
//	}
//	std::cout << "]\n";

	size_t state = 1;
	powerprefix = 0;
	factor = 1.0;
	powerunit.fill(0);
	for (unsigned char c : txt) {
		const size_t action = actionTable[(state << 8) + c];
		state = transitionTable[(state << 8) + c];
		if (state == 0)
			break;
//		std::cout << "State: " << state << " Action: " << action << '\n';
		Action(action);
	}
	{
		const size_t action = actionTable[state << 8];
		Action(action);
		state = transitionTable[state << 8];
//	std::cout << "State: " << state << " Action: " << action << '\n';
		if (state != Unit::stateOfValidUnit) {
			throw std::runtime_error(
					"Unit::ParseString - The string \"" + txt
							+ "\" is not a parsible unit.");
		}
	}
	factor *= std::pow(10.0, (double) powerprefix);
}

bool Unit::IsUnit(std::string txt) {
	size_t state = 1;
	for (unsigned char c : txt) {
		state = transitionTable[(state << 8) + c];
		if (state == 0)
			return false;
	}
	state = transitionTable[(state << 8)];
	return state == Unit::stateOfValidUnit;
}

// The code below is autogenerated. It contains the turbolexer-tables for units
// and the actions to change the variables.

void Unit::Action(const size_t action) {
	switch (action) {
	case 1:
		powerprefix = 18;
		break;
	case 2:
		powerprefix = 9;
		break;
	case 3:
		powerprefix = 6;
		break;
	case 4:
		powerprefix = 15;
		break;
	case 5:
		powerprefix = 30;
		break;
	case 6:
		powerprefix = 27;
		break;
	case 7:
		powerprefix = 12;
		break;
	case 8:
		powerprefix = 24;
		break;
	case 9:
		powerprefix = 21;
		break;
	case 10:
		powerprefix = -18;
		break;
	case 11:
		powerprefix = -2;
		break;
	case 12:
		powerprefix = -1;
		break;
	case 13:
		powerprefix = -15;
		break;
	case 14:
		powerprefix = 2;
		break;
	case 15:
		powerprefix = 3;
		break;
	case 16:
		powerprefix = -3;
		break;
	case 17:
		powerprefix = -9;
		break;
	case 18:
		powerprefix = -12;
		break;
	case 19:
		powerprefix = -30;
		break;
	case 20:
		powerprefix = -27;
		break;
	case 21:
		powerprefix = -6;
		break;
	case 22:
		powerprefix = -24;
		break;
	case 23:
		powerprefix = -21;
		break;
	case 24:
		// Inch
		powerunit[1] = 1;
		factor = 0.0254;
		break;
	case 25:
		// Feet
		powerunit[1] = 1;
		factor = 0.3048;
		break;
	case 26:
		// Ampere
		powerunit[3] = 1;
		break;
	case 27:
		// Coulomb
		powerunit[0] = 1;
		powerunit[3] = 1;
		break;
	case 28:
		// Farad
		powerunit[0] = 4;
		powerunit[1] = -2;
		powerunit[2] = -1;
		powerunit[3] = 2;
		break;
	case 29:
		// Henry
		powerunit[0] = -2;
		powerunit[1] = 2;
		powerunit[2] = 1;
		powerunit[3] = -2;
		break;
	case 30:
		// Joule
		powerunit[0] = -2;
		powerunit[1] = 2;
		powerunit[2] = 1;
		break;
	case 31:
		// Kelvin
		powerunit[4] = 1;
		break;
	case 32:
		// Litre
		powerunit[1] = 3;
		factor = 0.001;
		break;
	case 33:
		// Newton
		powerunit[0] = -2;
		powerunit[1] = 1;
		powerunit[2] = 1;
		break;
	case 34:
		// Siemens
		powerunit[0] = 3;
		powerunit[1] = -2;
		powerunit[2] = -1;
		powerunit[3] = 2;
		break;
	case 35:
		// Tesla
		powerprefix = 0;
		powerunit[0] = -2;
		powerunit[2] = 1;
		powerunit[3] = -1;
		break;
	case 36:
		// Volt
		powerunit[0] = -3;
		powerunit[1] = 2;
		powerunit[2] = 1;
		powerunit[3] = -1;
		break;
	case 37:
		// Watt
		powerunit[0] = -3;
		powerunit[1] = 2;
		powerunit[2] = 1;
		break;
	case 38:
		// Day
		powerprefix = 0;
		powerunit[0] = 1;
		factor = 86400;
		break;
	case 39:
		powerprefix = 1;
		break;
	case 40:
		// Gram
		powerunit[2] = 1;
		factor = 0.001;
		break;
	case 41:
		// Hour
		powerprefix = 0;
		powerunit[0] = 1;
		factor = 3600;
		break;
	case 42:
		// Metre
		powerprefix = 0;
		powerunit[1] = 1;
		break;
	case 43:
		// Second
		powerunit[0] = 1;
		break;
	case 44:
		// Tonne
		powerunit[2] = 1;
		factor = 1000;
		break;
	case 45:
		// Dalton
		powerprefix = 0;
		powerunit[2] = 1;
		factor = 1.6605390402e-27;
		break;
	case 46:
		// Becquerel
		powerunit[0] = -1;
		break;
	case 47:
		// Dalton
		powerunit[2] = 1;
		factor = 1.6605390402e-27;
		break;
	case 48:
		// Tesla
		powerunit[0] = -2;
		powerunit[2] = 1;
		powerunit[3] = -1;
		break;
	case 49:
		// Metre
		powerunit[1] = 1;
		break;
	case 50:
		// Gray
		powerprefix = 0;
		powerunit[0] = -2;
		powerunit[1] = 2;
		break;
	case 51:
		// Hertz
		powerunit[0] = -1;
		break;
	case 52:
		// Pascal
		powerprefix = 0;
		powerunit[0] = -2;
		powerunit[1] = -1;
		powerunit[2] = 1;
		break;
	case 53:
		// Sievert
		powerunit[0] = -2;
		powerunit[1] = 2;
		break;
	case 54:
		// Weber
		powerunit[0] = -2;
		powerunit[1] = 2;
		powerunit[2] = 1;
		powerunit[3] = -1;
		break;
	case 55:
		// Astronomical unit
		powerprefix = 0;
		powerunit[1] = 1;
		factor = 149597870700;
		break;
	case 56:
		// Candela
		powerprefix = 0;
		powerunit[6] = 1;
		break;
	case 57:
		// Electronvolt
		powerunit[0] = -2;
		powerunit[1] = 2;
		powerunit[2] = 1;
		factor = 1.602176634e-19;
		break;
	case 58:
		// Feet
		powerprefix = 0;
		powerunit[1] = 1;
		factor = 0.3048;
		break;
	case 59:
		// Hectare
		powerprefix = 0;
		powerunit[1] = 2;
		factor = 10000;
		break;
	case 60:
		// Lumen
		powerunit[6] = 1;
		break;
	case 61:
		// Lux
		powerunit[1] = -2;
		powerunit[6] = 1;
		break;
	case 62:
		// Parsec
		powerprefix = 0;
		powerunit[1] = 1;
		factor = 96939420213600000.0 / M_PI;
		break;
	case 63:
		// Steradian
		break;
	case 64:
		// Degree
		factor = M_PI / 180.0;
		break;
	case 65:
		// Angstrom
		powerunit[1] = 1;
		factor = 1e-10;
		break;
	case 66:
		// Ohm
		powerunit[0] = -3;
		powerunit[1] = 2;
		powerunit[2] = 1;
		powerunit[3] = -2;
		break;
	case 67:
		// Gray
		powerunit[0] = -2;
		powerunit[1] = 2;
		break;
	case 68:
		// Pascal
		powerunit[0] = -2;
		powerunit[1] = -1;
		powerunit[2] = 1;
		break;
	case 69:
		// Candela
		powerunit[6] = 1;
		break;
	case 70:
		// Parsec
		powerunit[1] = 1;
		factor = 96939420213600000.0 / M_PI;
		break;
	case 71:
		// Bar
		powerunit[0] = -2;
		powerunit[1] = -1;
		powerunit[2] = 1;
		factor = 100000;
		break;
	case 72:
		// Degree
		powerprefix = 0;
		factor = M_PI / 180.0;
		break;
	case 73:
		// Gon
		factor = M_PI / 200.0;
		break;
	case 74:
		// Katal
		powerprefix = 0;
		powerunit[0] = -1;
		powerunit[5] = 1;
		break;
	case 75:
		// Mil
		powerprefix = 0;
		powerunit[1] = 1;
		factor = 2.54e-05;
		break;
	case 76:
		// Minute
		powerprefix = 0;
		powerunit[0] = 1;
		factor = 60;
		break;
	case 77:
		// Mole
		powerprefix = 0;
		powerunit[5] = 1;
		break;
	case 78:
		// Radian
		powerprefix = 0;
		break;
	case 79:
		// Rpm
		powerprefix = 0;
		powerunit[0] = -1;
		factor = 1.0 / 60.0;
		break;
	case 80:
		// Katal
		powerunit[0] = -1;
		powerunit[5] = 1;
		break;
	case 81:
		// Mole
		powerunit[5] = 1;
		break;
	}
}

std::array<Unit::DerivedUnit, Unit::derivedunitcount> Unit::derivedUnits =
		Unit::InitDerivedUnits();

// ----------------------------------------------------------------------------
//  Tables for Turbolexer:

// Note: No constexpr here as this is supposed to compile for C++11 for now and
// std::array is not constexpr yet. The setup of the tables takes place once
// right before the main() function is run.

std::array<size_t, Unit::stateCount * Unit::charWidth> Unit::transitionTable =
		Unit::InitStateTransitionTable();
std::array<size_t, Unit::stateCount * Unit::charWidth> Unit::actionTable =
		Unit::InitActionTable();

std::array<size_t, Unit::stateCount * Unit::charWidth> Unit::InitStateTransitionTable() {
	std::array<size_t, Unit::stateCount * Unit::charWidth> table { };
	for (size_t state = 0; state < Unit::stateCount; ++state) {
		for (uint16_t c = 0; c < Unit::charWidth; ++c) {
			size_t value = 0;
			if (state == 1) {
				if (c == ' ')
					value = 1;
				if (c == 'E' || c == 'M' || (c >= 'Q' && c <= 'R')
						|| (c >= 'Y' && c <= 'Z') || c == 'n' || c == 'q'
						|| (c >= 'y' && c <= 'z'))
					value = 2;
				if (c == 'G')
					value = 3;
				if (c == 'P')
					value = 4;
				if (c == 'k')
					value = 5;
				if (c == 'T')
					value = 6;
				if (c == 'u')
					value = 7;
				if (c == 'h')
					value = 8;
				if (c == 'd')
					value = 9;
				if (c == 'a')
					value = 10;
				if (c == 'c')
					value = 11;
				if (c == 'f')
					value = 12;
				if (c == 'p')
					value = 13;
				if (c == 'r')
					value = 14;
				if (c == 'm')
					value = 15;
				if (c == '\'')
					value = 19;
				if (c == '"')
					value = 22;
				if (c == 'D')
					value = 64;
				if (c == 'i')
					value = 69;
				if (c == 'g')
					value = 79;
				if (c == 's')
					value = 83;
				if (c == 226)
					value = 84;
				if (c == 206)
					value = 91;
				if (c == 195)
					value = 92;
				if (c == 194)
					value = 98;
			}
			if ((state >= 1 && state <= 15)) {
				if (c == 'A')
					value = 16;
				if (c == 'C')
					value = 17;
				if (c == 'V')
					value = 18;
				if (c == 'F')
					value = 20;
				if (c == 'J')
					value = 21;
				if (c == 'N')
					value = 23;
				if (c == 'K')
					value = 24;
				if (c == 'L')
					value = 25;
				if (c == 'b')
					value = 67;
				if (c == 'W')
					value = 78;
				if (c == 'H')
					value = 81;
				if (c == 'S')
					value = 82;
				if (c == 'B')
					value = 89;
				if (c == 'O')
					value = 94;
				if (c == 'l')
					value = 99;
			}
			if ((state >= 2 && state <= 15)) {
				if (c == 'T')
					value = 29;
				if (c == 'g')
					value = 30;
				if (c == 's')
					value = 31;
				if (c == 'P')
					value = 65;
				if (c == 'k')
					value = 66;
				if (c == 'm')
					value = 80;
				if (c == 'G')
					value = 85;
				if (c == 206)
					value = 90;
			}
			if (state == 9) {
				if (c == 'a')
					value = 2;
				if (c == 'e')
					value = 96;
			}
			if (state == 14) {
				if (c == 'a')
					value = 75;
				if (c == 'p')
					value = 87;
			}
			if (state == 15) {
				if (c == 'o')
					value = 73;
				if (c == 'p')
					value = 86;
				if (c == 'i')
					value = 97;
			}
			if (state == 97) {
				if (c == 'l')
					value = 56;
				if (c == 'n')
					value = 57;
			}
			if (state == 98) {
				if (c == 181)
					value = 2;
				if (c == 176)
					value = 45;
			}
			if (state == 99) {
				if (c == 'm')
					value = 42;
				if (c == 'x')
					value = 43;
				if (c == 0)
					value = 63;
			}
			if (c == 0) {
				if ((state >= 6 && state <= 9) || (state >= 15 && state <= 62)
						|| (state >= 77 && state <= 83))
					value = 63;
			}
			if (c == 'a') {
				if (state == 64)
					value = 28;
				if (state == 4)
					value = 34;
				if (state == 8)
					value = 41;
				if (state == 65)
					value = 49;
				if (state == 5)
					value = 70;
				if (state == 66)
					value = 71;
				if (state == 67)
					value = 72;
			}
			if (c == 'c') {
				if ((state >= 86 && state <= 87))
					value = 51;
				if ((state >= 2 && state <= 12) || (state >= 14 && state <= 15))
					value = 76;
				if (state == 13)
					value = 77;
			}
			if (c == 'd') {
				if (state == 11)
					value = 38;
				if ((state >= 76 && state <= 77))
					value = 50;
				if (state == 75)
					value = 59;
			}
			if (c == 'e') {
				if ((state >= 1 && state <= 8) || (state >= 10 && state <= 15))
					value = 95;
			}
			if (c == 'l') {
				if (state == 73)
					value = 58;
				if (state == 74)
					value = 62;
			}
			if (c == 'm') {
				if (state == 88)
					value = 47;
				if (state == 87)
					value = 60;
			}
			if (c == 'n') {
				if (state == 69)
					value = 22;
				if (state == 68)
					value = 54;
			}
			if (c == 'o') {
				if (state == 79)
					value = 68;
				if (state == 80)
					value = 74;
			}
			if (c == 'r') {
				if (state == 83)
					value = 44;
				if (state == 72)
					value = 52;
			}
			if (c == 't') {
				if ((state >= 1 && state <= 11) || (state >= 13 && state <= 15))
					value = 26;
				if (state == 12)
					value = 40;
				if (state == 70)
					value = 55;
				if (state == 71)
					value = 61;
			}
			if (c == 'y') {
				if (state == 3)
					value = 32;
				if (state == 85)
					value = 48;
			}
			if ((state >= 2 && state <= 13) && c == 'p')
				value = 86;
			if (state == 10 && c == 'u')
				value = 37;
			if (state == 78 && c == 'b')
				value = 36;
			if (state == 81 && c == 'z')
				value = 33;
			if (state == 82 && c == 'v')
				value = 35;
			if (state == 84 && c == 132)
				value = 93;
			if (state == 89 && c == 'q')
				value = 27;
			if ((state >= 90 && state <= 91) && c == 169)
				value = 47;
			if (state == 91 && c == 188)
				value = 2;
			if (state == 92 && c == 133)
				value = 46;
			if (state == 93 && c == 171)
				value = 46;
			if (state == 94 && c == 'h')
				value = 88;
			if ((state >= 95 && state <= 96) && c == 'V')
				value = 39;
			if (state == 96 && c == 'g')
				value = 53;
			table[state * Unit::charWidth + c] = value;
		}
	}

	return table;
}

std::array<size_t, Unit::stateCount * Unit::charWidth> Unit::InitActionTable() {
	std::array<size_t, Unit::stateCount * Unit::charWidth> table { };
	for (size_t state = 0; state < Unit::stateCount; ++state) {
		for (uint16_t c = 0; c < Unit::charWidth; ++c) {
			size_t value = 0;
			if (state == 1) {
				if (c == 'E')
					value = 1;
				if (c == 'G')
					value = 2;
				if (c == 'M')
					value = 3;
				if (c == 'P')
					value = 4;
				if (c == 'Q')
					value = 5;
				if (c == 'R')
					value = 6;
				if (c == 'T')
					value = 7;
				if (c == 'Y')
					value = 8;
				if (c == 'Z')
					value = 9;
				if (c == 'a')
					value = 10;
				if (c == 'c')
					value = 11;
				if (c == 'd')
					value = 12;
				if (c == 'f')
					value = 13;
				if (c == 'h')
					value = 14;
				if (c == 'k')
					value = 15;
				if (c == 'm')
					value = 16;
				if (c == 'n')
					value = 17;
				if (c == 'p')
					value = 18;
				if (c == 'q')
					value = 19;
				if (c == 'r')
					value = 20;
				if (c == 'u')
					value = 21;
				if (c == 'y')
					value = 22;
				if (c == 'z')
					value = 23;
			}
			if (state == 9) {
				if (c == 0)
					value = 38;
				if (c == 'a')
					value = 39;
			}
			if (c == 0) {
				if (state == 22)
					value = 24;
				if (state == 19)
					value = 25;
				if (state == 16)
					value = 26;
				if (state == 17)
					value = 27;
				if (state == 20)
					value = 28;
				if (state == 81)
					value = 29;
				if (state == 21)
					value = 30;
				if (state == 24)
					value = 31;
				if (state == 25 || state == 99)
					value = 32;
				if (state == 23)
					value = 33;
				if (state == 82)
					value = 34;
				if (state == 6)
					value = 35;
				if (state == 18)
					value = 36;
				if (state == 78)
					value = 37;
				if (state == 30 || state == 79)
					value = 40;
				if (state == 8)
					value = 41;
				if (state == 15)
					value = 42;
				if (state == 31 || state == 83)
					value = 43;
				if (state == 26)
					value = 44;
				if (state == 7)
					value = 45;
				if (state == 27)
					value = 46;
				if (state == 28)
					value = 47;
				if (state == 29)
					value = 48;
				if (state == 80)
					value = 49;
				if (state == 32)
					value = 50;
				if (state == 33)
					value = 51;
				if (state == 34)
					value = 52;
				if (state == 35)
					value = 53;
				if (state == 36)
					value = 54;
				if (state == 37)
					value = 55;
				if (state == 38)
					value = 56;
				if (state == 39)
					value = 57;
				if (state == 40)
					value = 58;
				if (state == 41)
					value = 59;
				if (state == 42)
					value = 60;
				if (state == 43)
					value = 61;
				if (state == 77)
					value = 62;
				if (state == 44)
					value = 63;
				if (state == 45)
					value = 64;
				if (state == 46)
					value = 65;
				if (state == 47)
					value = 66;
				if (state == 48)
					value = 67;
				if (state == 49)
					value = 68;
				if (state == 50)
					value = 69;
				if (state == 51)
					value = 70;
				if (state == 52)
					value = 71;
				if (state == 53)
					value = 72;
				if (state == 54)
					value = 73;
				if (state == 55)
					value = 74;
				if (state == 56)
					value = 75;
				if (state == 57)
					value = 76;
				if (state == 58)
					value = 77;
				if (state == 59)
					value = 78;
				if (state == 60)
					value = 79;
				if (state == 61)
					value = 80;
				if (state == 62)
					value = 81;
			}
			if (state == 91 && c == 188)
				value = 21;
			if (state == 98 && c == 181)
				value = 21;

			table[state * Unit::charWidth + c] = value;
		}
	}

	return table;

}

std::array<Unit::DerivedUnit, Unit::derivedunitcount> Unit::InitDerivedUnits() {
	std::array<DerivedUnit, Unit::derivedunitcount> table;
	table[0].name = "F";
	table[0].exp = { { 4, -2, -1, 2, 0, 0, 0 } };
	table[1].name = "Ω";
	table[1].exp = { { -3, 2, 1, -2, 0, 0, 0 } };
	table[2].name = "S";
	table[2].exp = { { 3, -2, -1, 2, 0, 0, 0 } };
	table[3].name = "V";
	table[3].exp = { { -3, 2, 1, -1, 0, 0, 0 } };
	table[4].name = "H";
	table[4].exp = { { -2, 2, 1, -2, 0, 0, 0 } };
	table[5].name = "W";
	table[5].exp = { { -3, 2, 1, 0, 0, 0, 0 } };
	table[6].name = "Wb";
	table[6].exp = { { -2, 2, 1, -1, 0, 0, 0 } };
	table[7].name = "J";
	table[7].exp = { { -2, 2, 1, 0, 0, 0, 0 } };
	table[8].name = "N";
	table[8].exp = { { -2, 1, 1, 0, 0, 0, 0 } };
	table[9].name = "Pa";
	table[9].exp = { { -2, -1, 1, 0, 0, 0, 0 } };
	table[10].name = "T";
	table[10].exp = { { -2, 0, 1, -1, 0, 0, 0 } };
	table[11].name = "Gy";
	table[11].exp = { { -2, 2, 0, 0, 0, 0, 0 } };
	table[12].name = "lx";
	table[12].exp = { { 0, -2, 0, 0, 0, 0, 1 } };
	table[13].name = "C";
	table[13].exp = { { 1, 0, 0, 1, 0, 0, 0 } };
	table[14].name = "kat";
	table[14].exp = { { -1, 0, 0, 0, 0, 1, 0 } };
	table[15].name = "s";
	table[15].exp = { { 1, 0, 0, 0, 0, 0, 0 } };
	table[16].name = "m";
	table[16].exp = { { 0, 1, 0, 0, 0, 0, 0 } };
	table[17].name = "g"; // This is actually kg, but the correct prefixes have to be sorted out elsewhere.
	table[17].exp = { { 0, 0, 1, 0, 0, 0, 0 } };
	table[18].name = "A";
	table[18].exp = { { 0, 0, 0, 1, 0, 0, 0 } };
	table[19].name = "K";
	table[19].exp = { { 0, 0, 0, 0, 1, 0, 0 } };
	table[20].name = "mol";
	table[20].exp = { { 0, 0, 0, 0, 0, 1, 0 } };
	table[21].name = "cd";
	table[21].exp = { { 0, 0, 0, 0, 0, 0, 1 } };
	table[22].name = "Hz";
	table[22].exp = { { -1, 0, 0, 0, 0, 0, 0 } };
	return table;
}

