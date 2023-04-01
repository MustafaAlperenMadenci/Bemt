#include "BEMT.h"


void SolRotor::elemAreaCalc()
{
    int i = 0;
    for (i = 0; i < nOfSection; i++) {
        elemArea_m2[i] = elemWidth_m * elemChord_m[i];
    }
}
void SolRotor::rotorAreaCalc()
{
    rotorArea_m2 = M_PI * lenBlade * lenBlade;
}

void SolRotor::readRotorInputs(const std::string& filename)
{
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cout << "There is no file in given directory for rotor input file " << std::endl;
        std::cout << "Your current location is: " << std::endl;
        getCurrentWorkLoc();
        return;
    }
    else {
        std::cout << "Rotor inputs are being read ..." << std::endl;
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
        if (std::getline(iss, field_name, separator)) {
            string_trim(field_name);
            if (field_name == "Label") {
                iss >> stringVal;
                this->Label = stringVal;
            }
            else if (field_name == "Collective_Deg") {
                iss >> doubleVal;
                this->colAngDeg = doubleVal;
                this->colAngRad = (doubleVal / 180.0) * M_PI;
            }
            else if (field_name == "LongCyc_Deg") {
                iss >> doubleVal;
                this->lonAngDeg = doubleVal;
                this->lonAngRad = (doubleVal / 180.0) * M_PI;
            }
            else if (field_name == "LatCyc_Deg") {
                iss >> doubleVal;
                this->latAngDeg = doubleVal;
                this->latAngRad = (doubleVal / 180.0) * M_PI;
            }
            else if (field_name == "N_Of_Elements") {
                iss >> intVal;
                this->nOfSection = intVal;
            }
            else if (field_name == "N_Of_Azimuths") {
                iss >> intVal;
                this->nOfAzimuth = intVal;
            }
        }
    }
}


