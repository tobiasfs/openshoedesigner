///////////////////////////////////////////////////////////////////////////////
// Name               : SVD.cpp
// Purpose            : Singular Value Decomposition
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 29.11.2023
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

#include "SVD.h"

#include <algorithm>
#include <cfloat>
#include <cmath>
#include <iostream>
#include <numeric>
#include <stddef.h>
#include <stdexcept>

SVD::SVD(const Matrix &A) {
	Decompose(A);
}

void SVD::Decompose(const Matrix &A) {
	// Check A
	const size_t M = A.Size(0);
	const size_t N0 = A.Size(1);
	if (M < N0)
		throw std::runtime_error(
				"SVD::Decompose - The matrix A has more columns than rows. You must augment A with additional empty rows.");
	if (A.Numel() > M * N0)
		throw std::runtime_error(
				"SVD::Decompose - The matrix A has more than two dimensions.");
	if (A.IsEmpty()) {
		U.Reset();
		W.Reset();
		V.Reset();
		return;
	}

	U = A;
	U.SetVariableName("Ud");
	auto Uc = U.data();

	// Detect empty columns, remove columns from matrix
	empty.clear();
	for (size_t i = 0; i < N0; i++) {
		bool flagEmpty = true;
		size_t offs = i * M;
		for (size_t j = 0; j < M; j++) {
			if (fabs(Uc[offs]) > DBL_EPSILON) {
				flagEmpty = false;
				break;
			}
			offs++;
		}
		if (flagEmpty)
			empty.push_back(i);
	}
	if (empty.size() == N0) {
		U = Matrix::Eye(M, M);
		W = Matrix::Zeros(N0, 1);
		V = Matrix::Eye(N0, N0);
		return;
	}
	U.MapCols(Matrix::Mode::Remove, empty);
	const size_t N = U.Size(1);

	W.SetSize(N, 1);
	W.SetVariableName("Sd");
	auto Wc = W.data();

	V.SetSize(N, N);
	V.SetVariableName("Vd");
	auto Vc = V.data();
	Matrix RV1(N, 1);
	auto RV1c = RV1.data();

	const size_t Ucol = U.Size(0);
	const size_t Vcol = V.Size(0);

	// Householder reduction to bi-diagonal form
	double ANORM = 0.0;
	RV1c[0] = 0.0;
	for (size_t I = 0; I < N; I++) {
		double G = 0.0;
		double SCALE = 0.0;
		if (I <= (M - 1)) {
			size_t offs1 = I + I * Ucol;
			for (size_t K = I; K < M; K++) {
				SCALE += fabs(Uc[offs1]);
				//SCALE = SCALE + fabs(U(K, I));
				offs1++;
			}
			if (fabs(SCALE) > DBL_EPSILON) {
				double S1 = 0.0;
				size_t offs11 = I + I * Ucol;
				for (size_t K = I; K < M; K++) {
					Uc[offs11] /= SCALE;
					S1 += Uc[offs11] * Uc[offs11];
					//U(K, I) /= SCALE;
					//S1 += U(K, I) * U(K, I);
					offs11++;
				}
				//const double F1 = U(I, I);
				const double F1 = Uc[I + I * Ucol];
				G = -copysign(sqrt(S1), F1);
				const double H = F1 * G - S1;
				Uc[I + I * Ucol] = F1 - G;
				//U(I, I) = F1 - G;
				if (I != (N - 1)) {
					for (size_t J = (I + 1); J < N; J++) {
						double S2 = 0.0;
						size_t offs12 = I + I * Ucol;
						size_t offs2 = I + J * Ucol;
						for (size_t K = I; K < M; K++) {
							S2 += Uc[offs12] * Uc[offs2];
							//S2 += U(K, I) * U(K, J);
							offs12++;
							offs2++;
						}
						const double F2 = S2 / H;
						offs12 = I + I * Ucol;
						offs2 = I + J * Ucol;

						for (size_t K = I; K < M; K++) {
							Uc[offs2] += F2 * Uc[offs12];
							//U(K, J) += F2 * U(K, I);
							offs12++;
							offs2++;
						}
					}
				}
				offs11 = I + I * Ucol;
				for (size_t K = I; K < M; K++) {
					Uc[offs11] *= SCALE;
					//U(K, I) *= SCALE;
					offs11++;
				}
			}
		}
		Wc[I] = SCALE * G;
		G = 0.0;
		SCALE = 0.0;
		if (I <= (M - 1) && I != (N - 1)) {
			const size_t L = I + 1;
			size_t offs1 = I + L * Ucol;
			for (size_t K = L; K < N; K++) {
				SCALE += fabs(Uc[offs1]);
				//SCALE += fabs(U(I, K));
				offs1 += Ucol;
			}
			if (fabs(SCALE) > DBL_EPSILON) {
				double S3 = 0.0;
				size_t offs14 = I + L * Ucol;
				for (size_t K = L; K < N; K++) {
					Uc[offs14] /= SCALE;
					S3 += Uc[offs14] * Uc[offs14];
					//U(I, K) /= SCALE;
					//S3 += U(I, K) * U(I, K);
					offs14 += Ucol;
				}
				const double F = Uc[I + L * Ucol];
				//const double F = U(I, L);
				G = -copysign(sqrt(S3), F);
				const double H = 1.0 / (F * G - S3);
				Uc[I + L * Ucol] = F - G;
				//U(I, L) = F - G;
				offs14 = I + L * Ucol;
				for (size_t K = L; K < N; K++) {
					RV1c[K] = Uc[offs14] * H;
					//RV1c[K] = U(I, K) / H;
					offs14 += Ucol;
				}
				if (I != (M - 1)) {
					for (size_t J = L; J < M; J++) {
						double S2 = 0.0;
						size_t offs15 = J + L * Ucol;
						size_t offs2 = I + L * Ucol;
						const size_t inc = Ucol;
						for (size_t K = L; K < N; K++) {
							S2 += Uc[offs15] * Uc[offs2];
							//S2 += U(J, K) * U(I, K);
							offs15 += inc;
							offs2 += inc;
						}
						offs15 = J + L * Ucol;
						for (size_t K = L; K < N; K++) {
							Uc[offs15] += S2 * RV1c[K];
							offs15 += inc;
						}
					}
				}
				offs14 = I + L * Ucol;
				for (size_t K = L; K < N; K++) {
					Uc[offs14] *= SCALE;
					//U(I, K) *= SCALE;
					offs14 += Ucol;
				}
			}
		}
		ANORM = fmax(ANORM, fabs(Wc[I]) + fabs(RV1c[I]));
		if (I < (N - 1))
			RV1c[I + 1] = SCALE * G;
	}

	// Accumulation of right-hand transforms
	for (size_t I = N; I-- > 0;) {
		if (I < (N - 1)) {
			const size_t L = I + 1;
			const double G = RV1c[L];
			if (fabs(G) > DBL_EPSILON) {
				const double H = 1.0 / (Uc[I + L * Ucol] * G);
				//const double H = 1.0 / (U(I, L) * G);
				size_t offs1 = L + I * Vcol;
				size_t offs2 = I + L * Ucol;
				for (size_t J = L; J < N; J++) {
					Vc[offs1] = Uc[offs2] * H;
					//V(J, I) = U(I, J) * H; //(U(I, J) / U(I, L)) / G;
					offs1++;
					offs2 += Ucol;
				}
				for (size_t J = L; J < N; J++) {
					double S3 = 0.0;
					size_t offs16 = I + L * Ucol;
					size_t offs26 = L + J * Vcol;
					for (size_t K = L; K < N; K++) {
						S3 += Uc[offs16] * Vc[offs26];
						//S3 += U(I, K) * V(K, J);
						offs16 += Ucol;
						offs26++;
					}
					offs16 = L + J * Vcol;
					offs26 = L + I * Vcol;
					for (size_t K = L; K < N; K++) {
						Vc[offs16] += S3 * Vc[offs26];
						//V(K, J) += S3 * V(K, I);
						offs16++;
						offs26++;
					}
				}
			}
			size_t offs1 = I + L * Vcol;
			size_t offs2 = L + I * Vcol;
			for (size_t J = L; J < N; J++) {
				Vc[offs1] = 0.0;
				Vc[offs2] = 0.0;
				//V(I, J) = 0.0;
				//V(J, I) = 0.0;
				offs1 += Vcol;
				offs2++;
			}
		}
		Vc[I + I * Vcol] = 1.0;
		//V(I, I) = 1.0;
	}

	// Accumulation of left-hand transforms
	for (size_t I = N; I-- > 0;) {
		const size_t L = I + 1;
		double G = Wc[I];
		if (I < (N - 1)) {
			size_t offs1 = I + L * Ucol;
			size_t inc1 = Ucol;
			for (size_t J = L; J < N; J++) {
				Uc[offs1] = 0.0;
				//U(I, J) = 0.0;
				offs1 += inc1;
			}
		}
		if (fabs(G) > DBL_EPSILON) {
			G = 1.0 / G;
			if (I != (N - 1)) {
				for (size_t J = L; J < N; J++) {
					double S4 = 0.0;
					size_t offs1 = L + I * Ucol;
					size_t offs2 = L + J * Ucol;
					for (size_t K = L; K < M; K++) {
						S4 += Uc[offs1] * Uc[offs2];
						//S4 += U(K, I) * U(K, J);
						offs1++;
						offs2++;
					}
					const double F = (S4 / Uc[I + I * Ucol]) * G;
					//const double F = (S4 / U(I, I)) * G;
					offs1 = I + J * Ucol;
					offs2 = I + I * Ucol;
					for (size_t K = I; K < M; K++) {
						Uc[offs1] += F * Uc[offs2];
						//U(K, J) += F * U(K, I);
						offs1++;
						offs2++;
					}
				}
			}
			size_t offs1 = I + I * Ucol;
			for (size_t J = I; J < M; J++) {
				Uc[offs1] *= G;
				//U(J, I) *= G;
				offs1++;
			}
		} else {
			size_t offs1 = I + I * Ucol;
			for (size_t J = I; J < M; J++) {
				Uc[offs1] = 0.0;
				//U(J, I) = 0.0;
				offs1++;
			}
		}
		Uc[I + I * Ucol] += 1.0;
		//U(I, I) += 1.0;
	}

//	std::cout << "\nU = " << U << ";";
//	std::cout << "\nW = " << W << ";";
//	std::cout << "\nV = " << V << ";\n";
//	std::cout << "\nRV1 = " << RV1 << ";\n";

	// Diagonalization of the bidiagonal form
	for (size_t K = N; K-- > 0;) {
		// Loop over singular values
		for (size_t ITS = 0; ITS <= maxIterations; ITS++) {
			// Loop over allowed iterations
			bool flagRun1 = true;
			size_t L;
			size_t NM0;
			for (L = (K + 1); L-- > 0;) {
				// Test for splitting
				NM0 = L - 1;
				if (fabs((fabs(RV1c[L]) + ANORM) - ANORM) < DBL_EPSILON) {
					// go to 2
					flagRun1 = false;
					break;
				}
				if (L > 0 && fabs((fabs(Wc[NM0]) + ANORM) - ANORM) < DBL_EPSILON) {
					// go to 1
					break;
				}
			}
			if (flagRun1) {
				// 1
				double C3 = 0.0;
				double S3 = 1.0;

				for (size_t I = L; I < (K + 1); I++) {
					const double F2 = S3 * RV1c[I];
					if (fabs((fabs(F2) + ANORM) - ANORM) > DBL_EPSILON) {
						const double G3 = Wc[I];
						double H3 = sqrt(F2 * F2 + G3 * G3);
						Wc[I] = H3;
						H3 = 1.0 / H3;
						C3 = G3 * H3;
						S3 = -F2 * H3;
						size_t offs1 = NM0 * Ucol;
						size_t offs2 = I * Ucol;
						for (size_t J = 0; J < M; J++) {
//							const double Y = Uc[offs1];
//							const double Z = Uc[offs2];
//							Uc[offs1] = Y * C + Z * S;
//							Uc[offs2] = -Y * S + Z * C;
							const double Y = U(J, NM0);
							const double Z = U(J, I);
							U(J, NM0) = Y * C3 + Z * S3;
							U(J, I) = -Y * S3 + Z * C3;
							offs1++;
							offs2++;
						}
					}
				}
//				std::cout << "#";
			}
			// 2
			double Z5 = Wc[K];
			if (L == K) {
				// Convergence
				if (Z5 < 0.0) {
					// Singular value is made nonnegative
					Wc[K] = -Z5;
					size_t offs1 = K * Vcol;
					for (size_t J = 0; J < N; J++) {
						Vc[offs1] = -Vc[offs1];
						//V(J, K) = -V(J, K);
						offs1++;
					}
				}
				break;
			}
			if (flagRun1) {
				std::cout << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__
						<< ": This matrix has a very bad solvability.\n";
				break;
			}

			if (ITS == maxIterations)
				throw std::runtime_error("No convergence in 30 iterations.");
			const size_t NM = K - 1;
			double X5 = Wc[L]; // Shift from bottom 2-by-2 minor
			double Y5 = Wc[NM];
			double G5 = RV1c[NM];
			double H5 = RV1c[K];
			double F5 = ((Y5 - Z5) * (Y5 + Z5) + (G5 - H5) * (G5 + H5))
					/ (2.0 * H5 * Y5);
			G5 = sqrt(F5 * F5 + 1.0);
			F5 = ((X5 - Z5) * (X5 + Z5)
					+ H5 * ((Y5 / (F5 + copysign(G5, F5))) - H5)) / X5;
			// Next QR Transformation
			double C5 = 1.0;
			double S5 = 1.0;
			for (size_t J = L; J < (NM + 1); J++) {
				size_t I = J + 1;
				G5 = RV1c[I];
				Y5 = Wc[I];
				H5 = S5 * G5;
				G5 = C5 * G5;
				Z5 = sqrt(F5 * F5 + H5 * H5);
				RV1c[J] = Z5;
				C5 = F5 / Z5;
				S5 = H5 / Z5;
				F5 = X5 * C5 + G5 * S5;
				G5 = -X5 * S5 + G5 * C5;
				H5 = Y5 * S5;
				Y5 = Y5 * C5;

				size_t offs1 = 0 + J * Vcol;
				size_t offs2 = 0 + I * Vcol;
				for (size_t JJ = 0; JJ < N; JJ++) {
					const double X3 = Vc[offs1];
					const double Z3 = Vc[offs2];
					Vc[offs1] = X3 * C5 + Z3 * S5;
					Vc[offs2] = -X3 * S5 + Z3 * C5;
					//X = V(JJ, J);
					//Z = V(JJ, I);
					//V(JJ, J) = X * C + Z * S;
					//V(JJ, I) = -X * S + Z * C;
					offs1++;
					offs2++;
				}
				Z5 = sqrt(F5 * F5 + H5 * H5);
				Wc[J] = Z5;
				// Rotations can be arbitrary if Z == 0
				if (fabs(Z5) > DBL_EPSILON) {
					Z5 = 1.0 / Z5;
					C5 = F5 * Z5;
					S5 = H5 * Z5;
				}
				F5 = C5 * G5 + S5 * Y5;
				X5 = -S5 * G5 + C5 * Y5;
				offs1 = J * Ucol;
				offs2 = I * Ucol;
				for (size_t JJ = 0; JJ < M; JJ++) {
					const double Y3 = Uc[offs1];
					const double Z3 = Uc[offs2];
					Uc[offs1] = Y3 * C5 + Z3 * S5;
					Uc[offs2] = -Y3 * S5 + Z3 * C5;
					//const double Y = U(JJ, J);
					//const double Z = U(JJ, I);
					//U(JJ, J) = Y * C + Z * S;
					//U(JJ, I) = -Y * S + Z * C;
					offs1++;
					offs2++;
				}
			}
			RV1c[L] = 0.0;
			RV1c[K] = F5;
			Wc[K] = X5;
		}
	}

	if (!empty.empty()) {
		// Add additional 0.0 singular values. Expand V accordingly. (V stays
		// orthogonal.)
		W.SetSize(N0);
		V.MapRows(Matrix::Mode::AssignInverse, empty);
		V.SetSize(V.Size(0), N0);
		for (size_t i = 0; i < empty.size(); i++)
			V(empty[i], N + i) = 1.0;
	}

	if (sortW) {
		auto Wp = W.data();
		const size_t Nw = U.Size(1);
		if (Nw > N0)
			throw std::runtime_error(
					"SVD::Decompose() - U is wider than the count of W.");
		std::vector<size_t> idx(Nw);
		std::iota(idx.begin(), idx.end(), 0);
		stable_sort(idx.begin(), idx.end(), [&Wp](size_t i1, size_t i2) {
			return Wp[i1] > Wp[i2];
		});
		U.MapCols(Matrix::Mode::Assign, idx);
		for (size_t n = Nw; n < N0; n++)
			idx.push_back(n);
		W.MapRows(Matrix::Mode::Assign, idx);
		V.MapCols(Matrix::Mode::Assign, idx);
	}

//	std::cout << "\nA = " << _A << ";";
//	std::cout << "\nU = " << A << ";";
//	std::cout << "\nS = " << W << ";";
//	std::cout << "\nV = " << V << ";";
//	std::cout << '\n';
}

