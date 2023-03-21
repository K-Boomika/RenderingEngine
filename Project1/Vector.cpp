#include "Vector.h"
#include <vector>
#include <iostream>
#include <fstream>

Vector::Vector() {
	x = 0;
	y = 0;
	z = 0;
}

Vector::Vector(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

void Vector::unitVector() {
    double mag = 0;
    mag += pow(this->x, 2);
    mag += pow(this->y, 2);
    mag += pow(this->z, 2);
    mag = sqrt(mag);
	if (mag == 0) {
		this->x = 0;
		this->y = 0;
		this->z = 0;
		return;
	}
	x = x / mag;
	y = y / mag;
	z = z / mag;
}

Vector Vector::operator-(const Vector& v) {
	Vector sub;
	sub.x = this->x - v.x;
	sub.y = this->y - v.y;
	sub.z = this->z - v.z;
	return sub;
}

Vector Vector::operator+(const Vector& v) {
	Vector sub;
	sub.x = this->x + v.x;
	sub.y = this->y + v.y;
	sub.z = this->z + v.z;
	return sub;
}

Vector Vector::operator*(const Vector& v) {
	Vector mul;
	mul.x = this->y * v.z - this->z * v.y;
	mul.y = this->z * v.x - this->x * v.z;
	mul.z = this->x * v.y - this->y * v.x;
	return mul;
}

void Vector::print() {
	std::cout << x << " " << y << " " << z << std::endl;
}

double Vector::dot(Vector v) {
	return this->x * v.x + this->y * v.y + this->z * v.z;
}
