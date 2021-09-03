/** @file */
#include "rov.Quaternions.h"
#include <cmath>
/**
 * https://openclassrooms.com/forum/sujet/code-d-entrainement-heritage-en-c-les-quaternions-68787
 */
///////////////////////////////////////////////////////////////////////
/// //------------------- Constructors -------------------
// Default to zero vector
quat_t::quat_t(){
	w = 1;
	i = 0;
	j = 0;
	k = 0;
}

// Individual components
quat_t::quat_t(float _w, float _r, float _s, float _t){
	w = _w;
	i = _r;
	j = _s;
	k = _t;
}
quat_t quat_t::Conj(){
	return quat_t(w, -i, -j, -k);
}
double quat_t::Norm() const{
	return sqrt(w * w + i * i + j * j + k * k);
}
double quat_t::Cosq(const quat_t Q) const{
	return (w * Q.w + i * Q.i + j * Q.j + k * Q.k) / Norm() / Q.Norm();
}
void quat_t::Normalize(){
	double n = Norm();
	w /= n;
	i /= n;
	j /= n;
	k /= n;
}
quat_t& quat_t::operator*=(quat_t const &h){
	quat_t d = *this;
	w = (d.w * h.w) - (d.i * h.i) - (d.j * h.j) - (d.k * h.k);
	i = (d.w * h.i) + (d.i * h.w) + (d.j * h.k) - (d.k * h.j);
	j = (d.w * h.j) + (d.j * h.w) + (d.k * h.i) - (d.i * h.k);
	k = (d.w * h.k) + (d.k * h.w) + (d.i * h.j) - (d.j * h.i);
	return *this;
}
quat_t operator*(quat_t const &h1, quat_t const &h2){
	quat_t d = h1;
	d *= h2;
	return d;
}

quat_t ToQuaternion(double yaw, double pitch, double roll) // yaw (Z), pitch (Y), roll (X)
	{
// Abbreviations for the various angular functions
	double cy = cos(yaw * 0.5);
	double sy = sin(yaw * 0.5);
	double cp = cos(pitch * 0.5);
	double sp = sin(pitch * 0.5);
	double cr = cos(roll * 0.5);
	double sr = sin(roll * 0.5);

	quat_t q;
	q.w = cr * cp * cy + sr * sp * sy;
	q.i = sr * cp * cy - cr * sp * sy;
	q.j = cr * sp * cy + sr * cp * sy;
	q.k = cr * cp * sy - sr * sp * cy;

	return q;
}
//---------------------------------------------------------------------
/**
 * https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles#Euler_angles_to_quaternion_conversion
 * @param q
 * @return
 */
EulerAngles ToEulerAngles(quat_t q){
	EulerAngles angles;

// roll (x-axis rotation)
	double sinr_cosp = 2 * (q.w * q.i + q.j * q.k);
	double cosr_cosp = 1 - 2 * (q.i * q.i + q.j * q.j);
	angles.roll = std::atan2(sinr_cosp, cosr_cosp);

// pitch (y-axis rotation)
	double sinp = 2 * (q.w * q.j - q.k * q.i);
	if (std::abs(sinp) >= 1)
		angles.pitch = std::copysign(M_PI / 2, sinp); // use 90 degrees if out of range
	else
		angles.pitch = std::asin(sinp);

// yaw (z-axis rotation)
	double siny_cosp = 2 * (q.w * q.k + q.i * q.j);
	double cosy_cosp = 1 - 2 * (q.j * q.j + q.k * q.k);
	angles.yaw = std::atan2(siny_cosp, cosy_cosp);

	return angles;
}
Cardan fromQuat(const quat_t &Q){
	Cardan C = Cardan();
	C.r = atan2(2.0 * (Q.j * Q.k + Q.w * Q.i), Q.w * Q.w - Q.i * Q.i - Q.j * Q.j + Q.k * Q.k);
	C.p = asin(-2.0 * (Q.i * Q.k - Q.w * Q.j));
	C.y = atan2(2.0 * (Q.i * Q.j + Q.w * Q.k), Q.w * Q.w + Q.i * Q.i - Q.j * Q.j - Q.k * Q.k);
	return C;
}
quat_t fromCardan(const Cardan &C){
	quat_t q;
	/*
	q.w = cr * cp * cy + sr * sp * sy;
	q.i = sr * cp * cy - cr * sp * sy;
	q.j = cr * sp * cy + sr * cp * sy;
	q.k = cr * cp * sy - sr * sp * cy;
*/
	return q;
}
/*
Cardan fromQuat(const quat_t &Q){
double a12, a22, a31, a32, a33; // rotation matrix coefficients for Euler angles and gravity components
a12 = 2.0f * (q[1] * q[2] + q[0] * q[3]);
a22 = q[0] * q[0] + q[1] * q[1] - q[2] * q[2] - q[3] * q[3];
a31 = 2.0f * (q[0] * q[1] + q[2] * q[3]);
a32 = 2.0f * (q[1] * q[3] - q[0] * q[2]);
a33 = q[0] * q[0] - q[1] * q[1] - q[2] * q[2] + q[3] * q[3];
_SERIAL_7("\natt Aij=(%+10.3f,%+10.3f,%+10.3f,%+10.3f,%+10.3f)", a12, a22, a31, a32, a33);

Attitude.Grav_x = -a32;
Attitude.Grav_y = a31;
Attitude.Grav_z = a33;

Attitude.Nroll = -atan2f(a31, a33);
Attitude.Epitch = +asinf(a32);
Attitude.Dyaw = -atan2f(a12, a22) + (1.13333 * pi / 180); //LMD; // Local MAgnetic Declination at Danville, California is 13 degrees 48 minutes and 47 seconds on 2014-04-04
if (Attitude.Dyaw < 0)
	Attitude.Dyaw += 2 * pi; // Ensure yaw stays between 0 and 360
}
*/
///////////////////////////////////////////////////////////////////////
