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
    double etoshY_deg = 0.0; // earth to shaft y rotation // �leri e�ilen bir main rotor i�in bu negatif olmal�
    double etoshZ_deg = 0.0; // earth to shaft z rotation // Sola e�ilen bir tail rotor i�in bu pozitif olmal�
    double etoshX_rad = 0.0; // earth to shaft x rotation
    double etoshY_rad = 0.0; // earth to shaft y rotation // �leri e�ilen bir main rotor i�in bu negatif olmal�
    double etoshZ_rad = 0.0; // earth to shaft z rotation // Sola e�ilen bir tail rotor i�in bu pozitif olmal�


    std::string Label; // Label Of the Rotor
    //std::string inputFileName;
    std::vector<BladeSection> sections;
};

