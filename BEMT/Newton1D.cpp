#include "BEMT.h"


void Newton1D::NewtonRaphson(std::function<double(double)> func, double x0)
{
    double x = x0;
    double f = func(x);
    double f_prime = (func(x + epsilon) - f) / epsilon;
    int i = 0;
    while (std::abs(f) > epsilon && i <= max_iter)
    {
        
        x -= f / f_prime;
        f = func(x);
        f_prime = (func(x + epsilon) - f) / epsilon;
        i++;
    }
    if (i > max_iter) {
        status = 0;
        xSol = 0;
        numberOfIteration = i;
    }
    else {
        status = 1;
        xSol = x;
        numberOfIteration = i;
    }
}
