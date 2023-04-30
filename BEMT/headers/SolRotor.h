#pragma once
class SolRotor
{
private:
    // Newton Method Related Stuff
    static const int maxD = 10;
    int status = 0;
    int numberOfIteration = 0;
    int max_iter = 1000;
    double tol = 0.0001;
    double epsilon = 0.0001;
    void newtonMain(void (SolRotor::* method)(), double vars_init[maxD],
        double* vars_pointers[maxD], double* target_pointers[maxD], double targetVals[maxD],
        int dim, bool infoOut);
    void find_jacobiansND(void (SolRotor::* method)(), double* vars_pointers[maxD],
        double* target_pointers[maxD], double targetVals[maxD], int dim,
        double fn_jac[maxD][maxD], double fn[maxD], double jacMat[maxD][maxD]);
    double find2Norm(double fn[maxD],double* target_pointers[maxD], double targetVals[maxD],
        int dim);
    bool is_zero(double matrix[maxD][maxD], int dim);
    void cramerRuleND(double cramMat[maxD][maxD], double jacMat[maxD][maxD], double yMat[maxD],
        double& determinantCramMat, double& determinantAMat,
        double cram_res[maxD], double x0_diff[maxD], int dim);
    double determinantOfMatrixND(double matrix[maxD][maxD], int n);


public:
    std::string Label;
    std::string solverType;
    std::string inputFileName;
    
    std::string trimInputFileName;
    std::string trimVariablesFileName;
    std::string trimTargetsFileName;
    std::array<TrimVariable, 5> TrimVariablesArray;
    std::array<TrimTarget, 5> TrimTargetsArray;
    int TrimVariablesSize = 0;
    int TrimTargetsSize = 0;


    int nOfSection = 0;
    int nOfAzimuth = 0;
    double hingeOffSet = 0.0;



    static const int MaxSectionNumber = 50;
    static const int MaxAzimuthNumber = 500;

    Vector3D muVec;
    Vector3D muVecSh;
    Vector3D freeAirVecSh;
    double SpeedOfSound_ms = 0.0;
    double density_kgm3 = 0.0;
    std::array<AeroPolar, MaxSectionNumber> elemPolarsCl;
    std::array<AeroPolar, MaxSectionNumber> elemPolarsCd;
    std::array<AeroPolar, MaxSectionNumber> elemPolarsCm;

    std::array<double, MaxSectionNumber> radius_m;
    std::array<double, MaxSectionNumber> elemTwist_deg;
    std::array<double, MaxSectionNumber> elemTwist_rad;
    std::array<double, MaxSectionNumber> elemChord_m;
    std::array<double, MaxSectionNumber> elemArea_m2;
    std::array<double, MaxSectionNumber> elemRotSpeed_ms;
    
    std::array<double, MaxAzimuthNumber> azimuth_rad{ 0 };
    std::array<double, MaxAzimuthNumber + 1> flap_rad{ 0 };
    std::array<Vector3D, MaxAzimuthNumber + 1> flapRate_rads; // Bunlarda +1 var çünkü son turda 0 derecenin flap ve flapratei birdaha hesaplanýyor.
    std::array<double, MaxAzimuthNumber> flapRateRate_rads2{ 0 };
    std::array<double, MaxAzimuthNumber> PitchLinkMoment_Nm{ 0 };
    

    std::array<std::array<double, MaxSectionNumber>, MaxAzimuthNumber> elemCl{0.0};
    std::array<std::array<double, MaxSectionNumber>, MaxAzimuthNumber> elemCd{0.0};
    std::array<std::array<double, MaxSectionNumber>, MaxAzimuthNumber> elemCm{0.0};
    std::array<std::array<double, MaxSectionNumber>, MaxAzimuthNumber> elemAoa_rad{ 0.0 };
    std::array<std::array<double, MaxSectionNumber>, MaxAzimuthNumber> elemMach_nd{ 0.0 };
    
    std::array<std::array<std::array<double, 3>, MaxSectionNumber>, MaxAzimuthNumber> sectionTransformAngles; // order: z, y, x
    
