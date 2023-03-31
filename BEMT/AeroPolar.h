#pragma once
class AeroPolar
{
public:
    static const int MaxMachNumber = 30;
    static const int MaxAoaNumber = 150;

    int mach_len = 0;
    int aoa_len = 0;

    double mach[MaxMachNumber]{ 0 };
    double aoaDeg[MaxAoaNumber]{ 0 };
    double aoaRad[MaxAoaNumber]{ 0 };
    double data[MaxAoaNumber][MaxMachNumber]{ 0 };

    static void InterpolatePolar(const AeroPolar& polar1, double polar1Weight,
        const AeroPolar& polar2, double polar2Weight, AeroPolar& targetPolar);
    void textToStruct(std::string filename);
    double polFinder(double xmach, double xaoaRad) const;
    void makeItZero();
};


