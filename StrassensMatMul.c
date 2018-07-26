#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

#define N 4

typedef struct matrixStruct {
	int rStart;
	int rEnd;
	int cStart;
	int cEnd;
	int matS[N][N];
} matrix;

matrix matA = { 0, N - 1, 0, N - 1 };
matrix matB = { 0, N - 1, 0, N - 1 };
matrix matC;
int ctr = 0;

matrix inputMat(matrix mat) {
	int i, j;
	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++) {
			//scanf("%d", &mat.matS[i][j]);
			mat.matS[i][j] = rand();
		}
	return mat;
}

void writeMat(matrix mat)
{
	int i, j;
	for (i = mat.rStart; i <= mat.rEnd; i++) {
		for (j = mat.cStart; j <= mat.cEnd; j++)
			printf("%d ", mat.matS[i][j]);
		printf("\n");
	}
	printf("\n");
}

matrix addMat(matrix X, matrix Y) {
	matrix mat;
	int iX, iY, jX, jY, i, j;

	mat.rEnd = mat.cEnd = X.rEnd - X.rStart;
	mat.cStart = mat.rStart = 0;

	iY = Y.rStart;
	i = 0;
		for (iX = X.rStart; iX <= X.rEnd; iX++) 
		{
			for (jX = X.cStart, jY = Y.cStart, j = 0; jX <= X.cEnd; jX++, jY++, j++)
			{
				mat.matS[i][j] = X.matS[iX][jX] + Y.matS[iY][jY];
			}
			iY++;
			i++;
		}
	return mat;
}

matrix subMat(matrix X, matrix Y) {
	matrix mat;
	int iX, iY, jX, jY, i, j;

	mat.rEnd = mat.cEnd = X.rEnd - X.rStart;
	mat.cStart = mat.rStart = 0;

	iY = Y.rStart;
	i = 0;
		for (iX = X.rStart; iX <= X.rEnd; iX++) 
		{
			for (jX = X.cStart, jY = Y.cStart, j = 0; jX <= X.cEnd; jX++, jY++, j++)
			{
				mat.matS[i][j] = X.matS[iX][jX] - Y.matS[iY][jY];
			}
			iY++;
			i++;
		}
	return mat;
}

