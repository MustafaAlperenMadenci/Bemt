#include "BEMT.h"

GlobalVars& GlobalVars::getInstance()
{
    static GlobalVars instance;
    return instance;
}

void GlobalVars::readGlobalVariables(const std::string& filename)
{
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cout << "There is no file in given directory for global variables" << std::endl;
        std::cout << "Your current location is: " << std::endl;
        getCurrentWorkLoc();
        return;
    }
    else {
        std::cout << "The Global Variables are being read ..." << std::endl;
        getCurrentWorkLoc();
    }
    std::string line;
    int intVal;
    double doubleVal;
    char separator = '=';
    std::string stringVal;

    while (std::getline(inputFile, line)) {
        if (line.empty()) {
            continue;
        }
        std::istringstream iss(line);
        std::string field_name;
        if (std::getline(iss, field_name, separator) && (iss >> doubleVal)) {
            string_trim(field_name);
            if (field_name == "Temperature_C") {
                this->temp_c = doubleVal;
            }
            else if (field_name == "Density_kgm3") {
                this->density_kgm3 = doubleVal;
            }
            else if (field_name == "AirVelocityX_ms") {
                this->AirVelocityVec_ms.x = doubleVal;
            }
            else if (field_name == "AirVelocityY_ms") {
                this->AirVelocityVec_ms.y = doubleVal;
            }
            else if (field_name == "AirVelocityZ_ms") {
                this->AirVelocityVec_ms.z = doubleVal;
            }
        }
    }
    AirVelocityVec_ms = Vector3D(AirVelocityVec_ms.x, AirVelocityVec_ms.y, AirVelocityVec_ms.z);
}

double GlobalVars::getTemp_c()
{
    return temp_c;
}

double GlobalVars::getSpeedOfSound_ms()
{
    return sqrt(1.4 * 287.0 * (temp_c + 273.0));
}

double GlobalVars::getDensity_kgm3()
{
    return density_kgm3;
}

Vector3D GlobalVars::getMuVec(double tipSpeed_ms)
{

    return AirVelocityVec_ms / tipSpeed_ms;
}

