#ifndef __VECTOR3__
#define __VECTOR3__
#include <iostream>


class Vector2
{
public:
	float x,y;
	inline Vector2() {};
	inline Vector2(float _x, float _y):x(_x), y(_y){}
	inline Vector2(const Vector2& v): x(v.x), y(v.y){}
	inline ~Vector2() {};
	void zero();
	inline void set(float _x, float _y){x = _x; y = _y;}
	// Comparison Operators
	bool operator==(const Vector2& v) const;
	bool operator!=(const Vector2& v) const;
	bool operator> (const Vector2& v) const;
	bool operator>=(const Vector2& v) const;
	bool operator< (const Vector2& v) const;
	bool operator<=(const Vector2& v) const;

	// Algebraic Operations
	Vector2& operator=(const Vector2& v);
	Vector2 operator+(const Vector2& v) const;
	Vector2 operator-(const Vector2& v) const;
	Vector2 operator-() const;
	Vector2 operator*(float Scalar) const;
	Vector2 operator/(float Scalar) const;
	Vector2& operator+=(const Vector2& v);
	Vector2& operator-=(const Vector2& v);
	Vector2& operator*=(float Scalar);
	Vector2& operator/=(float Scalar);

	// Geometric Operations
	float Length() const;
	float SquareLength() const;
	float Dot(const Vector2& v) const;
	float Normalize();

	// Debug Operations
	friend std::ostream& operator<<(std::ostream& out, const Vector2& source);
	friend std::ostream& operator<<(const Vector2& source,std::ostream& out);


};


class Vector3
{
public:
	float x,y,z;
	Vector3();
	Vector3(float _x, float _y, float _z):x(_x), y(_y), z(_z){}
	Vector3(float val) :x(val), y(val), z(val) {};
	Vector3(const Vector3& v): x(v.x), y(v.y), z(v.z){}
	~Vector3();
	void zero();
	void set(float _x, float _y, float _z){x = _x; y = _y; z=_z;}
	// Comparison Operators
	bool operator==(const Vector3& v) const;
	bool operator!=(const Vector3& v) const;
	bool operator> (const Vector3& v) const;
	bool operator>=(const Vector3& v) const;
	bool operator< (const Vector3& v) const;
	bool operator<=(const Vector3& v) const;

	// Algebraic Operations
	Vector3& operator=(const Vector3& v);
	Vector3 operator+(const Vector3& v) const;
	Vector3 operator-(const Vector3& v) const;
	Vector3 operator-() const;
	Vector3 operator*(float Scalar) const;
	Vector3 operator/(float Scalar) const;
	Vector3& operator+=(const Vector3& v);
	Vector3& operator-=(const Vector3& v);
	Vector3& operator*=(float Scalar);
	Vector3& operator/=(float Scalar);

	// Geometric Operations
	float Length() const;
	float SquareLength() const;
	float Dot(const Vector3& v) const;
	float Normalize();
	Vector3 Cross(const Vector3& v) const;

	// Debug Operations
	friend std::ostream& operator<<(std::ostream& out, const Vector3& source);
	friend std::ostream& operator<<(const Vector3& source,std::ostream& out);


};


// This class finish but some operator doesn't really good. 
#pragma once
class Vector4
{
public:
	float x,y,z,w;
	Vector4(){};
	Vector4(float _x, float _y, float _z,float _w):x(_x), y(_y), z(_z),w(_w){}
	Vector4(const Vector4& v): x(v.x), y(v.y), z(v.z),w(v.w){}
	Vector4(const Vector3& v, float w): x(v.x), y(v.y), z(v.z),w(w){}
	~Vector4(){};
	void zero();
	void set(float _x, float _y, float _z,float _w){x = _x; y = _y; z=_z;w= _w;}
	// Comparison Operators
	bool operator==(const Vector4& v) const;
	bool operator!=(const Vector4& v) const;
	bool operator> (const Vector4& v) const;
	bool operator>=(const Vector4& v) const;
	bool operator< (const Vector4& v) const;
	bool operator<=(const Vector4& v) const;

	// Algebraic Operations
	Vector4& operator=(const Vector4& v);
	Vector4 operator+(const Vector4& v) const;
	Vector4 operator-(const Vector4& v) const;
	Vector4 operator-() const;
	Vector4 operator*(float Scalar) const;
	Vector4 operator/(float Scalar) const;
	Vector4& operator+=(const Vector4& v);
	Vector4& operator-=(const Vector4& v);
	Vector4& operator*=(float Scalar);
	Vector4& operator/=(float Scalar);

	// Geometric Operations
	float Length() const;
	float SquareLength() const;
	float Dot(const Vector4& v) const;
	float Normalize();

	// Debug Operations
	friend std::ostream& operator<<(std::ostream& out, const Vector4& source);
	friend std::ostream& operator<<(const Vector4& source,std::ostream& out);


};

typedef Vector3 Vec3;
typedef Vector2 Vec2;
typedef Vector4 Vec4;


#endif