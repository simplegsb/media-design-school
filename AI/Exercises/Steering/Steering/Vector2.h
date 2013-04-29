#ifndef VECTOR2_H_
#define VECTOR2_H_

struct Vector2
{
	float x;

	float y;

	Vector2();

	Vector2(float x, float y);

	float angleTo(const Vector2& other) const;

	float crossProduct(const Vector2& other) const;

	float dotProduct(const Vector2& other) const;

	float length() const;

	void normalise();

	Vector2 operator-(const Vector2& other) const;

	Vector2& operator-=(const Vector2& other);

	Vector2 operator+(const Vector2& other) const;

	Vector2& operator+=(const Vector2& other);

	Vector2 operator*(float scalar) const;

	Vector2& operator*=(float scalar);

	Vector2 operator*(const Vector2& other) const;

	Vector2& operator*=(const Vector2& other);

	void rotate(float angle);
};

#endif /* VECTOR2_H_ */
