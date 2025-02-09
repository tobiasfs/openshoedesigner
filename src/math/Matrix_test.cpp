///////////////////////////////////////////////////////////////////////////////
// Name               : Matrix_test.h
// Purpose            : Unit-tests for Matrix class
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 18.10.2023
// Copyright          : (C) 2023 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#ifdef USE_CPPUNIT

#include "Matrix.h"

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <string>
#include <vector>

class MatrixTest: public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE( MatrixTest );
	CPPUNIT_TEST(testDimensions);
	CPPUNIT_TEST(testDimensionSimplification);
	CPPUNIT_TEST(testTranspose2D);
	CPPUNIT_TEST(testTranspose3D);
	CPPUNIT_TEST_SUITE_END();
public:
	void testDimensions() {
		Matrix m(3, 5, 7);

		CPPUNIT_ASSERT_EQUAL((size_t ) 3, m.Size(0));
		CPPUNIT_ASSERT_EQUAL((size_t ) 5, m.Size(1));
		CPPUNIT_ASSERT_EQUAL((size_t ) 7, m.Size(2));
		CPPUNIT_ASSERT_EQUAL((size_t ) 3 * 5 * 7, m.Numel());
		m.Transpose(); // Switch first two dimensions
		CPPUNIT_ASSERT_EQUAL((size_t ) 5, m.Size(0));
		CPPUNIT_ASSERT_EQUAL((size_t ) 3, m.Size(1));
		CPPUNIT_ASSERT_EQUAL((size_t ) 7, m.Size(2));
		CPPUNIT_ASSERT_EQUAL((size_t ) 3 * 5 * 7, m.Numel());
	}

	void testDimensionSimplification() {
		{
			Matrix m(3, 5, 7, 1);
			auto S = m.GetDimensions();
			CPPUNIT_ASSERT_EQUAL((size_t ) 3, S.size());
			CPPUNIT_ASSERT_EQUAL((size_t ) 3, S[0]);
			CPPUNIT_ASSERT_EQUAL((size_t ) 5, S[1]);
			CPPUNIT_ASSERT_EQUAL((size_t ) 7, S[2]);
		}
		{
			Matrix m(5, 1, 7, 1);
			{
				auto S = m.GetDimensions();
				CPPUNIT_ASSERT_EQUAL((size_t ) 3, S.size());
				CPPUNIT_ASSERT_EQUAL((size_t ) 5, S[0]);
				CPPUNIT_ASSERT_EQUAL((size_t ) 1, S[1]);
				CPPUNIT_ASSERT_EQUAL((size_t ) 7, S[2]);
			}
			m.Squeeze();
			{
				auto S = m.GetDimensions();
				CPPUNIT_ASSERT_EQUAL((size_t ) 2, S.size());
				CPPUNIT_ASSERT_EQUAL((size_t ) 5, S[0]);
				CPPUNIT_ASSERT_EQUAL((size_t ) 7, S[1]);
			}
		}
		{
			Matrix m(1, 1, 1, 1);
			auto S = m.GetDimensions();
			CPPUNIT_ASSERT_EQUAL((size_t ) 1, S.size());
			CPPUNIT_ASSERT_EQUAL((size_t ) 1, S[0]);
			auto D = m.Size();
			CPPUNIT_ASSERT_EQUAL((size_t ) 4, D.size());
			CPPUNIT_ASSERT_EQUAL((size_t ) 1, D[0]);
			CPPUNIT_ASSERT_EQUAL((size_t ) 1, D[1]);
			CPPUNIT_ASSERT_EQUAL((size_t ) 1, D[2]);
			CPPUNIT_ASSERT_EQUAL((size_t ) 1, D[3]);
		}
		{
			Matrix m(1, 3, 1, 7);
			auto DM = m.GetMinDimensions();
			CPPUNIT_ASSERT_EQUAL((size_t ) 2, DM.size());
			CPPUNIT_ASSERT_EQUAL((size_t ) 3, DM[0]);
			CPPUNIT_ASSERT_EQUAL((size_t ) 7, DM[1]);
		}

	}

	void testTranspose2D() {
		Matrix m(2, 3);
		std::vector<double> v = { 1, 2, 3, 4, 5, 6 };

		m.Insert(v.data(), 6);

		CPPUNIT_ASSERT_EQUAL(1.0, m(0, 0));
		CPPUNIT_ASSERT_EQUAL(2.0, m(1, 0));
		CPPUNIT_ASSERT_EQUAL(3.0, m(0, 1));
		CPPUNIT_ASSERT_EQUAL(4.0, m(1, 1));
		CPPUNIT_ASSERT_EQUAL(5.0, m(0, 2));
		CPPUNIT_ASSERT_EQUAL(6.0, m(1, 2));

		m.Transpose();

		CPPUNIT_ASSERT_EQUAL(1.0, m(0, 0));
		CPPUNIT_ASSERT_EQUAL(2.0, m(0, 1));
		CPPUNIT_ASSERT_EQUAL(3.0, m(1, 0));
		CPPUNIT_ASSERT_EQUAL(4.0, m(1, 1));
		CPPUNIT_ASSERT_EQUAL(5.0, m(2, 0));
		CPPUNIT_ASSERT_EQUAL(6.0, m(2, 1));

	}

	void testTranspose3D() {
		Matrix m(2, 3, 4);
		for (size_t i = 1; i <= 24; ++i)
			m.Insert((double) i);
		CPPUNIT_ASSERT(m.IsFilled());

		CPPUNIT_ASSERT_EQUAL(1.0, m(0, 0));
		CPPUNIT_ASSERT_EQUAL(1.0, m(0, 0, 0));
		CPPUNIT_ASSERT_EQUAL(1.0, m(0, 0, 0, 0));

		CPPUNIT_ASSERT_EQUAL(2.0, m(1, 0));
		CPPUNIT_ASSERT_EQUAL(3.0, m(0, 1));
		CPPUNIT_ASSERT_EQUAL(3.0, m(0, 1, 0));

		CPPUNIT_ASSERT_EQUAL(7.0, m(0, 0, 1));
		CPPUNIT_ASSERT_EQUAL(8.0, m(1, 0, 1));
		CPPUNIT_ASSERT_EQUAL(9.0, m(0, 1, 1));

		CPPUNIT_ASSERT_EQUAL(13.0, m(0, 0, 2));
		CPPUNIT_ASSERT_EQUAL(15.0, m(0, 1, 2));
		CPPUNIT_ASSERT_EQUAL(24.0, m(1, 2, 3));

		m.Transpose();

		CPPUNIT_ASSERT_EQUAL((size_t )3, m.Size(0));
		CPPUNIT_ASSERT_EQUAL((size_t )2, m.Size(1));
		CPPUNIT_ASSERT_EQUAL((size_t )4, m.Size(2));

		CPPUNIT_ASSERT_EQUAL(1.0, m(0, 0));
		CPPUNIT_ASSERT_EQUAL(2.0, m(0, 1));
		CPPUNIT_ASSERT_EQUAL(7.0, m(0, 0, 1));

		CPPUNIT_ASSERT_EQUAL(7.0, m(0, 0, 1));
		CPPUNIT_ASSERT_EQUAL(9.0, m(1, 0, 1));
		CPPUNIT_ASSERT_EQUAL(8.0, m(0, 1, 1));

		CPPUNIT_ASSERT_EQUAL(3.0, m(1, 0));
		CPPUNIT_ASSERT_EQUAL(4.0, m(1, 1));
		CPPUNIT_ASSERT_EQUAL(5.0, m(2, 0));
		CPPUNIT_ASSERT_EQUAL(6.0, m(2, 1));

		CPPUNIT_ASSERT_EQUAL(13.0, m(0, 0, 2));
		CPPUNIT_ASSERT_EQUAL(15.0, m(1, 0, 2));
		CPPUNIT_ASSERT_EQUAL(24.0, m(2, 1, 3));

		CPPUNIT_ASSERT_EQUAL(14.0, m(0, 1, 2));

	}

};

CPPUNIT_TEST_SUITE_REGISTRATION(MatrixTest);
#endif
