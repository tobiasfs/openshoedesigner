///////////////////////////////////////////////////////////////////////////////
// Name               : LastAnalyse.cpp
// Purpose            : 
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 10.11.2024
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
#include "LastAnalyse.h"

#include "../../Config.h"
#include "../../math/FourierTransform.h"
#include "../../math/Kernel.h"
#include "../../math/KernelDensityEstimator.h"
#include "../../math/PCA.h"
#include "../../3D/OpenGL.h"
#include "../../3D/OpenGLMaterial.h"
#include "../../math/MEstimator.h"

#include <iostream>

bool LastAnalyse::CanRun() {
	return raw.use_count() > 0;
}

bool LastAnalyse::HasToRun() {
	return raw->IsModified();
}

void LastAnalyse::Run() {
	last = std::make_shared<LastModel>(*raw);
	AnalyseForm();
	last->Modify(true);
}

bool LastAnalyse::AnalyseForm() {
	DEBUGOUT << "*** Analyzing last ***\n";
	ReorientPCA();
	if (!ReorientSymmetry())
		return false;
	if (!ReorientSole())
		return false;
	ReorientFrontBack();
	ReorientLeftRight();

	FindAndReorientCenterplane();

	if (!FindMarker())
		return false;

	last->MarkMeasurements();

//	kde.Clear();
//	kde.XLinspace(0, 2 * M_PI, 360);
//	kde.XSetCyclic(2 * M_PI);
//	for(size_t n = 0; n < center.Size(); ++n){
//		const Vector3 temp = (center[(n + 1) % center.Size()] - center[n]);
//		const double a = atan2(-temp.x, temp.z);
//		kde.Insert(a, Kernel::Picard, temp.Abs(), 0.3);
//	}
//	kde.Attenuate(M_PI_2, Kernel::Tricube, 1.0, M_PI);
//	kde.Attenuate(M_PI_2, Kernel::Uniform, 1.0, 1.6);
//
//	kde.Normalize();
//	auto results = kde.FindPeaks();

//	FindOutline();

// For Testing purposes change front and back.
//	{
//		hull.ApplyTransformation(
//				AffineTransformMatrix::RotationAroundVector(Vector3(0, 0, 1),
//				M_PI));
//		bb.Clear();
//		for(size_t i = 0; i < hull.GetVertexCount(); ++i)
//			bb.Insert(hull.GetVertex(i));
//	}

//	MatlabFile mf("/tmp/test.mat");

//	return true;

//	{
//		const double Lmax = shape2.GetLength();
//		for(size_t n = 0; n < shape2.Size(); ++n){
//			const Vector3 temp = (shape2[(n + 1) % shape2.Size()] - shape2[n]);
//			double a = atan2(temp.y, -temp.z);
//			cde.Add(CoreDensityEstimator::Epanechnikov, 0.5, a,
//					temp.Abs() / Lmax);
//		}
//	}

//	coordsys = rawBB.GetCoordinateSystem();

//	cde.Add(CoreDensityEstimator::Epanechnikov, 1, 1, 1);
//	cde.Add(CoreDensityEstimator::Epanechnikov, 0.5, 2.5, 1);
	{
//		coordsys.SetOrigin(
//				Vector3(rawBB.xmin, (rawBB.ymax + rawBB.ymin) / 2, rawBB.zmin));
//		coordsys.SetEx(Vector3(rawBB.xmax - rawBB.xmin, 0, 0));
//		coordsys.SetEy(Vector3(0, 0, 1));
//		coordsys.CalculateEz();

//		PolyFilter pf;
//		pf.Init(3, bottom.Size());
//		Polynomial pbo = pf.Filter(bottom.GetZVectorD());
//		center.Clear();
//		for(double n = 0; n < bottom.Size(); n += 0.4){
//			center.PushBack(n/(double)bottom.Size(), pbo.Evaluate(n));
//		}

//		ParameterLimits lim;
//		NelderMeadOptimizer optim;
//
//		optim.param.clear();
//
//		optim.param.push_back(0.3);
//		optim.param.push_back(-1.0);
//		optim.param.push_back(0.2);
//
//		lim.AddLimit(0, -0.5, 0.5);
//		lim.AddLimit(1, -5, -0.5);
//		lim.AddLimit(2, 0.1, 0.3);
//
//		optim.param.push_back(0.8);
//		optim.param.push_back(1.0);
//		optim.param.push_back(0.2);
//
//		lim.AddLimit(3, 0.5, 0.9);
//		lim.AddLimit(4, 0.5, 2);
//		lim.AddLimit(5, 0.1, 0.3);
//
//		optim.param.push_back(-20 * M_PI / 180);
//		lim.AddLimit(6, -20 * M_PI / 180, 20 * M_PI / 180);
//
//		optim.reevalBest = true;
//		optim.evalLimit = 100;
//
//		bottom.Export("/tmp/bot.mat");
//
//		Vector3 c = bottom[bottom.Size() / 2];
//
//		optim.Start();
//		while(optim.IsRunning()){
//
//			center.XLinspace(0, 1.4, 101);
//			center.YInit(0);
//			center.Insert(optim.param[0], optim.param[1], optim.param[2],
//					KernelDensityEstimator::GaussianKernel);
//			center.Insert(optim.param[3], optim.param[4], optim.param[5],
//					KernelDensityEstimator::GaussianKernel);
//
//			center.Export("/tmp/cent_0.mat");
//
//			center.Integrate();
//			double off1 = center.YatX(coordsys.LocalX(c));
//			center += optim.param[6] - off1;
//			center.YLimit(-1.4, 1.4);
//
//			center.Export("/tmp/cent_1.mat");
//
//			center.AngleToPos();
//			center.Export("/tmp/cent_2.mat");
//
//			double off2 = center.YatX(coordsys.LocalX(c));
//			center += coordsys.LocalY(c) - off2;
//
//			center.Export("/tmp/cent_3.mat");
//
//			double err = 0;
//			for(size_t n = 0; n < bottom.Size(); ++n){
//				const double x = coordsys.LocalX(bottom[n]);
//				const double y = center.YatX(x);
//				const double y_target = coordsys.LocalY(bottom[n]);
//				err += (y - y_target) * (y - y_target);
//			}
//			optim.SetError(err + lim.Evaluate(optim.param));
//		}
//
//		DEBUGOUT << "param = [";
//		for(size_t n = 0; n < optim.param.size(); ++n){
//			if(n > 0) DEBUGOUT << ", ";
//			DEBUGOUT << optim.param[n];
//		}
//		DEBUGOUT << "];\n";
//		DEBUGOUT << "res_err = " << optim.ResidualError() << ";\n";
//		DEBUGOUT << "eval_count = " << optim.EvaluationsDone() << ";\n";

//		coordsys.SetOrigin(Vector3());
//				coordsys.SetEx(Vector3(0, 1, 0));
//				coordsys.SetEy(Vector3(0, 0, 1));
//				coordsys.CalculateEz();

//			Polygon3 pmin;
//			for(size_t n = Nmin; n < (N + Nmin); ++n){
//				const size_t m = n % N;
//				pmin += loop[m];
//				if(m == Nmax) break;
//			}
//			Polygon3 pmax;
//			for(size_t n = Nmax; n < (N + Nmax); ++n){
//				const size_t m = n % N;
//				pmax += loop[m];
//				if(m == Nmin) break;
//			}
//
//			pmin.Resample(100);
//			pmax.Resample(100);
//			pmax.Reverse();
//
//			for(size_t n = 0; n < 100; ++n)
//				pmin[n] += pmax[n];
//			pmin /= 2.0;
//
//			loop = pmin;
//		loop.Resample(100);
//		Vector3 rot = loop.GetRotationalAxis();
//		if(rot.y < 0) loop.Reverse();
//
//		coordsys.SetCenter(loop.GetCenter());
//		coordsys.ResetRotationAndScale();
	}
//	formfinder.AddPolygon(loop, loop.GetCount() / 8);

//	std::vector <Vector3>::iterator min = std::min_element(formfinder.b.begin(),
//			formfinder.b.end(), Vector3XLess);
//	size_t dist = std::distance(formfinder.b.begin(), min);
//	DEBUGOUT << dist << "\n";
//	std::rotate(formfinder.a.begin(), formfinder.a.begin() + dist,
//			formfinder.a.end());
//	std::rotate(formfinder.b.begin(), formfinder.b.begin() + dist,
//			formfinder.b.end());
//	std::rotate(formfinder.c.begin(), formfinder.c.begin() + dist,
//			formfinder.c.end());
//	std::rotate(formfinder.d.begin(), formfinder.d.begin() + dist,
//			formfinder.d.end());

//	MatlabMatrix F("F", 12, formfinder.a.size());
//	for(size_t n = 0; n < formfinder.a.size(); ++n){
//		F.Insert(formfinder.a[n].x);
//		F.Insert(formfinder.b[n].x);
//		F.Insert(formfinder.c[n].x);
//		F.Insert(formfinder.d[n].x);
//		F.Insert(formfinder.a[n].y);
//		F.Insert(formfinder.b[n].y);
//		F.Insert(formfinder.c[n].y);
//		F.Insert(formfinder.d[n].y);
//		F.Insert(formfinder.a[n].z);
//		F.Insert(formfinder.b[n].z);
//		F.Insert(formfinder.c[n].z);
//		F.Insert(formfinder.d[n].z);
//	}
//	F.Transpose();
//	mf.WriteMatrix(F);

//	MatlabMatrix P("P", 2, shape1.GetCount());
//			P.Insert(v0.Dot(localx));
//			P.Insert(v0.Dot(localy));
//	P.Transpose();
//	mf.WriteMatrix(P);
//
//
//	MatlabMatrix X("X", 3, ft.InRe.size());
//	for(size_t n = 0; n < ft.InRe.size(); ++n){
//		X.Insert(ft.t[n]);
//		X.Insert(ft.InRe[n]);
//		X.Insert(ft.InIm[n]);
//	}
//	X.Transpose();
//	mf.WriteMatrix(X);
//
//	MatlabMatrix Y("Y", 3, ft.f.size());
//	for(size_t n = 0; n < ft.f.size(); ++n){
//		Y.Insert(ft.f[n]);
//		Y.Insert(ft.OutRe[n]);
//		Y.Insert(ft.OutIm[n]);
//	}
//	Y.Transpose();
//	mf.WriteMatrix(Y);
//
//	MatlabMatrix S("S", symmetry.support.size());
//	for(size_t n = 0; n < symmetry.support.size(); ++n)
//		S.Insert(symmetry.support[n]);
//	mf.WriteMatrix(S);

//	mf.Close();

//	loop = shape1;

//	Vector3 temp;
//	for(size_t i = 0; i < loop.elements.Count(); i++)
//		temp += loop.elements[i];
//	pca.SetCenter(temp / loop.elements.Count());
//	for(size_t i = 0; i < loop.elements.Count(); i++)
//		pca.Add(loop.elements[i]);

	DEBUGOUT << "*** Analysis done ***\n";
	return true;
}

