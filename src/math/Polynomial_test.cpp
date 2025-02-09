// 10.12.2023

#ifdef USE_CPPUNIT

#include "Polynomial.h"

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "../system/Plotter.h"

class PolynomTest: public CppUnit::TestFixture {
CPPUNIT_TEST_SUITE (PolynomTest);
	CPPUNIT_TEST(testSetupValue);
	CPPUNIT_TEST(testSetupDerivative);
	CPPUNIT_TEST(testSetupBezier);
	CPPUNIT_TEST(testSetupInterpol);
	CPPUNIT_TEST(testSetup);
	CPPUNIT_TEST(testArithmetic);
	CPPUNIT_TEST(testPolynomDivision);
	CPPUNIT_TEST(testScale);
	CPPUNIT_TEST(testIntegral);
	CPPUNIT_TEST(testDerivative);
	CPPUNIT_TEST(testReduce);
	CPPUNIT_TEST(testAnalyse);
	CPPUNIT_TEST(testInvert);

	CPPUNIT_TEST_SUITE_END()
	;
public:

	void testSetupValue() {
		{
			Polynomial a = Polynomial::ByValue(0.0, 2.0);
			CPPUNIT_ASSERT(a.size() == 1);
			CPPUNIT_ASSERT(a.Order() == 0);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, a[0], 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, a(1234.0), 1e-9);
		}
		{
			Polynomial a = Polynomial::ByValue(0.0, 2.0, 3.0, 11.0);
			CPPUNIT_ASSERT(a.size() == 2);
			CPPUNIT_ASSERT(a.Order() == 1);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, a[0], 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, a[1], 1e-9);

