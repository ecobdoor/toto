#include "rov-Kine_000_WS3.h"
//#include "rov-CfgH(000_WS3).h"
#include "esp32_LOGS.h"
#include "esp32_SPRINT.h"
#include <algorithm> // transform for_each
//#include <functional> // minus
///////////////////////////////////////////////////////////////////////
Kine_1::Kine_1(s_kineCTX *KINECTX, const String NAME, int8_t *DBGMAX)
:
	Core(NAME.c_str(), DBGMAX),
		_kineCTX(*KINECTX)
{
	init_statistics();
	curMicroTS = micro_TS();
}
//---------------------------------------------------------------------
void Kine_1::telemetry_KEYFRAME(const size_t SIZ, int16_t &CNT, char *BUF)const{
	const t_mat4F &matext = Keyframe.MATEXT;
	for (size_t row = 0; row < 4; row++)
		for (size_t col = 0; col < 4; col++)
			SPrintF(SIZ, CNT, BUF, "%.10f;", matext[row][col]);
	//buffer[cnt - 1] = '\0'; // suppress last separator
}
//---------------------------------------------------------------------
void Kine_1::Clear(){
	init_statistics();
	Attitude = { };
	Keyframe = { };
}
//---------------------------------------------------------------------
void Kine_1::calc_KEYFRAME(){
//	Keyframe.kfMicroTS = Fusensor.microTS;
//ASSERT(ATITUD.microTS == FUSENS.microTS); // has been init by calc_ATTITUDE
	/*
	 KF.DXYZ[0] = Dx;
	 KF.DXYZ[1] = Dy;
	 KF.DXYZ[2] = Dz;
	 KF.DXYZ[3] = 1.0;
	 */
	t_mat4F &matext = Keyframe.MATEXT;
	float Qw = Fusensor.q[0];
	float Qx = Fusensor.q[1];
	float Qy = Fusensor.q[2];
	float Qz = Fusensor.q[3];
	float _2w2 = 2 * Qw * Qw, _2wx = 2 * Qw * Qx, _2wy = 2 * Qw * Qy, _2wz = 2 * Qw * Qz;
	float _2x2 = 2 * Qx * Qx, _2xy = 2 * Qx * Qy, _2xz = 2 * Qx * Qz;
	float _2y2 = 2 * Qy * Qy, _2yz = 2 * Qy * Qz;
	float _2z2 = 2 * Qz * Qz;
	float quality = _2w2 + _2x2 + _2y2 + _2z2 - 2;
	if (quality < Keyframe.qualityMin)
		Keyframe.qualityMin = quality;
	if (Keyframe.qualityMax < quality)
		Keyframe.qualityMax = quality;
//ASSERT(abs(KF.quality) < 2e-15);
	matext[0][0] = 1 - _2y2 - _2z2;
	matext[0][1] = _2xy - _2wz;
	matext[0][2] = _2xy + _2wy;
	matext[0][3] = Attitude.Xcur;

	matext[1][0] = _2xy + _2wz;
	matext[1][1] = 1 - _2x2 - _2z2;
	matext[1][2] = _2yz - _2wx;
	matext[1][3] = Attitude.Ycur;

	matext[2][0] = _2xz - _2wy;
	matext[2][1] = _2yz + _2wx;
	matext[2][2] = 1 - _2x2 - _2y2;
	matext[2][3] = Attitude.Zcur;

	matext[3][0] = 0.0;
	matext[3][1] = 0.0;
	matext[3][2] = 0.0;
	matext[3][3] = 1.0;
	Keyframe.DXYZ[0] = Attitude.Dx;
	Keyframe.DXYZ[1] = Attitude.Dy;
	Keyframe.DXYZ[2] = Attitude.Dz;
	Keyframe.DXYZ[3] = 1.0;
}
//---------------------------------------------------------------------
/**
 *https://www.google.com/search?q=roll+pitch+yaw+north+east+down&safe=active&client=ubuntu&hs=a7R&channel=fs&sxsrf=ALeKk00ooFYaDR_aaHhPMLRPhzL8tCZfMw:1593158791426&tbm=isch&source=iu&ictx=1&fir=AwxFZY9kMLb4uM%252Cgjw8nLbl3yPWZM%252C_&vet=1&usg=AI4_-kS7s5xFbb7FR_eJ97UW5N2GsKxbTQ&sa=X&ved=2ahUKEwjcstX-gp_qAhUBuRoKHUcVACEQ9QEwAHoECAgQHA&biw=1286&bih=659#imgrc=AwxFZY9kMLb4uM
 */
