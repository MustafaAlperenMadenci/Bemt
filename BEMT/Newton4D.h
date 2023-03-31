#pragma once
class Newton4D
{
public:
	int status = 0;
	int numberOfIteration = 0;
	int max_iter = 1000;
	double epsilon = 0.0001;
	double tol = 0.0001;
	double x1Sol = 0.0;
	double x2Sol = 0.0;
	double x3Sol = 0.0;
	double x4Sol = 0.0;

	void find_jacobians4D(std::function<void(double[], double[])> func, double f_res0[4], double x[4], double jacMat[4][4]);
	void determinantOfMatrix4D(double matrix[4][4], double* determinant);
	void cramerRule4D(double a_mat[4][4], double y_mat[4], double* x01_diff, double* x02_diff, double* x03_diff, double* x04_diff);
	void newtonMain(std::function<void(double[], double[])> objFunc, double x0[]);

	//void find_jacobians3D(std::function<void(double[], double[])> func, double f_res0[3], double x[3], double jacMat[3][3]);
	//void determinantOfMatrix3D(double matrix[3][3], double* determinant);
	//void cramerRule3D(double a_mat[3][3], double y_mat[3], double* x01_diff, double* x02_diff, double* x03_diff);
	//void newtonMain(std::function<void(double[], double[])> objFunc, double x0[]);


};

