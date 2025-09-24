///////////////////////////////////////////////////////////////////////////////
// Name               : LastModel.cpp
// Purpose            :
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 02.03.2019
// Copyright          : (C) 2019 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#include "LastModel.h"

#include "../../3D/Bender.h"
#include "../../3D/BoundingBox.h"
#include "../../3D/FileOBJ.h"
#include "../../3D/FileSTL.h"
#include "../../3D/OpenGLMaterial.h"
#include "../../3D/OpenGLText.h"
#include "../../3D/PolyCylinder.h"
#include "../../3D/TransformationMixer.h"
#include "../../math/FourierTransform.h"
#include "../../math/Kernel.h"
#include "../../math/KernelDensityEstimator.h"
#include "../../math/LinearOptimizer.h"
#include "../../math/MatlabFile.h"
#include "../../math/Matrix.h"
#include "../../math/MEstimator.h"
#include "../../math/NelderMeadOptimizer.h"
#include "../../math/ParameterLimits.h"
#include "../../math/PCA.h"

#include "../FootMeasurements.h"
#include "../object/Insole.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <iostream>
#include <ostream>

#include "../../3D/OpenGL.h"

LastModel::LastModel() {
//	center.XLinspace(0, 1, 101);
//	center.YInit(0);
//	center.Insert(0.3, -1.2, 0.3, BendLine::EpanechnikovKernel);
//	center.Insert(0.75, 1.3, 0.03, BendLine::GaussianKernel);
//	center.AngleToPos();

	paintTriangles = false;
	paintVertices = true;

//	Polynomial p = Polynomial::ByValue(-1, 1, 0, 0, 1, 1);
//	std::cout << p << "\n";

//	PolyFilter pf;
//	pf.Init(0, 40);
//	pf.Export("/tmp/PF_0.mat");
//	pf.Init(1, 40);
//	pf.Export("/tmp/PF_1.mat");
//	pf.Init(2, 40);
//	pf.Export("/tmp/PF_2.mat");
//	pf.Init(3, 40);
//	pf.Export("/tmp/PF_3.mat");

//	KernelDensityEstimator kde0;
//	kde0.XLinspace(-5, 5, 301);
//	kde0.Insert(0, 1, 1, KernelDensityEstimator::CauchyKernel);
//	kde0.Export("/tmp/CauchyKernel.mat");
//	kde0.YInit();
//	kde0.Insert(0, 1, 1, KernelDensityEstimator::CosineKernel);
//	kde0.Export("/tmp/CosineKernel.mat");
//	kde0.YInit();
//	kde0.Insert(0, 1, 1, KernelDensityEstimator::EpanechnikovKernel);
//	kde0.Export("/tmp/EpanechnikovKernel.mat");
//	kde0.YInit();
//	kde0.Insert(0, 1, 1, KernelDensityEstimator::GaussianKernel);
//	kde0.Export("/tmp/GaussianKernel.mat");
//	kde0.YInit();
//	kde0.Insert(0, 1, 1, KernelDensityEstimator::LogisticKernel);
//	kde0.Export("/tmp/LogisticKernel.mat");
//	kde0.YInit();
//	kde0.Insert(0, 1, 1, KernelDensityEstimator::PicardKernel);
//	kde0.Export("/tmp/PicardKernel.mat");
//	kde0.YInit();
//	kde0.Insert(0, 1, 1, KernelDensityEstimator::QuarticKernel);
//	kde0.Export("/tmp/QuarticKernel.mat");
//	kde0.YInit();
//	kde0.Insert(0, 1, 1, KernelDensityEstimator::SigmoidKernel);
//	kde0.Export("/tmp/SigmoidKernel.mat");
//	kde0.YInit();
//	kde0.Insert(0, 1, 1, KernelDensityEstimator::SilvermanKernel);
//	kde0.Export("/tmp/SilvermanKernel.mat");
//	kde0.YInit();
//	kde0.Insert(0, 1, 1, KernelDensityEstimator::TriangularKernel);
//	kde0.Export("/tmp/TriangularKernel.mat");
//	kde0.YInit();
//	kde0.Insert(0, 1, 1, KernelDensityEstimator::TricubeKernel);
//	kde0.Export("/tmp/TricubeKernel.mat");
//	kde0.YInit();
//	kde0.Insert(0, 1, 1, KernelDensityEstimator::TriweightKernel);
//	kde0.Export("/tmp/TriweightKernel.mat");
//	kde0.YInit();
//	kde0.Insert(0, 1, 1, KernelDensityEstimator::UniformKernel);
//	kde0.Export("/tmp/UniformKernel.mat");

}