void SolRotor::setVals(const Rotor& myRot) { // class oluþturulduðunda bunlarý doldurcak þekilde olmalý
    readRotorInputs(myRot.inputFileName);
    rotSpeedRps = myRot.RotationSpeed * (2 * M_PI) / 60.0;
    shaftAngularSpeed_rads.z = rotSpeedRps;  // Vector in shaft axis NOT IN EARTH AXIS !!!
    dt = (2 * M_PI / nOfAzimuth) / rotSpeedRps;
    lenBlade = myRot.LenBlade;
    vTip_ms = lenBlade* rotSpeedRps;

    nOfBlades = myRot.nOfBlades;
    hingeOffSet = myRot.hingeOffSet_nd;
    etoshX_deg = myRot.etoshX_deg;
    etoshY_deg = myRot.etoshY_deg;
    etoshZ_deg = myRot.etoshZ_deg;
    etoshX_rad = myRot.etoshX_rad;
    etoshY_rad = myRot.etoshY_rad;
    etoshZ_rad = myRot.etoshZ_rad;
    elemWidth_nd = (1.0) / nOfSection;
    elemWidth_m = elemWidth_nd * lenBlade;

    GlobalVars& globs = GlobalVars::getInstance();
    muVec = globs.getMuVec(vTip_ms);
    muVecSh = muVec.RotAx_X(etoshX_rad).RotAx_Y(etoshY_rad).RotAx_Z(etoshZ_rad); // Vector in SHAFT AXIS
    freeAirVecSh = muVecSh * (vTip_ms); // Vector in SHAFT AXIS
    SpeedOfSound_ms = globs.getSpeedOfSound_ms();
    density_kgm3 = globs.getDensity_kgm3();


    
    double closestLeft = 1.0;
    double closestRight = 0.0;
    double elemMidRad = 0.0;
    int i = 0;
    int j = 0;
    
    if (nOfSection > MaxSectionNumber) {
        std::cout << "NUMBER OF SECTION SIZE CAN NOT BE LARGER THAN " << MaxSectionNumber << " !!!!!!" << std::endl;
    }

    for (auto& ss : myRot.sections) {
        if (ss.RSt > ss.RFn || ss.RSt == ss.RFn) {
            std::cout << ss.Label << " section length is wrong !" << std::endl;
            exit(EXIT_FAILURE);
        }
        else {
            if (ss.RSt < closestLeft) {
                closestLeft = ss.RSt;
            }
            if (ss.RFn > closestRight) {
                closestRight = ss.RFn;
            }
        }
    }
    
    i = 0;
    j = 0;
    for (i = 0; i < myRot.nOfSections; i++) {
        for (int j = i + 1; j < myRot.nOfSections; j++) {
            if (myRot.sections[i].RFn > myRot.sections[j].RSt) {
                std::cout << "For the rotor " << myRot.Label << " 2 sections are intersecting." << std::endl;
                std::cout << "These are: " << myRot.sections[i].Label << " and " << myRot.sections[j].Label << std::endl;
                exit(EXIT_FAILURE);
            }
        }
    }
    j = 0;
    for (j = 0; j < nOfAzimuth; j++) {
        azimuth_rad[j] = (2 * M_PI / nOfAzimuth) * j;
    }
    
    i = 0; j = 0;

    for (i = 0; i < nOfSection; i++) {
        int status = 0;
        elemMidRad = (i + 0.5) * ((1.0) / nOfSection);
        radius_m[i] = elemMidRad * myRot.LenBlade;
        elemRotSpeed_ms[i] = rotSpeedRps * elemMidRad * myRot.LenBlade;
        for (auto& ss : myRot.sections) {
            
            if (elemMidRad <= closestLeft) {
                elemPolarsCl[i].makeItZero();
                elemPolarsCd[i].makeItZero();
                elemPolarsCm[i].makeItZero();
                status = 1;
            }
            else if (elemMidRad > closestRight) {
                elemPolarsCl[i].makeItZero();
                elemPolarsCd[i].makeItZero();
                elemPolarsCm[i].makeItZero();
                status = 1;
            }

            else if (elemMidRad > ss.RSt && elemMidRad <= ss.RFn) {
                
                elemPolarsCl[i] = (ss.clp);
                elemPolarsCd[i] = (ss.cdp);
                elemPolarsCm[i] = (ss.cmp);
                elemTwist_deg[i] = ((ss.TwFn - ss.TwSt) / (ss.RFn - ss.RSt)) * (elemMidRad - ss.RSt) + ss.TwSt;
                elemTwist_rad[i] = (elemTwist_deg[i] / 180.0) * M_PI;
                elemChord_m[i] = ((ss.ChordFn_m - ss.ChordSt_m) / (ss.RFn - ss.RSt)) * (elemMidRad - ss.RSt) + ss.ChordSt_m;
                status = 1;
            }
        }
        if (status == 0) {
            for (int k = 0; k < myRot.nOfSections - 1; k++) {
                int kPlus = k + 1;
                if (elemMidRad > myRot.sections[k].RFn && elemMidRad < myRot.sections[kPlus].RSt) {
                    double polarLeftWeight = (elemMidRad - myRot.sections[k].RFn) / (myRot.sections[kPlus].RSt - myRot.sections[k].RFn);
                    double polarRightWeight = 1.0 - polarLeftWeight;
                    AeroPolar::InterpolatePolar(myRot.sections[k].clp, polarLeftWeight, 
                        myRot.sections[kPlus].clp,polarRightWeight,elemPolarsCl[i]);
                    AeroPolar::InterpolatePolar(myRot.sections[k].cdp, polarLeftWeight,
                        myRot.sections[kPlus].cdp, polarRightWeight, elemPolarsCd[i]);
                    AeroPolar::InterpolatePolar(myRot.sections[k].cmp, polarLeftWeight,
                        myRot.sections[kPlus].cmp, polarRightWeight, elemPolarsCm[i]);
                    std::cout << "Polar interpolation is performed... !!" << std::endl;
                    elemTwist_deg[i] = myRot.sections[k].TwFn * polarLeftWeight + myRot.sections[kPlus].TwSt * polarRightWeight;
                    elemTwist_rad[i] = (elemTwist_deg[i] / 180.0) * M_PI;
                    elemChord_m[i] = myRot.sections[k].ChordFn_m * polarLeftWeight + myRot.sections[kPlus].ChordSt_m * polarRightWeight;

                }
            }
        }
    }
    
    bladeMassPerArea_kgm2 = myRot.bladeMassPerArea_kgm2;
    flapInertiaCalc(); // Calculating flap inertia from point mass, after everything is calculated;
    elemAreaCalc();
    rotorAreaCalc();

    return;
}


void SolRotor::flapInertiaCalc()
{
    int i = 0;
    double totalFlapInertia = 0.0;
    double dm = 0.0;
    for (i = 0; i < nOfSection; i++) {
        dm = elemWidth_m * elemChord_m[i] * bladeMassPerArea_kgm2;
        totalFlapInertia = totalFlapInertia + radius_m[i] * radius_m[i] * (dm);
    }
    bladeFlapInertia_kgm2 = totalFlapInertia;
}



void SolRotor::uniformInducedFormula()
{
    double muSquareOnShaft = muVecSh.x * muVecSh.x + muVecSh.y * muVecSh.y;
    double infLambSquare = (indLambEnd_nd + muVecSh.z) * (indLambEnd_nd + muVecSh.z); // inflow into rotor
    indLambResidual = (totalForceOnShaft.z) - (2.0 * density_kgm3 * rotorArea_m2 *
        vTip_ms * vTip_ms * sqrt(muSquareOnShaft + infLambSquare)) * indLambEnd_nd;

}
void SolRotor::uniformInducedCalculator()
{
    double* vars_pointers[maxD]{ nullptr };
    double* target_pointers[maxD]{ nullptr };
    double targetVals[maxD]{ 0.0 };
    double vars_init[maxD]{ 0.0 };
    int dim = 1;
    vars_pointers[0] = &(this->indLambEnd_nd);
    target_pointers[0] = &(this->indLambResidual);
    targetVals[0] = 0.0;
    vars_init[0] = indLamb0_nd;

    newtonMain(&SolRotor::uniformInducedFormula, vars_init, vars_pointers, target_pointers, targetVals, dim);

}
double inducedFinderResYedek(double vInd, double thrust, double rho, double area, double speed) {
    // This function find the residual of induced velocity calculation. It should be used
    // with a root finding algorithm like Newton method.
    double result = 0;
    result = thrust / (2 * rho * area * sqrt(speed * speed + vInd * vInd)) - vInd;
    return result;
}



