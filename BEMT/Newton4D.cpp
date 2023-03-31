#include "BEMT.h"

void Newton4D::find_jacobians4D(std::function<void(double[], double[])> func, double f_res0[4], double x[4], double jacMat[4][4])
{
    double fn_1[4];
    double fn_2[4];
    double fn_3[4];
    double fn_4[4];
    double f0_1 = f_res0[0];
    double f0_2 = f_res0[1];
    double f0_3 = f_res0[2];
    double f0_4 = f_res0[3];
    double x1 = x[0];
    double x2 = x[1];
    double x3 = x[2];
    double x4 = x[3];
    double x0_1[4] = { x[0] + epsilon, x[1], x[2], x[3] };
    double x0_2[4] = { x[0], x[1] + epsilon, x[2], x[3] };
    double x0_3[4] = { x[0], x[1], x[2] + epsilon, x[3] };
    double x0_4[4] = { x[0], x[1], x[2], x[3] + epsilon };
    func(x0_1, fn_1);
    func(x0_2, fn_2);
    func(x0_3, fn_3);
    func(x0_4, fn_4);
    jacMat[0][0] = (fn_1[0] - f0_1) / epsilon;
    jacMat[0][1] = (fn_1[1] - f0_2) / epsilon;
    jacMat[0][2] = (fn_1[2] - f0_3) / epsilon;
    jacMat[0][3] = (fn_1[3] - f0_4) / epsilon;
    jacMat[1][0] = (fn_2[0] - f0_1) / epsilon;
    jacMat[1][1] = (fn_2[1] - f0_2) / epsilon;
    jacMat[1][2] = (fn_2[2] - f0_3) / epsilon;
    jacMat[1][3] = (fn_2[3] - f0_4) / epsilon;
    jacMat[2][0] = (fn_3[0] - f0_1) / epsilon;
    jacMat[2][1] = (fn_3[1] - f0_2) / epsilon;
    jacMat[2][2] = (fn_3[2] - f0_3) / epsilon;
    jacMat[2][3] = (fn_3[3] - f0_4) / epsilon;
    jacMat[3][0] = (fn_4[0] - f0_1) / epsilon;
    jacMat[3][1] = (fn_4[1] - f0_2) / epsilon;
    jacMat[3][2] = (fn_4[2] - f0_3) / epsilon;
    jacMat[3][3] = (fn_4[3] - f0_4) / epsilon;
}

void Newton4D::determinantOfMatrix4D(double matrix[4][4], double* determinant)
{
    double a = matrix[0][0];
    double b = matrix[0][1];
    double c = matrix[0][2];
    double d = matrix[0][3];

    double e = matrix[1][0];
    double f = matrix[1][1];
    double g = matrix[1][2];
    double h = matrix[1][3];

    double i = matrix[2][0];
    double j = matrix[2][1];
    double k = matrix[2][2];
    double l = matrix[2][3];

    double m = matrix[3][0];
    double n = matrix[3][1];
    double o = matrix[3][2];
    double p = matrix[3][3];

    double det =
        a * (f * (k * p - l * o) - g * (j * p - l * n) + h * (j * o - k * n)) -
        b * (e * (k * p - l * o) - g * (i * p - l * m) + h * (i * o - k * m)) +
        c * (e * (j * p - l * n) - f * (i * p - l * m) + h * (i * n - j * m)) -
        d * (e * (j * o - k * n) - f * (i * o - k * m) + g * (i * n - j * m));

    *determinant = det;
}

void Newton4D::cramerRule4D(double a_mat[4][4], double y_mat[4], double* x01_diff, double* x02_diff, double* x03_diff, double* x04_diff)
{
    int size_y_mat = 4;
    int i, j;
    double cramMat[4][4]{ 0.0 };
    double res[4]{ 0. };
    double determinantCramMat, determinantAMat;
    for (i = 0; i < size_y_mat; i++) {
        for (int k = 0; k < size_y_mat; k++) {
            for (int l = 0; l < size_y_mat; l++) {
                cramMat[k][l] = a_mat[k][l];
            }
        }
        for (j = 0; j < size_y_mat; j++) {
            cramMat[j][i] = y_mat[j];
        }
        determinantOfMatrix4D(cramMat, &determinantCramMat); //call the 3x3 determinant function
        determinantOfMatrix4D(a_mat, &determinantAMat); //call the 3x3 determinant function
        res[i] = determinantCramMat / determinantAMat;
    }
    *x01_diff = res[0];
    *x02_diff = res[1];
    *x03_diff = res[2];
    *x04_diff = res[3];
}

void Newton4D::newtonMain(std::function<void(double[], double[])> objFunc, double x0[])
{

}