void LastModel::Paint() const {
//	tg.PaintLines();

	OpenGLMaterial white(OpenGLMaterial::Preset::WhitePlastic, 1.0);
	white.UseMaterial();

	//	loop.Paint(true, 0.25);

	bottomleft.Paint();
	bottomright.Paint();
	//	bottom.Paint(true);
	//	top.Paint(true);
	//	center.Paint(true);

	OpenGLMaterial green(OpenGLMaterial::Preset::GreenRubber, 1.0);
	green.UseMaterial();

	planeXZ.Paint();

	HeelGirth.Paint();
	WaistGirth.Paint();
	LittleToeGirth.Paint();
	BigToeGirth.Paint();

	OpenGLText font;

	glNormal3f(0, -1, 0);

	PaintMarker(0, font, "0");
	PaintMarker(idxZero, font, "Back");
	PaintMarker(idxHeel, font, "Heel");
	PaintMarker(idxHeelPoint, font, "HeelPoint");
	PaintMarker(idxHeelCenter, font, "HeelCenter");
	PaintMarker(idxWaistBottom, font, "Waist");
	PaintMarker(idxLittleToeBottom, font, "LittleToeBottom");
	PaintMarker(idxBigToeBottom, font, "BigToeBottom");
	PaintMarker(idxToeCenter, font, "ToeCenter");
	PaintMarker(idxToeTip, font, "ToeTip");
	PaintMarker(idxBigToeTop, font, "BigToeTop");
	PaintMarker(idxLittleToeTop, font, "LittleToeTop");
	PaintMarker(idxWaistTop, font, "WaistTop");
	PaintMarker(idxTop, font, "Front");

	OpenGLMaterial yellow(OpenGLMaterial::Preset::YellowPlastic, 1.0);
	yellow.UseMaterial();

	//	formfinder.Paint();
	glDisable(GL_CULL_FACE);
	if (planeXZ.Size() > idxHeelCenter) {
		Vector3 heelp = planeXZ[idxHeelCenter];
		Vector3 temp = heelp - Vector3(cos(heela), 0, sin(heela)) * 2.0;
		glBegin(GL_TRIANGLES);
		glVertex3f(heelp.x, heelp.y, heelp.z);
		glVertex3f(temp.x, temp.y, temp.z);
		glVertex3f(temp.x, temp.y, heelp.z);
		glEnd();
	}
	if (planeXZ.Size() > idxToeCenter) {
		Vector3 toep = planeXZ[idxToeCenter];
		Vector3 temp = toep + Vector3(cos(toea), 0, sin(toea)) * 2.0;
		glBegin(GL_TRIANGLES);
		glVertex3f(toep.x, toep.y, toep.z);
		glVertex3f(temp.x, temp.y, toep.z);
		glVertex3f(temp.x, temp.y, temp.z);
		glEnd();
	}
	glEnable(GL_CULL_FACE);

	glPushMatrix();
	//	coordsys.GLMultMatrix();

	glRotatef(90, 0, 1, 0);

	//	symmetry.Paint();
	//	kde.Paint();

	//	DependentVector::PaintCircle(0.125);
	//	DependentVector::PaintCircle(0.25);
	//	DependentVector::PaintCircle(0.5);

	glPopMatrix();

	OpenGLMaterial(OpenGLMaterial::Preset::YellowRubber).UseColor(1.0);
	glPushMatrix();

	//		coordsys.GLMultMatrix();
	glRotatef(90, 1, 0, 0);
	//	last->angleXZ.Paint();

	glTranslatef(0, 0, 0.001);
	OpenGLMaterial(OpenGLMaterial::Preset::GreenPlastic).UseColor(1.0);
	debugA.Paint();

	glTranslatef(0, 0, 0.001);
	OpenGLMaterial(OpenGLMaterial::Preset::cFuchsia).UseColor(1.0);
	debugB.Paint();

	glTranslatef(0, 0, 0.001);
	OpenGLMaterial(OpenGLMaterial::Preset::Obsidian).UseColor(1.0);
	debugC.Paint();

	glRotatef(90, 0, 0, 1);

	glTranslatef(0, 0, 0.001);
	OpenGLMaterial(OpenGLMaterial::Preset::CyanPlastic).UseColor(1.0);
	debug0.Paint();

	glTranslatef(0, 0, 0.001);
	OpenGLMaterial(OpenGLMaterial::Preset::RedPlastic).UseColor(1.0);
	debug1.Paint();

	glTranslatef(0, 0, 0.001);
	OpenGLMaterial(OpenGLMaterial::Preset::Copper).UseColor(1.0);
	debug2.Paint();

	glPopMatrix();
	OpenGLMaterial::EnableColors();
	//		coordsys.Paint(AffineTransformMatrix::Style::Lines);

	//	glBegin(GL_LINES);
	//	glColor3f(1, 0, 0);
	//	glNormal3f(0, 0, 1);
	//	glVertex3f(center.x, center.y, center.z);
	//	glVertex3f(center.x + rot.x, center.y + rot.y, center.z + rot.z);
	//	glColor3f(0, 1, 0);
	//	glVertex3f(center.x, center.y, center.z);
	//	glVertex3f(center.x + ort.x, center.y + ort.y, center.z + ort.z);
	//
	//	glEnd();

	//	for(size_t n = 0; n < center.N; n++){
	//		glVertex3d(center.x[n], 0, center.y[n]);
	//	}
	//	glEnd();

	//	tg.PaintLines();

	OpenGLMaterial(OpenGLMaterial::Preset::cGray).UseMaterial();
	//	rawBB.Paint();

	OpenGLMaterial::EnableColors();
	OpenGLMaterial(OpenGLMaterial::Preset::cGray).UseColor();
	ObjectGeometry::Paint();

}

