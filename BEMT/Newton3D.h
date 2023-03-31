#pragma once
class Newton3D
{
private:
	static const int soc = 3;

	double x0[soc]{ 0.0 };
	double fn[soc]{ 0.0 };
	double jacMat[soc][soc]{ 0.0 };
	double yMat[soc]{ 0.0 };
	double fn_jac[soc][soc]{ 0.0 };
	double x0_eps[soc][soc]{ 0.0 };
	double cramMat[soc][soc]{ 0.0 };
	double cram_res[soc]{ 0.0 };
	double x0_diff[soc]{ 0.0 };

	double determinantCramMat = 0.0;
	double determinantAMat = 0.0;


public:
	int status = 0;
	int numberOfIteration = 0;
	int max_iter = 1000;
	double epsilon = 0.0001;
	double tol = 0.0001;
	//double xSol = 0.0;
	//double ySol = 0.0;
	//double zSol = 0.0;
	double xSol[soc]{ 0.0 };

	void find_jacobians3D(std::function<void(double[], double[])> func);
	double determinantOfMatrix3D(double matrix[soc][soc],int n);
	void cramerRule3D();
	void newtonMain(std::function<void(double[], double[])> objFunc, double x0[]);
	
	bool is_zero(double matrix[soc][soc]);
	double find2Norm(double target[soc]);

};

