///////////////////////////////////////////////////////////////////////////////
// Name               : OpenGLPick.h
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

#ifndef OPENGLPICK_H
#define OPENGLPICK_H

/*!\class OpenGLPick
 * \brief Storage for the result of an OpenGL picking operation
 * \ingroup View3D
 *
 * The result is stored as it is delivered from the OpenGL call.
 * Functions are provided to interpret the results.
 *
 * This class provides a buffer, that is filled by the OnPick function of the OpenGLCanvas.
 *
 * In the main code this is called by using the associated OpenGLCanvas and passing an object of this
 * class to the OnPick function.
 *
 * \code
 * void FrameMain::On3DSelect(wxMouseEvent& event)
 * {
 *   int x = event.GetX();
 *   int y = event.GetY();
 *   OpenGLPick result;
 *   m_canvas->OnPick(result, x, y);
 *   if(result.HasHits()){
 *     result.SortByNear();
 *     if(result.Get(0, 0) == 1){
 *       <...>
 *     }
 *   }
 * }
 * \endcode
 *
 * \note When extending this class:\n
 * The internal buffer is organized as a list of GLuint. Each record consists of\n
 *   [Number of levels N], [Near value], [Far value], [Name0], ..., [NameN]\n
 * Thus each record can have a different size. All of this is hidden, when using the
 * interface in this class.
 */

#include <cstddef>
#include <string>
#include <vector>

#include "OpenGL.h"

class OpenGLPick {
	friend class OpenGLCanvas;
public:
	explicit OpenGLPick(size_t bufferSize = 512);

protected:
//	bool SetBufferSize(GLsizei newSize); //!< Initialize the pick buffer

	GLsizei GetBufferSize() const; //!< OpenGLCanvas requests the size of the buffer
	GLuint* GetBuffer(); //!< OpenGLCanvas requests the pointer to the buffer
	void SetHits(GLuint hits); //!< OpenGLCanvas tells this class the number of hits found

public:
	/**\brief Sort the results by the Near value
	 *
	 * Sorts the values in the result buffer by the near value, so that the
	 * closests hit are first.
	 */
	void SortByNear();

	/**\brief Check if something was hit
	 *
	 * Are there any hits for the checked position at all?
	 *
	 * \return bool
	 */
	bool HasHits() const;

	/**\brief Number of recorded hits
	 *
	 * \return Count
	 */
	size_t HitCount() const;

	/**\brief Number of IDs a hit has
	 *
	 * As each object hit, can have a different number of IDs assigned, this
	 * is returned from this function.
	 *
	 * If an hitIdx outside of the results, a 0 is returned.
	 *
	 * \param idxHit Hit to check.
	 * \return Number of IDs.
	 */
	size_t IDCount(size_t idxHit) const;

	/**\brief Get the IDs on the namestack.
	 *
	 * \return the Value
	 */
	GLuint Get(size_t idxHit, size_t idxID);
	GLuint Near(size_t idxHit); //!< Return the Near value of the hit
	GLuint Far(size_t idxHit); //!< Return the Far value of the hit

//	/**\brief Find the nearest result with level0 == L0.
//	 *
//	 * Returns -1, if there is nothing found.
//	 */
//	size_t NearestWithLevel0(size_t idxHit);
//	size_t NearestWithLevel01(size_t idxHit, size_t idxID); //!< Find the nearest result with Name0 == L0 and Name1 = L1. Returns -1, if there is nothing found.

	std::string ToString() const;

private:
	void MoveBufferPos(size_t idxHit) const; //!< Move the pointer in the result buffer.

private:
	std::vector<GLuint> buffer;
	std::vector<GLuint> sort;
	bool bufferAssigned = false;
	size_t results = 0;
	mutable size_t pos = 0;
	mutable size_t hitpos = 0;

};

#endif /* OPENGLPICK_H */

