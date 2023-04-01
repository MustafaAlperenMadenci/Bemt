#pragma once
class AllRotors
{
public:
    int nOfRotors = 0;
    std::vector<Rotor> rotors;
    void textToStruct(std::string filename, AeroPolar* targetPolar, std::string rotorLabel, std::string sectionLabel);
    void readPolars();
    void readAllRotorsData(const std::string& fileName);
};