matrix mulMat(matrix X, matrix Y) {
	matrix A, B, C, D, E, F, G, H;
	matrix P1, P2, P3, P4, P5, P6, P7;
	matrix Q1, Q2, Q3, Q4;
	matrix matAns;

	int iX, jX, i, j;
	int n = X.rEnd - X.rStart + 1;
	if (n <= 2) {
		matrix matAns1 = X;
		int a, b, c, d, e, f, g, h;
#pragma omp parallel sections shared(X, Y)
		{

#pragma omp section
			{
				a = X.matS[X.rStart][X.cStart];
				b = X.matS[X.rStart][X.cStart + 1];
			}
#pragma omp section
			{
				c = X.matS[X.rStart + 1][X.cStart];
				d = X.matS[X.rStart + 1][X.cStart + 1];
			}
#pragma omp section
			{
				e = Y.matS[Y.rStart][Y.cStart];
				f = Y.matS[Y.rStart][Y.cStart + 1];
			}
#pragma omp section
			{
				g = Y.matS[Y.rStart + 1][Y.cStart];
				h = Y.matS[Y.rStart + 1][Y.cStart + 1];
			}

		}
			matAns1.matS[matAns1.rStart][matAns1.cStart] = a * e + b * g;
			matAns1.matS[matAns1.rStart][matAns1.cStart + 1] = a * f + b * h;
			matAns1.matS[matAns1.rStart + 1][matAns1.cStart] = c * e + d * g;
			matAns1.matS[matAns1.rStart + 1][matAns1.cStart + 1] = c * f + d * h;

			return matAns1;
	}
	matAns.rStart = matAns.cStart = 0;
	matAns.rEnd = matAns.cEnd = n - 1;

	A = B = C = D = X;
	E = F = G = H = Y;

#pragma omp parallel sections firstprivate(X, Y)
	{
#pragma omp section
		{
			A.rStart = X.rStart;
			A.rEnd = X.rEnd / 2;
			A.cStart = X.cStart;
			A.cEnd = X.cEnd / 2;
		}
#pragma omp section
		{
			B.rStart = X.rStart;
			B.rEnd = X.rEnd / 2;
			B.cStart = (X.cEnd / 2) + 1;
			B.cEnd = X.cEnd;
		}
#pragma omp section
		{
			C.rStart = (X.rEnd / 2) + 1;
			C.rEnd = X.rEnd;
			C.cStart = X.cStart;
			C.cEnd = X.cEnd / 2;
		}
#pragma omp section
		{
			D.rStart = (X.rEnd / 2) + 1;
			D.rEnd = X.rEnd;
			D.cStart = (X.cEnd / 2) + 1;
			D.cEnd = X.cEnd;
		}
#pragma omp section
		{
			E.rStart = Y.rStart;
			E.rEnd = Y.rEnd / 2;
			E.cStart = Y.cStart;
			E.cEnd = Y.cEnd / 2;
		}
#pragma omp section
		{
			F.rStart = Y.rStart;
			F.rEnd = Y.rEnd / 2;
			F.cStart = (Y.cEnd / 2) + 1;
			F.cEnd = Y.cEnd;
		}
#pragma omp section
		{
			G.rStart = (Y.rEnd / 2) + 1;
			G.rEnd = Y.rEnd;
			G.cStart = Y.cStart;
			G.cEnd = Y.cEnd / 2;
		}
#pragma omp section
		{
			H.rStart = (Y.rEnd / 2) + 1;
			H.rEnd = Y.rEnd;
			H.cStart = (Y.cEnd / 2) + 1;
			H.cEnd = Y.cEnd;
		}
	}

#pragma omp parallel sections
	{
#pragma omp section
		P1 = mulMat(A, subMat(F, H));
#pragma omp section
		P2 = mulMat(addMat(A, B), H);
#pragma omp section
		P3 = mulMat(addMat(C, D), E);
#pragma omp section
		P4 = mulMat(D, subMat(G, E));
#pragma omp section
		P5 = mulMat(addMat(A, D), addMat(E, H));
#pragma omp section
		P6 = mulMat(subMat(B, D), addMat(G, H));
#pragma omp section
		P7 = mulMat(subMat(A, C), addMat(E, F));
	}

	Q1 = addMat(subMat(addMat(P5, P4), P2), P6);
	Q2 = addMat(P1, P2);
	Q3 = addMat(P3, P4);
	Q4 = subMat(subMat(addMat(P1, P5), P3), P7);

	for (iX = Q1.rStart, i = 0; iX <= Q1.rEnd; iX++, i++)
		for (jX = Q1.cStart, j = 0; jX <= Q1.cEnd; jX++, j++)
			matAns.matS[i][j] = Q1.matS[iX][jX];

	for (iX = Q2.rStart, i = 0; iX <= Q2.rEnd; iX++, i++)
		for (jX = Q2.cStart, j = n / 2; jX <= Q2.cEnd; jX++, j++)
			matAns.matS[i][j] = Q2.matS[iX][jX];

	for (iX = Q3.rStart, i = n / 2; iX <= Q3.rEnd; iX++, i++)
		for (jX = Q3.cStart, j = 0; jX <= Q3.cEnd; jX++, j++)
			matAns.matS[i][j] = Q3.matS[iX][jX];

	for (iX = Q4.rStart, i = n / 2; iX <= Q4.rEnd; iX++, i++)
		for (jX = Q4.cStart, j = n / 2; jX <= Q4.cEnd; jX++, j++)
			matAns.matS[i][j] = Q4.matS[iX][jX];
	
	return matAns;
}

void main() {

	matA = inputMat(matA);
	matB = inputMat(matB);

	writeMat(matA);
	writeMat(matB);

	printf("\nThe ans\n");
	double start, end;
	start = omp_get_wtime();
	mulMat(matA, matB);
	end = omp_get_wtime();
	writeMat(mulMat(matA, matB));
	printf("Time of Execution: %f msec\n", (end - start) * 1000);
	system("pause");
}