			// Switched points
			CPPUNIT_ASSERT_DOUBLES_EQUAL(6.5, a(1.5), 1e-9);
			Polynomial b = Polynomial::ByValue(3.0, 11.0, 0.0, 2.0);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, b[0], 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, b[1], 1e-9);

		}
		{
			Polynomial a = Polynomial::ByValue(0.0, 2.0, 1.0, 4.0, 3.0, 5.0);
			CPPUNIT_ASSERT(a.size() == 3);
			CPPUNIT_ASSERT(a.Order() == 2);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, a[0], 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(2.5, a[1], 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.5, a[2], 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(-4748.0, a(100.0), 1e-9);
		}
		{
			Polynomial a = Polynomial::ByValue(0.0, 2.0, -1.0, 3.0, 5.0, 9.0,
					1.0, 1.0);
			CPPUNIT_ASSERT(a.size() == 4);
			CPPUNIT_ASSERT(a.Order() == 3);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, a[0], 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.1, a[1], 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, a[2], 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(0.1, a[3], 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(261.0, a(14.0), 1e-9);
		}
	}

	void testSetupDerivative() {
		{
			Polynomial a = Polynomial::ByDerivative(1.0, 5.0, 3.0);
			CPPUNIT_ASSERT(a.size() == 2);
			CPPUNIT_ASSERT(a.Order() == 1);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, a[0], 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, a[1], 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, a(1.0), 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(8.0, a(2.0), 1e-9);
		}
		{
			Polynomial a = Polynomial::ByDerivative(0.0, 1.0, 0.0, 2.0, 0.0);
			CPPUNIT_ASSERT(a.size() == 3);
			CPPUNIT_ASSERT(a.Order() == 2);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, a[0], 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, a[1], 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.25, a[2], 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(0.75, a(1.0), 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, a(2.0), 1e-9);
		}
		{
			Polynomial a = Polynomial::ByDerivative(1.0, 5.0, 3.0, 3.0, 1.0,
					-3.0);
			CPPUNIT_ASSERT(a.size() == 4);
			CPPUNIT_ASSERT(a.Order() == 3);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(-3.5, a[0], 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(15.0, a[1], 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(-7.5, a[2], 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, a[3], 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, a(1.0), 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(4.5, a(2.0), 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, a(3.0), 1e-9);
		}
	}

	void testSetupBezier() {
		{
			Polynomial a = Polynomial::ByBezier(7.0);
			CPPUNIT_ASSERT(a.size() == 1);
			CPPUNIT_ASSERT(a.Order() == 0);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(7.0, a[0], 1e-9);
			std::vector<double> c = a.GetBezier();
			CPPUNIT_ASSERT(c.size() == 1);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(7.0, c[0], 1e-9);
		}
		{
			Polynomial a = Polynomial::ByBezier(7.0, 11.0);
			CPPUNIT_ASSERT(a.size() == 2);
			CPPUNIT_ASSERT(a.Order() == 1);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(7.0, a[0], 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0, a[1], 1e-9);
			std::vector<double> c = a.GetBezier();
			CPPUNIT_ASSERT(c.size() == 2);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(7.0, c[0], 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(11.0, c[1], 1e-9);
		}
		{
			Polynomial a = Polynomial::ByBezier(7.0, 11.0, 5.0);
			CPPUNIT_ASSERT(a.size() == 3);
			CPPUNIT_ASSERT(a.Order() == 2);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(7.0, a[0], 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(8.0, a[1], 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(-10.0, a[2], 1e-9);
			std::vector<double> c = a.GetBezier();
			CPPUNIT_ASSERT(c.size() == 3);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(7.0, c[0], 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(11.0, c[1], 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, c[2], 1e-9);
		}
		{
			Polynomial a = Polynomial::ByBezier(7.0, 5.0, 15.0, 9.0);
			CPPUNIT_ASSERT(a.size() == 4);
			CPPUNIT_ASSERT(a.Order() == 3);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(7.0, a[0], 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(-6.0, a[1], 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(36.0, a[2], 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(-28.0, a[3], 1e-9);
			std::vector<double> c = a.GetBezier();
			CPPUNIT_ASSERT(c.size() == 4);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(7.0, c[0], 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, c[1], 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(15.0, c[2], 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(9.0, c[3], 1e-9);
		}
		{
			// Do the same Bezier-interpolation by using Bernstein-polynoms:
			Polynomial b0 = Polynomial::Bernstein(4, 0);
			Polynomial b1 = Polynomial::Bernstein(4, 1);
			Polynomial b2 = Polynomial::Bernstein(4, 2);
			Polynomial b3 = Polynomial::Bernstein(4, 3);
			Polynomial a = b0 * 7.0 + b1 * 5.0 + b2 * 15.0 + b3 * 9.0;
//			Plotter(b0).Plot(0, 1);
//			Plotter(b1).Plot(0, 1);
//			Plotter(b2).Plot(0, 1);
//			Plotter(b3).Plot(0, 1);
			CPPUNIT_ASSERT(a.size() == 4);
			CPPUNIT_ASSERT(a.Order() == 3);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(7.0, a[0], 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(-6.0, a[1], 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(36.0, a[2], 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(-28.0, a[3], 1e-9);
		}

	}
	void testSetupInterpol() {
		{
			std::vector<double> r =
					{ -5.0, -4, 0, -3.0, 0.0, 1.0, 1.5, 4.0, 5.0 };
			std::vector<double> v = { -725.0, -351.0, 5.0, -127.0, 5.0, -11.0,
					-10.0, 265.0, 585.0 };

			Polynomial a = Polynomial::ByVector(r, v, 3);
			CPPUNIT_ASSERT(a.size() == 4);
			CPPUNIT_ASSERT(a.Order() == 3);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, a[0], 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(-19.0, a[1], 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(-3.0, a[2], 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(6.0, a[3], 1e-9);
		}

		{
			// Lagrange-Interpolation:
			Polynomial b0 = Polynomial::Lagrange(4, 0);
			Polynomial b1 = Polynomial::Lagrange(4, 1);
			Polynomial b2 = Polynomial::Lagrange(4, 2);
			Polynomial b3 = Polynomial::Lagrange(4, 3);
			Polynomial a = b0 * 7.0 + b1 * 5.0 + b2 * 15.0 + b3 * 9.0;
			CPPUNIT_ASSERT(a.size() == 4);
			CPPUNIT_ASSERT(a.Order() == 3);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(7.0, a[0], 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(-52.0, a[1], 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(180.0, a[2], 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(-126.0, a[3], 1e-9);
		}
	}

	void testSetup() {
		{
			Polynomial a(3);
			CPPUNIT_ASSERT(a.size() == 3);
			CPPUNIT_ASSERT(a.Order() == 2);
		}
		{
			Polynomial b( { 1, 2, 3, 4, 5 });
			CPPUNIT_ASSERT(b.size() == 5);
			CPPUNIT_ASSERT(b.Order() == 4);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, b[0], 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, b[4], 1e-9);
			b.Resize(3);
			CPPUNIT_ASSERT(b.size() == 3);
			CPPUNIT_ASSERT(b.Order() == 2);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, b[0], 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, b[2], 1e-9);

			CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, b(0.0), 1e-9); //! Zahlen neu berechnen
			CPPUNIT_ASSERT_DOUBLES_EQUAL(6.0, b(1.0), 1e-9); //! Zahlen neu berechnen
			CPPUNIT_ASSERT_DOUBLES_EQUAL(17.0, b(2.0), 1e-9); //! Zahlen neu berechnen
		}
		{
			Polynomial c;
			CPPUNIT_ASSERT(c.size() == 0);
			CPPUNIT_ASSERT(c.Order() == 0);
		}
		{
			Polynomial d( { 0 });
			CPPUNIT_ASSERT(d.size() == 1);
			CPPUNIT_ASSERT(d.Order() == 0);
		}

	}

	void testArithmetic() {

		Polynomial a { 1, 2, 3 };
		Polynomial b { 4, 5 };

		Polynomial c = a + 3;
		Polynomial d = a + b;
		Polynomial e = a - 1.5;
		Polynomial f = b + a + b;
		Polynomial g = a * 2.0;
		Polynomial h = a * a;
		Polynomial i = a * b;
		Polynomial j = a / 2.0;
		Polynomial k = (a + (b * 2.0)) / 3.0;
		Polynomial m = a;
		m -= b;
		Polynomial n = b;
		n -= a;

		CPPUNIT_ASSERT(c.size() == 3);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0, c[0], 1e-9);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, c[1], 1e-9);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, c[2], 1e-9);

		CPPUNIT_ASSERT(d.size() == 3);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, d[0], 1e-9);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(7.0, d[1], 1e-9);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, d[2], 1e-9);

		CPPUNIT_ASSERT(e.size() == 3);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.5, e[0], 1e-9);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, e[1], 1e-9);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, e[2], 1e-9);

		CPPUNIT_ASSERT(f.size() == 3);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(9.0, f[0], 1e-9);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(12.0, f[1], 1e-9);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, f[2], 1e-9);

		CPPUNIT_ASSERT(g.size() == 3);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, g[0], 1e-9);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0, g[1], 1e-9);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(6.0, g[2], 1e-9);

		CPPUNIT_ASSERT(h.size() == 5);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, h[0], 1e-9);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0, h[1], 1e-9);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(10.0, h[2], 1e-9);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(12.0, h[3], 1e-9);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(9.0, h[4], 1e-9);

		CPPUNIT_ASSERT(i.size() == 4);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0, i[0], 1e-9);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(13.0, i[1], 1e-9);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(22.0, i[2], 1e-9);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(15.0, i[3], 1e-9);

		CPPUNIT_ASSERT(j.size() == 3);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, j[0], 1e-9);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, j[1], 1e-9);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.5, j[2], 1e-9);

		CPPUNIT_ASSERT(k.size() == 3);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, k[0], 1e-9);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0, k[1], 1e-9);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, k[2], 1e-9);

		CPPUNIT_ASSERT(m.size() == 3);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-3.0, m[0], 1e-9);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-3.0, m[1], 1e-9);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, m[2], 1e-9);

		CPPUNIT_ASSERT(n.size() == 3);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, n[0], 1e-9);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, n[1], 1e-9);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-3.0, n[2], 1e-9);

	}

	void testPolynomDivision() {
		Polynomial a { 11, 21, 28, 43, 38, 24 };
		Polynomial b { 4, 5, 6 };
		Polynomial q;
		Polynomial r;
		std::tie(q, r) = a / b;
		CPPUNIT_ASSERT(q.size() == 4);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, q[0], 1e-9);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, q[1], 1e-9);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, q[2], 1e-9);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0, q[3], 1e-9);
		CPPUNIT_ASSERT(r.size() == 2);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(7.0, r[0], 1e-9);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(8.0, r[1], 1e-9);
	}

	void testScale() {
		Polynomial a = Polynomial::ByValue(0.0, 0.0, 0.5, 1.0, 1.0, 0.0);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, a(0.0), 1e-9);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, a(0.5), 1e-9);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, a(1.0), 1e-9);
		{
			Polynomial b = a;
			b.ScaleX(2.0);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, b(0.0), 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, b(1.0), 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, b(2.0), 1e-9);
		}
		{
			Polynomial b = a;
			b.ScaleY(2.0);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, b(0.0), 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, b(0.5), 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, b(1.0), 1e-9);
		}
		{
			Polynomial b = a;
			b.ShiftX(3.0);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, b(3.0), 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, b(3.5), 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, b(4.0), 1e-9);
		}
		{
			Polynomial b = a;
			b.ShiftY(3.0);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, b(0.0), 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0, b(0.5), 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, b(1.0), 1e-9);
		}
