#include "./headers/BEMT.h"

void getSolverInfo(SolverInfo& solverInfoInstance)
{
    std::string input;
    std::cout << "Enter the Solver Info file: " << std::endl;
    std::getline(std::cin, input);
    solverInfoInstance.SolverFileStr = input;
    solverInfoInstance.getInfo();
}
void getGlobalInfo(GlobalVars& globs, std::string& SourceFile)
{
    globs.readGlobalVariables(SourceFile);
}

void getRotorsInfo(AllRotors& AllRotorsInstance, std::string& SourceFile)
{
    AllRotorsInstance.readAllRotorsData(SourceFile);
    AllRotorsInstance.readPolars();
}
void prepareSolutionRotor(Rotor& rotor, SolverInfo& solverInfoInstance, SolRotor& solrotor)
{
    solrotor.setVals(rotor);
    solrotor.inputFileName = solverInfoInstance.RotorSolverInputFileStr;
}
void solveSolutionRotor(SolRotor& solrotor)
{
    auto start2 = high_resolution_clock::now();
    solrotor.readRotorInputs();
    solrotor.rotateSteadyRotor();
    auto stop2 = high_resolution_clock::now();
    auto duration2 = duration_cast<microseconds>(stop2 - start2);
    std::cout << duration2.count() << " microseconds" << std::endl;
    if (1) {
        solrotor.saveSummaryData();
        solrotor.saveElem2DData();
        solrotor.saveElem1DData();
        solrotor.saveLiftVecElem();
        solrotor.saveLiftVecShaft();
        solrotor.saveDragVecElem();
        solrotor.saveDragVecShaft();
        solrotor.savePitchMomentVecElem();
        solrotor.savePitchMomentVecShaft();
        solrotor.saveMomentVecShaft();
    }
}

int main()
{
    //Read Solver Info Start
    SolverInfo solverInfoInstance;
    getSolverInfo(solverInfoInstance);
    //Read Solver Info Start

    //Read Global Variables Start
    GlobalVars& globs = GlobalVars::getInstance();
    getGlobalInfo(globs, solverInfoInstance.EnviromentFileStr);
    //Read Global Variables End

    //Read Rotors info start
    AllRotors myRotors;
    getRotorsInfo(myRotors, solverInfoInstance.modelFileStr);
    //Read Rotors info end

    SolRotor* MySolRotor = new SolRotor();
    prepareSolutionRotor(myRotors.rotors[0], solverInfoInstance, *MySolRotor);

    solveSolutionRotor(*MySolRotor);


    delete MySolRotor;
    std::cout << "Hello World!\n";
    return 1;
}