void SolRotor::rotateRotor() {
    flap_rad[0] = flap0_rad;
    flapRate_rads[0] = Vector3D(0.0, flapRate0_rads, 0.0);
    Vector3D indVel(0.0, 0.0, (-indLamb0_nd * vTip_ms)); // Vector in shaft axis

    double sliceLift = 0.0;
    double sliceDrag = 0.0;
    double slicePitchMoment = 0.0;
    double resVelSq = 0.0;

    for (int i = 0; i < nOfAzimuth; i++) {

        double totalHingeFlapMoment = 0;
        for (int j = 0; j < nOfSection; j++) {
            double sliceMass_kg = elemWidth_m * elemChord_m[j] * bladeMassPerArea_kgm2;
            Vector3D sliceWeightVectorShaft = Vector3D(0.0, 0.0, -1 * sliceMass_kg * 9.81)
                .RotAx_X(etoshX_rad).RotAx_Y(etoshY_rad).RotAx_Z(etoshZ_rad);

            double sectionAngRad = colAngRad + elemTwist_rad[j] + latAngRad * cos(azimuth_rad[i]) + lonAngRad * sin(azimuth_rad[i]);
            sectionTransformAngles[i][j] = { azimuth_rad[i], flap_rad[i], sectionAngRad };
            // I need to convert all air velocity vectors to section axis !!
            // Note: Some of the transformations should be reconsidered when hinge off set is included.

            //Induced and free air vel. calculated at section axis.
            Vector3D indVel_sec = indVel.RotAx_Z(azimuth_rad[i]).RotAx_Y(flap_rad[i]).RotAx_X(sectionAngRad);
            Vector3D freeAir_sec = freeAirVecSh.RotAx_Z(azimuth_rad[i]).RotAx_Y(flap_rad[i]).RotAx_X(sectionAngRad);
            //.

            // Shaft axisindeki bir + x e bakan bir bladein verilen flapla geleceði konum hesaplanýyor.
            // Daha sonra bu konum shaft etrafýnda azimAng kadar döndürülüyor.
            // Son çýkan deðerler shaft axisindeki radius deðerleri.
            // Burada genede bir kontrol etmek gerekebilir #$#$#$#$#$#$#$#$#$#$#
            Vector3D radius_inter(radius_m[j], 0.0, 0.0); // Radius vector in shaft axis
            Vector3D hingeLoc_inter = Vector3D(hingeOffSet * lenBlade, 0.0, 0.0);
            Vector3D radiusFromHinge_inter = (radius_inter - hingeLoc_inter).RotVec_Y(flap_rad[i]);
            Vector3D radiusFromShaft_inter = hingeLoc_inter + radiusFromHinge_inter;
            Vector3D radiusFromShaft = radiusFromShaft_inter.RotVec_Z(azimuth_rad[i]);
            Vector3D hingeLoc = hingeLoc_inter.RotVec_Z(azimuth_rad[i]);
            Vector3D radiusFromHinge = radiusFromShaft - hingeLoc;
            //.

            //Calculation of rotation speed on shaft axis, then convert it to section axis
            Vector3D rotationSpeed = shaftAngularSpeed_rads ^ radiusFromShaft;
            Vector3D rotationSpeed_sec = rotationSpeed.RotAx_Z(azimuth_rad[i]).RotAx_Y(flap_rad[i]).RotAx_X(sectionAngRad);
            //.

            // Flap Rate vector is rotated on azimuth, then speed is calculated on shaft axis.
            // Then velocity on shaft axis is calculated on section axis
            // Flap rate vectörünü ilk for loopun sonundada döndürebilirim aslýnda. Ama þuan burada döndürüyorum.
            Vector3D flapRate_sh = ((flapRate_rads[i].RotVec_Z(azimuth_rad[i])) ^ radiusFromHinge); // Shaft axis flap hýzý
            Vector3D flapRate_sec = flapRate_sh.RotAx_Z(azimuth_rad[i]).RotAx_Y(flap_rad[i]).RotAx_X(sectionAngRad);
            //.

            // Burda air ve blade için sec ax vel. topluyorum.
            // Bu toplamlarý hepsi sec.ax olmalý.
            Vector3D totalAirSpeed_sec = indVel_sec + freeAir_sec;
            Vector3D totalBladeSpeed_sec = rotationSpeed_sec + flapRate_sec;
            Vector3D totalSpeed_sec = totalAirSpeed_sec - totalBladeSpeed_sec;
            // Burda mach hesaplanýrken direk r deðerini almak yanlýþ
            //.

            elemAoa_rad[i][j] = atan2(totalSpeed_sec.z, -totalSpeed_sec.y); // y tersine döndü çünkü airfoil axisinde y yönü ters.
            elemMach_nd[i][j] = sqrt((totalSpeed_sec.z * totalSpeed_sec.z) + (totalSpeed_sec.y * totalSpeed_sec.y)) / (SpeedOfSound_ms);
            if (std::isnan(elemAoa_rad[i][j]) || std::isnan(elemMach_nd[i][j])) {
                std::cout << "smthing NaN" << std::endl;
            }

            elemCl[i][j] = elemPolarsCl[j].polFinder(elemMach_nd[i][j], elemAoa_rad[i][j]);
            elemCd[i][j] = elemPolarsCd[j].polFinder(elemMach_nd[i][j], elemAoa_rad[i][j]);
            elemCm[i][j] = elemPolarsCm[j].polFinder(elemMach_nd[i][j], elemAoa_rad[i][j]);


            resVelSq = (totalSpeed_sec.y * totalSpeed_sec.y) + (totalSpeed_sec.z * totalSpeed_sec.z);
            sliceLift = 0.5 * density_kgm3 * resVelSq * elemCl[i][j] * elemArea_m2[j]; // section area olsa daha iyi
            sliceDrag = 0.5 * density_kgm3 * resVelSq * elemCd[i][j] * elemArea_m2[j]; // section area olsa daha iyi
            slicePitchMoment = 0.5 * density_kgm3 * (resVelSq)*elemCm[i][j] * elemChord_m[j] * elemArea_m2[j]; // section area olsa daha iyi


            elemLift_N_sec[i][j] = Vector3D(0.0, sliceLift * sin(elemAoa_rad[i][j]), sliceLift * cos(elemAoa_rad[i][j])); // Vector in slice axis
            elemDrag_N_sec[i][j] = Vector3D(0.0, -1 * sliceDrag * cos(elemAoa_rad[i][j]), sliceDrag * sin(elemAoa_rad[i][j]));
            elemPitch_Nm_sec[i][j] = Vector3D(-1 * slicePitchMoment, 0.0, 0.0);


            elemLift_N_sha[i][j] = elemLift_N_sec[i][j].RotAx_X(-sectionAngRad).RotAx_Y(-flap_rad[i]).RotAx_Z(-azimuth_rad[i]);
            elemDrag_N_sha[i][j] = elemDrag_N_sec[i][j].RotAx_X(-sectionAngRad).RotAx_Y(-flap_rad[i]).RotAx_Z(-azimuth_rad[i]);
            elemPitch_Nm_sha[i][j] = elemPitch_Nm_sec[i][j].RotAx_X(-sectionAngRad).RotAx_Y(-flap_rad[i]).RotAx_Z(-azimuth_rad[i]);
            elemMoment_Nm_sha[i][j] = radiusFromShaft ^ (elemLift_N_sha[i][j] + elemDrag_N_sha[i][j] + sliceWeightVectorShaft)
                + elemPitch_Nm_sha[i][j];


            Vector3D sliceWeightVec = sliceWeightVectorShaft.RotAx_Z(azimuth_rad[i]).RotAx_Y(flap_rad[i]).RotAx_X(sectionAngRad);

            Vector3D flapDueToLift = radiusFromHinge ^ elemLift_N_sec[i][j];
            Vector3D flapDueToDrag = radiusFromHinge ^ elemDrag_N_sec[i][j];
            Vector3D flapDueToWeight = radiusFromHinge ^ sliceWeightVec;
            Vector3D flapDueToCentrifugal = radiusFromHinge ^ ((shaftAngularSpeed_rads ^ (rotationSpeed)) * (-1 * sliceMass_kg));
            // Centrifugal olaný eklemiyorum çünkü betadoubledot hesabýnda zaten onun etkisi var.

            // Burda hingedeki momenti bulmak için bir sectionun üzerinde oluþan kuvvetleri önce shaft axisine çevirip,
            // sonrasýnda radius ile cross productýný alýyorum. Daha sonra bu kuvveti azimuth yönünde döndürerek 
            // y componentinin flapa denk gelmesini saðlýyorum.
            //Vector3D sliceMomentHinge = (radiusFromHinge ^ ((elemLift_N_sec[i][j] + elemDrag_N_sec[i][j] + sliceWeightVec)
            //    .RotAx_X(-sectionAngRad).RotAx_Y(-flap_rad[i]).RotAx_Z(-azimuth_rad[i])))
            //    .RotAx_Z(azimuth_rad[i]);
            Vector3D sliceMomentHinge = (radiusFromHinge ^ (elemLift_N_sha[i][j] + elemDrag_N_sha[i][j] + sliceWeightVectorShaft))
                .RotAx_Z(azimuth_rad[i]);
            //.

            totalHingeFlapMoment = totalHingeFlapMoment + sliceMomentHinge.y;


        }


        flapRateRate_rads2[i] = totalHingeFlapMoment / bladeFlapInertia_kgm2 - flap_rad[i] * rotSpeedRps * rotSpeedRps;
        flap_rad[i + 1] = flap_rad[i] + flapRate_rads[i].y * dt;
        flapRate_rads[i + 1] = Vector3D(0.0, flapRate_rads[i].y + flapRateRate_rads2[i] * dt, 0.0);


    }
    
    flapEnd_rad = flap_rad[nOfAzimuth];
    flapRateEnd_rads = flapRate_rads[nOfAzimuth].y;


    integrateForceMoment();
    uniformInducedCalculator(); // This will update the indLambEnd_nd
    
    flapCheck_rad = flapEnd_rad - flap0_rad;
    flapRateCheck_rads = flapRateEnd_rads - flapRate0_rads;
    indLambCheck_nd = indLamb0_nd - indLambEnd_nd;

}

