/** @file E_rover-tst_quat.h */
#pragma once
#include "rov.Quaternions.h"
#include "Arduino.h"
String dump_C(const Cardan C, const char *TITLE = nullptr){
	char buffer[64];
	sprintf(buffer, "%s=[%9.3f°,%9.3f°,%9.3f°]",
		(TITLE) ? TITLE : "...", C.r * RAD_TO_DEG, C.p * RAD_TO_DEG, C.y * RAD_TO_DEG);
	return String(buffer);
}
String dump_Q(const quat_t Q, const char *TITLE = nullptr){
	Cardan C= fromQuat(Q);
	char buffer[128];
	//double n=Q.Norm();
	sprintf(buffer, "%s=[%10.4f,%10.4f,%10.4f,%10.4f] %11.4e %s",
		(TITLE) ? TITLE : "...", Q.w, Q.i, Q.j, Q.k, Q.Norm() - 1, dump_C(C, "C").c_str());
	return String(buffer);
}
String dump_E(const EulerAngles E, const char *TITLE = nullptr){
	char buffer[64];
	sprintf(buffer, "%s=[%9.3f°,%9.3f°,%9.3f°]",
		(TITLE) ? TITLE : "...", E.roll * RAD_TO_DEG, E.pitch * RAD_TO_DEG, E.yaw * RAD_TO_DEG);
	return String(buffer);
}
void test_Quat_errorDEG(double Ro, double Po, double Yo, double dR, double dP, double dY){
	Serial.printf("\n---- Mesure d'erreur ---------------------------------");
	Serial.printf("\n");
	EulerAngles EE = {
		.roll = dR * DEG_TO_RAD,
		.pitch = dP * DEG_TO_RAD,
		.yaw = dY * DEG_TO_RAD
	};
	quat_t QQ = ToQuaternion(EE.yaw, EE.pitch, EE.roll);
	Serial.printf("\n%s => %s",		dump_E(EE, "EE").c_str(), dump_Q(QQ, "QQ").c_str());

	EulerAngles Eo = {
		.roll = Ro * DEG_TO_RAD,
		.pitch = Po * DEG_TO_RAD,
		.yaw = Yo * DEG_TO_RAD
	};
	quat_t Qo = ToQuaternion(Eo.yaw, Eo.pitch, Eo.roll);
	Serial.printf("\n%s => %s", dump_E(Eo, "Eo").c_str(), dump_Q(Qo, "Qo").c_str());
	Serial.printf("\n");

	quat_t Q;
	Q = QQ * Qo;
	Q = Qo*QQ;
	EulerAngles E = ToEulerAngles(Q);
	Serial.printf("\n%s <= %s", dump_E(E, "E ").c_str(), dump_Q(Q, "Q ").c_str());
/*
	EulerAngles e1 = {
		.roll = (Ro + dR) * DEG_TO_RAD,
		.pitch = (Po + dP) * DEG_TO_RAD,
		.yaw = (Yo + dY) * DEG_TO_RAD
	};
	Serial.printf("\n%s => %s", dump_E(e1, "e1").c_str(), dump_Q(q1, "q1").c_str());
	quat_t q1 = ToQuaternion(e1.yaw, e1.pitch, e1.roll);
	Q = q1.Conj() * Qo;
	Q = Qo.Conj() * q1;
	Serial.printf(" QQ.Cosq(Q)=%10.4e (%10.5f°)", QQ.Cosq(Q), acos(QQ.Cosq(Q)) * RAD_TO_DEG);
	Q.Normalize();
	E = ToEulerAngles(Q);
	Serial.printf("\n%s <= %s", dump_E(E, "E ").c_str(), dump_Q(Q, "Q ").c_str());
	Serial.printf(" QQ.Cosq(Q)=%10.4e (%10.5f°)", QQ.Cosq(Q), acos(QQ.Cosq(Q)) * RAD_TO_DEG);
*/
	Serial.printf("\n");
}
void test_Quaternions(){
	Serial.printf("\n================== TEST QUATERNIONS ==================");
	Serial.printf("\n");
	quat_t Q = quat_t();
	Serial.printf("\n%s", dump_Q(Q, "Q").c_str());
	Serial.printf("\n");
	EulerAngles A = { .roll = 179 * DEG_TO_RAD, .pitch = 0, .yaw = 0 };
	quat_t R = ToQuaternion(A.yaw, A.pitch, A.roll);
	Serial.printf("\n%s => %s", dump_E(A, "A").c_str(), dump_Q(R, "R").c_str());

	EulerAngles B = { .roll = PI, .pitch = 0, .yaw = 0 };
	quat_t T = ToQuaternion(B.yaw, B.pitch, B.roll);
	Serial.printf("\n%s => %s", dump_E(B, "B").c_str(), dump_Q(T, "T").c_str());

	Serial.printf("\n");
	EulerAngles C = ToEulerAngles(T.Conj());
	Serial.printf("\n%s <= %s", dump_E(C, "C").c_str(), dump_Q(T.Conj(), "B̅B̄").c_str());
	Serial.printf("\n");
	quat_t E = R * T.Conj();
	EulerAngles D = ToEulerAngles(E);
	Serial.printf("\n%s <= %s", dump_E(D, "D").c_str(), dump_Q(E, "E").c_str());
	Serial.printf("\n");
	test_Quat_errorDEG(0, 0, 0, 1, 2, 3);
	test_Quat_errorDEG(10, -50, 60, 4, 0, 0);
	test_Quat_errorDEG(10, -50, 60, 0, 4, 0);
	test_Quat_errorDEG(10, -50, 60, 0, 0, 4);
	Serial.printf("\n======================================================");
}
///////////////////////////////////////////////////////////////////////

