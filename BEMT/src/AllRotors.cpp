#include "../headers/BEMT.h"

void AllRotors::textToStruct(std::string filename, AeroPolar* targetPolar, std::string rotorLabel, std::string sectionLabel) {
    char* endp;
    int i = 0;
    int j = 0;
    char line[255];
    FILE* file_pointer = fopen(filename.c_str(), "r");
    if (file_pointer == NULL) {
        std::cout << "Unable to open the file named " << filename << " for Rotor: "
            << rotorLabel << " Section: " << sectionLabel << std::endl;
        return;
    }

    std::cout << filename << " is reading " << "for Rotor: " << rotorLabel
        << " for Section: " << sectionLabel << std::endl;
    while (fgets(line, 255, file_pointer)) {
        j = 0;
        char* field = strtok(line, ",");
        while (field) {
            if (i == 0 && j == 0) {
                field = strtok(NULL, ",");
            }
            else if (i == 0 && j != 0) {
                targetPolar->mach[j - 1] = strtod(field, &endp);
                targetPolar->mach_len++;
                field = strtok(NULL, ",");
            }
            else if (i != 0 && j == 0) {
                targetPolar->aoaDeg[i - 1] = strtod(field, &endp);
                targetPolar->aoaRad[i - 1] = targetPolar->aoaDeg[i - 1] * (M_PI / 180.);
                targetPolar->aoa_len++;
                field = strtok(NULL, ",");
            }
            else {
                targetPolar->data[i - 1][j - 1] = strtod(field, &endp);
                field = strtok(NULL, ",");
            }
            j++;
        }
        i++;
    }
    fclose(file_pointer);
    std::cout << filename << " reading is done " << "for Rotor: " << rotorLabel
        << "for Section: " << sectionLabel << std::endl;
}

void AllRotors::readPolars() {

    for (auto& singleRotor : rotors) {
        for (auto& singleSection : singleRotor.sections) {
            textToStruct(singleSection.ClSource, &singleSection.clp, singleRotor.Label, singleSection.Label);
            textToStruct(singleSection.CdSource, &singleSection.cdp, singleRotor.Label, singleSection.Label);
            textToStruct(singleSection.CmSource, &singleSection.cmp, singleRotor.Label, singleSection.Label);
        }

    }
}