void SolRotor::integrateForceMoment()
{
    totalLiftOnHinge = Vector3D(0.0, 0.0, 0.0);
    totalDragOnHinge = Vector3D(0.0, 0.0, 0.0);
    totalMomentOnShaft = Vector3D(0.0, 0.0, 0.0);
    PitchLinkMoment_Nm = { 0.0 };
    for (int i = 0; i < nOfAzimuth; i++) {
        for (int j = 0; j < nOfSection; j++) {
            
            totalLiftOnHinge = totalLiftOnHinge + elemLift_N_sha[i][j];
            totalDragOnHinge = totalDragOnHinge + elemDrag_N_sha[i][j];
            totalMomentOnShaft = totalMomentOnShaft + elemMoment_Nm_sha[i][j];

            PitchLinkMoment_Nm[i] = PitchLinkMoment_Nm[i] + elemPitch_Nm_sec[i][j].x;
        }
    }
    totalMomentOnShaft = (totalMomentOnShaft / nOfAzimuth) * nOfBlades;
    totalLiftOnHinge = (totalLiftOnHinge / nOfAzimuth) * nOfBlades;
    totalDragOnHinge = (totalDragOnHinge / nOfAzimuth) * nOfBlades;
    totalForceOnShaft = totalLiftOnHinge + totalDragOnHinge;
    PowerAppliedToShaft = totalMomentOnShaft * shaftAngularSpeed_rads;
}