void LastAnalyse::ReorientPCA() { // Find orientation of mesh

	PCA pca;
	Vector3 center;
	for (size_t i = 0; i < last->VertexCount(); ++i)
		center += last->GetVertex(i);
	center /= (double) last->VertexCount();
	pca.SetCenter(center);
	for (size_t i = 0; i < last->VertexCount(); ++i)
		pca.Add(last->GetVertex(i));
	pca.Calculate();
// Make coordinate system right handed
	if ((pca.X * pca.Y).Dot(pca.Z) > 0)
		pca.Y = -pca.Y;
// Remove orientation
	AffineTransformMatrix temp(pca.X, pca.Y, pca.Z, pca.center);
	temp.Invert();
	last->Transform(temp);
}

bool LastAnalyse::ReorientSymmetry() {
// Scan Shape for symmetry
	{
//		AffineTransformMatrix comp;
//		comp *= AffineTransformMatrix::RotationAroundVector(Vector3(1, 0, 0),
//				1.0);
//		last->Transform(comp);
//		UpdateRawBoundingBox();
	}

	last->UpdateRawBoundingBox();
	AffineTransformMatrix bbc = last->rawBB.GetCoordinateSystem();

	Symmetry symmetry;
	symmetry.Init(180);
	for (double cut = 0.2; cut < 0.81; cut += 0.2) {
		Polygon3 section = last->IntersectPlane(Vector3(1, 0, 0),
				bbc.GlobalX(cut));

		Vector3 rot = section.GetRotationalAxis();
		if (rot.x < 0)
			rot = -rot;

		AffineTransformMatrix coordsys;
		coordsys.SetOrigin(section.GetCenter());
		coordsys.SetEx(Vector3(0, 1, 0));
		coordsys.SetEy(Vector3(0, 0, 1));
		coordsys.SetEz(Vector3(1, 0, 0));

		FourierTransform ft;
		ft.TSetSize(section.Size());
		for (size_t n = 0; n < section.Size(); ++n) {
			ft.t[n] = atan2(coordsys.LocalY(section[n]),
					coordsys.LocalX(section[n]));
			ft.InRe[n] = (section[n] - coordsys.GetOrigin()).Abs();
			ft.InIm[n] = 0.0;
		}
		ft.TUnwrap();
		ft.TSetLoopLength(2 * M_PI);
		ft.TScale(1.0 / (2 * M_PI));
		ft.FLinspace(0, 30, 31);
		ft.Transform();
		ft.SingleSidedResult();
		symmetry.AddTransform(ft);
	}

	symmetry.Normalize();
	auto results = symmetry.FindPeaks(0.01);
	if (results.empty())
		return false;
	AffineTransformMatrix comp;
	comp *= AffineTransformMatrix::RotationAroundVector(Vector3(1, 0, 0),
	M_PI_2 - results[0].x);
//	DEBUGOUT << "Peak at " << results[0].x * 180.0 / M_PI << "degrees.\n";
//	DEBUGOUT << "Rotate by: " << (M_PI_2 - results[0].x) * 180.0 / M_PI
//			<< " degrees.\n";

	last->Transform(comp);
	return true;
}

