#include "BEMT.h"

void NewtonND::find_jacobiansND(std::function<void(double[], double[])> func)
{

    for (int i = 0; i < curD; i++) {
        for (int j = 0; j < curD; j++) {
            x0_eps[i][j] = x0[j];
            if (i == j)
                x0_eps[i][j] = x0_eps[i][j] + epsilon;
        }
        func(x0_eps[i], fn_jac[i]);
    }
    for (int i = 0; i < curD; i++) {
        for (int j = 0; j < curD; j++) {
            jacMat[i][j] = (fn_jac[j][i] - fn[i]) / epsilon;
        }
    }

}

double NewtonND::determinantOfMatrixND(double matrix[maxD][maxD], int n) {
    double det = 0;
    double submatrix[maxD][maxD]{ 0.0 };
    if (n == 2)
        return ((matrix[0][0] * matrix[1][1]) - (matrix[1][0] * matrix[0][1]));
    else if (n == 1) {
        return matrix[0][0];
    }
    else {
        for (int x = 0; x < n; x++) {
            int subi = 0;
            for (int i = 1; i < n; i++) {
                int subj = 0;
                for (int j = 0; j < n; j++) {
                    if (j == x)
                        continue;
                    submatrix[subi][subj] = matrix[i][j];
                    subj++;
                }
                subi++;
            }
            det = det + ((double)pow(-1, x) * matrix[0][x] * determinantOfMatrixND(submatrix, n - 1));
        }
    }
    return det;
}

void NewtonND::cramerRuleND()
{
    for (int i = 0; i < curD; i++) {
        for (int k = 0; k < curD; k++) {
            for (int l = 0; l < curD; l++) {
                cramMat[k][l] = jacMat[k][l];
            }
        }
        for (int j = 0; j < curD; j++) {
            cramMat[j][i] = yMat[j];
        }
        determinantCramMat = determinantOfMatrixND(cramMat, curD);
        determinantAMat = determinantOfMatrixND(jacMat, curD);
        cram_res[i] = determinantCramMat / determinantAMat;
        x0_diff[i] = cram_res[i];
    }
}

void NewtonND::newtonMain(std::function<void(double[], double[])> objFunc, double x_init[], int dim)
{
    curD = dim;
    // Initiliaze the x-input array
    for (int i = 0; i < curD; i++) {
        x0[i] = x_init[i];
    }

    for (int i = 0; i < max_iter; i++) {
        objFunc(x0, fn);

        if (find2Norm(fn) < tol) {
            std::cout << "Solution is found after " << i << " iterations\n";
            for (int m = 0; m < curD; m++) {
                xSol[m] = x0[m];
            }
            numberOfIteration = i;
            status = 1;
            return;
        }

        find_jacobiansND(objFunc); // This will change the values in jacMat matrix !

        for (int j = 0; j < curD; j++) {
            yMat[j] = -fn[j];
        }
        if (is_zero(jacMat)) {
            std::cout << "Zero Jacobian. No solution found!";
            for (int m = 0; m < curD; m++) {
                xSol[m] = 0.0;
            }
            numberOfIteration = i;
            status = 0;
            return;
        }
        cramerRuleND();
        for (int subi = 0; subi < curD; subi++) {
            x0[subi] = x0[subi] + x0_diff[subi];
        }
    }
    std::cout << "Exceeded maximum iterations. No solution found. !";
}

bool NewtonND::is_zero(double matrix[maxD][maxD])
{
    int i = 0; int j = 0;
    for (; i < curD; i++) {
        for (; j < curD; j++) {
            if (matrix[i][j] == 0) {
                return true;
            }
        }
    }
    return false;
}
double NewtonND::find2Norm(double target[maxD])
{
    double total = 0.0;
    for (int i = 0; i < curD; i++) {
        total = total + target[i] * target[i];
    }
    double norm2 = sqrt(total);
    return norm2;

    //double total = 0.0;
    //
    //for (int i = 0; i < dim; i++) {
    //    if (*(target_pointers[i]) == 0.0) {
    //        if (target[i] == 0.0) {
    //            total = total + 0.0;
    //        }
    //        else {
    //            total = total + 1;
    //        }
    //    }
    //    else {
    //        total = total + (target[i] / *(target_pointers[i])) * (target[i] / *(target_pointers[i]));
    //    }
    //
    //}
    //
    //double norm2 = sqrt(total);
    //return norm2;
}