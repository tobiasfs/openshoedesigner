///////////////////////////////////////////////////////////////////////////////
// Name               : LastNormalize.cpp
// Purpose            : 
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 12.11.2024
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
#include "LastNormalize.h"

#include "../../3D/Polygon3.h"
#include "../../3D/OpenGL.h"
#include "../../3D/OpenGLMaterial.h"
#include "../../math/Exporter.h"
#include "../../math/FourierTransform.h"
#include "../../math/Kernel.h"
#include "../../math/KernelDensityEstimator.h"
#include "../../math/PCA.h"
#include "../../math/PolyFilter.h"
#include "../../math/Polynomial.h"
#include "../../math/Symmetry.h"

#include <iostream>
#include <sstream>
#include <stdexcept>

LastNormalize::LastNormalize() {
	out = std::make_shared<ObjectGeometry>();
}

std::string LastNormalize::GetName() const {
	return "LastNormalize";
}

bool LastNormalize::CanRun() {
	std::string missing;

	if (!lastReorient)
		missing += missing.empty() ? "\"lastReorient\"" : ", \"lastReorient\"";
	if (!in)
		missing += missing.empty() ? "\"in\"" : ", \"in\"";
	if (!out)
		missing += missing.empty() ? "\"out\"" : ", \"out\"";

	if (!missing.empty()) {
		std::ostringstream err;
		err << __FILE__ << ":" << __LINE__ << ":" << GetName() << "::"
				<< __FUNCTION__ << " -";
		err << "The variables " << missing << " are not connected.";
		error = err.str();
		throw std::logic_error(err.str());
	}

	error.clear();

	return error.empty();
}

bool LastNormalize::Propagate() {
	if (!lastReorient || !in || !out)
		return false;

	bool modify = false;

	if (!in->IsValid() || lastReorient->IsModified()) {
		modify |= out->IsValid();
		out->MarkValid(false);
	}
	if (out->IsNeeded()) {
		modify |= !in->IsNeeded();
		in->MarkNeeded(true);
	}
	return modify;
}

bool LastNormalize::HasToRun() {
	return in && in->IsValid() && out && !out->IsValid() && out->IsNeeded();
}

void LastNormalize::Run() {
	*out = *in;
//	out->Transform(AffineTransformMatrix::Scaling(0.1));
	out->UpdateBoundingBox();

	if (lastReorient->GetSelectionIdx() != 1) {
		out->MarkValid(true);
		out->MarkNeeded(false);
		return;
	}

//	out->paintNormals = true;

	DEBUGOUT << "Before LastNormalize::Run: Volume: " << out->GetVolume()
			<< '\n';

	ReorientPCA();
	ReorientSymmetry();
	ReorientSole();
	ReorientFrontBack();
	ReorientLeftRight();

//	debug0 = *out;
//	debug1 = *out;

	out->UpdateBoundingBox();
	out->CalculateNormals();

	DEBUGOUT << "After LastNormalize::Run: Volume: " << out->GetVolume()
			<< '\n';
//	DEBUGOUT << "In " << __LINE__ << ": Curvature = "
//			<< out->GetNormalCurvature() << " rad\n";

	out->MarkValid(true);
	out->MarkNeeded(false);
}

#ifdef DEBUG
void LastNormalize::Paint() const {
	glPushMatrix();
//	glRotatef(-90, 0, 1, 0);
//	glRotatef(90, 1, 0, 0);
//	symmetry.Paint();

//	kde.Paint();

//	OpenGLMaterial mat0(OpenGLMaterial::Preset::RedPlastic);
//	OpenGLMaterial mat1(OpenGLMaterial::Preset::GreenPlastic);
//	mat1.UseColor();
//	debug1.Paint();
//	mat0.UseColor();
//	debug0.Paint();

	glPopMatrix();
}
#endif

