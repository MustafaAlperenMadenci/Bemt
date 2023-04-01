#pragma once
class Newton1D
{
public:
	int status = 0;
	int numberOfIteration = 0;
	int max_iter = 1000;
	double epsilon = 0.0001;
	double xSol = 0.0;


    void NewtonRaphson(std::function<double(double)> func, double x0);
		
};

