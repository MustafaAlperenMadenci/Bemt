#include "../headers/BEMT.h"

Vector3D::Vector3D() {
    x = 0.0;
    y = 0.0;
    z = 0.0;
    r = 0.0;
}

Vector3D::Vector3D(double xInp, double yInp, double zInp)
{
    x = xInp;
    y = yInp;
    z = zInp;
    r = sqrt(xInp * xInp + yInp * yInp + zInp * zInp);
}

Vector3D Vector3D::Add(const Vector3D& other) const
{
    return Vector3D(x + other.x , y + other.y, z + other.z);
}

Vector3D Vector3D::operator+(const Vector3D& other) const
{
    return Add(other);
}

Vector3D Vector3D::Substract(const Vector3D& other) const
{
    return Vector3D(x - other.x, y - other.y , z - other.z);
}

Vector3D Vector3D::operator-(const Vector3D& other) const
{
    return Substract(other);
}

double Vector3D::Multiply(const Vector3D& other) const
{
    return (x * other.x + y * other.y + z * other.z);
}
double Vector3D::operator*(const Vector3D& other) const
{
    return Multiply(other);
}

Vector3D Vector3D::Multiply(const double& other) const
{
    return Vector3D(x * other , y * other , z * other);
}

Vector3D Vector3D::operator*(const double& other) const
{
    return Multiply(other);
}

Vector3D Vector3D::CrossProduct(const Vector3D& other) const
{
    double nextX = this->y * other.z - this->z * other.y;
    double nextY = this->z * other.x - this->x * other.z;
    double nextZ = this->x * other.y - this->y * other.x;
    return Vector3D(nextX, nextY, nextZ);
}


Vector3D Vector3D::operator^(const Vector3D& other) const
{
    return CrossProduct(other);
}

Vector3D Vector3D::Divide(const Vector3D& other) const
{
    return Vector3D(x / other.x , y / other.y , z / other.z);
}

Vector3D Vector3D::operator/(const Vector3D& other) const
{
    return Divide(other);
}

Vector3D Vector3D::Divide(const double& other) const
{
    return Vector3D(x / other, y / other, z / other);
}

Vector3D Vector3D::operator/(const double& other) const
{
    return Divide(other);
}


Vector3D Vector3D::RotAx_X(double xAngRad) {

    double s_xAng = sin(xAngRad);
    double c_xAng = cos(xAngRad);

    std::array<std::array<double, 3>, 3> matrix;
    matrix[0][0] = 1.0;
    matrix[0][1] = 0.0;
    matrix[0][2] = 0.0;
    matrix[1][0] = 0.0;
    matrix[1][1] = c_xAng;
    matrix[1][2] = s_xAng;
    matrix[2][0] = 0.0;
    matrix[2][1] = -s_xAng;
    matrix[2][2] = c_xAng;

    double newX = (this->x * matrix[0][0]) + (this->y * matrix[0][1]) + (this->z * matrix[0][2]);
    double newY = (this->x * matrix[1][0]) + (this->y * matrix[1][1]) + (this->z * matrix[1][2]);
    double newZ = (this->x * matrix[2][0]) + (this->y * matrix[2][1]) + (this->z * matrix[2][2]);

    Vector3D newVec(newX, newY, newZ);

    return newVec;
}

