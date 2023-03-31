# -*- coding: utf-8 -*-
"""
Created on Sun Mar 19 01:11:23 2023

@author: musta
"""

def generate_jacobian_function(dimensions):
    code = "void find_jacobians" + str(dimensions) + "D(std::function<void(double[], double[])> func, double f_res0[" + str(dimensions) + "], double x[" + str(dimensions) + "], double jacMat[" + str(dimensions) + "][" + str(dimensions) + "])\n{\n"
    for i in range(dimensions):
        code += "    double fn_" + str(i+1) + "[" + str(dimensions) + "];\n"
    code += "    double f0_1 = f_res0[0];\n"
    for i in range(1, dimensions):
        code += "    double f0_" + str(i+1) + " = f_res0[" + str(i) + "];\n"
    code += "    double x1 = x[0];\n"
    for i in range(1, dimensions):
        code += "    double x" + str(i+1) + " = x[" + str(i) + "];\n"
    for i in range(dimensions):
        x0_i = ["x[" + str(j) + "]" if j != i else "x[" + str(j) + "] + epsilon" for j in range(dimensions)]
        code += "    double x0_" + str(i+1) + "[" + str(dimensions) + "] = { " + ", ".join(x0_i) + " };\n"
    for i in range(dimensions):
        code += "    func(x0_" + str(i+1) + ", fn_" + str(i+1) + ");\n"
    for i in range(dimensions):
        for j in range(dimensions):
            if j == 0:
                code += "    jacMat[" + str(i) + "][0] = (fn_" + str(i+1) + "[0] - f0_1) / epsilon;\n"
            else:
                code += "    jacMat[" + str(i) + "][" + str(j) + "] = (fn_" + str(i+1) + "[" + str(j) + "] - f0_" + str(j+1) + ") / epsilon;\n"
    code += "}"
    return code

num_dimensions = 4
cpp_code = generate_jacobian_function(num_dimensions)

print(cpp_code)
with open("jacobian" + str(num_dimensions) + "d.cpp", "w") as f:
    f.write(cpp_code)