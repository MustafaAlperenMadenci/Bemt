#pragma once
class NewtonND
{
private:
	static const int maxD = 10;
	int curD = 0;

	double x0[maxD]{ 0.0 };
	double fn[maxD]{ 0.0 };
	double jacMat[maxD][maxD]{ 0.0 };
	double yMat[maxD]{ 0.0 };
	double fn_jac[maxD][maxD]{ 0.0 };
	double x0_eps[maxD][maxD]{ 0.0 };
	double cramMat[maxD][maxD]{ 0.0 };
	double cram_res[maxD]{ 0.0 };
	double x0_diff[maxD]{ 0.0 };

	double determinantCramMat = 0.0;
	double determinantAMat = 0.0;


public:
	int status = 0;
	int numberOfIteration = 0;
	int max_iter = 1000;
	double epsilon = 0.0001;
	double tol = 0.0001;
	double xSol[maxD]{ 0.0 };

	void find_jacobiansND(std::function<void(double[], double[])> func);
	double determinantOfMatrixND(double matrix[maxD][maxD], int n);
	void cramerRuleND();
	void newtonMain(std::function<void(double[], double[])> objFunc, double x0[], int dim);

	bool is_zero(double matrix[maxD][maxD]);
	double find2Norm(double target[maxD]);

};