void SolRotor::saveSummaryData()
{
    std::filesystem::create_directory(Label);
    std::string solSumFileName = Label + "/SolutionSummary.dat";
    std::ofstream outfile(solSumFileName);
    if (!outfile.is_open()) {
        std::cerr << "Failed to open file " << solSumFileName << " for writing" << std::endl;
        return;
    }
    outfile << "Total Lift Force in X Direction = " << totalLiftOnHinge.x << "\n";
    outfile << "Total Lift Force in Y Direction = " << totalLiftOnHinge.y << "\n";
    outfile << "Total Lift Force in Z Direction = " << totalLiftOnHinge.z << "\n";
    outfile << "Total Drag Force in X Direction = " << totalDragOnHinge.x << "\n";
    outfile << "Total Drag Force in Y Direction = " << totalDragOnHinge.y << "\n";
    outfile << "Total Drag Force in Z Direction = " << totalDragOnHinge.z << "\n";
    outfile << "Total Moment on Shaft in X Direction = " << totalMomentOnShaft.x << "\n";
    outfile << "Total Moment on Shaft in Y Direction = " << totalMomentOnShaft.y << "\n";
    outfile << "Total Moment on Shaft in Z Direction = " << totalMomentOnShaft.z << "\n";
    outfile << "Total Power Applied on Shaft [W] = " << PowerAppliedToShaft << "\n";
    outfile.close();

}

