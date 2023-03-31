#pragma once
class Vector3D
{
public:
	double x = 0.0;
	double y = 0.0;
	double z = 0.0;
	double r = 0.0;
	Vector3D();
	Vector3D(double xInp, double yInp, double zInp);

	Vector3D Add(const Vector3D& other) const;
	Vector3D operator+(const Vector3D& other) const;
	Vector3D Substract(const Vector3D& other) const;
	Vector3D operator-(const Vector3D& other) const;
	
	double Multiply(const Vector3D& other) const; // Multiply belki cross product olabilir
	double operator*(const Vector3D& other) const;
	Vector3D Multiply(const double& other) const;
	Vector3D operator*(const double& other) const;

	Vector3D Divide(const Vector3D& other) const;
	Vector3D operator/(const Vector3D& other) const;
	Vector3D Divide(const double& other) const;
	Vector3D operator/(const double& other) const;

	Vector3D CrossProduct(const Vector3D& other) const;
	Vector3D operator^(const Vector3D& other) const;

	Vector3D RotAx_X(double xAngRad);
	Vector3D RotAx_Y(double yAngRad);
	Vector3D RotAx_Z(double zAngRad);
	Vector3D RotVec_X(double xAngRad);
	Vector3D RotVec_Y(double yAngRad);
	Vector3D RotVec_Z(double zAngRad);
};

