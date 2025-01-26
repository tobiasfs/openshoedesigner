///////////////////////////////////////////////////////////////////////////////
// Name               : SemaphoreTryLocker.h
// Purpose            : 
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 16.12.2024
// Copyright          : (C) 2024 Tobias Schaefer <tobiassch@users.sourceforge.net>
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
#ifndef SEMAPHORETRYLOCKER_H_
#define SEMAPHORETRYLOCKER_H_

/** \class SemaphoreTryLocker
 * 	\code #include "SemaphoreTryLocker.h"\endcode
 *  \brief Non-blocking semaphore locker
 *
 * Modeled after the wxMutexLocker from the wxWidgets thread header.
 */

#include <wx/thread.h>

class SemaphoreTryLocker {
public:
	SemaphoreTryLocker() = delete;
	SemaphoreTryLocker(wxSemaphore &semaphore_) :
			semaphore(semaphore_), isOK(false) {
		isOK = (semaphore_.TryWait() == wxSEMA_NO_ERROR);
	}
	virtual ~SemaphoreTryLocker() {
		if (isOK)
			semaphore.Post();
	}
	SemaphoreTryLocker(const SemaphoreTryLocker &other) = delete;
	SemaphoreTryLocker& operator=(const SemaphoreTryLocker &other) = delete;

	bool IsOK() const {
		return isOK;
	}
	void UnLock() {
		semaphore.Post();
		isOK = false;
	}
private:
	wxSemaphore &semaphore;
	bool isOK = false;
};

#endif /* SEMAPHORETRYLOCKER_H_ */
