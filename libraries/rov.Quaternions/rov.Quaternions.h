/** @file */
#pragma once
///////////////////////////////////////////////////////////////////////
class quat_t;
class Cardan {
public:
	double r, p, y;
	Cardan(){
		r = 0;
		p = 0;
		y = 0;
	}
	Cardan(float _r, float _p, float _y){
		r = _r;
		p = _p;
		y = _y;
	}
};
typedef struct {
	double roll, pitch, yaw;
} EulerAngles;
///////////////////////////////////////////////////////////////////////
//typedef struct {
class quat_t {
public:
	double w, i, j, k;
	// Constructors:
	quat_t();
	quat_t(float, float, float, float);
	quat_t& operator*=(quat_t const&);
	quat_t Conj();
	double Norm() const;
	double Cosq(const quat_t Q) const;
	void Normalize();
};
quat_t operator*(quat_t const&, quat_t const&);
//}quat_t;

///////////////////////////////////////////////////////////////////////
quat_t ToQuaternion(double yaw, double pitch, double roll); // yaw (Z), pitch (Y), roll (X)
EulerAngles ToEulerAngles(quat_t q);
Cardan fromQuat(const quat_t &Q);
quat_t fromCardan(const Cardan &C);
//Cardan fromQuat(const quat_t &Q);
#include <cmath>
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
