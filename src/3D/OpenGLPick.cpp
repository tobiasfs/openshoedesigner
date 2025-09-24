///////////////////////////////////////////////////////////////////////////////
// Name               : OpenGLPick.cpp
// Purpose            : Storage and interpretaton for OpenGL picking results
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 18.01.2015
// Copyright          : (C) 2015 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#include "OpenGLPick.h"

#include <sstream>
#include <stdexcept>

OpenGLPick::OpenGLPick(size_t bufferSize) {
	buffer.resize(bufferSize);
	sort.resize(bufferSize);
}

//bool OpenGLPick::SetBufferSize(GLsizei newSize) {
//	// Do not change or deallocate the buffer, while it is assigned to
//	// an OpenGL context.
//	// Probably this problem will never arise. But if it happens, it would
//	// be a major headache to debug.
//	// (buffer assigned to OGL -> buffer delete[] -> OGL tries to write -> SEG_FAULT)
//	// The bufferAssigned flag is used to mitigate the problem.
//	if (bufferAssigned)
//		return false;
//
//	// Create new buffer and copy the old content.
//	GLuint * temp = new GLuint[newSize];
//
//	size_t m = bufferSize;
//	if (newSize < m)
//		m = newSize;
//	for (size_t n = 0; n < m; n++)
//		temp[n] = buffer[n];
//	if (buffer != nullptr)
//		delete[] buffer;
//	buffer = temp;
//
//	if (sort != nullptr)
//		delete[] sort;
//	sort = new GLuint[newSize];
//
//	bufferSize = newSize;
//	pos = 0;
//	hitpos = 0;
//	return true;
//}

GLsizei OpenGLPick::GetBufferSize() const {
	return (GLsizei) buffer.size();
}

GLuint* OpenGLPick::GetBuffer() {
	// If the pointer to the buffer is requested, it is most likely, that
	// the buffer is about to be assigned to an OpenGL context.
	bufferAssigned = true;
	return buffer.data();
}

void OpenGLPick::SetHits(GLuint hits) {
	// If the hits have been requested from the OpenGL context and entered into
	// this function, the buffer is most likely not assigned to the OpenGL context
	// anymore.
	bufferAssigned = false;
	this->results = (size_t) hits;
	pos = 0;
	hitpos = 0;
}

void OpenGLPick::SortByNear() {
	if (results < 2)
		return;
	bool sorted = false;

	// Bubble sort
	while (!sorted) {
		sorted = true;
		size_t p1 = 0;
		for (size_t r = 0; r < results - 1; r++) {
			if (r > 0)
				p1 += buffer[p1] + 3;
			const size_t p2 = p1 + buffer[p1] + 3;

			const GLuint near1 = buffer[p1 + 1];
			const GLuint near2 = buffer[p2 + 1];
			if (near1 > near2) {

				// Swapping the results
				const size_t N1 = buffer[p1] + 3; // Number of uints in result m
				const size_t N2 = buffer[p2] + 3; // Number of uints in result m+1
				// Copy everything to the sort buffer
				for (size_t n = 0; n < (N1 + N2); n++)
					sort[n] = buffer[p1 + n];
				// Pointer to keep track of position
				size_t p = p1;
				// Copy back result m+1
				for (size_t n = 0; n < N2; n++)
					buffer[p++] = sort[N1 + n];
				// Copy back result m
				for (size_t n = 0; n < N1; n++)
					buffer[p++] = sort[n];

				sorted = false;
			}
		}
		// Here a backwards pass could be written, but the results have varying
		// sizes, so is would result in a very complicated iteration scheme.
	}
	pos = 0;
	hitpos = 0;
}

bool OpenGLPick::HasHits() const {
	return results > 0;
}

size_t OpenGLPick::HitCount() const {
	return results;
}

size_t OpenGLPick::IDCount(size_t hitIdx) const {
	if (hitIdx > results)
		return (size_t) 0;
	MoveBufferPos(hitIdx);
	return (size_t) buffer[pos];
}

GLuint OpenGLPick::Get(size_t idxHit, size_t idxLevel) {
	if (idxHit >= results)
		throw std::logic_error(
				"OpenGLPick::Get - The selected idxHit is outside of the available results.");
	MoveBufferPos(idxHit);
	GLuint N = buffer[pos];
	if (idxLevel >= N)
		throw std::logic_error(
				"OpenGLPick::Get - The selected idxLevel is outside of the available namestack for the selected hit.");
	return buffer[pos + 3 + idxLevel];
}

GLuint OpenGLPick::Near(size_t idxHit) {
	if (idxHit >= results)
		throw std::logic_error(
				"OpenGLPick::Near - The selected idxHit is outside of the available results.");
	MoveBufferPos(idxHit);
	return buffer[pos + 1];
}

GLuint OpenGLPick::Far(size_t idxHit) {
	if (idxHit >= results)
		throw std::logic_error(
				"OpenGLPick::Far - The selected idxHit is outside of the available results.");
	MoveBufferPos(idxHit);
	return buffer[pos + 2];
}

std::string OpenGLPick::ToString() const {
	std::stringstream out;

	for (size_t idxHit = 0; idxHit < results; idxHit++) {
		MoveBufferPos(idxHit);
		out << " " << idxHit << " -> ";
		const size_t IDs = buffer[pos];
		out << "Near: " << buffer[pos + 1];
		out << " Far: " << buffer[pos + 2];
		out << " [";
		for (size_t ID = 0; ID < IDs; ID++) {
			if (ID > 0)
				out << ", ";
			out << buffer[pos + ID + 3];
		}
		out << "]\n";
	}
	if (results == 0)
		out << "No hits.\n";
	return out.str();
}

//size_t OpenGLPick::NearestWithLevel0(size_t L0) {
//	size_t temp = 0;
//	for (size_t n = 0; n < results; n++) {
//		MoveBufferPos(n);
//		GLuint N = buffer[pos];
//		if (N < 1)
//			continue;
//		if (buffer[pos + 3] != L0)
//			continue;
//		temp = n;
//		break;
//	}
//	return temp;
//}
//
//int OpenGLPick::NearestWithLevel01(unsigned int L0, unsigned int L1) {
//	int temp = -1;
//	for (unsigned int n = 0; n < results; n++) {
//		MoveBufferPos(n);
//		GLuint N = buffer[pos];
//		if (N < 2)
//			continue;
//		if (buffer[pos + 3] != L0 || buffer[pos + 4] != L1)
//			continue;
//		temp = n;
//		break;
//	}
//	return temp;
//}

void OpenGLPick::MoveBufferPos(size_t idxHit) const {
	if (idxHit >= results)
		throw std::logic_error(
				"OpenGLPick::MoveBufferPos - The new index of the bufferpos is outside the hit buffer.");
	if (idxHit < hitpos) {
		// Rewind
		pos = 0;
		hitpos = 0;
	}
	while (hitpos != idxHit && pos < buffer.size()) {
		pos += buffer[pos] + 3;
		hitpos++;
	}
	if (pos >= buffer.size())
		throw std::logic_error(
				"OpenGLPick::MoveBufferPos - Inconsistent buffer returned from OpenGL.");

}