//		Plotter(a).Plot(0.0, 1.0);
	}

	void testIntegral() {
		Polynomial a { 1, 2 };
		CPPUNIT_ASSERT_DOUBLES_EQUAL(10.0, a.Integral(1.0, 3.0), 1e-9);

		Polynomial b = a.Integral(2);
		CPPUNIT_ASSERT(b.size() == 4);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, b[0], 1e-9);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, b[1], 1e-9);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, b[2], 1e-9);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0 / 3.0, b[3], 1e-9);

		a.Integrate();
		CPPUNIT_ASSERT(a.size() == 3);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, a[0], 1e-9);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, a[1], 1e-9);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, a[2], 1e-9);
	}
	void testDerivative() {
		{
			Polynomial a { 5, 6, 7 };
			Polynomial b = a.Derivative(1);
			Polynomial c = a.Derivative(2);
			Polynomial d = a.Derivative(3);
			Polynomial e = a.Derivative(4);

			CPPUNIT_ASSERT(b.size() == 2);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(6.0, b[0], 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(14.0, b[1], 1e-9);
			CPPUNIT_ASSERT(c.size() == 1);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(14.0, c[0], 1e-9);
			CPPUNIT_ASSERT(d.size() == 1);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, d[0], 1e-9);
			CPPUNIT_ASSERT(e.size() == 1);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, e[0], 1e-9);
		}
		{
			Polynomial a { 1, 2, 3, 4, 5 };
			a.Derive();
			CPPUNIT_ASSERT(a.size() == 4);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, a[0], 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(6.0, a[1], 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(12.0, a[2], 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(20.0, a[3], 1e-9);
			a.Derive(2);
			CPPUNIT_ASSERT(a.size() == 2);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(24.0, a[0], 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(120.0, a[1], 1e-9);
			a.Derive(2);
			CPPUNIT_ASSERT(a.size() == 1);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, a[0], 1e-9);
		}
	}
	void testReduce() {
		Polynomial a { 4, 3, 2, 1 };
		Polynomial b = a.Reduce(3, 0.0, 5.0);
		Polynomial c = a.Reduce(5, 0.0, 5.0);
		CPPUNIT_ASSERT(b.size() == 3);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0, b[0], 1e-9);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-9.5, b[1], 1e-9);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(9.5, b[2], 1e-9);

		CPPUNIT_ASSERT(c.size() == 5);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0, c[0], 1e-9);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, c[1], 1e-9);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, c[2], 1e-9);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, c[3], 1e-9);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, c[4], 1e-9);
	}

	void testAnalyse() {
		Polynomial p { 7, -52, 180, -126 };

		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.7748, p.ExtremumPos(), 1e-4);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.4762, p.InflectionPoint(), 1e-4);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.1775, p.ExtremumNeg(), 1e-4);

	}

	void testInvert() {
		{
			Polynomial a { 3, 5 };
			Polynomial b = a.Inverse();
			CPPUNIT_ASSERT(b.size() == 2);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.6, b[0], 1e-9);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(0.2, b[1], 1e-9);
		}
		{

			// Not tested, because the current algorithm is unstable.

//			Polynomial a { 0, -1, 1 };
//			Polynomial b = a.Inverse(0.5, 1.0,5);
//			std::cout << b << '\n';
//			CPPUNIT_ASSERT(b.size() == 2);
//			CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, b[0], 1e-9);
//			CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, b[1], 1e-9);
		}

	}

};
CPPUNIT_TEST_SUITE_REGISTRATION(PolynomTest);
#endif