bool LastAnalyse::ReorientSole() {
	// Find sole
	// Class-global for debugging / painting during development
	KernelDensityEstimator kde;
	kde.Clear();
	kde.XLinspace(0, 2 * M_PI, 360);
	kde.XSetCyclic(2 * M_PI);

	last->UpdateRawBoundingBox();
	AffineTransformMatrix bbc = last->rawBB.GetCoordinateSystem();
	for (double cut = 0.2; cut < 0.81; cut += 0.2) {
		Polygon3 section = last->IntersectPlane(Vector3(1, 0, 0),
				bbc.GlobalX(cut));

		Vector3 rot = section.GetRotationalAxis();
		if (rot.x > 0)
			section.Reverse();

		const double Lmax = section.GetLength();
		for (size_t n = 0; n < section.Size(); ++n) {
			const Vector3 temp =
					(section[(n + 1) % section.Size()] - section[n]);
			double a = atan2(temp.y, -temp.z);
			kde.Insert(a, Kernel::Silverman, temp.Abs() / Lmax, 0.2);
		}
	}

	kde.Normalize();

	kde.Attenuate(0, Kernel::Cauchy, 0.75, 0.5);
	kde.Attenuate(M_PI, Kernel::Cauchy, 0.75, 0.5);

	auto results = kde.FindPeaks(0.1);
	if (results.empty())
		return false;
	AffineTransformMatrix comp;
	comp *= AffineTransformMatrix::RotationAroundVector(Vector3(1, 0, 0),
			3 * M_PI_2 - results[0].x);
	last->Transform(comp);
	return true;
}

void LastAnalyse::ReorientFrontBack() { // Test for front/back reversal

//		loop.Clear();
	std::vector<double> ratio;
	last->UpdateRawBoundingBox();
	AffineTransformMatrix bbc = last->rawBB.GetCoordinateSystem();
	for (double cut = 0.1; cut < 0.91; cut += 0.1) {
		Polygon3 section = last->IntersectPlane(Vector3(1, 0, 0),
				bbc.GlobalX(cut));
		BoundingBox temp;
		for (size_t n = 0; n < section.Size(); ++n)
			temp.Insert(section[n]);
		ratio.push_back(temp.GetSizeZ() / temp.GetSizeY());
//			loop += section.GetCenter();
	}
	PolyFilter pfr(2, ratio.size());
	Polynomial pr = pfr.Filter(ratio);
	pr.ShiftX(1);
	pr.ScaleX(0.1);

//		test.Clear();
//		for(double r = 0; r <= 1; r += 0.01)
//			test += Vector3(bb.xmin + bb.GetSizeX() * r, 0.0, pr.Evaluate(r));

	pr.Derive();
	if (pr(0.5) > 0.0) {
		// Reverse last
//		DEBUGOUT << "Reverse last.\n";
		last->Transform(
				AffineTransformMatrix::RotationAroundVector(Vector3(0, 0, 1),
				M_PI));
		last->UpdateRawBoundingBox();
	}

//		double maxr;
//		if(pr.ExtremumPos(maxr)){
//			DEBUGOUT << "maxr = " << maxr << "\n";
//		}
//
//		std::vector <double> temp = loop.GetZVectorD();
//		PolyFilter pf(3, loop.Size());
//		Polynomial pz = pf.Filter(temp);
//		pz.ShiftX(1);
//		pz.ScaleX(0.1);
//
//		loop.Clear();
//		for(double r = 0; r <= 1; r += 0.01)
//			loop += Vector3(bb.xmin + bb.GetSizeX() * r, 0.0, pz.Evaluate(r));
//
//		double pp;
//		double pn;
//		if(pz.ExtremumPos(pp) && pz.ExtremumNeg(pn)){
//			if(pp > pn && pp < 1.0 && pn > 0.0){
//				// Revert last
//				DEBUGOUT << "Rotate last by 180.\n";
//				hull.Transform(
//						AffineTransformMatrix::RotationAroundVector(
//								Vector3(0, 0, 1),
//								M_PI));
//			}
//		}
}

