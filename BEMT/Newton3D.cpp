#include "BEMT.h"


void Newton3D::find_jacobians3D(std::function<void(double[], double[])> func)
{

    for (int i = 0; i < soc; i++) {
        for (int j = 0; j < soc; j++) {
            x0_eps[i][j] = x0[j];
            if (i == j)
                x0_eps[i][j] = x0_eps[i][j] + epsilon;
        }
        func(x0_eps[i], fn_jac[i]);
    }
    for (int i = 0; i < soc; i++) {
        for (int j = 0; j < soc; j++) {
            jacMat[i][j] = (fn_jac[j][i] - fn[i]) / epsilon;
        }
    }

}

double Newton3D::determinantOfMatrix3D(double matrix[soc][soc], int n) {
    double det = 0;
    double submatrix[soc][soc]{ 0.0 };
    if (n == 2)
        return ((matrix[0][0] * matrix[1][1]) - (matrix[1][0] * matrix[0][1]));
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
            det = det + ((double)pow(-1, x) * matrix[0][x] * determinantOfMatrix3D(submatrix, n - 1));
        }
    }
    return det;
}

void Newton3D::cramerRule3D()
{
    for (int i = 0; i < soc; i++) {
        for (int k = 0; k < soc; k++) {
            for (int l = 0; l < soc; l++) {
                cramMat[k][l] = jacMat[k][l];
            }
        }
        for (int j = 0; j < soc; j++) {
            cramMat[j][i] = yMat[j];
        }
        determinantCramMat = determinantOfMatrix3D(cramMat, soc);
        determinantAMat = determinantOfMatrix3D(jacMat, soc);
        cram_res[i] = determinantCramMat / determinantAMat;
        x0_diff[i] = cram_res[i];
    }
}

void Newton3D::newtonMain(std::function<void(double[], double[])> objFunc, double x_init[])
{
    // Initiliaze the x-input array
    for (int i = 0; i < soc; i++) {
        x0[i] = x_init[i];
    }

    for (int i = 0; i < max_iter; i++) {
        objFunc(x0, fn);

        if (find2Norm(fn) < tol) {
            std::cout << "Solution is found after " << i << " iterations\n";
            for (int m = 0; m < soc; m++) {
                xSol[m] = x0[m];
            }
            numberOfIteration = i;
            status = 1;
            return;
        }

        find_jacobians3D(objFunc); // This will change the values in jacMat matrix !
        
        for (int j = 0; j < soc; j++) {
            yMat[j] = -fn[j];
        }
        if (is_zero(jacMat)) {
            std::cout << "Zero Jacobian. No solution found!";
            for (int m = 0; m < soc; m++) {
                xSol[m] = 0.0;
            }
            numberOfIteration = i;
            status = 0;
            return;
        }
        cramerRule3D();
        for (int subi = 0; subi < soc; subi++) {
            x0[subi] = x0[subi] + x0_diff[subi];
        }
    }
    std::cout << "Exceeded maximum iterations. No solution found. !";
}

bool Newton3D::is_zero(double matrix[soc][soc])
{
    int i = 0; int j = 0;
    for (; i < soc; i++) {
        for (; j < soc; j++) {
            if (matrix[i][j] == 0) {
                return true;
            }
        }
    }
    return false;
}
double Newton3D::find2Norm(double target[soc])
{
    double total = 0.0;
    for (int i = 0; i < soc; i++) {
        total = total + target[i] * target[i];
    }
    double norm2 = sqrt(total);
    return norm2;
}



