#pragma once
class BladeSection
{
public:
    double RSt = 0.0; // Section Start
    double RFn = 0.0; // Section End
    double TwSt = 0.0; // Start Angle Rad
    double TwFn = 0.0; // Finish Angle Rad Buraya degree olsa ve method ile rad yapsa daha iyi !
    double ChordSt_m = 0.0; 
    double ChordFn_m = 0.0;

    std::string ClSource; // Source location of Cl file
    std::string CdSource; // Source location of Cd file    
    std::string CmSource; // Source location of Cm file
    std::string Label; // Label of the section, maybe airfoil name
    AeroPolar clp; // polar of cl
    AeroPolar cdp; // polar of cd
    AeroPolar cmp; // polar of cm
};