void LastAnalyse::ReorientLeftRight() {

// Scan for left/right

//		double ip = 0.5;
	Polygon3 loop;
	loop.Clear();

//		kde.XLinspace(0, 1, 100);
//		kde.XSetLinear();
	AffineTransformMatrix bbc = last->rawBB.GetCoordinateSystem();
	for (double cut = 0.1; cut < 0.91; cut += 0.1) {
		Polygon3 section = last->IntersectPlane(Vector3(1, 0, 0),
				bbc.GlobalX(cut));
		loop.AddEdgeToVertex(section.GetCenter());

		//			BoundingBox bb2;
		//			for(size_t n = 0; n < section.Size(); ++n)
		//				bb2.Insert(section[n]);

		//			kde.Insert(cut, bb2.GetSizeZ() / bb2.GetSizeY(), 0.3,
		//					KernelDensityEstimator::TriweightKernel);
	}
//		kde.NormalizeByWeightSum();

	std::vector<double> temp = loop.GetYVectorD();
	PolyFilter pf(2, loop.Size());
	Polynomial py = pf.Filter(temp);
	py.ShiftX(1);

	py.ScaleX(0.1);

//		test.Clear();
//		for(double r = 0; r <= 1; r += 0.01)
//			test += Vector3(bb.xmin + bb.GetSizeX() * r, py.Evaluate(r), 0.0);

	py.ScaleX(1.0 / last->rawBB.GetSizeX()); // Normale with lastlength
	py.ScaleY(1.0 / last->rawBB.GetSizeY()); // Normalize with lastwidth

			//		DEBUGOUT << "py = " << py << ";\n";

	double chir = py[0];
//		if(py[0] > 1.0) DEBUGOUT << "Right last\n";
//		if(py[0] < -1.0) DEBUGOUT << "Left last\n";

//		pf.Export("/tmp/pf.mat");

//		if(pz.InflectionPoint(ip)) DEBUGOUT << "Inflection point: " << ip
//				<< "\n";

//		coordsys.SetCenter(Vector3(bb.xmin, 0, 0));
//		coordsys.SetEx(Vector3(bb.GetSizeX(), 0, 0));
//		coordsys.SetEy(Vector3(0, 0, 1));
//		coordsys.CalculateEz();
	KernelDensityEstimator kde;
	kde.Clear();
	kde.XLinspace(0, 2 * M_PI, 360);
	kde.XSetCyclic(2 * M_PI);

	loop = last->IntersectPlane(Vector3(1, 0, 0), bbc.GlobalX(0.5));

	Vector3 rot = loop.GetRotationalAxis();
	if (rot.x > 0)
		loop.Reverse();

	const double Lmax = loop.GetLength();
	for (size_t n = 0; n < loop.Size(); ++n) {
		const Vector3 temp = (loop[(n + 1) % loop.Size()] - loop[n]);
		double a = atan2(temp.y, -temp.z);
		kde.Insert(a, Kernel::Sigmoid, temp.Abs() / Lmax, 0.3);
	}

	kde.Normalize();

//		kde.Attenuate(0, 0.75, 0.5, KernelDensityEstimator::CauchyKernel);
//		kde.Attenuate(M_PI, 0.75, 0.5, KernelDensityEstimator::CauchyKernel);

	double minRight = 1e9;
	double minLeft = 1e9;
	for (size_t n = 0; n < kde.Size(); ++n) {
		if (kde.X(n) > M_PI && kde.X(n) < 3 * M_PI_2 && kde.Y(n) < minRight)
			minRight = kde.Y(n);
		if (kde.X(n) < 2 * M_PI && kde.X(n) > 3 * M_PI_2 && kde.Y(n) < minLeft)
			minLeft = kde.Y(n);

	}

//		DEBUGOUT << "minLeft = " << minLeft << ";\n";
//		DEBUGOUT << "minRight = " << minRight << ";\n";

	chir += 4.0 * (minLeft - minRight) / (minLeft + minRight);

	if (chir < -0.5) {
		AffineTransformMatrix temp;
		temp.ScaleGlobal(1.0, -1.0, 1.0);
		last->Transform(temp);

		//			DEBUGOUT << "Flip sides left to right.\n";

		last->UpdateRawBoundingBox();
	}

//		DEBUGOUT << chir << " ";
//		if(chir > 0.0)
//			DEBUGOUT << " = Right last ";
//		else
//			DEBUGOUT << " = Left last ";
//		if(fabs(chir) < 0.5) DEBUGOUT << "(Insole recommended)";
//		DEBUGOUT << "\n";

}

void LastAnalyse::FindAndReorientCenterplane() {
	const double param_soleangle = 25.0 * M_PI / 180.0;

	AffineTransformMatrix bbc = last->rawBB.GetCoordinateSystem();
	Polygon3 section = last->IntersectPlane(Vector3(1, 0, 0), bbc.GlobalX(0.2));
	{
		Vector3 r = section.GetRotationalAxis();
		if (r.x < 0)
			section.Reverse();
	}
	const size_t N = section.Size();
	size_t Nmin = 0;
	for (size_t n = 0; n < N; ++n)
		if (section[n].z < section[Nmin].z)
			Nmin = n;

	size_t p0 = 0;
	size_t p1 = 0;
	for (size_t n = Nmin; n < Nmin + N; ++n) {
		const Vector3 n0 = section.Normal(n % N);
		const Vector3 n1 = section.Normal((n + 1) % N);
		const double a0 = atan2(n0.y, n0.z);
		const double a1 = atan2(n1.y, n1.z);
		if (a0 > param_soleangle && a1 <= param_soleangle)
			p0 = n;
		if (a0 >= -param_soleangle && a1 < -param_soleangle) {
			p1 = n + 1;
			break;
		}
	}
	Vector3 c = (section[p0 % N] + section[p1 % N]) / 2.0;

//	loop.Clear();
//	loop.InsertPoint(section[p0 % N], section.Normal(p0 % N));
//	loop.InsertPoint(section[p1 % N], section.Normal(p1 % N));

	last->planeXZ = last->IntersectPlane(Vector3(0, 1, 0), c.y);
	{
		// Reverse the loop to run in a positive direction in the x-z coordinate system.
		// (Left-handed coordinate system)
		Vector3 r = last->planeXZ.GetRotationalAxis();
		if (r.y > 0)
			last->planeXZ.Reverse();
	}

	{
		KernelDensityEstimator kde;
		kde.Clear();
		kde.XLinspace(0, 2 * M_PI, 360);
		kde.XSetCyclic(2 * M_PI);
		for (size_t n = 0; n < last->planeXZ.Size(); ++n) {
			const Vector3 temp = (last->planeXZ[(n + 1) % last->planeXZ.Size()]
					- last->planeXZ[n]);
			const double a = atan2(-temp.x, temp.z);
			const double hx = bbc.LocalX(last->planeXZ[n]);
			const double hz = bbc.LocalZ(last->planeXZ[n]);
			double g = Kernel::Integrated::Cosine(-5.0 * (hx - 0.5));
			g *= Kernel::Integrated::Cosine(5.0 * (hz - 0.5));
			kde.Insert(a, Kernel::Sigmoid, temp.Abs() * g, 0.2);
		}
		kde.Normalize();
		auto results = kde.FindPeaks();
		double topangle = results[0].x;

		AffineTransformMatrix temp = AffineTransformMatrix::RotationXYZ(0,
				topangle - M_PI_2, 0);
		last->Transform(temp);

		last->planeXZ.Transform(temp);
		last->UpdateRawBoundingBox();
		bbc = last->rawBB.GetCoordinateSystem();
		last->planeXZ.RotateOrigin(bbc.Transform(Vector3(0, 0.5, 3)));
	}

// Calculate the angles on the the shape
	{
		last->planeXZ.Filter(11);
		const size_t N = last->planeXZ.Size();
		last->angleXZ.Clear();
		const double Lmax = 1;		//last->planeXZ.GetLength();
		double L = 0.0;
		for (size_t n = 0; n < N; ++n) {
			Vector3 temp = (last->planeXZ[(n + 1) % N] - last->planeXZ[n]);
			const double h = atan2(temp.z, temp.x);
			last->angleXZ.PushBack((double) L / (double) Lmax, h);
			L += temp.Abs();
		}
		if (last->angleXZ[0] > 0.0)
			last->angleXZ[0] -= 2 * M_PI;
		last->angleXZ.Unwrap(M_PI);
	}
}

