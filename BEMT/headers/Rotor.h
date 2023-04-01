#pragma once
class Rotor
{
public:
    int nOfBlades = 0;
    int nOfSections = 0;
    int nOfSolutionElements = 0;
    int nOfSolutionAzimuths = 0;
    double LenBlade = 0.0; // blade Length
    double RotationSpeed = 0.0; //rpm
    double bladeMassPerArea_kgm2 = 0.0;
    double hingeOffSet_nd = 0.0;
    double etoshX_deg = 0.0; // earth to shaft x rotation
    double etoshY_deg = 0.0; // earth to shaft y rotation // Ýleri eðilen bir main rotor için bu negatif olmalý
    double etoshZ_deg = 0.0; // earth to shaft z rotation // Sola eðilen bir tail rotor için bu pozitif olmalý
    double etoshX_rad = 0.0; // earth to shaft x rotation
    double etoshY_rad = 0.0; // earth to shaft y rotation // Ýleri eðilen bir main rotor için bu negatif olmalý
    double etoshZ_rad = 0.0; // earth to shaft z rotation // Sola eðilen bir tail rotor için bu pozitif olmalý


    std::string Label; // Label Of the Rotor
    //std::string inputFileName;
    std::vector<BladeSection> sections;
};