void Kine_1::calc_ATTITUDE(const float LMD){

	double q[4] = {
		(double)Fusensor.q[0],
		(double)Fusensor.q[1],
		(double)Fusensor.q[2],
		(double)Fusensor.q[3]
	};

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
// https://github.com/kriswiner/MPU9250/issues/223
	Attitude.Ax = Fusensor.ax - a32;
	Attitude.Ay = Fusensor.ay - a31;
	Attitude.Az = Fusensor.az - a33;

	/*
	 Attitude.Ax = Fusensor.ax * (1 + a31);
	 Attitude.Ay = Fusensor.ay * (1 + a32);
	 Attitude.Az = Fusensor.az * (1 - a33);
	 */
	if (abs(Attitude.Ax) < _kineCTX.thr_linAcc)
		Attitude.Ax = 0;
	if (abs(Attitude.Ay) < _kineCTX.thr_linAcc)
		Attitude.Ay = 0;
	if (abs(Attitude.Az) < _kineCTX.thr_linAcc)
		Attitude.Az = 0;

	Attitude.imuTik = Fusensor.tik;
	Attitude.colDist = Fusensor.distUSD;

	double Dt = deltaMicroTS / 1000000.0; // µs to sec
	double Dt2 = Dt * Dt;
	Attitude.Dx = Attitude.Vx * Dt + 0.5 * Attitude.Ax * Dt2;
	Attitude.Dy = Attitude.Vy * Dt + 0.5 * Attitude.Ay * Dt2;
	Attitude.Dz = Attitude.Vz * Dt + 0.5 * Attitude.Az * Dt2;
	/* Soit en 1 seconde
	 Attitude.Dx = Attitude.Vx  + 0.5 * Attitude.Ax * Dt;
	 Attitude.Dy = Attitude.Vy  + 0.5 * Attitude.Ay * Dt;
	 Attitude.Dz = Attitude.Vz  + 0.5 * Attitude.Az * Dt;
	 */
	Attitude.Vx += Attitude.Ax * Dt; //10(m/s) + 1e-3(m/s²)* 1e-5(s) pb de décimales !!!
	Attitude.Vy += Attitude.Ay * Dt;
	Attitude.Vz += Attitude.Az * Dt;
	/*
	 DX = ATT.Vx * Dt + 0.25 * (oldAx + ATT.Ax) * Dt2;
	 DY = ATT.Vy * Dt + 0.25 * (oldAy + ATT.Ay) * Dt2;
	 DZ = ATT.Vz * Dt + 0.25 * (oldAz + ATT.Az) * Dt2;
	 */
	Attitude.Xcur += Attitude.Dx;
	Attitude.Ycur += Attitude.Dy;
	Attitude.Zcur += Attitude.Dz;
	/*
	 ATT.Vx += 0.5 * (oldAx + ATT.Ax) * Dt;
	 ATT.Vy += 0.5 * (oldAy + ATT.Ay) * Dt;
	 ATT.Vz += 0.5 * (oldAz + ATT.Az) * Dt;
	 */
//	Attitude.attMicroTS = Fusensor.microTS; // after Dxyz computation
}
//---------------------------------------------------------------------
void Kine_1::telemetry_FUSENSOR(const size_t SIZ, int16_t &CNT, char *BUF)const {
	SPrintF(SIZ, CNT, BUF, "%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f",
		Fusensor.ax, Fusensor.ay, Fusensor.az,
		Fusensor.gx, Fusensor.gy, Fusensor.gz,
		Fusensor.mx, Fusensor.my, Fusensor.mz,
		Fusensor.q[0], Fusensor.q[1], Fusensor.q[2], Fusensor.q[3]);
}
//---------------------------------------------------------------------
void Kine_1::telemetry_ATTITUDE(const size_t SIZ, int16_t &CNT, char *BUF)const {
	SPrintF(SIZ, CNT, BUF, "%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f",
		Attitude.Ax, Attitude.Ay, Attitude.Az,
		Attitude.Nroll, Attitude.Epitch, Attitude.Dyaw,
		Attitude.Grav_x, Attitude.Grav_y, Attitude.Grav_z,
		Fusensor.q[0], Fusensor.q[1], Fusensor.q[2], Fusensor.q[3]);
}
//---------------------------------------------------------------------
void Kine_1::updateRoverKine( const float LMD){
	deltaMicroTS = Fusensor.microTS-curMicroTS;
	curMicroTS = Fusensor.microTS;
	calc_ATTITUDE(LMD);
	calc_KEYFRAME();
}
//---------------------------------------------------------------------
/**
 * @fn void H_AccGyrMag::dumpPush_FUSENSOR(const size_t SIZ, int16_t &CNT, char *BUF, s_kineFusensor &ITM)
 * @brief appends s_kineFusensor data to buffer BUF
 * @param SIZ size of buffer
 * @param CNT position in buffer
 * @param BUF buffer
 * @param ITM s_kineFusensor
 */