bool LastAnalyse::FindMarker() {
	auto toRad = [](double x) {
		return x * M_PI / 180.0;
	};
	auto toDeg = [](double x) {
		return x * 180.0 / M_PI;
	};

	const size_t N = last->planeXZ.Size();

// Temporary markerpoints for the top of the last
	{
		last->idxZero = last->angleXZ.IatY(toRad(-160),
				DependentVector::Direction::first_risingabove);
		if (last->idxZero >= N)
			last->idxZero = 0;
		last->idxTop = last->angleXZ.IatY(toRad(160),
				DependentVector::Direction::last_risingabove);
	}

// Temporary markerpoints for toes and heel
	{
		last->idxHeel = last->angleXZ.IatY(toRad(-90),
				DependentVector::Direction::first_risingabove);
		last->idxHeelPoint = last->angleXZ.IatY(toRad(-70),
				DependentVector::Direction::first_risingabove);
		last->idxToeTip = last->angleXZ.IatY(toRad(+90),
				DependentVector::Direction::first_risingabove);
		if (last->idxHeelPoint > last->angleXZ.Size())
			return false;
		if (last->idxToeTip > last->angleXZ.Size())
			return false;
	}

// Estimate the angles for the heel and ball area of the sole
	{
		MEstimator estheel;
		estheel.XLinspace(toRad(-90), toRad(90), 301);
		estheel.EstimateY(last->angleXZ, MEstimator::AndrewWave(), 0.03,
				last->idxHeelPoint, last->idxToeTip,
				Kernel::SubDiv(Kernel::Integrated::Triangular, 0.35 * 2.0 - 1.0,
						-0.2));
		estheel.Normalize();
		auto valleyheel = estheel.FindValleys();
		if (valleyheel.empty())
			return false;
		last->heela = valleyheel[0].x;
		if (valleyheel.size() >= 1
				&& RelValAt(estheel, valleyheel[1].x) > 0.8) {
			DEBUGOUT << "No clear heel area.\n";
		}
	}

	{
		MEstimator esttoes;
		esttoes.XLinspace(toRad(-90), toRad(90), 301);
		esttoes.EstimateY(last->angleXZ, MEstimator::AndrewWave(), 0.03,
				last->idxHeelPoint, last->idxToeTip,
				Kernel::SubDiv(Kernel::Integrated::Triangular, 0.65 * 2.0 - 1.0,
						0.2));
		esttoes.Normalize();
		auto valleytoe = esttoes.FindValleys();
		if (valleytoe.empty())
			return false;
		last->toea = valleytoe[0].x;
		if (valleytoe.size() >= 1 && RelValAt(esttoes, valleytoe[1].x) > 0.8) {
			DEBUGOUT << "No clear toe area.\n";
		}
	}

// Recalculate toe & heel and support points.
	{
		last->idxHeelPoint = last->angleXZ.IatY(
				last->heela - last->param_soleangle,
				DependentVector::Direction::first_risingabove);
		last->idxToeTip = last->angleXZ.IatY(last->toea + toRad(90),
				DependentVector::Direction::first_risingabove);
		last->idxToePoint = last->angleXZ.IatY(
				last->toea + last->param_soleangle,
				DependentVector::Direction::first_risingabove);

		const double xheel = last->angleXZ.X(last->idxHeelPoint);
		const double xtoes = last->angleXZ.X(last->idxToeTip);
		last->idxHeelCenter = last->angleXZ.IatX(xheel + (xtoes - xheel) / 6);
		last->idxWaistBottom = last->angleXZ.IatX(
				xheel + (xtoes - xheel) * 0.4);
		last->idxLittleToeBottom = last->angleXZ.IatX(
				xheel + (xtoes - xheel) * 0.62);
		last->idxBigToeBottom = last->angleXZ.IatX(
				xheel + (xtoes - xheel) * 0.62);
		const double xball = last->angleXZ.X(last->idxBigToeBottom);
		last->idxToeCenter = last->angleXZ.IatX(xball + (xtoes - xball) * 0.5);
		last->idxHeel = last->angleXZ.IatY(last->heela - toRad(90),
				DependentVector::Direction::last_risingabove, 0,
				last->idxHeelCenter);
	}

// Mark left and right outside lines
	FindOutline();

// Find the ball measurement angle
	{
		const Vector3 p = last->planeXZ[last->idxLittleToeBottom];
		Vector3 a, b;
		double da = DBL_MAX;
		double db = DBL_MAX;
		for (size_t n = 0; n < last->bottomleft.Size(); ++n) {
			const double d = (last->bottomleft[n] - p).Abs2();
			if (d < da) {
				da = d;
				a = last->bottomleft[n];
			}
		}
		for (size_t n = 0; n < last->bottomright.Size(); ++n) {
			const double d = (last->bottomright[n] - p).Abs2();
			if (d < da) {
				da = d;
				b = last->bottomright[n];
			}
		}
		const double w = (a - b).Abs();
		const double d = w * sin(toRad(10));
		const double xBigToe = last->angleXZ.X(last->idxLittleToeBottom) + d;
		last->idxBigToeBottom = last->angleXZ.IatX(xBigToe);
	}

// Recalculate heel point
	{
		const double xfront = last->angleXZ.X(last->idxTop);
		const double xend = last->angleXZ.X(last->idxZero) + 1.0;
		size_t idxTopMiddle = last->angleXZ.IatX((xend + xfront) / 2.0);
		auto orth = OrthogonalPoint(last->planeXZ[idxTopMiddle]);
		size_t temp = orth.IatY(0.0,
				DependentVector::Direction::last_risingabove,
				last->idxHeelPoint, last->idxBigToeBottom);
		if (temp < last->idxHeelCenter)
			last->idxHeelCenter = temp;
	}

// Place the marker on top of the last
// The markers are place so that a measurement tape wrapped around
// the last would be shortest.
	{
		size_t idx0 = last->angleXZ.IatY(last->toea + toRad(135),
				DependentVector::Direction::first_risingabove);
		last->idxBigToeTop = FindTopPoint(last->idxBigToeBottom, idx0,
				last->idxTop);
		last->idxLittleToeTop = FindTopPoint(last->idxLittleToeBottom, idx0,
				last->idxTop);
		last->idxWaistTop = FindTopPoint(last->idxWaistBottom,
				last->idxBigToeTop, last->idxTop);
		last->idxTop = FindTopPoint(last->idxHeelPoint, last->idxBigToeTop,
				last->idxTop);
	}

//	bool hasBallArea = true;
//	{
//		MEstimator est;
//		est.EstimateX(last->angleXZ, MEstimator::AndrewWave(), 0.03, toea,
//				last->idxBigToeBottom, last->idxToeTip);
//		est.Normalize();
////		debugB = est;
////		DEBUGOUT << "y0 = " << est.Y(last->idxBallBottom) << "\n";
//		const double v0 = est.XatY(0.5,
//				DependentVector::Direction::first_fallingbelow);
//		const double v1 = est.XatY(0.5,
//				DependentVector::Direction::last_risingabove);
//		const double v = (v0 + v1) / 2.0;
//		last->idxToeCenter = last->angleXZ.IatX(v);
//		hasBallArea = (est.Y(last->idxBigToeBottom) > 0.5);
//		debugA = est;
//	}

// Estimate the position of the foot arch
//	bool hasArchArea = false;
//	{
//		auto valleys = last->angleXZ.FindValleys(M_PI, last->idxHeelCenter, last->idxToeCenter);
//
//		const double xheel = last->angleXZ.X(last->idxHeelCenter);
//		const double xtoes = last->angleXZ.X(last->idxToeCenter);
//		last->idxLittleToeBottom = last->angleXZ.IatX((xheel + xtoes) / 2.0);
//		if(valleys.empty()){
//			DEBUGOUT << "Very round last without arch. (MBT last?)\n";
//		}else{
//			if(valleys[0].y > heela - toRad(2)){
//				DEBUGOUT << "The last has a very flat arch (only "
//						<< toDeg(heela - valleys[0].y) << " deg).\n";
//			}else{
//				hasArchArea = true;
//				last->idxLittleToeBottom = valleys[0].idx;
//				const double ymiddle = (heela + valleys[0].y) / 2.0;
//				{
//					const double ydelta = heela - valleys[0].y;
//					DEBUGOUT << "Arch bow: " << toDeg(ydelta) << " deg\n";
//				}
//				last->idxWaistBottom = last->angleXZ.IatY(ymiddle,
//						DependentVector::Direction::first_fallingbelow,
//						last->idxHeelCenter, last->idxLittleToeBottom);
//			}
//
//			if(valleys[0].y > toea - toRad(2)){
//				DEBUGOUT << "The last has a very flat toe bend (only "
//						<< toDeg(toea - valleys[0].y) << " deg).\n";
//			}else{
//				const double ymiddle = (toea + valleys[0].y) / 2.0;
//							{
//								const double ydelta = heela - valleys[0].y;
//								DEBUGOUT << "Arch bow: " << toDeg(ydelta) << " deg\n";
//							}
//				last->idxBigToeBottom = last->angleXZ.IatY(ymiddle,
//						DependentVector::Direction::first_risingabove,
//						last->idxLittleToeBottom, last->idxToeTip);
//			}
//
//		}
//	}
//	if(!hasBallArea){
//		const double x0 = test.X(last->idxBallBottom);
//		const double x1 = test.X(last->idxToeCenter);
//		markerindex[7] = test.IatX((x0 + x1) / 2.0);
//	}

// Estimate the ball area
//	{
//		MEstimator est;
//		est.XLinspace(toRad(-90), toRad(90), 301);
//		est.EstimateY(test, MEstimator::AndrewWave(), 0.03, last->idxInstepBottom,
//				last->idxToeTip);
//
//		const auto vmax = est.Max();
//		const auto vmin = est.Min();
//		auto valleys = est.FindValleys();
//
//		if(valleys.size() == 0) return false;
//		if(valleys.size() == 1
//				|| valleys[1].y > (vmin.y + (vmax.y - vmin.y) * 0.8)){
//			toea = valleys[0].x;
//			DEBUGOUT << "No clear ball area in last.\n";
//		}else{
//			if(valleys[0].idx > valleys[1].idx) std::swap(valleys[0],
//					valleys[1]);
//
//			toea = valleys[1].x;
//			auto min
//		}
//		debug2 = est;
//	}
// Estimate angle and position of heel-area
//	{
//		MEstimator est;
//		est.XLinspace(toRad(-90), toRad(90), 301);
//		est.EstimateY(test, MEstimator::AndrewWave(), 0.03, last->idxHeelPoint,
//				last->idxInstepBottom);
//		auto valleys = est.FindValleys();
//		if(valleys.empty()) return false;
//		heela = valleys[0].x;
//
//		est.EstimateX(test, MEstimator::AndrewWave(), 0.05, heela,
//				last->idxHeelPoint, last->idxInstepBottom);
//		const double mean = est.Mean();
//		double v0 = est.XatY(mean,
//				DependentVector::Direction::first_fallingbelow);
//		double v1 = est.XatY(mean,
//				DependentVector::Direction::last_risingabove);
//		double v = (v0 + v1) / 2.0;
//		last->idxHeelCenter = test.IatX(v);
//
//		debug0 = est;
//	}
//
//	{
//		MEstimator est;
//		est.XLinspace(toRad(-90), toRad(90), 301);
//		est.EstimateY(test, MEstimator::AndrewWave(), 0.03, last->idxWaistBottom,
//				last->idxToeTip);
//		auto valleys = est.FindValleys();
//		if(valleys.empty()) return false;
//		toea = valleys[0].x;
//
//		est.EstimateX(test, MEstimator::AndrewWave(), 0.05, toea,
//				last->idxWaistBottom, last->idxToeTip);
//		const double mean = est.Mean();
//		double v0 = est.XatY(mean,
//				DependentVector::Direction::first_fallingbelow);
//		double v1 = est.XatY(mean,
//				DependentVector::Direction::last_risingabove);
//		double v = (v0 + v1) / 2.0;
//		last->idxToeCenter = test.IatX(v);

//		debug1 = est;
//	}

//	// Temorary place Markers at the top of the last
//	{
//		const double A0 = test.X(last->idxHeelPoint) - test.X(last->idxToeTip);
//		const double A1 = test.X(last->idxHeelCenter) - test.X(last->idxToeTip);
//		const double A2 = test.X(last->idxWaistBottom) - test.X(last->idxToeTip);
//		const double A3 = test.X(last->idxBallBottom) - test.X(last->idxToeTip);
//		const double B0 = test.X(last->idxTop) - test.X(last->idxToeTip);
//		const double B1 = A1 / A0 * B0 + test.X(last->idxToeTip);
//		const double B2 = A2 / A0 * B0 + test.X(last->idxToeTip);
//		const double B3 = A3 / A0 * B0 + test.X(last->idxToeTip);
//		last->idxInstepTop = test.IatX(B1);
//		last->idxWaistTop = test.IatX(B2);
//		last->idxBallTop = test.IatX(B3);
//	}

//	debug2.XLinspace(toRad(-90), toRad(90), 301);
//	auto e = MEstimator::AndrewWave();
//	for(size_t n = 0; n < debug2.Size(); ++n)
//		debug2.Y(n) = e.Rho(debug2.X(n) / 0.05);

// Statistics for magic numbers
//	{
//		const double A0 = test.X(last->idxToeTip) - test.X(last->idxHeelPoint);
//		for(size_t n = 3; n < 9; ++n){
//			const double A1 = test.X(markerindex[n]) - test.X(last->idxHeelPoint);
//			DEBUGOUT << (char) ('@' + n) << " - " << 100 * A1 / A0 << "%\n";
//		}
//	}
// Extramarkers for heel and toeangle

//	est.XLinspace(-M_PI, M_PI, 301);
//	auto f = MEstimator::AndrewWave();
//	est.EstimateY(test, f, 0.01);
//	est /= est.Area();
//	est *= 2;
//	est.XLinspace(0, 1, 301);

// Rescale test for displaying.
//	last->angleXZ /= M_PI;
	return true;
}