    std::array<std::array<Vector3D, MaxSectionNumber>, MaxAzimuthNumber> elemLift_N_sec;
    std::array<std::array<Vector3D, MaxSectionNumber>, MaxAzimuthNumber> elemDrag_N_sec;
    std::array<std::array<Vector3D, MaxSectionNumber>, MaxAzimuthNumber> elemPitch_Nm_sec;

    std::array<std::array<Vector3D, MaxSectionNumber>, MaxAzimuthNumber> elemLift_N_sha;
    std::array<std::array<Vector3D, MaxSectionNumber>, MaxAzimuthNumber> elemDrag_N_sha;
    std::array<std::array<Vector3D, MaxSectionNumber>, MaxAzimuthNumber> elemPitch_Nm_sha;
    std::array<std::array<Vector3D, MaxSectionNumber>, MaxAzimuthNumber> elemMoment_Nm_sha; // Bunu nasýl alcam emin deðilm
    //std::vector<std::vector<Vector3D>> sectionLiftHinge;
    //std::vector<std::vector<Vector3D>> sectionDragHinge;
    //std::vector<std::vector<Vector3D>> sectionHingeFlapMoment;
    
    Vector3D totalLiftOnHinge = Vector3D(0.0, 0.0,0.0);
    Vector3D totalDragOnHinge = Vector3D(0.0, 0.0, 0.0);
    Vector3D totalForceOnShaft = Vector3D(0.0, 0.0, 0.0);
    Vector3D totalMomentOnShaft = Vector3D(0.0, 0.0, 0.0);
    double lenBlade = 0.0;
    double nOfBlades = 0.0;
    double rotSpeedRps = 0.0;
    double rotorArea_m2 = 0.0;
    Vector3D shaftAngularSpeed_rads = Vector3D(0.0,0.0,0.0);
    //double widthOfSection;
    double elemWidth_nd = 0.0;
    double elemWidth_m = 0.0;
    
    double bladeMassPerArea_kgm2 = 0.0; // bu bölü metrekare
    double bladeFlapInertia_kgm2 = 0.0; // bu çarpý metrekare

    double dt =  0.0;
    double vTip_ms = 0.0;
    double etoshX_deg = 0.0; // earth to shaft x rotation
    double etoshY_deg = 0.0; // earth to shaft y rotation // Ýleri eðilen bir main rotor için bu negatif olmalý
    double etoshZ_deg = 0.0; // earth to shaft z rotation // Sola eðilen bir tail rotor için bu pozitif olmalý
    double etoshX_rad = 0.0; // earth to shaft x rotation
    double etoshY_rad = 0.0; // earth to shaft y rotation // Ýleri eðilen bir main rotor için bu negatif olmalý
    double etoshZ_rad = 0.0; // earth to shaft z rotation // Sola eðilen bir tail rotor için bu pozitif olmalý
    
    double colAngDeg = 0.0;
    double colAngRad = 0.0;
    double latAngDeg = 0.0;
    double latAngRad = 0.0;
    double lonAngDeg = 0.0;
    double lonAngRad = 0.0;
    
    double flap0_rad = 0.0;
    double flapEnd_rad = 0.0;
    double flapCheck_rad = 100.0;
    double flapRate0_rads = 0.0;
    double flapRateEnd_rads = 0.0;
    double flapRateCheck_rads = 100.0;
    double indLamb0_nd = 0.0;
    double indLambEnd_nd = 0.0;
    double indLambCheck_nd = 100.0;
    double indLambResidual = 0.0;

    double PowerAppliedToShaft = 0.0;

    void flapInertiaCalc();
    void elemAreaCalc();
    void rotorAreaCalc();
    void readRotorInputs();
    void readTrimInputFile();
    void readTrimVariables();
    void readTrimTargets();
    void setVals(const Rotor& myRot);


    void rotateRotor();
    void rotateSteadyRotor();
    void uniformInducedFormula();
    void uniformInducedCalculator();
    void trimRotorNew();
    
    void integrateForceMoment();
    void saveSummaryData();

    void saveLiftVecElem();
    void saveLiftVecShaft();
    void saveDragVecElem();
    void saveDragVecShaft();
    void savePitchMomentVecElem();
    void savePitchMomentVecShaft();
    void saveMomentVecShaft();
    void saveElem2DData();
    void saveElem1DData();

    void equateUnits();



};

