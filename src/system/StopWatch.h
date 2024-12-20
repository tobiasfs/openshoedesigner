///////////////////////////////////////////////////////////////////////////////
// Name               : StopWatch.h
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

#ifndef SYSTEM_STOPWATCH_H
#define SYSTEM_STOPWATCH_H

/*!\class StopWatch
 * \brief Universal stopwatch
 *
 * Return the duration between object construction or Start() and Stop().
 *
 * The durations can be queried with GetSecondsCPU() and GetSecondsWall().
 *
 * For multiple Start(), the last one counts. (If the stopwatch is constructed
 * and later the function Start() is called, time is counted from the call to
 * that function on.
 *
 * Stop() updated the internal result variables. multiple stops without a start
 * in between only update the result variables, but the stopwatch keeps on
 * counting.
 *
 * Reset() and Restart() both reset the internal result variables to zero and
 * restart the timekeeping process.
 *
 * Supports C++98 and C++11.
 */

#include <time.h>
#if __cplusplus >= 201103L
#include <chrono>
#endif

class StopWatch {
public:
	StopWatch();
	virtual ~StopWatch() = default;

	void Start(); //!< Start the counter
	void Restart(); //!< Synonymous to Reset().
	void Stop(); //!< Stop the counter and add to result.
	void Reset(); //!< Reset the accumulated time.

	double GetSecondsCPU() const;
	double GetSecondsWall() const;

private:
	clock_t t0CPU = 0;
	double deltaCPU = 0.0;
	double deltaWall = 0.0;

#if __cplusplus >= 201103L
	std::chrono::steady_clock::time_point t0Wall;
#else
	time_t t0Wall;
#endif
};

#endif /* SYSTEM_STOPWATCH_H */
