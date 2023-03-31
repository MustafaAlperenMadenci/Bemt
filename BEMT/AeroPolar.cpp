#include "BEMT.h"


void AeroPolar::InterpolatePolar(const AeroPolar& polar1, double polar1Weight,const AeroPolar& polar2, double polar2Weight, AeroPolar& targetPolar)
{
    if (polar1.aoa_len != polar2.aoa_len) {
        std::cout << "Polar interpolation FAILED, due to different AOA size..." << std::endl;
        exit(EXIT_FAILURE);
    }
    targetPolar.aoa_len = polar1.aoa_len;

    if (polar1.mach_len != polar2.mach_len) {
        std::cout << "Polar interpolation FAILED, due to different MACH size..." << std::endl;
        exit(EXIT_FAILURE);
    }
    targetPolar.mach_len = polar1.mach_len;
    for (int i = 0; i < polar1.aoa_len; i++) {
        if ((polar1.aoaDeg[i] != polar2.aoaDeg[i]) || (polar1.aoaRad[i] != polar2.aoaRad[i])) {
            std::cout << "Polar interpolation FAILED, due to different AOA values..." << std::endl;
            exit(EXIT_FAILURE);
        }
        else {
            targetPolar.aoaDeg[i] = polar1.aoaDeg[i];
            targetPolar.aoaRad[i] = polar1.aoaRad[i];
        }
    }

    for (int i = 0; i < polar1.mach_len; i++) {
        if ((polar1.mach[i] != polar2.mach[i])) {
            std::cout << "Polar interpolation FAILED, due to different MACH values..." << std::endl;
            exit(EXIT_FAILURE);
        }
        else {
            targetPolar.mach[i] = polar1.mach[i];
        }
    }
    for (int i = 0; i < polar1.aoa_len; i++) {
        for (int j = 0; j < polar1.mach_len; j++) {
            targetPolar.data[i][j] = polar1.data[i][j] * polar1Weight + polar2.data[i][j] * polar2Weight;
        }
    }
    return;
}


void AeroPolar::textToStruct(std::string filename) {
    char* endp;
    int i = 0;
    int j = 0;
    char line[255];
    FILE* file_pointer = fopen(filename.c_str(), "r");

    while (fgets(line, 255, file_pointer)) {
        j = 0;
        char* field = strtok(line, ",");
        while (field) {
            if (i == 0 && j == 0) {
                field = strtok(NULL, ",");
            }
            else if (i == 0 && j != 0) {
                mach[j - 1] = strtod(field, &endp);
                mach_len++;
                field = strtok(NULL, ",");
            }
            else if (i != 0 && j == 0) {
                aoaDeg[i - 1] = strtod(field, &endp);
                aoaRad[i - 1] = aoaDeg[i - 1] * (M_PI / 180.);
                aoa_len++;
                field = strtok(NULL, ",");
            }
            else {
                data[i - 1][j - 1] = strtod(field, &endp);
                field = strtok(NULL, ",");
            }
            j++;
        }
        i++;
    }
    fclose(file_pointer);
}

double AeroPolar::polFinder(double xmach, double xaoaRad) const
{
    int i = 1;
    int j = 1;
    double interp1Low = 0.0;
    double interp1High = 0.0;
    double interpFinal = 0.0;
    if (xaoaRad <= aoaRad[0]) {
        i = 1;
        printf("Angle of Attack Outside Defined Range--Warning!!\n");
    }
    else if (xaoaRad >= aoaRad[aoa_len - 1]) {
        i = aoa_len - 1;
        printf("Angle of Attack Outside Defined Range--Warning!!\n");
    }
    else {
        while (!(xaoaRad > aoaRad[i - 1] && xaoaRad <= aoaRad[i])) {
            i++;
        }
    }
    if (xmach <= mach[0]) {
        j = 1;
        printf("Mach Number Outside Defined Range--Warning!!\n");
    }
    else if (xmach >= mach[mach_len - 1]) {
        j = mach_len - 1;
        printf("Mach Number Outside Defined Range--Warning!!\n");
    }
    else {
        while (!(xmach > mach[j - 1] && xmach <= mach[j])) {
            j++;
        }
    }

    interp1Low = ((data[i - 1][j] - data[i - 1][j - 1]) / (mach[j] - mach[j - 1])) * (xmach - mach[j - 1]) + data[i - 1][j - 1];
    interp1High = ((data[i][j] - data[i][j - 1]) / (mach[j] - mach[j - 1])) * (xmach - mach[j - 1]) + data[i][j - 1];
    interpFinal = ((interp1High - interp1Low) / (aoaRad[i] - aoaRad[i - 1])) * (xaoaRad - aoaRad[i - 1]) + interp1Low;

    return interpFinal;
}

void AeroPolar::makeItZero()
{
    this->mach_len = 2;
    this->aoa_len = 2;
    this->aoaDeg[0] = -180.0;
    this->aoaDeg[1] = +180.0;
    this->aoaRad[0] = (-180.0 / 180.0) * M_PI;
    this->aoaRad[1] = (+180.0 / 180.0) * M_PI;
    this->mach[0] = 0.0;
    this->mach[1] = 2.0;
}
