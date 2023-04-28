#pragma once

class TrimVariable
{
public:
	double* varPtr = nullptr;
	double initialVal = 0.0;
	double maxLimVal = 0.0;
	double minLimVal = 0.0;
	bool OnOff = false;
	std::string varName;
};

class TrimTarget
{
public:
	double* targetPtr = nullptr;
	double targetVal = 0.0;
};