void LastNormalize::ReorientPCA() {
	Exporter ex("/tmp/geo.mat");
	ex.Add(*out);
	ex.Close();

	// Find orientation of mesh
	PCA pca;
//	Vector3 center = out->GetCentroid();
	Vector3 center = out->GetCenter();

	pca.SetCenter(center);

//	for (size_t i = 0; i < out->TriangleCount(); ++i)
//		pca.Add(out->GetTetraederCenter(i), out->GetTetraederVolume(i));
	for (size_t i = 0; i < out->VertexCount(); ++i)
		pca.Add(out->GetVertex(i));
	pca.Calculate();
	// Make coordinate system right handed
	if ((pca.X * pca.Y).Dot(pca.Z) < 0.0)
		pca.Y = -pca.Y;
	// Remove orientation
	AffineTransformMatrix temp(pca.X, pca.Y, pca.Z, pca.center);
	temp.Orthogonalize();
	temp.Invert();

	out->Transform(temp);
	out->UpdateBoundingBox();
}

void LastNormalize::ReorientSymmetry() {
// Scan Shape for symmetry
	{
//		AffineTransformMatrix comp;
//		comp *= AffineTransformMatrix::RotationAroundVector(Vector3(1, 0, 0),
//				1.0);
//		last->Transform(comp);
//		UpdateRawBoundingBox();
	}

//	Exporter ex("/tmp/debug.mat", "X");

	AffineTransformMatrix bbc = out->BB.GetCoordinateSystem();
//		debug.Clear();
	Symmetry symmetry;
	symmetry.Init(180);
	for (double cut = 0.2; cut < 0.81; cut += 0.2) {
		Polygon3 section = out->IntersectPlane(Vector3(1, 0, 0),
				bbc.GlobalX(cut));

//		ex.Add(section);

		Vector3 rot = section.GetRotationalAxis();
		if (rot.x < 0)
			rot = -rot;

		AffineTransformMatrix coordsys;
		coordsys.SetOrigin(section.GetCenter());
		coordsys.SetEx(Vector3(0, 1, 0));
		coordsys.SetEy(Vector3(0, 0, 1));
		coordsys.CalculateEz();

		FourierTransform ft;
		ft.TSetSize(section.Size());
		for (size_t n = 0; n < section.Size(); ++n) {
			const double lx = coordsys.LocalX(section[n]);
			const double ly = coordsys.LocalY(section[n]);
//			debug.AddEdgeToVertex({lx,ly,0});
			ft.x[n].t = atan2(ly, lx);
			ft.XSet(n, (section[n] - coordsys.GetOrigin()).Abs());
		}
		ft.TSort();
		ft.TUnwrap();
		ft.TSetLoopLength(2 * M_PI);
		ft.TScale(1.0 / (2 * M_PI));
		ft.FLinspace(0, 30, 31);
		ft.Transform();
		ft.SingleSidedResult();
		symmetry.AddTransform(ft);
//		debug.CloseLoopNextGroup();
	}

	symmetry.Normalize();

//	ex.Add(symmetry, "symmetry");

	auto results = symmetry.FindPeaks(0.01);
	if (results.empty()) {
		std::ostringstream err;
		err << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << " - ";
		err << "The function did not find any features.";
		throw std::runtime_error(err.str());
	}
	AffineTransformMatrix comp;
	comp *= AffineTransformMatrix::RotationAroundVector(Vector3(1, 0, 0),
			-M_PI_2 - results[0].x);
	DEBUGOUT << "Peak at " << results[0].x * 180.0 / M_PI << " degrees.\n";
	DEBUGOUT << "Rotate by: " << (M_PI_2 - results[0].x) * 180.0 / M_PI
			<< " degrees.\n";

	out->Transform(comp);
	out->UpdateBoundingBox();
}