void SolRotor::saveLiftVecElem()
{
    std::filesystem::create_directory(Label);
    std::string solSumFileName = Label + "/SectionLiftVec_2D.dat";
    std::ofstream outfile(solSumFileName);
    if (!outfile.is_open()) {
        std::cerr << "Failed to open file " << solSumFileName << " for writing" << std::endl;
        return;
    }

    outfile << "Azimuth Angle" << "," << "Radius Location" << "," << "Lift_X," << "Lift_Y," << "Lift_Z,"
        << "Lift_Mag" << "\n";
    for (int i = 0; i < nOfAzimuth; i++) {
        for (int j = 0; j < nOfSection; j++) {
            outfile << azimuth_rad[i] << "," << radius_m[j] << "," << elemLift_N_sec[i][j].x << "," << elemLift_N_sec[i][j].y
                << "," << elemLift_N_sec[i][j].z << "," << elemLift_N_sec[i][j].r  << "\n";
        }
    }
    outfile.close();
}
void SolRotor::saveLiftVecShaft()
{
    std::filesystem::create_directory(Label);
    std::string solSumFileName = Label + "/ShaftLiftVec_2D.dat";
    std::ofstream outfile(solSumFileName);
    if (!outfile.is_open()) {
        std::cerr << "Failed to open file " << solSumFileName << " for writing" << std::endl;
        return;
    }

    outfile << "Azimuth Angle" << "," << "Radius Location" << "," << "Lift_X," << "Lift_Y," << "Lift_Z,"
        << "Lift_Mag" << "\n";
    for (int i = 0; i < nOfAzimuth; i++) {
        for (int j = 0; j < nOfSection; j++) {
            outfile << azimuth_rad[i] << "," << radius_m[j] << "," << elemLift_N_sha[i][j].x << "," 
                << elemLift_N_sha[i][j].y
                << "," << elemLift_N_sha[i][j].z << ","
                << elemLift_N_sha[i][j].r << "\n";
        }
    }
    outfile.close();
}
void SolRotor::saveDragVecElem()
{
    std::filesystem::create_directory(Label);
    std::string solSumFileName = Label + "/SectionDragVec_2D.dat";
    std::ofstream outfile(solSumFileName);
    if (!outfile.is_open()) {
        std::cerr << "Failed to open file " << solSumFileName << " for writing" << std::endl;
        return;
    }

    outfile << "Azimuth Angle" << "," << "Radius Location" << "," << "Drag_X," << "Drag_Y,"
        << "Drag_Z,"
        << "Drag_Mag" << "\n";
    for (int i = 0; i < nOfAzimuth; i++) {
        for (int j = 0; j < nOfSection; j++) {
            outfile << azimuth_rad[i] << "," << radius_m[j] << "," << elemDrag_N_sec[i][j].x 
                << "," << elemDrag_N_sec[i][j].y
                << "," << elemDrag_N_sec[i][j].z << "," << elemDrag_N_sec[i][j].r << "\n";
        }
    }
    outfile.close();
}
void SolRotor::saveDragVecShaft()
{
    std::filesystem::create_directory(Label);
    std::string solSumFileName = Label + "/ShaftDragVec_2D.dat";
    std::ofstream outfile(solSumFileName);
    if (!outfile.is_open()) {
        std::cerr << "Failed to open file " << solSumFileName << " for writing" << std::endl;
        return;
    }

    outfile << "Azimuth Angle" << "," << "Radius Location" << "," << "Drag_X," << "Drag_Y,"
        << "Drag_Z,"
        << "Drag_Mag" << "\n";
    for (int i = 0; i < nOfAzimuth; i++) {
        for (int j = 0; j < nOfSection; j++) {
            outfile << azimuth_rad[i] << "," << radius_m[j] << "," << elemDrag_N_sha[i][j].x
                << "," << elemDrag_N_sha[i][j].y
                << "," << elemDrag_N_sha[i][j].z << "," << elemDrag_N_sha[i][j].r << "\n";
        }
    }
    outfile.close();
}
void SolRotor::savePitchMomentVecElem()
{
    std::filesystem::create_directory(Label);
    std::string solSumFileName = Label + "/SectionPitchMomentVec_2D.dat";
    std::ofstream outfile(solSumFileName);
    if (!outfile.is_open()) {
        std::cerr << "Failed to open file " << solSumFileName << " for writing" << std::endl;
        return;
    }

    outfile << "Azimuth Angle" << "," << "Radius Location" << "," << "Pitch_X," << "Pitch_Y,"
        << "Pitch_Z,"
        << "Pitch_Mag" << "\n";
    for (int i = 0; i < nOfAzimuth; i++) {
        for (int j = 0; j < nOfSection; j++) {
            outfile << azimuth_rad[i] << "," << radius_m[j] << "," << elemPitch_Nm_sec[i][j].x
                << "," << elemPitch_Nm_sec[i][j].y
                << "," << elemPitch_Nm_sec[i][j].z << "," << elemPitch_Nm_sec[i][j].r << "\n";
        }
    }
    outfile.close();
}
void SolRotor::savePitchMomentVecShaft()
{
    std::filesystem::create_directory(Label);
    std::string solSumFileName = Label + "/ShaftPitchMomentVec_2D.dat";
    std::ofstream outfile(solSumFileName);
    if (!outfile.is_open()) {
        std::cerr << "Failed to open file " << solSumFileName << " for writing" << std::endl;
        return;
    }

    outfile << "Azimuth Angle" << "," << "Radius Location" << "," << "Pitch_X," << "Pitch_Y,"
        << "Pitch_Z,"
        << "Pitch_Mag" << "\n";
    for (int i = 0; i < nOfAzimuth; i++) {
        for (int j = 0; j < nOfSection; j++) {
            outfile << azimuth_rad[i] << "," << radius_m[j] << "," << elemPitch_Nm_sha[i][j].x
                << "," << elemPitch_Nm_sha[i][j].y
                << "," << elemPitch_Nm_sha[i][j].z << "," << elemPitch_Nm_sha[i][j].r << "\n";
        }
    }
    outfile.close();
}
void SolRotor::saveMomentVecShaft()
{
    std::filesystem::create_directory(Label);
    std::string solSumFileName = Label + "/ShaftMomentVec_2D.dat";
    std::ofstream outfile(solSumFileName);
    if (!outfile.is_open()) {
        std::cerr << "Failed to open file " << solSumFileName << " for writing" << std::endl;
        return;
    }

    outfile << "Azimuth Angle" << "," << "Radius Location" << "," << "Moment_X," << "Moment_Y,"
        << "Moment_Z,"
        << "Moment_Mag" << "\n";
    for (int i = 0; i < nOfAzimuth; i++) {
        for (int j = 0; j < nOfSection; j++) {
            outfile << azimuth_rad[i] << "," << radius_m[j] << "," << elemMoment_Nm_sha[i][j].x
                << "," << elemMoment_Nm_sha[i][j].y
                << "," << elemMoment_Nm_sha[i][j].z << "," << elemMoment_Nm_sha[i][j].r << "\n";
        }
    }
    outfile.close();
}