void LastModel::PaintMarker(const size_t idx, const OpenGLText &font,
		const std::string &text) const {
	if (idx >= planeXZ.Size())
		return;
	Vector3 normal =
			(planeXZ[(idx + 1) % planeXZ.Size()] - planeXZ[idx]).Normal();
	normal = Vector3(normal.z, 0, -normal.x);
	const Vector3 pos = planeXZ[idx];
	const Vector3 pos2 = pos + normal * 0.5;
	glBegin(GL_LINES);
	glVertex3f(pos.x, pos.y, pos.z);
	glVertex3f(pos2.x, pos2.y, pos2.z);
	glEnd();
	glPushMatrix();
	glTranslatef(pos2.x, pos2.y, pos2.z);
	glScalef(0.1, 0.1, 0.1);
	font.Paint(text);
	glPopMatrix();

}

void LastModel::UpdateForm(const Insole &insole,
		const FootMeasurements &measurements) {

//	resized.smooth = true;
	tg = TestGrid(Vector3(-0.1, -0.1, -0.2), Vector3(0.3, 0.1, 0.2));
	tg.SetCellSize(0.01);
	tg.Reset();

	Polygon3 lastPlane = planeXZ;
	Polygon3 lastLeft = bottomleft;
	Polygon3 lastRight = bottomright;
	Polygon3 insoleCenter;	// = insole.inside;

	throw std::runtime_error(
			__FILE__ " - inside and outside not part of insole anymore. Modify algorithm!");

	for (size_t n = 0; n < insoleCenter.Size(); n++)
		insoleCenter[n].y = 0.0;
	Polygon3 lastCenter;
	for (size_t n = idxHeelPoint; n <= idxToePoint; n++)
		lastCenter.AddVertex(lastPlane[n]);

	Polynomial centerHeight;
	{
		const double L0 = lastCenter.GetLength();
//			const double v0 =
//					(lastPlane[idxHeelPoint] - lastPlane[idxZero]).Abs() / 2;
//			const double v1 = (lastPlane[idxWaistBottom]
//					- lastPlane[idxWaistTop]).Abs() / 2;
//			const double v2 = (lastPlane[idxLittleToeBottom]
//					- lastPlane[idxLittleToeTop]).Abs() / 2;
//			const double v3 = (lastPlane[idxBigToeBottom]
//					- lastPlane[idxBigToeTop]).Abs() / 2;
//			centerHeight = Polynomial::ByBezier(v0, v1, v2, v3);
		centerHeight = Polynomial::ByBezier(L0 / 5, L0 / 5, L0 / 10, L0 / 10);
	}

	{
		// Resize last to fit the length of the insole.
		const double scale = insoleCenter.GetLength() / lastCenter.GetLength();
		AffineTransformMatrix m;
		m.ScaleGlobal(scale, scale, scale);
		m *= AffineTransformMatrix::RotationXYZ(0, heela, 0);

		// Move heelpoint to insole
		Vector3 shift = insoleCenter[0] - m(lastCenter[0]);
		m.TranslateGlobal(shift.x, shift.y, shift.z);

		Transform(m);
		lastLeft.Transform(m);
		lastRight.Transform(m);
		lastPlane.Transform(m);
		lastCenter.Transform(m);
	}

	{
		// Adjust last angles

		Polynomial pDistance = Polynomial::ByValue(0, 0, 1,
				insoleCenter.GetLength());
		Bender b;

		Vector3 c0 = (lastPlane[idxHeelPoint] + lastPlane[idxTop]) / 2;
		b.from0.SetOrigin(c0);
		b.from0.SetEz((lastPlane[idxTop] - lastPlane[idxHeelCenter]).Normal());
		b.from0.SetEy(Vector3(0, 1, 0));
		b.from0.CalculateEx();

		Vector3 c1 = (lastPlane[idxWaistBottom] + lastPlane[idxWaistTop]) / 2;
		b.from1.SetOrigin(c1);
		b.from1.SetEz(
				(lastPlane[idxWaistTop] - lastPlane[idxWaistBottom]).Normal());
		b.from1.SetEy(Vector3(0, 1, 0));
		b.from1.CalculateEx();

		auto I0 = insoleCenter.At(0.05);
		auto I1 = insoleCenter.At(0.4);
		auto A0 = lastCenter.At(0.05);
		auto A1 = lastCenter.At(0.4);

		double I0a = atan2(I0.dir.z, I0.dir.x);
		double I1a = atan2(I1.dir.z, I1.dir.x);
		double A0a = atan2(A0.dir.z, A0.dir.x);
		double A1a = atan2(A1.dir.z, A1.dir.x);

		double da0 = A0a - I0a;
		double da = (A1a - A0a) - (I1a - I0a);

		AffineTransformMatrix d = b.from1 / b.from0;

		b.to0 = b.from0;
		b.to0 = b.to0 * AffineTransformMatrix::RotationXYZ(0, da0, 0);

//		d = AffineTransformMatrix::RotationXYZ(0, da0 / 2, 0) * d
//				* AffineTransformMatrix::RotationXYZ(0, da0 / 2, 0);
		b.to1 = b.to0 * d;
//		b.to1 = b.to1 * AffineTransformMatrix::RotationXYZ(0, da0, 0);
		b.Calculate();
//		resized.Transform(b);
//		lastLeft.ApplyTransformation(b);
//		lastRight.ApplyTransformation(b);
//		lastPlane.ApplyTransformation(b);
//		lastCenter.ApplyTransformation(b);
//		tg.Transform(b);
	}

	{
		// Align last along the center of the insole
		const double L0 = lastCenter.GetLength();
		const size_t N = 9;
		centerHeight = Polynomial::ByBezier(L0 / 5, L0 / 10, L0 / 10, L0 / 10);
		centerHeight.ScaleX(N + 2);
		centerHeight.ShiftX(-1);
		Polynomial pDistance = Polynomial::ByValue(-1, 0, N + 1,
				insoleCenter.GetLength());
//		DEBUGOUT<< centerHeight << "\n";
		for (size_t n = 0; n < N; n++) {
			const double L0 = pDistance(n);
			const double L1 = pDistance(n + 1);
			auto I0 = insoleCenter.At(L0);
			auto I1 = insoleCenter.At(L1);
			auto A0 = lastCenter.At(L0);
			auto A1 = lastCenter.At(L1);
			const double h0 = 0; //centerHeight(n);
			const double h1 = 0; //centerHeight(n + 1);

			Bender b;
			b.from0.SetEx(A0.dir);
			b.from0.SetEy(Vector3(0, 1, 0));
			b.from0.CalculateEz();
			b.from0.SetOrigin(A0.pos + b.from0.GetEz() * h0);
			b.from1.SetEx(A1.dir);
			b.from1.SetEy(Vector3(0, 1, 0));
			b.from1.CalculateEz();
			b.from1.SetOrigin(A1.pos + b.from0.GetEz() * h1);
			b.to0.SetEx(I0.dir);
			b.to0.SetEy(Vector3(0, 1, 0));
			b.to0.CalculateEz();
			b.to0.SetOrigin(I0.pos);
			b.to1.SetEx(I1.dir);
			b.to1.SetEy(Vector3(0, 1, 0));
			b.to1.CalculateEz();
			b.to1.SetOrigin(I1.pos);
			b.Calculate();

//			resized.Transform(b);
//			lastLeft.ApplyTransformation(b);
//			lastRight.ApplyTransformation(b);
//			lastPlane.ApplyTransformation(b);
//			lastCenter.ApplyTransformation(b);

//			tg.Transform(b);
		}
	}

	{
		// Align centerlines

		TransformationMixer m;
		const size_t N = 10;
		Polynomial pDistance = Polynomial::ByValue(0, -0.5, N - 1, 0.5);
		pDistance.ScaleY(0.95);
		pDistance.ShiftY(0.5);
		pDistance.ScaleY(insoleCenter.GetLength());
		Polynomial dDistance = pDistance.Derivative();
		const double width = measurements.footLength->ToDouble() / N;
		auto func = Kernel::Scale(Kernel::Cauchy, width);
		for (size_t n = 0; n < N; n++) {
			const double L0 = pDistance(n);
			auto I0 = insoleCenter.At(L0);
			auto A0 = lastCenter.At(L0);

			Vector3 translate = I0.pos - A0.pos;
			AffineTransformMatrix tr;
			tr.TranslateGlobal(translate.x, translate.y, translate.z);
			m.AddPlane(A0.pos, A0.dir, func, tr);
//			m.AddCylinder(A0.p, Vector3(0, 1, 0), func, tr);
		}
		Transform(m);
		lastLeft.Transform(m);
		lastRight.Transform(m);
		lastPlane.Transform(m);
		lastCenter.Transform(m);
//		tg.Transform(m);
	}
	/*
	 {
	 TransformationMixer m;
	 AffineTransformMatrix tr;

	 m.SetBackground(1.0);

	 auto func = Kernel::Scale(Kernel::Gaussian, 0.02,
	 1.0 / Kernel::Gaussian(0));

	 tr.TranslateGlobal(0, 0, 0.2);
	 m.AddCylinder(Vector3(0.2, 0, 0), Vector3(0, 1, 0), func, tr);
	 //		m.AddSphere(Vector3(0.2, 0, 0), func, tr);
	 tr.TranslateGlobal(0, 0, -0.4);
	 m.AddCylinder(Vector3(0.0, 0, 0), Vector3(0, 1, 0), func, tr);

	 tg.Transform(m);
	 }
	 */
	{
		// Scale width of the last

		TransformationMixer m;
		const size_t N = 15;
		Polynomial pDistance = Polynomial::ByValue(0, -0.5, N - 1, 0.5);
		pDistance.ScaleY(0.95);
		pDistance.ShiftY(0.5);
		pDistance.ScaleY(insoleCenter.GetLength());
		Polynomial dDistance = pDistance.Derivative();
		const double width = measurements.footLength->ToDouble() / N;
		auto func = Kernel::Scale(Kernel::Gaussian, width);

		Polygon3 sideCenter = lastLeft;
		for (size_t n = 0; n < sideCenter.Size(); n++)
			sideCenter[n] = (sideCenter[n] + lastRight[n]) / 2.0;

		for (size_t n = 0; n < N; n++) {
			const double L0 = pDistance(n);

			auto I0 = insoleCenter.At(L0);
			size_t idx = I0.idx;
			if (I0.rel > 0.5)
				++idx;
			Vector3 insoleR; // = insole.inside[idx];
			Vector3 insoleL; // = insole.outside[idx];

			auto A0 = lastCenter.At(L0);
			auto [lastIdx, lastGroup] = sideCenter.ClosestPoint(A0.pos);

			Vector3 lastR = lastRight[lastIdx];
			Vector3 lastL = lastLeft[lastIdx];

			AffineTransformMatrix tr;
			const double sc = (insoleR.y - insoleL.y) / (lastR.y - lastL.y);
			tr.ScaleGlobal(1, sc, 1);
			tr.TranslateGlobal(0, (insoleR.y - lastR.y * sc), 0);

			m.AddPlane(A0.pos, A0.dir, func, tr);
//			m.AddCylinder(A0.p, Vector3(0, 1, 0), func, tr);
		}
		Transform(m);
		lastLeft.Transform(m);
		lastRight.Transform(m);
		lastPlane.Transform(m);
		lastCenter.Transform(m);
//		tg.Transform(m);
	}
	/*
	 {
	 // Scale girth of the last
	 TransformationMixer m;
	 //		m.SetBackground(1.0);
	 const double width = insoleCenter.GetLength() / 4;
	 auto func = Kernel::Scale(Kernel::NormalizeY(Kernel::Gaussian), width);

	 //		Polynomial E;
	 //		E.Resize(9);
	 //		E[8] = 1.0;
	 //		E[6] = 1.0 / 4.0;
	 //		E[4] = 1.0 / 64.0;
	 //		E[2] = 1.0 / 256.0;
	 //		E[0] = 25.0 / 16384.0;
	 //		E *= M_PI;

	 //		DEBUGOUT << E << "\n";
	 {
	 LastModel::Cut cut = CalculateCut(resized, lastPlane[idxHeelPoint],
	 lastPlane[idxTop]);
	 const AffineTransformMatrix tr = RotatedScale(cut,
	 measurements.heelGirth->ToDouble());
	 m.AddPlane(cut.m.GetOrigin(), cut.m.GetEz(), func, tr);
	 }

	 {
	 LastModel::Cut cut = CalculateCut(resized,
	 lastPlane[idxWaistBottom], lastPlane[idxWaistTop]);
	 const AffineTransformMatrix tr = RotatedScale(cut,
	 measurements.waistGirth->ToDouble());
	 m.AddPlane(cut.m.GetOrigin(), cut.m.GetEz(), func, tr);
	 }
	 {
	 LastModel::Cut cut = CalculateCut(resized,
	 lastPlane[idxLittleToeBottom], lastPlane[idxLittleToeTop]);
	 const AffineTransformMatrix tr = RotatedScale(cut,
	 measurements.littleToeGirth->ToDouble());
	 m.AddPlane(cut.m.GetOrigin(), cut.m.GetEz(), func, tr);
	 }
	 {
	 LastModel::Cut cut = CalculateCut(resized,
	 lastPlane[idxBigToeBottom], lastPlane[idxBigToeTop]);
	 const AffineTransformMatrix tr = RotatedScale(cut,
	 measurements.bigToeGirth->ToDouble());
	 m.AddPlane(cut.m.GetOrigin(), cut.m.GetEz(), func, tr);
	 }

	 //		m.SetBackground(1.0);
	 resized.Transform(m);
	 lastLeft.Transform(m);
	 lastRight.Transform(m);
	 lastPlane.Transform(m);
	 lastCenter.Transform(m);
	 tg.Transform(m);

	 }

	 {
	 // Align centerlines (again)

	 TransformationMixer m;
	 const size_t N = 10;
	 Polynomial pDistance = Polynomial::ByValue(0, -0.5, N - 1, 0.5);
	 pDistance.ScaleY(0.98);
	 pDistance.ShiftY(0.5);
	 pDistance.ScaleY(insoleCenter.GetLength());
	 Polynomial dDistance = pDistance.Derivative();
	 const double width = measurements.footLength->ToDouble() / N * 2.0;
	 resized.const double scale = insoleCenter.GetLength() / lastCenter.GetLength();

	 auto func = Kernel::Scale(Kernel::NormalizeY(Kernel::Gaussian), width);

	 for (size_t n = 0; n < N; n++) {
	 const double L0 = pDistance(n);
	 auto I0 = insoleCenter.At(L0);
	 auto A0 = lastCenter.At(L0 / scale);

	 Vector3 translate = I0.pos - A0.pos;
	 AffineTransformMatrix tr;
	 tr.TranslateGlobal(translate.x, translate.y, translate.z);
	 //			m.AddPlane(A0.p, A0.d, func, tr);
	 m.AddCylinder(A0.pos, Vector3(0, 1, 0), func, tr);
	 }
	 resized.Transform(m);
	 lastLeft.Transform(m);
	 lastRight.Transform(m);
	 lastPlane.Transform(m);
	 lastCenter.Transform(m);
	 //		tg.Transform(m);
	 }

	 {
	 // Scale width of the last (again)

	 TransformationMixer m;
	 const size_t N = 15;
	 Polynomial pDistance = Polynomial::ByValue(0, -0.5, N - 1, 0.5);
	 pDistance.ScaleY(0.95);
	 pDistance.ShiftY(0.5);
	 pDistance.ScaleY(insoleCenter.GetLength());
	 Polynomial dDistance = pDistance.Derivative();
	 const double width = measurements.footLength->ToDouble();
	 auto func = Kernel::Scale(Kernel::Gaussian, width / N);
	 Polygon3 sideCenter = lastLeft;
	 for (size_t n = 0; n < sideCenter.Size(); n++)
	 sideCenter[n] += (sideCenter[n] + lastRight[n]) / 2.0;

	 for (size_t n = 0; n < N; n++) {
	 const double L0 = pDistance(n);

	 auto I0 = insoleCenter.At(L0);
	 size_t idx = I0.idx;
	 if (I0.rel > 0.5)
	 ++idx;
	 Vector3 insoleR = insole.inside[idx];
	 Vector3 insoleL = insole.outside[idx];

	 auto A0 = lastCenter.At(L0);
	 auto [lastIdx, lastGroup] = sideCenter.ClosestPoint(A0.pos);

	 Vector3 lastR = lastRight[lastIdx];
	 Vector3 lastL = lastLeft[lastIdx];

	 AffineTransformMatrix tr;
	 const double sc = (insoleR.y - insoleL.y) / (lastR.y - lastL.y);
	 tr.ScaleGlobal(1, sc, 1);
	 tr.TranslateGlobal(0, (insoleR.y - lastR.y * sc), 0);

	 m.AddPlane(A0.pos, A0.dir, func, tr);
	 //			m.AddCylinder(A0.p, Vector3(0, 1, 0), func, tr);
	 }
	 resized.Transform(m);
	 lastLeft.Transform(m);
	 lastRight.Transform(m);
	 lastPlane.Transform(m);
	 lastCenter.Transform(m);
	 //		tg.Transform(m);
	 }

	 {
	 LastModel::Cut cutHeelGirth = CalculateCut(resized,
	 lastPlane[idxHeelPoint], lastPlane[idxTop]);
	 LastModel::Cut cutWaistGirth = CalculateCut(resized,
	 lastPlane[idxWaistBottom], lastPlane[idxWaistTop]);
	 LastModel::Cut cutLittleToeGirth = CalculateCut(resized,
	 lastPlane[idxLittleToeBottom], lastPlane[idxLittleToeTop]);
	 LastModel::Cut cutBigToeGirth = CalculateCut(resized,
	 lastPlane[idxBigToeBottom], lastPlane[idxBigToeTop]);

	 DEBUGOUT << "HeelGirth - " << measurements.heelGirth->ToDouble() * 100
	 << " cm -> " << cutHeelGirth.p.GetLength() * 100 << " cm\n";
	 DEBUGOUT << "WaistGirth - " << measurements.waistGirth->ToDouble() * 100
	 << " cm -> " << cutWaistGirth.p.GetLength() * 100 << " cm\n";
	 DEBUGOUT << "LittleToeGirth - "
	 << measurements.littleToeGirth->ToDouble() * 100 << " cm -> "
	 << cutLittleToeGirth.p.GetLength() * 100 << " cm\n";
	 DEBUGOUT << "BigToeGirth - "
	 << measurements.bigToeGirth->ToDouble() * 100 << " cm -> "
	 << cutBigToeGirth.p.GetLength() * 100 << " cm\n";
	 }

	 //	TransformationMixer mixer;
	 //	resized.Transform(mixervector);

	 {
	 const Vector3 temp = lastPlane[idxHeelCenter + 1]
	 - lastPlane[idxHeelCenter];
	 const Vector3 nHeel(-temp.z, 0, temp.x);
	 const double hHeel =
	 (lastPlane[idxHeel] - lastPlane[idxHeelCenter]).Dot(
	 nHeel.Normal());

	 const Vector3 temp2 = lastPlane[idxToeCenter + 1]
	 - lastPlane[idxToeCenter];
	 const Vector3 nToe(-temp2.z, 0, temp2.x);
	 const double hToe =
	 (lastPlane[idxToeTip] - lastPlane[idxToeCenter]).Dot(
	 nToe.Normal());
	 height08 = Polynomial::ByBezier(hHeel, hHeel, hToe);
	 }

	 return;

	 BoundingBox bb;
	 for(size_t i = 0; i < hull.GetCountVertices(); i++)
	 bb.Insert(hull.GetVertex(i));
	 AffineTransformMatrix bbc = bb.GetCoordinateSystem();

	 scalevalues.resize(3, 0.001);

	 NelderMeadOptimizer optim;
	 optim.param.push_back(scalevalues[0]);
	 optim.param.push_back(scalevalues[1]);
	 optim.param.push_back(scalevalues[2]);

	 ParameterLimits limits;
	 limits.AddLimit(0, 0.001, 100, 1, 1000);
	 limits.AddLimit(1, 0.001, 100, 1, 1000);
	 limits.AddLimit(2, 0.001, 100, 1, 1000);

	 optim.simplexSpread = 1e-3;
	 optim.errorLimit = 1e-8;
	 optim.reevalBest = true;
	 optim.Start();
	 while(optim.IsRunning()){
	 Polynomial s = Polynomial::ByDerivative(0, optim.param[1], 0, 1,
	 optim.param[2], 0);
	 const double sx = optim.param[0];
	 for(size_t n = 0; n < resized.Size(); n++){
	 Vector3 temp = hull[n];
	 const double x = bbc.LocalX(temp);
	 const double syz = s.Evaluate(x);
	 resized[n].Set(temp.x * sx, temp.y * syz, temp.z * syz);
	 }
	 double err = limits.Evaluate(optim.param);

	 err += pow((center.GetLength() * sx) - measurements.footLength->ToDouble(),
	 2.0);

	 AffineTransformMatrix bbc2 = bbc;
	 bbc2.ScaleGlobal(sx, s[0.5], s[0.5]);

	 Polygon3 section = resized.IntersectPlane(Vector3(1, 0, 0),
	 bbc2.GlobalX(0.75));
	 Polygon3 section2 = resized.IntersectPlane(Vector3(1, 0, 0),
	 bbc2.GlobalX(0.5));
	 Polygon3 section3 = resized.IntersectPlane(Vector3(1, 0, -0.5),
	 bbc2.GlobalX(0.4));

	 err += pow((section.GetLength() - measurements.ballGirth->ToDouble()), 2.0);
	 err += pow((section2.GetLength() - measurements.waistGirth->ToDouble()), 2.0);
	 err += pow((section3.GetLength() - measurements.instepGirth->ToDouble()),
	 2.0);
	 loop = section2;
	 //		if(mirrored){
	 //			data.Scale(optim.param[0], -optim.param[1], optim.param[1]);
	 //		}else{
	 //			data.Scale(optim.param[0], optim.param[1], optim.param[1]);
	 //		}
	 //		double err = pow(
	 //				measurements.footLength->ToDouble() - data.dx * data.sections.size(),
	 //				2.0)
	 //				+ pow(
	 //						measurements.ballGirth->ToDouble()
	 //								- data.sections[37].GetLength(), 2);
	 //		if(optim.param[0] <= 0) err += 1e6 - optim.param[0];
	 //		if(optim.param[1] <= 0) err += 1e6 - optim.param[1];

	 DEBUGOUT << "ballGirth: " << measurements.ballGirth->ToDouble() << " <== "
	 << section.GetLength() << "\n";
	 DEBUGOUT << "waistGirth: " << measurements.waistGirth->ToDouble() << " <== "
	 << section2.GetLength() << "\n";
	 DEBUGOUT << "instepGirth: " << measurements.instepGirth->ToDouble()
	 << " <== " << section3.GetLength() << "\n";

	 optim.SetError(err);
	 }

	 DEBUGOUT << "Min. Err: " << optim.ResidualError() << "\n";
	 //	DEBUGOUT << data.dx * data.sections.size() << " m\n";
	 //	DEBUGOUT << data.sections[37].GetLength() << " m\n";
	 */

	TransformationMixer tm;

	const Vector3 bendpoint(1.2, 0, -0.2);

	AffineTransformMatrix rot;
	rot = AffineTransformMatrix::RotationAroundVector( { 0, 1, 0 },
			-45.0 * M_PI / 180.0);
	rot.TranslateLocal(-1.2, 0, 0.2);
	rot.TranslateGlobal(1.2, 0, -0.2);

	tg.Set(Vector3(-2, -1, -1), Vector3(3, 1, 1));
	tg.SetCellSize(0.1);

	tm.AddPlane( { 1.2 + 0.5, 0, -0.2 }, { 1, 0, 0 },
			Kernel::Shift(Kernel::Scale(Kernel::Integrated::Cosine, 0.5), 0.0),
			rot);
//	tm[0].TranslateLocal(0, 0, -1);
//	tm[0] = rot;

	double angle = 45.0 / 180.0 * M_PI;
	auto unitBend = [angle](const Vector3 &v) -> Vector3 {
		if (v.x <= 0.0)
			return v;
		const double fp = tan(fmin(v.x, 1.0) * angle / 2.0) * angle;
		const double sc = (fp + fmax(v.x - 1.0, 0.0)) / (v.x - fp);
		const double co = cos(angle * fmin(v.x, 1.0));
		const double si = sin(angle * fmin(v.x, 1.0));
		const Vector3 d((v.x - fp) * sc, v.y, v.z);
		const Vector3 r(co * d.x - si * d.z, d.y, co * d.z + si * d.x);
		return Vector3((r.x + fp), r.y, r.z);
	};

//	for(size_t n = 0; n < resized.Size(); n++)
//		resized[n] = unitBend(raw[n] - bendpoint) + bendpoint;
//	for(auto & p : tg.p)
//		p = unitBend(p - bendpoint) + bendpoint;

	{
		Bender b;
		b.from0.TranslateGlobal(1, 0, 0);

		b.from1.TranslateGlobal(1, 0, 0);
//	b.from1 *= AffineTransformMatrix::RotationXYZ(0, -M_PI / 6, 0);
		b.from1.TranslateLocal(1, 0, 0);
//	b.from1 *= AffineTransformMatrix::RotationXYZ(0, -M_PI / 6, 0);

		b.to0.TranslateGlobal(1, 0, 0);

		b.to1.TranslateLocal(1, 0, 0);
		b.to1 *= AffineTransformMatrix::RotationXYZ(0, M_PI / 10, 0);
		b.to1.TranslateLocal(1, 0, 0);
		b.to1 *= AffineTransformMatrix::RotationXYZ(0, M_PI / 10, 0);

		b.Calculate();

//	tg.p[0] = b(tg.p[16000]);

//		for(auto & p : tg.p)
//			p = b(p);
	}
	{
		Bender b;
		b.from0.TranslateGlobal(1, 0, 0);

		b.from1.TranslateGlobal(1, 0, 0);
		b.from1 *= AffineTransformMatrix::RotationXYZ(0, M_PI / 10, 0);
		b.from1.TranslateLocal(1, 0, 0);
		b.from1 *= AffineTransformMatrix::RotationXYZ(0, M_PI / 10, 0);

		b.to0.TranslateGlobal(1, 0, 0);

		b.to1.TranslateLocal(1, 0, 0);
		//	b.to1 *= AffineTransformMatrix::RotationXYZ(0, M_PI / 10, 0);
		b.to1.TranslateLocal(1, 0, 0);
		//	b.to1 *= AffineTransformMatrix::RotationXYZ(0, M_PI / 10, 0);

		b.Calculate();

		Transform(b);
	}

//	NelderMeadOptimizer optim;
//	optim.simplexSpread = 1e-3;
//	optim.errorLimit = 1e-8;
//	optim.reevalBest = true;
//
//	optim.param.push_back(sx);
//	optim.param.push_back((sy + sz) / 2);
//	optim.Start();
//	while(optim.IsRunning()){
//		center.InitAngle(0);
//		center.SetBend(-1.2, 0.3, 0.3, BendLine::Epanechnikov);
//		center.SetBend(1.3, 0.75, 0.03, BendLine::Gauss);
//		center.Finish();
//	}
}

LastModel::LastModel(const Geometry &geo) :
		ObjectGeometry(geo) {
}

void LastModel::Transform(std::function<Vector3(Vector3)> func) {
	for (auto &p : tg.p)
		p = func(p);

	for (size_t n = 0; n < CountVertices(); n++)
		v[n] = func(v[n]);
}

void LastModel::Mirror() {
	AffineTransformMatrix m;
	m.ScaleGlobal(1, -1, 1);
	Transform(m);
}