Matrix SVD::Solve(const Matrix &Y, double maxCond, double minAllowed) const {

	const size_t NI = V.Size(0);
	const size_t NJ = W.Numel();
	const size_t NK = U.Size(0);
	const size_t NL = Y.Numel() / Y.Size(0);

	if (V.Size(1) != NJ)
		throw std::runtime_error(
				"V has to have as many columns as there are singular values.");
	if (U.Size(1) != NJ)
		throw std::runtime_error(
				"U has to have as many columns as there are singular values.");
	if (Y.Size(0) != NK)
		throw std::runtime_error("Y has to have as many rows as U has rows.");

	auto dims = Y.GetDimensions();
	if (dims.empty())
		throw std::runtime_error("SVD::Solve - Y has no dimensions.");
	dims[0] = NI;
	Matrix X = Matrix::Zeros(dims);

	const double smax = W.AllMax();
	const double smin = (std::isinf(maxCond)) ? 0.0 : (smax / maxCond);

	auto Uc = U.data();
	auto Wc = W.data();
	auto Vc = V.data();
	auto Xc = X.data();
	auto Yc = Y.data();
	const size_t Ucol = U.Size(0);
	const size_t Vcol = V.Size(0);

	for (size_t J = 0; J < NJ; J++) {
		const double SV = Wc[J];
		if (SV < smin || SV < minAllowed || SV <= DBL_EPSILON)
			continue;
		const double SVI = 1.0 / SV;
		for (size_t L = 0; L < NL; L++) {
			size_t idxY = L * NK;
			size_t idxU = J * Ucol;
			for (size_t K = 0; K < NK; K++) {
				size_t idxX = L * NI;
				size_t idxV = J * Vcol;
				for (size_t I = 0; I < NI; I++) {
					Xc[idxX] += Vc[idxV] * SVI * Uc[idxU] * Yc[idxY];
					idxV++;
					idxX++;
				}
				idxU++;
				idxY++;
			}
		}
	}

	return X;
}

Matrix SVD::Variation(double maxCond, double minAllowed) const {
	const size_t NI = V.Size(0);
	Matrix X = Matrix::Eye(NI, NI);

	const double smax = W.AllMax();
	const double smin = (std::isinf(maxCond)) ? 0.0 : (smax / maxCond);

	auto Wc = W.data();
	auto Vc = V.data();
	const size_t Vcol = V.Size(0);

	for (size_t J = 0; J < Vcol; J++) {
		const double SV = Wc[J];
		if (SV < smin || SV < minAllowed || SV <= DBL_EPSILON)
			continue;
		for (size_t I = 0; I < NI; I++) {
			for (size_t K = 0; K < NI; K++) {
				X(I, K) -= V(I, J) * V(K, J);
			}
		}
	}
	return X;
}

double SVD::Cond() const {
	return W.AllMax() / W.AllMin();
}