void SolRotor::saveElem2DData()
{

    std::filesystem::create_directory(Label);
    std::string solSumFileName = Label + "/Data_2D.dat";
    std::ofstream outfile(solSumFileName);
    if (!outfile.is_open()) {
        std::cerr << "Failed to open file " << solSumFileName << " for writing" << std::endl;
        return;
    }

    outfile << "Azimuth Angle" << "," << "Radius Location" << "," << "Cl," << "Cd," << "Cm,"
        << "AoA," << "Mach" << "\n";
    for (int i = 0; i < nOfAzimuth; i++) {
        for (int j = 0; j < nOfSection; j++) {
            outfile << azimuth_rad[i] << "," << radius_m[j] << "," << elemCl[i][j] << "," << elemCd[i][j]
                << "," << elemCm[i][j] << "," << elemAoa_rad[i][j] << "," << elemMach_nd[i][j] << "\n";
        }
    }
    outfile.close();
}

void SolRotor::saveElem1DData()
{

    std::filesystem::create_directory(Label);
    std::string solSumFileName = Label + "/Data_1D.dat";
    std::ofstream outfile(solSumFileName);
    if (!outfile.is_open()) {
        std::cerr << "Failed to open file " << solSumFileName << " for writing" << std::endl;
        return;
    }

    outfile << "Azimuth Angle," << "Flap," << "FlapRate," << "FlapAcceleration," << "PitchLinkMoment" << '\n';
    for (int i = 0; i < nOfAzimuth; i++) {        
            outfile << azimuth_rad[i] << "," << flap_rad[i] << "," << flapRate_rads[i].y
                << "," << flapRateRate_rads2[i] << "," << PitchLinkMoment_Nm[i] << "\n";
    }
    outfile.close();
}

void SolRotor::find_jacobiansND(void (SolRotor::* method)(), double* vars_pointers[maxD],
    double* target_pointers[maxD], double targetVals[maxD],int dim,
    double fn_jac[maxD][maxD], double fn[maxD], double jacMat[maxD][maxD])
{
    for (int i = 0; i < dim; i++) {
        *(vars_pointers[i]) = *(vars_pointers[i]) + epsilon;
        (this->*method)();
        for (int j = 0; j < dim; j++)
            fn_jac[i][j] = *(target_pointers[j]) - targetVals[j];
        *(vars_pointers[i]) = *(vars_pointers[i]) - epsilon;
    }

    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            jacMat[i][j] = (fn_jac[j][i] - fn[i]) / epsilon;
        }
    }
}

double SolRotor::determinantOfMatrixND(double matrix[maxD][maxD], int n)
{
    double det = 0;
    double submatrix[maxD][maxD]{ 0.0 };
    if (n == 2)
        return ((matrix[0][0] * matrix[1][1]) - (matrix[1][0] * matrix[0][1]));
    else if (n == 1) {
        return matrix[0][0];
    }
    else {
        for (int x = 0; x < n; x++) {
            int subi = 0;
            for (int i = 1; i < n; i++) {
                int subj = 0;
                for (int j = 0; j < n; j++) {
                    if (j == x)
                        continue;
                    submatrix[subi][subj] = matrix[i][j];
                    subj++;
                }
                subi++;
            }
            det = det + ((double)pow(-1, x) * matrix[0][x] * determinantOfMatrixND(submatrix, n - 1));
        }
    }
    return det;
}

void SolRotor::cramerRuleND(double cramMat[maxD][maxD],double jacMat[maxD][maxD], double yMat[maxD],
    double& determinantCramMat, double& determinantAMat,
    double cram_res[maxD], double x0_diff[maxD], int dim)
{
    for (int i = 0; i < dim; i++) {
        for (int k = 0; k < dim; k++) {
            for (int l = 0; l < dim; l++) {
                cramMat[k][l] = jacMat[k][l];
            }
        }
        for (int j = 0; j < dim; j++) {
            cramMat[j][i] = yMat[j];
        }
        determinantCramMat = determinantOfMatrixND(cramMat, dim);
        determinantAMat = determinantOfMatrixND(jacMat, dim);
        cram_res[i] = determinantCramMat / determinantAMat;
        x0_diff[i] = cram_res[i];
    }
}

