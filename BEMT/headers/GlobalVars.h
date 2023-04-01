#pragma once
class GlobalVars
{
public:
	static GlobalVars& getInstance();
	void readGlobalVariables(const std::string& filename);
	double getTemp_c();
	double getSpeedOfSound_ms();
	double getDensity_kgm3();
	Vector3D getMuVec(double tipSpeed_ms);


private:
	GlobalVars() {};
	GlobalVars(const GlobalVars&) = delete;
	GlobalVars& operator=(const GlobalVars&) = delete;
	double temp_c = 0.;
	double density_kgm3 = 0.;
	Vector3D AirVelocityVec_ms;
};

