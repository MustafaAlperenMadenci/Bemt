#pragma once
class SolverInfo
{
private:
	
public:
	SolverInfo();
	SolverInfo(std::string SolverFileStr) {
		SolverFileStr = SolverFileStr;
	}
	std::string SolverFileStr;
	std::string modelTypeStr;
	std::string modelFileStr;
	std::string SolverTypeStr;
	std::string EnviromentFileStr;
	std::string RotorSolverInputFileStr;

	void getInfo();
};