void LastAnalyse::FindOutline() {
// Find the outline of the sole

//		loop = hull.IntersectPlane(Vector3(0, 1, 0), bbc.GlobalY(0, 0.5));

	const size_t Ncut = 50;

	const Polynomial rotation = Polynomial::ByValue(0, last->heela, Ncut - 1,
			last->toea);
	const Polynomial cutAt = Polynomial::ByValue(0, 0.05, Ncut - 1, 0.95);

	const Vector3 xHeel = last->planeXZ[last->idxHeel];
	const Vector3 xToe = last->planeXZ[last->idxToeTip];

	last->bottomleft.Clear();
	last->bottomright.Clear();

//	AffineTransformMatrix bbc = rawBB.GetCoordinateSystem();

	for (size_t n = 0; n < Ncut; ++n) {
		const double a = rotation(n);
		const Vector3 normal(cos(a), 0, sin(a));
		const Vector3 pos = xHeel + (xToe - xHeel) * cutAt(n);
		Polygon3 section = last->IntersectPlane(normal, normal.Dot(pos));

		Vector3 r = section.GetRotationalAxis();
		if (r.x < 0)
			section.Reverse();

//		if(n == 25) loop = section;

		const size_t N = section.Size();
		size_t Nmin = 0;
		for (size_t n = 0; n < N; ++n)
			if (section[n].z < section[Nmin].z)
				Nmin = n;

		for (size_t n = 0; n < N; ++n) {
			const Vector3 n0 = section.Normal((Nmin + n) % N);
			const Vector3 n1 = section.Normal((Nmin + n + 1) % N);
			const double a0 = atan2(n0.y, -n0.z);
			const double a1 = atan2(n1.y, -n1.z);
			if (a0 < last->param_soleangle && a1 >= last->param_soleangle) {
				const double s = (last->param_soleangle - a0) / (a1 - a0);
				const Vector3 v0 = section[(Nmin + n) % N];
				const Vector3 v1 = section[(Nmin + n + 1) % N];
				last->bottomleft.AddEdgeToVertex(
						Geometry::Vertex((v1 - v0) * s + v0,
								((n1 - n0) * s + n0).Normal()));
				break;
			}
		}

		for (size_t n = 0; n < N; ++n) {
			const Vector3 n0 = section.Normal((Nmin + N - n) % N);
			const Vector3 n1 = section.Normal((Nmin + N - (n + 1)) % N);
			const double a0 = atan2(n0.y, -n0.z);
			const double a1 = atan2(n1.y, -n1.z);
			if (a0 > -last->param_soleangle && a1 <= -last->param_soleangle) {
				const double s = (-last->param_soleangle - a0) / (a1 - a0);
				const Vector3 v0 = section[(Nmin + N - n) % N];
				const Vector3 v1 = section[(Nmin + N - (n + 1)) % N];
				last->bottomright.AddEdgeToVertex(
						Geometry::Vertex((v1 - v0) * s + v0,
								((n1 - n0) * s + n0).Normal()));
				break;
			}
		}
	}

	last->bottom.Clear();
	last->top.Clear();
	for (size_t n = 0; n < last->bottomright.Size(); ++n) {
		const Vector3 temp = (last->bottomright[n] + last->bottomleft[n]) / 2;
		last->bottom.AddEdgeToVertex(
				Geometry::Vertex(
						last->IntersectArrow(temp, Vector3(0, 0, -1))));
		last->top.AddEdgeToVertex(
				Geometry::Vertex(last->IntersectArrow(temp, Vector3(0, 0, 1))));
	}
//	last->planeXZ.Clear();
//	for(size_t n = 0; n < bottomright.Size(); ++n){
//		last->planeXZ.InsertPoint((top[n] + bottom[n] * 2.0) / 3.0);
//	}
//	{
//		const size_t iEnd = center.Size() - 1;
//		Vector3 temp = last->IntersectArrow(center[0], center[0] - center[3]);
//		center[0] = temp;
//		temp = last->IntersectArrow(center[iEnd],
//				center[iEnd] - center[iEnd - 3]);
//		center[iEnd] = temp;
//	}
}

