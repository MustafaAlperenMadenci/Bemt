#include "./headers/BEMT.h"


int main()
{
    GlobalVars& globs = GlobalVars::getInstance();
    globs.readGlobalVariables("GlobalVariables.dat");
    AllRotors myRotors;
    myRotors.readAllRotorsData("RotorProps.dat");
    myRotors.readPolars();

    std::vector<SolRotor*> mySolRotors; // Burasý önemli, heape kaydediyor silindiðinden emin olunmalý.
    

    int i = 0;
    
    for (auto & myRotor: myRotors.rotors) {
        mySolRotors.push_back(new SolRotor());
        mySolRotors[i]->setVals(myRotors.rotors[i]);
        
        auto start2 = high_resolution_clock::now();
        mySolRotors[i]->rotateSteadyRotor();
        auto stop2 = high_resolution_clock::now();
        auto duration2 = duration_cast<microseconds>(stop2 - start2);
        std::cout << duration2.count() << " microseconds" << std::endl;

        
        if (1) {
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


        auto start4 = high_resolution_clock::now();
        mySolRotors[i]->trimRotor();
        auto stop4 = high_resolution_clock::now();
        auto duration4 = duration_cast<microseconds>(stop4 - start4);
        std::cout << duration4.count() << " microseconds" << std::endl;

        i++;
    }

    


    //deletin classes
    for (auto ptr : mySolRotors) {
        delete ptr;
    }
    std::cout << "Hello World!\n";
    return 1;
}