Vector3D Vector3D::RotAx_Y(double yAngRad) {

    double s_yAng = sin(yAngRad);
    double c_yAng = cos(yAngRad);

    std::array<std::array<double, 3>, 3> matrix;
    matrix[0][0] = c_yAng;
    matrix[0][1] = 0.0;
    matrix[0][2] = -s_yAng;
    matrix[1][0] = 0.0;
    matrix[1][1] = 1.0;
    matrix[1][2] = 0.0;
    matrix[2][0] = s_yAng;
    matrix[2][1] = 0.0;
    matrix[2][2] = c_yAng;

    double newX = (this->x * matrix[0][0]) + (this->y * matrix[0][1]) + (this->z * matrix[0][2]);
    double newY = (this->x * matrix[1][0]) + (this->y * matrix[1][1]) + (this->z * matrix[1][2]);
    double newZ = (this->x * matrix[2][0]) + (this->y * matrix[2][1]) + (this->z * matrix[2][2]);
    
    Vector3D newVec(newX, newY, newZ);

    return newVec;
}
Vector3D Vector3D::RotAx_Z(double zAngRad) {

    double s_zAng = sin(zAngRad);
    double c_zAng = cos(zAngRad);

    std::array<std::array<double, 3>, 3> matrix;
    matrix[0][0] = c_zAng;
    matrix[0][1] = s_zAng;
    matrix[0][2] = 0.0;
    matrix[1][0] = -s_zAng;
    matrix[1][1] = c_zAng;
    matrix[1][2] = 0.0;
    matrix[2][0] = 0.0;
    matrix[2][1] = 0.0;
    matrix[2][2] = 1.0;

    double newX = (this->x * matrix[0][0]) + (this->y * matrix[0][1]) + (this->z * matrix[0][2]);
    double newY = (this->x * matrix[1][0]) + (this->y * matrix[1][1]) + (this->z * matrix[1][2]);
    double newZ = (this->x * matrix[2][0]) + (this->y * matrix[2][1]) + (this->z * matrix[2][2]);

    Vector3D newVec(newX, newY, newZ);

    return newVec;
}




Vector3D Vector3D::RotVec_X(double xAngRad) {

    double s_xAng = sin(xAngRad);
    double c_xAng = cos(xAngRad);

    std::array<std::array<double, 3>, 3> matrix;
    matrix[0][0] = 1.0;
    matrix[0][1] = 0.0;
    matrix[0][2] = 0.0;
    matrix[1][0] = 0.0;
    matrix[1][1] = c_xAng;
    matrix[1][2] = -s_xAng;
    matrix[2][0] = 0.0;
    matrix[2][1] = s_xAng;
    matrix[2][2] = c_xAng;

    double newX = (this->x * matrix[0][0]) + (this->y * matrix[0][1]) + (this->z * matrix[0][2]);
    double newY = (this->x * matrix[1][0]) + (this->y * matrix[1][1]) + (this->z * matrix[1][2]);
    double newZ = (this->x * matrix[2][0]) + (this->y * matrix[2][1]) + (this->z * matrix[2][2]);

    Vector3D newVec(newX, newY, newZ);

    return newVec;
}

Vector3D Vector3D::RotVec_Y(double yAngRad) {

    double s_yAng = sin(yAngRad);
    double c_yAng = cos(yAngRad);

    std::array<std::array<double, 3>, 3> matrix;
    matrix[0][0] = c_yAng;
    matrix[0][1] = 0.0;
    matrix[0][2] = s_yAng;
    matrix[1][0] = 0.0;
    matrix[1][1] = 1.0;
    matrix[1][2] = 0.0;
    matrix[2][0] = -s_yAng;
    matrix[2][1] = 0.0;
    matrix[2][2] = c_yAng;

    double newX = (this->x * matrix[0][0]) + (this->y * matrix[0][1]) + (this->z * matrix[0][2]);
    double newY = (this->x * matrix[1][0]) + (this->y * matrix[1][1]) + (this->z * matrix[1][2]);
    double newZ = (this->x * matrix[2][0]) + (this->y * matrix[2][1]) + (this->z * matrix[2][2]);

    Vector3D newVec(newX, newY, newZ);

    return newVec;
}
Vector3D Vector3D::RotVec_Z(double zAngRad) {

    double s_zAng = sin(zAngRad);
    double c_zAng = cos(zAngRad);

    std::array<std::array<double, 3>, 3> matrix;
    matrix[0][0] = c_zAng;
    matrix[0][1] = -s_zAng;
    matrix[0][2] = 0.0;
    matrix[1][0] = s_zAng;
    matrix[1][1] = c_zAng;
    matrix[1][2] = 0.0;
    matrix[2][0] = 0.0;
    matrix[2][1] = 0.0;
    matrix[2][2] = 1.0;

    double newX = (this->x * matrix[0][0]) + (this->y * matrix[0][1]) + (this->z * matrix[0][2]);
    double newY = (this->x * matrix[1][0]) + (this->y * matrix[1][1]) + (this->z * matrix[1][2]);
    double newZ = (this->x * matrix[2][0]) + (this->y * matrix[2][1]) + (this->z * matrix[2][2]);

    Vector3D newVec(newX, newY, newZ);

    return newVec;
}