size_t LastAnalyse::FindTopPoint(size_t idxBottom, size_t idxStart,
		size_t idxEnd) const {
	auto orth = OrthogonalPoint(last->planeXZ[idxBottom]);
	for (size_t n = 0; n < orth.Size(); ++n)
		orth.X(n) = last->angleXZ.X(n);
	size_t idx = orth.IatY(0, DependentVector::Direction::first_risingabove,
			idxStart, idxEnd);
	if (idx > idxEnd) {
		return idxEnd;
	} else {
		return idx;
	}
}

DependentVector LastAnalyse::OrthogonalPoint(const Vector3 &p) const {
	const size_t N = last->planeXZ.Size();
	DependentVector temp;
	temp.XLinspace(0, 1, N);
	for (size_t n = 0; n < N; ++n) {
		const Vector3 d =
				(last->planeXZ[(n + 1) % N] - last->planeXZ[n]).Normal();
		const Vector3 c = (last->planeXZ[n] - p).Normal();
		temp.Y(n) = d.Dot(c);
	}
	return temp;
}

double LastAnalyse::RelValAt(const DependentVector &est, double x) {
	auto vmin = est.Min();
	auto vmax = est.Max();
	double v = est.YatX(x);
	return (v - vmax.y) / (vmin.y - vmax.y);
}

