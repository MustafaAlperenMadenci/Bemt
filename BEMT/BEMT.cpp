#include "./headers/BEMT.h"

//On my second computer i wrote this.

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

    if (solverInfoInstance.SolverTypeStr == "Steady")
    {
        solrotor.inputFileName = solverInfoInstance.RotorSolverInputFileStr;
        solrotor.readRotorInputs();
        if (solrotor.solverType != "Steady")
        {
            std::cout << "Solver Type and Solver Input Files are incompatible.." << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    else if (solverInfoInstance.SolverTypeStr == "Trim")
    {
        solrotor.trimInputFileName = solverInfoInstance.RotorSolverInputFileStr;
        solrotor.readTrimInputFile();
        if (solrotor.solverType != "Trim")
        {
            std::cout << "Solver Type and Solver Input Files are incompatible.." << std::endl;
            exit(EXIT_FAILURE);
        }
        solrotor.readTrimVariables();

    }
    
}
void solveSolutionRotor(SolRotor& solrotor)
{

    auto start2 = high_resolution_clock::now();
    //solrotor.readRotorInputs();
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

    MySolRotor->trimRotor();
    //std::vector<SolRotor*> mySolRotors; // Burasý önemli, heape kaydediyor silindiðinden emin olunmalý.
    //
    //
    //int i = 0;
    //
    //for (auto & myRotor: myRotors.rotors) {
    //    mySolRotors.push_back(new SolRotor());
    //    mySolRotors[i]->setVals(myRotors.rotors[i]);
    //    
    //    auto start2 = high_resolution_clock::now();
    //    mySolRotors[i]->rotateSteadyRotor();
    //    auto stop2 = high_resolution_clock::now();
    //    auto duration2 = duration_cast<microseconds>(stop2 - start2);
    //    std::cout << duration2.count() << " microseconds" << std::endl;
    //
    //    
    //    if (1) {
    //        mySolRotors[i]->saveSummaryData();
    //        mySolRotors[i]->saveElem2DData();
    //        mySolRotors[i]->saveElem1DData();
    //        mySolRotors[i]->saveLiftVecElem();
    //        mySolRotors[i]->saveLiftVecShaft();
    //        mySolRotors[i]->saveDragVecElem();
    //        mySolRotors[i]->saveDragVecShaft();
    //        mySolRotors[i]->savePitchMomentVecElem();
    //        mySolRotors[i]->savePitchMomentVecShaft();
    //        mySolRotors[i]->saveMomentVecShaft();
    //    }
    //
    //
    //    auto start4 = high_resolution_clock::now();
    //    mySolRotors[i]->trimRotor();
    //    auto stop4 = high_resolution_clock::now();
    //    auto duration4 = duration_cast<microseconds>(stop4 - start4);
    //    std::cout << duration4.count() << " microseconds" << std::endl;
    //
    //    i++;
    //}

    


    //deletin classes
    //for (auto ptr : mySolRotors) {
    //    delete ptr;
    //}
    delete MySolRotor;
    std::cout << "Hello World!\n";
    return 1;
}


