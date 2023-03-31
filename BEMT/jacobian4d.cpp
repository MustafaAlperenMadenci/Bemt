void find_jacobians4D(std::function<void(double[], double[])> func, double f_res0[4], double x[4], double jacMat[4][4])
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