void Kine_1::dump_FUSENSOR(const size_t SIZ, int16_t &CNT, char *BUF, const char *NL0,
	const char *NLN){
	if (0 < _kineCTX.udpLvl_Fusensor) {
		//SPrintF(SIZ,CNT,BUF, "==== FUSENSOR %llu µs, radar", RTitem.microTS);
		SPrintF(SIZ, CNT, BUF, "%sQUATER[%+10.3f%+10.3f%+10.3f%+10.3f] T:%6.2f°C", NL0,
			Fusensor.q[0], Fusensor.q[1], Fusensor.q[2], Fusensor.q[3], Fusensor.temp);
		if (Fusensor.distUSD)
			SPrintF(SIZ, CNT, BUF, ", radar:%7.3f m", Fusensor.distUSD);
		else
			SPrintF(SIZ, CNT, BUF, ", radar missing");
	}
	if (1 < _kineCTX.udpLvl_Fusensor) {
		stat_An.calc_Norm(Fusensor.ax, Fusensor.ay, Fusensor.az);
		stat_Gn.calc_Norm(Fusensor.gx, Fusensor.gy, Fusensor.gz);
		stat_Mn.calc_Norm(Fusensor.mx, Fusensor.my, Fusensor.mz);
		SPrintF(SIZ, CNT, BUF, "%s Accel %+10.3f%+10.3f%+10.3f mg  %s", NLN,
			1000.0 * Fusensor.ax, 1000.0 * Fusensor.ay, 1000.0 * Fusensor.az,
			stat_An.norm("  ∥Ñ∥-1", 1000.0).c_str());
		SPrintF(SIZ, CNT, BUF, "%s Gyros %+10.3f%+10.3f%+10.3f °/s %s", NLN,
			Fusensor.gx, Fusensor.gy, Fusensor.gz,
			stat_Gn.norm("  ∥Ñ∥  ").c_str());
		SPrintF(SIZ, CNT, BUF, "%s Magne %+10.3f%+10.3f%+10.3f ?G  %s", NLN,
			Fusensor.mx, Fusensor.my, Fusensor.mz,
			stat_Mn.norm("  ∥Ñ∥  ").c_str());
	}
	/*√‼|a∥∣|∫*/
}
//---------------------------------------------------------------------
void Kine_1::dump_ATTITUDE(const size_t SIZ, int16_t &CNT, char *BUF, const char *NL0,
	const char *NLN){
//SPrintF(sizeof(buffer), cnt, buffer, "==== ATTIT %llu µs",ATT.microTS);
//	char N2[3]="\n\n";
	if (0 < _kineCTX.udpLvl_Attitude) {
		SPrintF(SIZ, CNT, BUF, "%sATTIT [%+10.3f%+10.3f%+10.3f]° ∂t=%8.3f ms", NL0,
			Attitude.Nroll * 180.0f / pi, Attitude.Epitch * 180.0f / pi,
			Attitude.Dyaw * 180.0f / pi, deltaMicroTS / 1000.0);
		stat_Grav.calc_Norm(Attitude.Grav_x, Attitude.Grav_y, Attitude.Grav_z);
		SPrintF(SIZ, CNT, BUF, "%s Grav  %+10.3f%+10.3f%+10.3f mg    %s", NLN,
			1000.0f * Attitude.Grav_x, 1000.0f * Attitude.Grav_y, 1000.0f * Attitude.Grav_z,
			stat_Grav.norm("∥Ñ∥  ", 1000.0).c_str());
	}
	if (1 < _kineCTX.udpLvl_Attitude) {
		double Dt = deltaMicroTS / 1000000.0; // µs to sec

		stat_As.calc_Norm(Attitude.Ax, Attitude.Ay, Attitude.Az);
		//stat_Vs.calc_Cent(Attitude.Vx, Attitude.Vy, Attitude.Vz);
		stat_Ds.calc_Norm(Attitude.Dx / Dt, Attitude.Dy / Dt, Attitude.Dz / Dt);
		SPrintF(SIZ, CNT, BUF, "%s Accel %+10.3f%+10.3f%+10.3f mm/s² %s", NLN,
			Attitude.Ax * 1000.0f, Attitude.Ay * 1000.0f, Attitude.Az * 1000.0f,
			stat_As.norm("∥Ñ∥  ", 1000.0).c_str());
		SPrintF(SIZ, CNT, BUF, "%s V+A∂t %+10.3f%+10.3f%+10.3f mm/s", NLN,
			Attitude.Vx * 1000.0f, Attitude.Vy * 1000.0f, Attitude.Vz * 1000.0f);
		SPrintF(SIZ, CNT, BUF, "%s ∆s%%∂t %+10.3f%+10.3f%+10.3f mm    %s", NLN,
			Attitude.Dx * 1000.0f, Attitude.Dy * 1000.0f, Attitude.Dz * 1000.0f,
			stat_Ds.norm("∥Ñ∥%s", 1000.0).c_str());
		SPrintF(SIZ, CNT, BUF, "%s Posit %+10.3f%+10.3f%+10.3f m", NLN,
			Attitude.Xcur, Attitude.Ycur, Attitude.Zcur);
	}
}
//---------------------------------------------------------------------
void Kine_1::dump_KFMATRIX(const size_t SIZ, int16_t &CNT, char *BUF,
	const char *NL0, const char *NLN){
//SPrintF(sizeof(buffer), cnt, buffer,""==== KEYFRAME %llu µs\nDxyz=[",	KF.microTS);
	if (0 < _kineCTX.udpLvl_keyFrame) {
		SPrintF(SIZ, CNT, BUF, "%sKF_XYZ[", NL0);
		for (size_t i = 0; i < 3; i++)
			SPrintF(SIZ, CNT, BUF, "%+10.3f", Keyframe.DXYZ[i]); //\%+.3e
		SPrintF(SIZ, CNT, BUF, "]m         %+10.3e<  έψιλο  <%+10.3e",
			Keyframe.qualityMin, Keyframe.qualityMax);
	}
	if (1 < _kineCTX.udpLvl_keyFrame) {
		for (size_t l = 0; l < 4; l++) {
			SPrintF(SIZ, CNT, BUF, "%sMext |", NLN);
			for (size_t r = 0; r < 4; r++)
				SPrintF(SIZ, CNT, BUF, "\%+10.3f", Keyframe.MATEXT[l][r]);
			SPrintF(SIZ, CNT, BUF, " |");
		}
	}
}
//-------------------------------------------------------
void Kine_1::Predict(){
//integrateAcc(K, *_roverFifo.rq, _roverFifo._da);
}
//---------------------------------------------------------------------
String S_Kine(const uint64_t MTS, const uint8_t IDX, const char CAR, const t_vct3F &ACC,
	const t_vct3F &GYR, const t_vct3F &MAG, const t_vct3F &SPD, const char *MSG){
	char buffer[512];
	int16_t cnt = 0;
	SPrintF(sizeof(buffer), cnt, buffer,
		"%13llu %2i %c | %+9.4f,%+9.4f,%+9.4f | %+9.4f,%+9.4f,%+9.4f | %+9.4f,%+9.4f,%+9.4f | %+9.4f,%+9.4f,%+9.4f | '%s'",
		MTS, IDX, CAR, ACC[0], ACC[1], ACC[2], GYR[0], GYR[1], GYR[2], MAG[0], MAG[1],
		MAG[2], SPD[0], SPD[1], SPD[2], MSG);
	return String(buffer);
}
//---------------------------------------------------------------------
/**
 * |00 01 02 sx|
 * |10 11 12 sy|
 * |20 21 22 sz|
 * | 0  0  0  1|
 * sy
 * sz
 * 1
 Kine_1::updatePosition(const uint32_t DTMS, const speedMat & XYZ_SPEED,
 const speedMat & ABC_SPEED) {
 coord_Vct pos;
 coord_Vct dxyz;
 coord_Vct dabc;
 for(size_t l=0;l<4,l++) {
 dxyz[l]=XYZ_SPEED[l]*DTMS;
 dabc[l]=ABC_SPEED[l]*DTMS;
 }

 for(size_t l=0;l<4,l++) {
 for(size_t r=0;r<4,r++) {/media/DEV/ROB_000/setup/SET_000_WS3/rov-Hard_IMU_000_WS3.cpp
 pos[l]+=SPEED[r][l]*POS[r];
 }
 }

 return pos;
 }
 */