void AllRotors::readAllRotorsData(const std::string& fileName) {
    std::ifstream inputFile(fileName);
    if (!inputFile.is_open()) {
        std::cout << "There is no file in given directory" << std::endl;
        std::cout << "Your current location is: " << std::endl;
        getCurrentWorkLoc();
        return;
    }
    else {
        std::cout << "The rotor file is found" << std::endl;
        getCurrentWorkLoc();
    }

    std::string line;
    std::string idef;
    std::string equalSign;
    int intVal;
    double doubleVal;
    std::string stringVal;
    while (std::getline(inputFile, line)) {
        if (line.empty()) {
            continue;
        }
        std::istringstream iss(line);
        iss >> idef;
        if (idef[0] == '!' && idef[1] == '*') {
            std::cout << "Reading the general info..." << std::endl;
            while (std::getline(inputFile, line)) {
                if (line.empty()) {
                    continue;
                }
                std::istringstream iss2(line);
                int value;
                iss2 >> idef;
                if (idef == "N_Of_Rotors") {
                    iss2 >> equalSign >> value;

                    this->nOfRotors = value;
                    std::cout << "Number of Rotors is = " << this->nOfRotors << std::endl;
                }
                else if (idef[0] == '!' && idef[1] == '!' && idef[2] == '*') {
                    std::cout << "Reading the rotor info..." << std::endl;
                    Rotor* myRotor = new Rotor();
                    while (std::getline(inputFile, line)) {
                        if (line.empty()) {
                            continue;
                        }
                        std::istringstream iss3(line);
                        iss3 >> idef;
                        if (idef == "Label") {
                            iss3 >> equalSign >> stringVal;
                            myRotor->Label = stringVal;
                            std::cout << "Rotor Name is : " << myRotor->Label << std::endl;
                        }
                        else if (idef == "N_Of_Blades") {
                            iss3 >> equalSign >> intVal;
                            myRotor->nOfBlades = intVal;
                            std::cout << "Number of Blades is = " << myRotor->nOfBlades << std::endl;
                        }
                        else if (idef == "N_Of_Sections") {
                            iss3 >> equalSign >> intVal;
                            myRotor->nOfSections = intVal;
                            std::cout << "Number of Sections is = " << myRotor->nOfSections << std::endl;
                        }
                        else if (idef == "Len_Blade") {
                            iss3 >> equalSign >> doubleVal;
                            myRotor->LenBlade = doubleVal;
                            std::cout << "Length of Blade is = " << myRotor->LenBlade << std::endl;
                        }
                        else if (idef == "BladeMassPerArea") {
                            iss3 >> equalSign >> doubleVal;
                            myRotor->bladeMassPerArea_kgm2 = doubleVal;
                            std::cout << "Mass Per Area is = " << myRotor->bladeMassPerArea_kgm2 << std::endl;
                        }
                        else if (idef == "Rotation_Speed") {
                            iss3 >> equalSign >> doubleVal;
                            myRotor->RotationSpeed = doubleVal;
                            std::cout << "Rotation Speed [rps] of Blade is = " << myRotor->RotationSpeed << std::endl;
                        }
                        else if (idef == "Hinge_Offset") {
                            iss3 >> equalSign >> doubleVal;
                            myRotor->hingeOffSet_nd = doubleVal;
                            std::cout << "Hinge Offset of rotor is = " << myRotor->hingeOffSet_nd << std::endl;
                        }
                        else if (idef == "EarthToShaft_XRot_deg") {
                            iss3 >> equalSign >> doubleVal;
                            myRotor->etoshX_deg = doubleVal;
                            std::cout << "Earth to shaft X rotation is = " << myRotor->etoshX_deg << std::endl;
                            myRotor->etoshX_rad = (myRotor->etoshX_deg / 180.0) * M_PI;
                        }
                        else if (idef == "EarthToShaft_YRot_deg") {
                            iss3 >> equalSign >> doubleVal;
                            myRotor->etoshY_deg = doubleVal;
                            std::cout << "Earth to shaft X rotation is = " << myRotor->etoshY_deg << std::endl;
                            myRotor->etoshY_rad = (myRotor->etoshY_deg / 180.0) * M_PI;
                        }
                        else if (idef == "EarthToShaft_ZRot_deg") {
                            iss3 >> equalSign >> doubleVal;
                            myRotor->etoshZ_deg = doubleVal;
                            std::cout << "Earth to shaft Z rotation is = " << myRotor->etoshZ_deg << std::endl;
                            myRotor->etoshZ_rad = (myRotor->etoshZ_deg / 180.0) * M_PI;
                        }
                        else if (idef == "N_Of_SolutionElements") {
                            iss3 >> equalSign >> intVal;
                            myRotor->nOfSolutionElements = intVal;
                            std::cout << "Number of solution elements is = " << myRotor->nOfSolutionElements << std::endl;
                        }
                        else if (idef == "N_Of_SolutionAzimuths") {
                            iss3 >> equalSign >> intVal;
                            myRotor->nOfSolutionAzimuths = intVal;
                            std::cout << "Number of solution azimuths is = " << myRotor->nOfSolutionAzimuths << std::endl;
                        }
                        else if (idef[0] == '!' && idef[1] == '!' && idef[2] == '!' && idef[3] == '*') {
                            std::cout << "Reading the Section info..." << std::endl;
                            //BladeSection mySec;
                            BladeSection* mySec = new BladeSection();
                            while (std::getline(inputFile, line)) {
                                if (line.empty()) {
                                    continue;
                                }
                                std::istringstream iss4(line);
                                iss4 >> idef;
                                if (idef == "Label") {
                                    iss4 >> equalSign >> stringVal;
                                    mySec->Label = stringVal;
                                    std::cout << "Section name is : " << mySec->Label << std::endl;
                                }
                                else if (idef == "LStart") {
                                    iss4 >> equalSign >> doubleVal;
                                    mySec->RSt = doubleVal;
                                    std::cout << "Section starts from R =  : " << mySec->RSt << std::endl;
                                }
                                else if (idef == "LEnd") {
                                    iss4 >> equalSign >> doubleVal;
                                    mySec->RFn = doubleVal;
                                    std::cout << "Section ends at R =  : " << mySec->RFn << std::endl;
                                }
                                else if (idef == "TwStart") {
                                    iss4 >> equalSign >> doubleVal;
                                    mySec->TwSt = doubleVal;
                                    std::cout << "Section angle at start =  : " << mySec->TwSt << std::endl;
                                }
                                else if (idef == "TwEnd") {
                                    iss4 >> equalSign >> doubleVal;
                                    mySec->TwFn = doubleVal;
                                    std::cout << "Section angle at start =  : " << mySec->TwFn << std::endl;
                                }
                                else if (idef == "ChordStart_m") {
                                    iss4 >> equalSign >> doubleVal;
                                    mySec->ChordSt_m = doubleVal;
                                    std::cout << "Section chord start is =  : " << mySec->ChordSt_m << std::endl;
                                }
                                else if (idef == "ChordEnd_m") {
                                    iss4 >> equalSign >> doubleVal;
                                    mySec->ChordFn_m = doubleVal;
                                    std::cout << "Section chord end is =  : " << mySec->ChordFn_m << std::endl;
                                }
                                else if (idef == "ClSourceFile") {
                                    iss4 >> equalSign >> stringVal;
                                    mySec->ClSource = stringVal;
                                    std::cout << "Section Cl source file is : " << mySec->ClSource << std::endl;
                                }
                                else if (idef == "CdSourceFile") {
                                    iss4 >> equalSign >> stringVal;
                                    mySec->CdSource = stringVal;
                                    std::cout << "Section Cd source file is : " << mySec->CdSource << std::endl;
                                }
                                else if (idef == "CmSourceFile") {
                                    iss4 >> equalSign >> stringVal;
                                    mySec->CmSource = stringVal;
                                    std::cout << "Section Cm source file is : " << mySec->CmSource << std::endl;
                                }
                                else if (idef[0] == '!' && idef[1] == '!' && idef[2] == '!' && idef[3] == '-') {
                                    std::cout << "Reading the Section info Complete" << std::endl;
                                    myRotor->sections.push_back(*mySec);
                                    delete mySec;
                                    break;
                                }
                            }
                        }
                        else if (idef[0] == '!' && idef[1] == '!' && idef[2] == '-') {
                            std::cout << "Reading the rotor info Complete" << std::endl;
                            this->rotors.push_back(*myRotor);
                            delete myRotor;
                            break;
                        }
                    }
                }
                else if (idef[0] == '!' && idef[1] == '-') {
                    std::cout << "Rotor Input reading is Done" << std::endl;
                    break;
                }
                else {
                    std::cout << "Wrong Input Format!! Code 1" << std::endl;
                }
            }
        }
    }

    inputFile.close();
}
