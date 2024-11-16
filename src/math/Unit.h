///////////////////////////////////////////////////////////////////////////////
// Name               : Unit.h
// Purpose            : Stores a physical unit
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

#ifndef MATH_UNIT_H
#define MATH_UNIT_H

/*!\class Unit
 * \brief Stores a physical unit
 *
 * This class stores an unit and the conversion from SI to that unit and back.
 *
 * The base units are (official sorting):
 *  * Second
 *  * Meter
 *  * Kilogram
 *  * Ampere
 *  * Kelvin
 *  * Mole
 *  * Candela
 *
 * Shifted units like °C or °F for temperatures are not taken into
 * consideration. This class only works with units, that have the same zero
 * point as the SI units (= virtually all others).
 *
 * The prefixes used are:
 *
 *  Name: | Power:
 *  ------|--------
 *    y   | -24
 *    z   | -21
 *    a   | -18
 *    f   | -15
 *    p   | -12
 *    n   | -9
 *    μ   | -6  (Unicode/UTF-8: 206, 188)
 *    µ   | -6  (Unicode/UTF-8: 194, 181)
 *    u   | -6  (The small letter u is used as a stand-in)
 *    m   | -3
 *    c   | -2
 *    d   | -1
 *    da  | 1
 *    h   | 2
 *    k   | 3
 *    M   | 6
 *    G   | 9
 *    T   | 12
 *    P   | 15
 *    E   | 18
 *    Z   | 21
 *    Y   | 24
 *
 * The micro prefix 'mu' is represented by three different characters.
 * The character on the keyboard [AltGr]+[M], the character used in
 * Wikipedia and the stand-in of a simple lowercase 'u'.
 *
 *
 * The factor is the multiplier to convert into the other unit.
 * E.g.:
 *  -  1 cm = 0.01 m        --> factor = 1/100
 *  -  1 rpm = 1/60 1/s     --> factor = 1/60
 *  -  1 mm = 1e-6 m        --> factor = 1e-6
 *  -  1 g  = 1e-3 kg       --> factor = 1e-3
 *  -  1 deg = M_PI/180 rad --> factor = M_PI/180
 *
 * The available types for the units are maintained in CollectionUnits.
 *
 * The used types are set in DialogSetupUnits.
 */

#include <array>
#include <cstdint>
#include <iostream>
#include <string>

class Unit {
public:
	enum class Base : uint_fast8_t {
		s = 0, m = 1, kg = 2, A = 3, K = 4, mol = 5, cd = 6
	};

	Unit() = default;
	explicit Unit(int s, int m = 0, int kg = 0, int A = 0, int K = 0, int mol = 0,
			int cd = 0);
	explicit Unit(const std::string &otherName);
	Unit(const std::string &otherName, const double factor,
			const std::string &SIName);

	/**\brief Templated constructors
	 *
	 * To pass an arbitrary number of information on the unit, templates are
	 * used.
	 *
	 * Thus making it possible to write something like:
	 *  * Unit distance("m", 1, Unit::Base::m, 1);
	 *  * Unit resistance("Ohm", 1, Unit::Base::A, -1, Unit::Base::V, 1);
	 *
	 * \param otherName The none SI name of the unit. Can of course be an SI
	 * 	name as well.
	 * \param factor Conversion factor from the other unit to the SI unit.
	 * \param idx The index of the unit in the unit vector. Here a value of the
	 *  enumeration Base is expected.
	 * \param power The exponent of that particular base unit.
	 * \param rest The rest of the input to the variadic constructor template.
	 */
	template<typename ... Types>
	Unit(const std::string &otherName, const double factor, Base idx, int power,
			Types ... rest) :
			otherName(otherName), factor(factor) {
		powerunit[(size_t) idx] = power;
		Set(rest...);
	}

	template<typename ... Types>
	void Set(Base idx, int power, Types ... rest) {
		powerunit[(size_t) idx] = power;
		Set(rest...);
	}
	void Set() {
		SIName = ToString();
	}

	int Get(Base idx) const;

	std::string GetOtherName() const;
	std::string GetSIName() const;

	double FromSI(const double value) const;
	double ToSI(const double value) const;

	std::string TextFromSI(const double value, int digitsAfterComma = -1) const;
	std::string TextFromSIWithUnit(const double value,
			int digitsAfterComma = -1) const;

	void Power(const double exponent);

	Unit& operator*=(const Unit &rhs);
	const Unit operator*(const Unit &rhs) const;

	Unit& operator/=(const Unit &rhs);
	const Unit operator/(const Unit &rhs) const;

	bool operator==(const Unit &rhs) const;
	bool operator!=(const Unit &rhs) const;

	bool operator==(const std::string &rhs) const;
	bool operator!=(const std::string &rhs) const;

	bool NoUnit() const; ///< Check if the unit is empty. The conversion factor can differ from 1.0, but the base-unit-powers have to be 0.
	static bool IsUnit(std::string txt); ///< Check a string, if it can be parsed as a unit.

	/**\brief Generate a unit string conforming to the Modelica requirements
	 *
	 * In the Modelica manual chapter 19 - "Unit Expressions" the way Modelica
	 * uses units is described.
	 *
	 * Its description of a unit is more compact as some details are left out.
	 *
	 * Examples:
	 *  * N.m
	 *  * kg.m/s2
	 *  * kg.m.s-2
	 *  * m/s
	 *  * J/(kg.K)
	 *  * J.kg-1.K-1
	 *
	 */
	std::string ToString(bool useNegativeExponents = false, bool modelicastyle =
			false) const;

	friend std::ostream& operator<<(std::ostream &out, const Unit &unit);

private:
	std::string SIName; ///< The name in SI: m, m/s, 1/s, ...
	std::string otherName; ///< The name in the "other" system: h, cm, dm, rpm, inch, mil, mm, ...
	double factor = 1.0; ///< SI-value / factor = outside-value

	int powerprefix = 0; ///< Power for the units (normally a multiple of 3)

	static const size_t unitcount = 7; // Number of base units, might be increased, if units are extended by for example an base-unit of currency
	std::array<int, unitcount> powerunit = { { 0, 0, 0, 0, 0, 0, 0 } };

	class DerivedUnit {
	public:
		DerivedUnit() = default;
		DerivedUnit(const std::string &name,
				const std::array<int, unitcount> &exp) :
				name(name), exp(exp) {
		}
		int Compare(const std::array<int, unitcount> &rhs) const;
		int Diff(const std::array<int, unitcount> &rhs, int factor) const;
		std::string name;
		std::array<int, unitcount> exp;
	};
	static const size_t derivedunitcount = 24;
	static std::array<DerivedUnit, derivedunitcount> InitDerivedUnits();
	static std::array<DerivedUnit, derivedunitcount> derivedUnits;

	void ParseString(const std::string &txt); ///< Turbolexer to parse-in a string with a prefix and an unit.
	void Action(const size_t action); ///< Action that manipulates the variables in this class. Controlled by the Lexers state-machine.

	std::string code;
	size_t pos = 0;
	size_t row = 0;
	size_t col = 0;
//	double value = 0.0;
	std::string id;

	// Lexertables
	static const size_t stateCount = 100;
	static const size_t stateOfValidUnit = 63;
	static const size_t charWidth = 256; // Normal 8-bit byte

	static std::array<size_t, stateCount * charWidth> InitStateTransitionTable();
	static std::array<size_t, stateCount * charWidth> InitActionTable();

	static std::array<size_t, stateCount * charWidth> transitionTable;
	static std::array<size_t, stateCount * charWidth> actionTable;

};

#endif /* MATH_UNIT_H */

