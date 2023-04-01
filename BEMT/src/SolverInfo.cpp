#include "../headers/BEMT.h"

SolverInfo::SolverInfo()
{
}

void SolverInfo::getInfo()
{
	std::ifstream inputFile(SolverFileStr);
    if (!inputFile.is_open()) {
        std::cout << "There is no file in given directory for SolverFile " << std::endl;
        std::cout << "Your current location is: " << std::endl;
        getCurrentWorkLoc();
        exit(EXIT_FAILURE);
        return;
    }
    else {
        std::cout << "Rotor inputs are being read ..." << std::endl;
        getCurrentWorkLoc();
    }
    std::string line;
    std::string stringVal;
    char separator = '=';

    while (std::getline(inputFile, line)) {
        if (line.empty()) {
            continue;
        }
        std::istringstream iss(line);
        std::string field_name;
        if (std::getline(iss, field_name, separator)) {
            string_trim(field_name);
            if (field_name == "ModelType") {
                iss >> stringVal;
                this->modelTypeStr = stringVal;
            }
            else if (field_name == "ModelFile") {
                iss >> stringVal;
                this->modelFileStr = stringVal;
            }
            else if (field_name == "SolverType") {
                iss >> stringVal;
                this->SolverTypeStr = stringVal;
            }
            else if (field_name == "EnviromentFile") {
                iss >> stringVal;
                this->EnviromentFileStr = stringVal;
            }
            else if (field_name == "RotorSolverInputFile") {
                iss >> stringVal;
                this->RotorSolverInputFileStr = stringVal;
            }
        }
    }
}
