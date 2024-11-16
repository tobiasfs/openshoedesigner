/*
 * SVD_test.cpp
 *
 *  Created on: 29.11.2023
 *      Author: toby
 */

#ifdef USE_CPPUNIT

#include "SVD.h"

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "../system/StopWatch.h"
#include "MatlabFile.h"

#include <random>
#include <string>
#include <vector>

class SVDTest: public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE( SVDTest );
	CPPUNIT_TEST(testBroken);
	CPPUNIT_TEST(testMissing);
	CPPUNIT_TEST(testEmpty);
	CPPUNIT_TEST(testSparse);
	CPPUNIT_TEST(testSpeed);
	CPPUNIT_TEST(testTransform);
	CPPUNIT_TEST(testInterpolation);
	CPPUNIT_TEST(testError);
	CPPUNIT_TEST_SUITE_END();
public:

	const double epsilon = 2e-13;

	bool isRowOrthonormal(const Matrix &m) {
		Matrix temp = m * m.T();
		temp = temp - Matrix::Eye(temp);
		return temp.MaxAbs() < epsilon;
	}

	bool isColumnOrthonormal(const Matrix &m) {
		Matrix temp = m.T() * m;
		temp = temp - Matrix::Eye(temp);
		return temp.MaxAbs() < epsilon;
	}

	bool isOrthogonal(const Matrix &m) {
		return isRowOrthonormal(m) && isColumnOrthonormal(m);
	}

	void testSpeed() {

		std::mt19937 gen { 1234321 };
		std::normal_distribution<double> dist { 5.0, 2.0 };
		Matrix A(251, 101); // Note: both are prime numbers.
		for (size_t i = 0; i < A.Numel(); ++i)
			A.Insert(dist(gen));

		SVD svd;
		StopWatch sw;

		sw.Start();
		svd.Decompose(A);
		sw.Stop();

		Matrix U = svd.U;
		Matrix S = svd.W;
		Matrix V = svd.V;

		Matrix A2 = U * Matrix::Diag(S, U.Size(1), V.Size(1)) * V.T();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("U is not column-orthonormal.", true,
				isColumnOrthonormal(U));
		CPPUNIT_ASSERT_EQUAL_MESSAGE("V is not orthogonal.", true,
				isOrthogonal(V));

		Matrix T = A2 - A;
		CPPUNIT_ASSERT_LESS(epsilon, T.MaxAbs());
		CPPUNIT_ASSERT_LESS(1.0, sw.GetSecondsCPU());

//		std::cout << "\nDecomposition of " << A.Numel() << " values took "
//				<< sw.GetSecondsCPU() << " s.\n";
//		std::cout << "Conditional number = " << svd.Cond() << "\n";
//		std::cout << "Reconstruction error = " << T.MaxAbs() << "\n";
	}

	void testSparse() {

		Matrix A = Matrix::Zeros(251, 101);
		{
			// The seed is selected to generate a matrix with bad condition-
			// number.
			std::mt19937 gen(1006);
			std::normal_distribution<double> dVal { 5.0, 0.0 };
			std::uniform_int_distribution<int> dRow(0, A.Size(0) - 1);
			std::uniform_int_distribution<int> dCol(0, A.Size(1) - 1);
			for (size_t i = 0; i < A.Size(0); ++i) {
				const size_t c = dCol(gen);
				const double v = dVal(gen);
				A(i, c) = v;
			}
			for (size_t i = 0; i < A.Size(1); ++i) {
				const size_t r = dRow(gen);
				const double v = dVal(gen);
				A(r, i) = v;
			}
		}

//		A.SetVariableName("A");
//		MatlabFile mf("/tmp/out.mat");
//		mf.WriteMatrix(A);
//		mf.Close();

		SVD svd;
		StopWatch sw;

		sw.Start();
		svd.Decompose(A);
		sw.Stop();

		Matrix U = svd.U;
		Matrix S = svd.W;
		Matrix V = svd.V;

		Matrix A2 = U * Matrix::Diag(S, U.Size(1), V.Size(1)) * V.T();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("U is not column-orthonormal.", true,
				isColumnOrthonormal(U));
		CPPUNIT_ASSERT_EQUAL_MESSAGE("V is not orthogonal.", true,
				isOrthogonal(V));

		Matrix T = A2 - A;
		CPPUNIT_ASSERT_LESS(epsilon, T.MaxAbs());

//		std::cout << "\nDecomposition of " << A.Numel() << " values took "
//				<< sw.GetSecondsCPU() << " s.\n";
//		std::cout << "Conditional number = " << svd.Cond() << "\n";
//		std::cout << "Reconstruction error = " << T.MaxAbs() << "\n";
	}

	void testMissing() {

//		Matrix A = Matrix::Zeros(251, 101);
		Matrix A = Matrix::Zeros(5, 3);
		{
			// The generated matrix has some all-zero rows and some all-zero
			// columns.
			std::mt19937 gen(1); // 1, 15, 213
			std::normal_distribution<double> dVal { 0.0, 5.0 };
			std::uniform_int_distribution<int> dRow(0, A.Size(0) - 1);
			std::uniform_int_distribution<int> dCol(0, A.Size(1) - 1);
			for (size_t i = 0; i < ((A.Size(0) + A.Size(1)) / 1); ++i) {
				const size_t r = dRow(gen);
				const size_t c = dCol(gen);
				const double v = dVal(gen);
				A(r, c) = v;
			}
		}

		SVD svd;
		StopWatch sw;

		sw.Start();
		svd.Decompose(A);
		sw.Stop();

		Matrix A2 = svd.U * Matrix::Diag(svd.W, svd.U.Size(1), svd.V.Size(1))
				* svd.V.T();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("U is not column-orthonormal.", true,
				isColumnOrthonormal(svd.U));
		CPPUNIT_ASSERT_EQUAL_MESSAGE("V is not orthogonal.", true,
				isOrthogonal(svd.V));

		Matrix T = A2 - A;

//		std::cout << "\nDecomposition of " << A.Numel() << " values took "
//				<< sw.GetSecondsCPU() << " s.\n";
//		std::cout << "Conditional number = " << svd.Cond() << "\n";
//		std::cout << "Reconstruction error = " << T.MaxAbs() << "\n";

		CPPUNIT_ASSERT_LESS(epsilon, T.MaxAbs());
	}

	void testBroken() {

		//		Matrix A = Matrix::Zeros(251, 101);
		Matrix A = Matrix::Zeros(3, 3);
//		Matrix A = Matrix::Zeros(2, 2);
//		A.Insert( { 1, 0, 1, 0 });
//		A.Insert( { 0, 0, 0, 1, 1, 1, 3, 3, 3 });
		A.Insert( { 0, 0, 0, 1, 0, 0, 1, 1, 1 });

		SVD svd;
		StopWatch sw;

		sw.Start();
		svd.Decompose(A);
		sw.Stop();

		{
//			Matrix U = svd.U;
//			Matrix S = Matrix::Diag(svd.W, svd.U.Size(1), svd.V.Size(1));
//			Matrix V = svd.V;
//
//			A.SetVariableName("A");
//			U.SetVariableName("U");
//			S.SetVariableName("S");
//			V.SetVariableName("V");
//			MatlabFile mf("/tmp/out.mat");
//			mf.WriteMatrix(A);
//			mf.WriteMatrix(U);
//			mf.WriteMatrix(S);
//			mf.WriteMatrix(V);
//			mf.Close();
		}

		Matrix A2 = svd.U * Matrix::Diag(svd.W, svd.U.Size(1), svd.V.Size(1))
				* svd.V.T();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("U is not column-orthonormal.", true,
				isColumnOrthonormal(svd.U));
		CPPUNIT_ASSERT_EQUAL_MESSAGE("V is not orthogonal.", true,
				isOrthogonal(svd.V));

		Matrix T = A2 - A;
//		{
//			std::cout << "\nA = " << A << ";\n";
//			std::cout << "U = " << svd.U << ";\n";
//			std::cout << "S = "
//					<< Matrix::Diag(svd.Winv, svd.U.Size(1), svd.V.Size(1))
//					<< ";\n";
//			std::cout << "V = " << svd.V << ";\n";
//			std::cout << "A2 = " << A2 << ";\n";
//		}
//		std::cout << "\nDecomposition of " << A.Numel() << " values took "
//				<< sw.GetSecondsCPU() << " s.\n";
//		std::cout << "Conditional number = " << svd.Cond() << "\n";
//		std::cout << "Reconstruction error = " << T.MaxAbs() << "\n";

		CPPUNIT_ASSERT_LESS(epsilon, T.MaxAbs());
	}

	void testEmpty() {
		{
			// Empty row
			Matrix A(3, 3);
			A.Insert( { 0, 3, 3, 0, 1, 4, 0, 8, 1 });

//			std::cout << "\nA = " << A << ";\n";

			SVD svd(A);

			Matrix U = svd.U;
			Matrix S = svd.W;
			Matrix V = svd.V;

			Matrix A2 = U * Matrix::Diag(S, U.Size(1), V.Size(1)) * V.T();

			CPPUNIT_ASSERT_EQUAL_MESSAGE("U is not column-orthonormal.", true,
					isColumnOrthonormal(U));
			CPPUNIT_ASSERT_EQUAL_MESSAGE("V is not orthogonal.", true,
					isOrthogonal(V));

			Matrix T = A2 - A;
			CPPUNIT_ASSERT_LESS(epsilon, T.MaxAbs());
		}
		{
			// Empty column
			Matrix A(3, 3);
			A.Insert( { 1, 2, 3, 0, 0, 0, 4, 5, 1 });
			SVD svd(A);

			Matrix U = svd.U;
			Matrix S = svd.W;
			Matrix V = svd.V;

			Matrix A2 = U * Matrix::Diag(S, U.Size(1), V.Size(1)) * V.T();

			CPPUNIT_ASSERT_EQUAL_MESSAGE("U is not column-orthonormal.", true,
					isColumnOrthonormal(U));
			CPPUNIT_ASSERT_EQUAL_MESSAGE("V is not orthogonal.", true,
					isOrthogonal(V));

			Matrix T = A2 - A;
			CPPUNIT_ASSERT_LESS(epsilon, T.MaxAbs());
		}

		{
			// Empty column and row
			Matrix A(3, 3);
			A.Insert( { 1, 0, 3, 0, 0, 0, 4, 0, 1 });
			SVD svd(A);

			Matrix U = svd.U;
			Matrix S = svd.W;
			Matrix V = svd.V;

			Matrix A2 = U * Matrix::Diag(S, U.Size(1), V.Size(1)) * V.T();

			CPPUNIT_ASSERT_EQUAL_MESSAGE("U is not column-orthonormal.", true,
					isColumnOrthonormal(U));
			CPPUNIT_ASSERT_EQUAL_MESSAGE("V is not orthogonal.", true,
					isOrthogonal(V));

			Matrix T = A2 - A;
			CPPUNIT_ASSERT_LESS(epsilon, T.MaxAbs());
		}

	}

	void testTransform() {
		{
			Matrix A(2, 2);
			A.Insert( { 1, 2, 3, 1 });
			SVD svd(A);

			Matrix U = svd.U;
			Matrix S = svd.W;
			Matrix V = svd.V;

			Matrix A2 = U * Matrix::Diag(S, U.Size(1), V.Size(1)) * V.T();

			CPPUNIT_ASSERT_EQUAL_MESSAGE("U is not column-orthonormal.", true,
					isColumnOrthonormal(U));
			CPPUNIT_ASSERT_EQUAL_MESSAGE("V is not orthogonal.", true,
					isOrthogonal(V));

			Matrix T = A2 - A;
			CPPUNIT_ASSERT_LESS(epsilon, T.MaxAbs());
		}
		{
			Matrix A(4, 3);
			std::vector<double> v = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };

			A.Insert(v.data(), v.size());

			SVD svd;
			svd.Decompose(A);

			Matrix U = svd.U;
			Matrix S = svd.W;
			Matrix V = svd.V;

			Matrix A2 = U * Matrix::Diag(S, U.Size(1), V.Size(1)) * V.T();

			CPPUNIT_ASSERT_EQUAL_MESSAGE("U is not column-orthonormal.", true,
					isColumnOrthonormal(U));
			CPPUNIT_ASSERT_EQUAL_MESSAGE("V is not orthogonal.", true,
					isOrthogonal(V));

			Matrix T = A2 - A;
			CPPUNIT_ASSERT_LESS(epsilon, T.MaxAbs());
		}

	}

	void testInterpolation() {
		{
			Matrix A(3, 2);
			Matrix X(3, 1);
			A.Insert( { 1, 1, 1, 0, 1, 2 });
			X.Insert( { 2, 3, 4 });
			SVD svd(A);

//		Matrix C = svd.V * Matrix::Diag(svd.Winv, svd.V.Size(1), svd.U.Size(1))
//				* svd.U.T() * X;

			Matrix C = svd.Solve(X);

//		{
//			Matrix U = svd.U;
//			Matrix S = Matrix::Diag(svd.W, svd.U.Size(1), svd.V.Size(1));
//			Matrix V = svd.V;
//
//			A.SetVariableName("A");
//			U.SetVariableName("U");
//			S.SetVariableName("S");
//			V.SetVariableName("V");
//			MatlabFile mf("/tmp/out.mat");
//			mf.WriteMatrix(A);
//			mf.WriteMatrix(U);
//			mf.WriteMatrix(S);
//			mf.WriteMatrix(V);
//			mf.Close();
//		}

			// Offset =
			CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, C(0, 0), epsilon);
			// Slope =
			CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, C(1, 0), epsilon);

		}
		{

		}

	}

	void testError() {
		{
			Matrix A(3, 2);
			SVD svd;
			CPPUNIT_ASSERT_NO_THROW(svd.Decompose(A));
		}
		{
			Matrix A(4, 3, 2);
			SVD svd;
			CPPUNIT_ASSERT_THROW(svd.Decompose(A), std::runtime_error);
		}
		{
			Matrix A(2, 3);
			SVD svd;
			CPPUNIT_ASSERT_THROW(svd.Decompose(A), std::runtime_error);
		}
	}

};

CPPUNIT_TEST_SUITE_REGISTRATION(SVDTest);
#endif