void SolRotor::newtonMain(void (SolRotor::* method)(), double vars_init[maxD],
    double* vars_pointers[maxD], double* target_pointers[maxD], double targetVals[maxD],
    int dim)
{
    double x0[maxD]{ 0.0 };
    double fn[maxD]{ 0.0 };
    double jacMat[maxD][maxD]{ 0.0 };
    double yMat[maxD]{ 0.0 };
    double fn_jac[maxD][maxD]{ 0.0 };
    double x0_eps[maxD][maxD]{ 0.0 };
    double cramMat[maxD][maxD]{ 0.0 };
    double cram_res[maxD]{ 0.0 };
    double x0_diff[maxD]{ 0.0 };
    
    double determinantCramMat = 0.0;
    double determinantAMat = 0.0;
    double xSol[maxD]{ 0.0 };


    for (int i = 0; i < dim; i++)
        *(vars_pointers[i]) = vars_init[i];

    for (int iter = 0; iter < max_iter; iter++) {
        // Call the function 
        (this->*method)();
        for (int i = 0; i < dim; i++)
            fn[i] = *(target_pointers[i]) - targetVals[i];
        
        if (find2Norm(fn, target_pointers, targetVals, dim) < tol) {
            //std::cout << "Norm2 is less than tolerance..." << std::endl;
            std::cout << "Solution is found after " << iter << " iterations\n";
            for (int m = 0; m < dim; m++) {
                xSol[m] = *(vars_pointers[m]);
            }
            numberOfIteration = iter;
            status = 1;
            return;
        }

        find_jacobiansND(method, vars_pointers, target_pointers, targetVals,dim, fn_jac, fn, jacMat);
        for (int j = 0; j < dim; j++) {
            yMat[j] = -fn[j];
        }
        if (is_zero(jacMat, dim)) {
            std::cout << "Zero Jacobian. No solution found!";
            for (int m = 0; m < dim; m++) {
                xSol[m] = 0.0;
            }
            numberOfIteration = iter;
            status = 0;
            return;
        }

        cramerRuleND(cramMat, jacMat, yMat, determinantCramMat, determinantAMat,
            cram_res, x0_diff, dim);
        for (int subi = 0; subi < dim; subi++) {
            *(vars_pointers[subi]) = *(vars_pointers[subi]) + x0_diff[subi];
        }
    }
    std::cout << "Exceeded maximum iterations. No solution found. !" << std::endl;

}

bool SolRotor::is_zero(double matrix[maxD][maxD],int dim)
{
    int i = 0; int j = 0;
    for (; i < dim; i++) {
        for (; j < dim; j++) {
            if (matrix[i][j] == 0) {
                return true;
            }
        }
    }
    return false;
}

double SolRotor::find2Norm(double fn[maxD], double* target_pointers[maxD],double targetVals[maxD], int dim)
{

    double total = 0.0;
    double residual = 0.0;
    for (int i = 0; i < dim; i++) {
        if (*(target_pointers[i]) == 0.0) {
            if (targetVals[i] == 0.0) {
                residual = 0.0;
            }
            else {
                residual= (targetVals[i] - *target_pointers[i]) / targetVals[i];
            }
        }
        else {
            if (targetVals[i] == 0.0) {
                residual = *target_pointers[i];
            }
            else {
                residual = (targetVals[i] - *target_pointers[i]) / targetVals[i];
            }
            
        }
        total = total + residual * residual;
        
    }
    double norm2 = sqrt(total);
    return norm2;
}




void SolRotor::rotateSteadyRotor() {
    double* vars_pointers[maxD]{nullptr};
    double* target_pointers[maxD]{ nullptr };
    double targetVals[maxD]{ 0.0 };
    double vars_init[maxD]{ 0.0 };
    int dim = 3;

    vars_pointers[0] = &(this->flap0_rad);
    vars_pointers[1] = &(this->flapRate0_rads);
    vars_pointers[2] = &(this->indLamb0_nd);
    target_pointers[0] = &(this->flapCheck_rad);
    target_pointers[1] = &(this->flapRateCheck_rads);
    target_pointers[2] = &(this->indLambCheck_nd);
    targetVals[0] = 0.0;
    targetVals[1] = 0.0;
    targetVals[2] = 0.0;
    vars_init[0] = -0.001;
    vars_init[1] = -0.001;
    vars_init[2] = 0.001;
    
    newtonMain(&SolRotor::rotateRotor, vars_init, vars_pointers, target_pointers, targetVals, dim);

}

void SolRotor::trimRotor() {



    double* vars_pointers[maxD]{ nullptr };
    double* target_pointers[maxD]{ nullptr };
    double targetVals[maxD]{ 0.0 };
    double vars_init[maxD]{ 0.0 };
    int dim = 3;
    vars_pointers[0] = &(this->colAngRad);
    vars_pointers[1] = &(this->lonAngRad);
    vars_pointers[2] = &(this->latAngRad);
    target_pointers[0] = &(this->totalForceOnShaft.z);
    target_pointers[1] = &(this->totalForceOnShaft.x);
    target_pointers[2] = &(this->totalForceOnShaft.y);
    targetVals[0] = 22000.0;
    targetVals[1] = 5000.0;
    targetVals[2] = 2500.0;
    vars_init[0] = 0.1;
    vars_init[1] = 0.1;
    vars_init[2] = 0.1;

    newtonMain(&SolRotor::rotateSteadyRotor, vars_init, vars_pointers, target_pointers, targetVals, dim);

    std::cout << "It is really messed up !!" << std::endl;
}


