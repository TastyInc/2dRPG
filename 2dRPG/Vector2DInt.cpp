#include "Vector2DInt.hpp"

Vector2DInt::Vector2DInt() {
	x = 0.0f;
	y = 0.0f;
}


Vector2DInt::Vector2DInt(int x, int y) {
	this->x = x;
	this->y = y;
}

Vector2DInt& Vector2DInt::Add(const Vector2DInt& vec) {
	this->x += vec.x;
	this->y += vec.y;

	return *this;
}

Vector2DInt& Vector2DInt::Subtract(const Vector2DInt& vec) {
	this->x -= vec.x;
	this->y -= vec.y;

	return *this;
}

Vector2DInt& Vector2DInt::Multiply(const Vector2DInt& vec) {
	this->x *= vec.x;
	this->y *= vec.y;

	return *this;
}

Vector2DInt& Vector2DInt::Divide(const Vector2DInt& vec) {
	this->x /= vec.x;
	this->y /= vec.y;

	return *this;
}

Vector2DInt& operator+(Vector2DInt& v1, const Vector2DInt& v2) {

	return v1.Add(v2);
}

Vector2DInt& operator-(Vector2DInt& v1, const Vector2DInt& v2) {

	return v1.Subtract(v2);
}

Vector2DInt& operator*(Vector2DInt& v1, const Vector2DInt& v2) {

	return v1.Multiply(v2);
}

Vector2DInt& operator/(Vector2DInt& v1, const Vector2DInt& v2) {

	return v1.Divide(v2);
}

Vector2DInt& Vector2DInt::operator+=(const Vector2DInt& vec) {
	return this->Add(vec);
}

Vector2DInt& Vector2DInt::operator-=(const Vector2DInt& vec) {
	return this->Subtract(vec);
}

Vector2DInt& Vector2DInt::operator*=(const Vector2DInt& vec) {
	return this->Multiply(vec);
}

Vector2DInt& Vector2DInt::operator/=(const Vector2DInt& vec) {
	return this->Divide(vec);
}

Vector2DInt& Vector2DInt::operator*(const int& i) {
	this->x *= i;
	this->y *= i;

	return *this;
}

Vector2DInt& Vector2DInt::Zero() {
	this->x = 0;
	this->y = 0;

	return *this;
}

std::ostream& operator<<(std::ostream& stream, const Vector2DInt& vec) {
	stream << "(" << vec.x << "," << vec.y << ")";
	return stream;
}