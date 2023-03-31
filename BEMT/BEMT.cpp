
#include "BEMT.h"

int main()
{
    GlobalVars& globs = GlobalVars::getInstance();
    globs.readGlobalVariables("GlobalVariables.dat");
    AllRotors myRotors;
    myRotors.readAllRotorsData("RotorProps.dat");
    myRotors.readPolars();

    std::vector<SolRotor*> mySolRotors; // Burasý önemli, heape kaydediyor silindiðinden emin olunmalý.
    
    double Solved_Input[3] = { 0.0, 0.0, 0.0 };
    double Output[3];
    NewtonND NewtonRaphsonSolver;
    int i = 0;
    
    for (auto & myRotor: myRotors.rotors) {
        double Init_Input[3] = { -0.001,-0.001,0.001 };
        mySolRotors.push_back(new SolRotor());
        mySolRotors[i]->setVals(myRotors.rotors[i]);
        
        auto start2 = high_resolution_clock::now();
        //NewtonRaphsonSolver.newtonMain([&](double in[], double out[]) { mySolRotors[i]->rotateSolRot(in, out); }, Init_Input, 3);
        mySolRotors[i]->rotateSteadyRotor();
        auto stop2 = high_resolution_clock::now();
        auto duration2 = duration_cast<microseconds>(stop2 - start2);
        std::cout << duration2.count() << " microseconds" << std::endl;
        Solved_Input[0] = NewtonRaphsonSolver.xSol[0]; Solved_Input[1] = NewtonRaphsonSolver.xSol[1]; Solved_Input[2] = NewtonRaphsonSolver.xSol[2];
        mySolRotors[i]->rotateSolRot(Solved_Input, Output);

        //mySolRotors[i]->flap0_rad = Solved_Input[0];
        //mySolRotors[i]->flapRate0_rads = Solved_Input[1];
        //mySolRotors[i]->indLamb0_nd = Solved_Input[2];
        //mySolRotors[i]->rotateRotor();
        
        if (NewtonRaphsonSolver.status == 1) {
            mySolRotors[i]->saveSummaryData();
            mySolRotors[i]->saveElem2DData();
            mySolRotors[i]->saveElem1DData();
            mySolRotors[i]->saveLiftVecElem();
            mySolRotors[i]->saveLiftVecShaft();
            mySolRotors[i]->saveDragVecElem();
            mySolRotors[i]->saveDragVecShaft();
            mySolRotors[i]->savePitchMomentVecElem();
            mySolRotors[i]->savePitchMomentVecShaft();
            mySolRotors[i]->saveMomentVecShaft();
        }
        auto start3 = high_resolution_clock::now();
        mySolRotors[i]->trimRotorDummy();
        auto stop3 = high_resolution_clock::now();
        auto duration3 = duration_cast<microseconds>(stop3 - start3);
        std::cout << duration3.count() << " microseconds" << std::endl;
        i++;
    }

    


    //deletin classes
    for (auto ptr : mySolRotors) {
        delete ptr;
    }
    std::cout << "Hello World!\n";
    return 1;
}