LastAnalyse::Cut LastAnalyse::CalculateCut(const Geometry &hull,
		const Vector3 &p0, const Vector3 &p1) {
	Vector3 n = (p1 - p0).Normal();
	Vector3 n2(-n.z, 0, n.x);
	LastAnalyse::Cut temp;
	temp.p = hull.IntersectPlane(n2, n2.Dot(p0));
	temp.m.SetOrigin(p0);
	temp.m.SetEx(n);
	temp.m.SetEy(Vector3(0, 1, 0));
	temp.m.SetEz(n2);
	return temp;
}

AffineTransformMatrix LastAnalyse::RotatedScale(const Cut &c, double target) {
	const Vector3 n = c.m.GetEx();
	const double angle = atan2(-n.z, n.x);
	const AffineTransformMatrix rotBack = AffineTransformMatrix::RotationXYZ(0,
			-angle, 0);
	const AffineTransformMatrix rotForward = AffineTransformMatrix::RotationXYZ(
			0, angle, 0);

	Polygon3 base = c.p;
	base.Transform(rotBack);
	BoundingBox bb = CalculateBoundingBox(base);
	const double a = bb.GetSizeY() / 2.0;
	const double b = bb.GetSizeX() / 2.0;
	const double bTarget = target / M_PI - b;
	const double bCurrent = c.p.GetLength() / M_PI - b;
	const double scale = bTarget / bCurrent;

	AffineTransformMatrix tr;

	tr.ScaleGlobal(scale, 1.0, 1.0);
	tr = rotForward * tr * rotBack;
	Vector3 origin = c.m.GetOrigin();
	tr.TranslateGlobal(-origin.x, -origin.y, -origin.z);
	tr.TranslateLocal(origin.x, origin.y, origin.z);

	base = c.p;
	base.Transform(tr);
	double L2 = base.GetLength();

	Polynomial p = Polynomial::ByValue(1.0, c.p.GetLength(), scale, L2)
			- target;

	double scale2 = p.FindZero(1.0);
	tr.SetIdentity();
	tr.ScaleGlobal(scale2, 1.0, 1.0);
	tr = rotForward * tr * rotBack;
	origin = c.m.GetOrigin();
	tr.TranslateGlobal(-origin.x, -origin.y, -origin.z);
	tr.TranslateLocal(origin.x, origin.y, origin.z);

//	LinearOptimizer opt;
//	opt.param = 1.0;
//	opt.spread = 0.5;
//	opt.Start();
//	try {
//		while (opt.IsRunning()) {
//			opt.SetError(L2 - target);
//		}
//	} catch (const std::exception & ex) {
//		std::cerr << "Exception: " << ex.what() << "\n";
//	}

//
//	BoundingBox bbRaw = CalculateBoundingBox(c.p);
//	Vector3 nNew = base.GetRotationalAxis();
//	Vector3 nRaw = c.p.GetRotationalAxis();
//	auto U =
//			[](double a, double b) {const double L = (a-b)*(a-b)/((a+b)*(a+b)); return M_PI*(a+b)*(1+3*L/(10+sqrt(4-3*L)));};

	return tr;
}

bool LastAnalyse::Vector3XLess(const Vector3 a, const Vector3 b) {
	return a.x < b.x;
}

BoundingBox LastAnalyse::CalculateBoundingBox(const Polygon3 &polygon) {
	BoundingBox temp;
	const size_t N = polygon.Size();
	for (size_t n = 0; n < N; ++n)
		temp.Insert(polygon[n]);
	return temp;
}

void LastAnalyse::Paint() const {


}