void LastNormalize::ReorientSole() {

	KernelDensityEstimator kde;
	kde.Clear();
	kde.XLinspace(0, 2 * M_PI, 360);
	kde.XSetCyclic(2 * M_PI);

	AffineTransformMatrix bbc = out->BB.GetCoordinateSystem();
	for (double cut = 0.2; cut < 0.81; cut += 0.2) {
		Polygon3 section = out->IntersectPlane(Vector3(1, 0, 0),
				bbc.GlobalX(cut));

		Vector3 rot = section.GetRotationalAxis();
		if (rot.x > 0)
			section.Reverse();

		const double Lmax = section.GetLength();
		for (size_t n = 0; n < section.EdgeCount(); ++n) {
			const Vector3 temp = section.GetEdgeVertex(n, 1)
					- section.GetEdgeVertex(n, 0);
			double a = atan2(temp.y, -temp.z);
			kde.Insert(a, Kernel::Silverman, temp.Abs() / Lmax, 0.2);
		}
	}

	kde.Normalize();

	kde.Attenuate(0, Kernel::Cauchy, 0.75, 0.5);
	kde.Attenuate(M_PI, Kernel::Cauchy, 0.75, 0.5);

	auto results = kde.FindPeaks(0.1);
	if (results.empty()) {
		std::ostringstream err;
		err << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << " - ";
		err << "The function did not find any features.";
		throw std::runtime_error(err.str());
	}
	AffineTransformMatrix comp;
	comp *= AffineTransformMatrix::RotationAroundVector(Vector3(1, 0, 0),
			3 * M_PI_2 - results[0].x);
	out->Transform(comp);
	out->UpdateBoundingBox();
}

void LastNormalize::ReorientFrontBack() {

//		loop.Clear();
	std::vector<double> ratio;
	AffineTransformMatrix bbc = out->BB.GetCoordinateSystem();
	for (double cut = 0.1; cut < 0.91; cut += 0.1) {
		Polygon3 section = out->IntersectPlane(Vector3(1, 0, 0),
				bbc.GlobalX(cut));
		BoundingBox temp;
		for (size_t n = 0; n < section.Size(); ++n)
			temp.Insert(section[n]);
		ratio.push_back(temp.GetSizeZ() / temp.GetSizeY());
//			loop += section.GetCenter();
	}
	PolyFilter pfr(2, ratio.size());
	Polynomial pr = pfr.Filter(ratio);
//	pr.ShiftX(1);
//	pr.ScaleX(0.1);

//		test.Clear();
//		for(double r = 0; r <= 1; r += 0.01)
//			test += Vector3(bb.xmin + bb.GetSizeX() * r, 0.0, pr.Evaluate(r));

	pr.Derive();
	if (pr(0.5) > 0.0) {
		// Reverse last
//		DEBUGOUT << "Reverse last.\n";
		out->Transform(
				AffineTransformMatrix::RotationAroundVector(Vector3(0, 0, 1),
				M_PI));
		out->UpdateBoundingBox();
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

void LastNormalize::ReorientLeftRight() {

// Scan for left/right

//		double ip = 0.5;
	Polygon3 loop;
	loop.Clear();

//		kde.XLinspace(0, 1, 100);
//		kde.XSetLinear();
	AffineTransformMatrix bbc = out->BB.GetCoordinateSystem();
	for (double cut = 0.1; cut < 0.91; cut += 0.1) {
		Polygon3 section = out->IntersectPlane(Vector3(1, 0, 0),
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

	py.ScaleX(1.0 / out->BB.GetSizeX()); // Normale with lastlength
	py.ScaleY(1.0 / out->BB.GetSizeY()); // Normalize with lastwidth

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

	loop = out->IntersectPlane(Vector3(1, 0, 0), bbc.GlobalX(0.5));

	Vector3 rot = loop.GetRotationalAxis();
	if (rot.x > 0) {
		loop.Reverse();
		loop.SortLoop();
	}
	const double Lmax = loop.GetLength();
	for (size_t n = 0; n < loop.Size(); ++n) {
		const Vector3 dirvect = (loop[(n + 1) % loop.Size()] - loop[n]);
		double a = atan2(dirvect.y, -dirvect.z);
		kde.Insert(a, Kernel::Sigmoid, dirvect.Abs() / Lmax, 0.3);
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

	if (chir > 0.5) {
		AffineTransformMatrix flipY;
		flipY.ScaleGlobal(1.0, -1.0, 1.0);
		out->Transform(flipY);
		DEBUGOUT << "Flip sides left to right.\n";

		out->UpdateBoundingBox();
	}

//		DEBUGOUT << chir << " ";
//		if(chir > 0.0)
//			DEBUGOUT << " = Right last ";
//		else
//			DEBUGOUT << " = Left last ";
//		if(fabs(chir) < 0.5) DEBUGOUT << "(Insole recommended)";
//		DEBUGOUT << "\n";

}