//-------------------------_avg_Attitude------------------------------
//  INTEGRATION
/*
 TMS in µicro-secondes
 Retou//---------------------------------------------------------------------
 void Kine_1::update RoverKine(const s_kineFusensor & FUSENS, const float LMD) {
 calc _ATTITUDE(_avg_Attitude, FUSENS, LMD);
 calc _KEYFRAME(_avg_Keyframe, _avg_Attitude, FUSENS);
 _rover RQ.enqueue(_avg_Keyframe); // recoppie le KF ????????????????
 }
 rne l'intégrale par seconde


 Warning... computation is done going back in time
 */
/*
 void Kine_1::integrateAcc(kine ModelData & inK, ringQueue<kine ModelData> & RQ,
 const uint8_t DEPTH) {
 class Integrator {
 private:
 uint8_t _depth;
 uint64_t _savMts;
 t_vct3F _savAcc;
 int _count = 0;
 public:
 t_vct3F dSpd;
 Integrator(kine ModelData & inK, const uint8_t DEPTH) { //constructeur
 _depth = DEPTH;
 _savMts = inK.microTS;
 _savAcc = inK.Acc;
 }
 void operator()(kineModelData & inK) {
 _count++;s_kineFusensor
 float dt = (_savMts - inK.microTS) / 2000000.0 / _depth; // seconds !!
 std::transform(_savAcc.begin(), _savAcc.end(), inK.Acc.begin(), dSpd.begin(),
 std::plus<float>());

 // std::transform(_savAcc.begin(), _savAcc.end(), dSpd.begin(),			 [&dSpd](auto& c) {return c*dt;});

 for (auto i = 0; i < 3; i++) {
 dSpd[i] = dSpd[i] * dt;
 }
 _savMts = inK.microTS;
 _savAcc = inK.Acc;
 addMSG(inK, "I");
 }
 int count() const {
 return _count;
 }
 };
 Integrator integrale(inK, DEPTH); //instanciation
 integrale = for_each(RQ.begin(), RQ.stop(DEPTH), integrale);
 if (integrale.count() > 0) {
 std::transform(inK.Spd.begin(), inK.Spd.end(), integrale.dSpd.begin(),
 inK.Spd.begin(), std::plus<float>()a31a31);
 //	 addMSG(inK, (String((*it).INT, 3) + "+" + String(integrale.Sx(), 3) + "/" + String(integrale.count(), 3)).c_str());
 } else {
 //_DEBUG _ERR( "No ringqueue to integrate acceleration");
 }
 }
 */
///////////////////////////////////////////////////////////////////////
