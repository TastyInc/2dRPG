#pragma once
#include <iostream>

class Vector2DInt {
public:
	int x;
	int y;

	Vector2DInt();
	Vector2DInt(int x, int y);

	Vector2DInt& Add(const Vector2DInt& vec);
	Vector2DInt& Subtract(const Vector2DInt& vec);
	Vector2DInt& Multiply(const Vector2DInt& vec);
	Vector2DInt& Divide(const Vector2DInt& vec);

	friend Vector2DInt& operator+(Vector2DInt& v1, const Vector2DInt& v2);
	friend Vector2DInt& operator-(Vector2DInt& v1, const Vector2DInt& v2);
	friend Vector2DInt& operator*(Vector2DInt& v1, const Vector2DInt& v2);
	friend Vector2DInt& operator/(Vector2DInt& v1, const Vector2DInt& v2);

	Vector2DInt& operator+=(const Vector2DInt& vec);
	Vector2DInt& operator-=(const Vector2DInt& vec);
	Vector2DInt& operator*=(const Vector2DInt& vec);
	Vector2DInt& operator/=(const Vector2DInt& vec);

	Vector2DInt& operator*(const int& i);
	Vector2DInt& Zero();

	friend std::ostream& operator<<(std::ostream& stream, const Vector2DInt& vec);
};