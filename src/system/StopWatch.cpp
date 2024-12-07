///////////////////////////////////////////////////////////////////////////////
// Name               : StopWatch.cpp
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 07.05.2017
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

#include "StopWatch.h"

StopWatch::StopWatch() {
	deltaCPU = 0.0;
	deltaWall = 0.0;
	t0CPU = clock();
#if __cplusplus >= 201103L
	t0Wall = std::chrono::steady_clock::now();
#else
	t0Wall = time(NULL);
#endif
}

StopWatch::~StopWatch() {
}

void StopWatch::Reset() {
	deltaCPU = 0.0;
	deltaWall = 0.0;
	t0CPU = clock();
#if __cplusplus >= 201103L
	t0Wall = std::chrono::steady_clock::now();
#else
	t0Wall = time(NULL);
#endif
}

void StopWatch::Restart() {
	deltaCPU = 0.0;
	deltaWall = 0.0;
	t0CPU = clock();
#if __cplusplus >= 201103L
	t0Wall = std::chrono::steady_clock::now();
#else
	t0Wall = time(NULL);
#endif
}

void StopWatch::Start() {
	t0CPU = clock();
#if __cplusplus >= 201103L
	t0Wall = std::chrono::steady_clock::now();
#else
	t0Wall = time(NULL);
#endif
}

void StopWatch::Stop() {
	deltaCPU += ((double) (clock() - t0CPU)) / CLOCKS_PER_SEC;
#if __cplusplus >= 201103L
	deltaWall += (std::chrono::duration_cast<std::chrono::duration<double>>(
			std::chrono::steady_clock::now() - t0Wall)).count();
	t0Wall = std::chrono::steady_clock::now();
#else
	deltaWall += ((double) difftime(time(NULL), t0Wall));
	t0Wall = time(NULL);
#endif
	t0CPU = clock();
}

double StopWatch::GetSecondsCPU() const {
	return deltaCPU;
}

double StopWatch::GetSecondsWall() const {
	return deltaWall;
}
