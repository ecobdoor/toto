#include "rov-Kine_000_WS3.h"
//#include "rov-CfgH(000_WS3).h"
#include "esp32_LOGS.h"
#include <algorithm> // transform for_each
#include <functional> // minus
///////////////////////////////////////////////////////////////////////
Kine_1::Kine_1(s_kineCTX *KINECTX, const String NAME, int8_t *DBGMAX)
:
	Core(NAME.c_str(), DBGMAX),
		_kineCTX(*KINECTX),
		_roverRQ(*(new t_rqKF(DEF_KINE_RQSIZE, 2))),
		_driveRQ(*(new t_rqKF(DEF_KINE_RQSIZE, 2))){
}
//---------------------------------------------------------------------
String Kine_1::csv_Keykf(const s_kineKeyframe &KF){
	const t_mat4F &matext = KF.MATEXT;
	char buffer[256];
	int16_t cnt = 0;
	SPrintF(sizeof(buffer), cnt, buffer, "%llu;", KF.isrMicroTS / 1000);
	for (size_t l = 0; l < 3; l++)
		for (size_t r = 0; r < 4; r++)
			SPrintF(sizeof(buffer), cnt, buffer, "%15.10f;", matext[l][r]);
	buffer[cnt - 1] = '\0'; // suppress last separator
	return String(buffer);
}
//---------------------------------------------------------------------
void Kine_1::telemetry(){
	//_DEBUG_(0, 0,  "\nTelemetry %i", _kineCTX.telemetry);
	if (_kineCTX.telemetry) {
		_kineCTX.telemetry = _CSV_("@wk:999;%s", csv_Keykf(_last_Keyframe).c_str());
	}
	//_DEBUG_(0, 0,  "\nTelemetry %i", _kineCTX.telemetry);
}
//---------------------------------------------------------------------
void Kine_1::clear_Kine_Rover(){
	_last_Attitude.lin_ax = 0;
	_last_Attitude.lin_ay = 0;
	_last_Attitude.lin_az = 0;
	_last_Attitude.lin_Vx = 0;
	_last_Attitude.lin_Vy = 0;
	_last_Attitude.lin_Vz = 0;
	_last_Attitude.Xcur = 0;
	_last_Attitude.Ycur = 0;
	_last_Attitude.Zcur = 0;
	_last_Attitude.isrMicroTS = 0;

}
//---------------------------------------------------------------------
void Kine_1::calc_KEYFRAME(s_kineKeyframe &KF,
	const float Xcur, const float Ycur, const float Zcur,
	const s_fusensors &FUSENS){
	KF.isrMicroTS = FUSENS.isrMicroTS;
	//assert(ATITUD.isrMicroTS == FUSENS.isrMicroTS); // has been init by calc_ATTITUDE
	/*
	 KF.DXYZ[0] = Dx;
	 KF.DXYZ[1] = Dy;
	 KF.DXYZ[2] = Dz;
	 KF.DXYZ[3] = 1.0;
	 */
	t_mat4F &matext = KF.MATEXT;
	float Qw = FUSENS.q[0];
	float Qx = FUSENS.q[1];
	float Qy = FUSENS.q[2];
	float Qz = FUSENS.q[3];
	float _2w2 = 2 * Qw * Qw, _2wx = 2 * Qw * Qx, _2wy = 2 * Qw * Qy, _2wz = 2 * Qw * Qz;
	float _2x2 = 2 * Qx * Qx, _2xy = 2 * Qx * Qy, _2xz = 2 * Qx * Qz;
	float _2y2 = 2 * Qy * Qy, _2yz = 2 * Qy * Qz;
	float _2z2 = 2 * Qz * Qz;
	float quality = _2w2 + _2x2 + _2y2 + _2z2 - 2;
	if (quality < KF.qualityMin)
		KF.qualityMin = quality;
	if (KF.qualityMax<quality)
		KF.qualityMax = quality;
	//assert(abs(KF.quality) < 2e-15);
	matext[0][0] = 1 - _2y2 - _2z2;
	matext[0][1] = _2xy - _2wz;
	matext[0][2] = _2xy + _2wy;
	matext[0][3] = Xcur;

	matext[1][0] = _2xy + _2wz;
	matext[1][1] = 1 - _2x2 - _2z2;
	matext[1][2] = _2yz - _2wx;
	matext[1][3] = Ycur;

	matext[2][0] = _2xz - _2wy;
	matext[2][1] = _2yz + _2wx;
	matext[2][2] = 1 - _2x2 - _2y2;
	matext[2][3] = Zcur;

	matext[3][0] = 0.0;
	matext[3][1] = 0.0;
	matext[3][2] = 0.0;
	matext[3][3] = 1.0;
}
//---------------------------------------------------------------------
/**
 *https://www.google.com/search?q=roll+pitch+yaw+north+east+down&safe=active&client=ubuntu&hs=a7R&channel=fs&sxsrf=ALeKk00ooFYaDR_aaHhPMLRPhzL8tCZfMw:1593158791426&tbm=isch&source=iu&ictx=1&fir=AwxFZY9kMLb4uM%252Cgjw8nLbl3yPWZM%252C_&vet=1&usg=AI4_-kS7s5xFbb7FR_eJ97UW5N2GsKxbTQ&sa=X&ved=2ahUKEwjcstX-gp_qAhUBuRoKHUcVACEQ9QEwAHoECAgQHA&biw=1286&bih=659#imgrc=AwxFZY9kMLb4uM
 */
void Kine_1::calc_ATTITUDE(s_kineAttitude &ATTITUDE,
	float &DX, float &DY, float &DZ,
	const s_fusensors &FUSENS,
	const float LMD){
	float oldAx = ATTITUDE.lin_ax;
	float oldAy = ATTITUDE.lin_ay;
	float oldAz = ATTITUDE.lin_az;
	/*
	 float oldXc = ATTITUDE.Xcur;
	 float oldYc = ATTITUDE.Ycur;
	 float oldZc = ATTITUDE.Zcur;
	 */
	float a12, a22, a31, a32, a33; // rotation matrix coefficients for Euler angles and gravity components

	float q[4] = { FUSENS.q[0], FUSENS.q[1], FUSENS.q[2], FUSENS.q[3] };
	a12 = 2.0f * (q[1] * q[2] + q[0] * q[3]);
	a22 = q[0] * q[0] + q[1] * q[1] - q[2] * q[2] - q[3] * q[3];
	ATTITUDE.Dyaw = atan2f(a12, a22) + (1.13333 * pi / 180); //LMD; // Local MAgnetic Declination at Danville, California is 13 degrees 48 minutes and 47 seconds on 2014-04-04
	if (ATTITUDE.Dyaw < 0)
		ATTITUDE.Dyaw += 2 * pi; // Ensure yaw stays between 0 and 360

	a31 = 2.0f * (q[0] * q[1] + q[2] * q[3]);
	a33 = q[0] * q[0] - q[1] * q[1] - q[2] * q[2] + q[3] * q[3];
	ATTITUDE.Nroll = atan2f(a31, a33);

	a32 = 2.0f * (q[1] * q[3] - q[0] * q[2]);
	ATTITUDE.Epitch = -asinf(a32);
	_SERIAL_7("\natt Aij=(%+10.3f,%+10.3f,%+10.3f,%+10.3f,%+10.3f)",
		a12, a22, a31, a32, a33);
	ATTITUDE.lin_ax = FUSENS.ax * (1 + a31);
	ATTITUDE.lin_ay = FUSENS.ay * (1 + a32);
	ATTITUDE.lin_az = FUSENS.az * (1 - a33);
	/*
	 Original
	 ATTITUDE.lin_ax = FUSENS.ax + a31;
	 ATTITUDE.lin_ay = FUSENS.ay + a32;
	 ATTITUDE.lin_az = FUSENS.az - a33;
	 Bad test ???
	 ATTITUDE.lin_ax = FUSENS.ax - a32;
	 ATTITUDE.lin_ay = FUSENS.ay - a31;
	 ATTITUDE.lin_az = FUSENS.az;// - a33;
	 */
	ATTITUDE.Grav_x = -a31;
	ATTITUDE.Grav_y = -a32;
	ATTITUDE.Grav_z = 0; //+a33;
	ATTITUDE.imuTik = FUSENS.tik;
	ATTITUDE.colDist = FUSENS.distUSD;
	float Dt = (FUSENS.isrMicroTS - ATTITUDE.isrMicroTS) / 1000000.0; // µs to sec
	ATTITUDE.lin_Vx += ATTITUDE.lin_ax * Dt;
	ATTITUDE.lin_Vy += ATTITUDE.lin_ay * Dt;
	ATTITUDE.lin_Vz += ATTITUDE.lin_az * Dt;
	//float Dt2 = Dt * Dt;
	DX = ATTITUDE.lin_Vx * Dt; //+ 0.5 * ATTITUDE.lin_ax * Dt2;
	DY = ATTITUDE.lin_Vy * Dt; // + 0.5 * ATTITUDE.lin_ay * Dt2;
	DZ = ATTITUDE.lin_Vz * Dt; // + 0.5 * ATTITUDE.lin_az * Dt2;
	/*
	 DX = ATTITUDE.lin_Vx * Dt + 0.25 * (oldAx + ATTITUDE.lin_ax) * Dt2;
	 DY = ATTITUDE.lin_Vy * Dt + 0.25 * (oldAy + ATTITUDE.lin_ay) * Dt2;
	 DZ = ATTITUDE.lin_Vz * Dt + 0.25 * (oldAz + ATTITUDE.lin_az) * Dt2;
	 */
	ATTITUDE.Xcur += DX;
	ATTITUDE.Ycur += DY;
	ATTITUDE.Zcur += DZ;
	/*
	 ATTITUDE.lin_Vx += 0.5 * (oldAx + ATTITUDE.lin_ax) * Dt;
	 ATTITUDE.lin_Vy += 0.5 * (oldAy + ATTITUDE.lin_ay) * Dt;
	 ATTITUDE.lin_Vz += 0.5 * (oldAz + ATTITUDE.lin_az) * Dt;
	 */
	ATTITUDE.isrMicroTS = FUSENS.isrMicroTS; // after Dxyz computation

}
//---------------------------------------------------------------------
void Kine_1::updateRoverKine(const s_fusensors &FUSENS, const float LMD){
	float Dx;
	float Dy;
	float Dz;
	calc_ATTITUDE(_last_Attitude, Dx, Dy, Dz, FUSENS, LMD);
	calc_KEYFRAME(_last_Keyframe,
		_last_Attitude.Xcur, _last_Attitude.Ycur, _last_Attitude.Zcur,
		FUSENS);
	_last_Keyframe.DXYZ[0] = Dx;
	_last_Keyframe.DXYZ[1] = Dy;
	_last_Keyframe.DXYZ[2] = Dz;
	_last_Keyframe.DXYZ[3] = 1.0;
	_roverRQ.enqueue(_last_Keyframe); // recoppie le KF ????????????????
	telemetry();
}
//---------------------------------------------------------------------
String Kine_1::dumpPush_ATTITUDE(const s_kineAttitude &ATTITUDE){
	char buffer[512];
	buffer[0] = 0;
	int16_t cnt = 0;
	//SPrintF(sizeof(buffer), cnt, buffer, "==== ATTITUDE %llu µs",ATTITUDE.isrMicroTS);
	if (0 < _kineCTX.dmpLvl_Attitude) {
		SPrintF(sizeof(buffer), cnt, buffer, "NED rpy:(%+10.3f%+10.3f%+10.3f)°",
			ATTITUDE.Nroll * 180.0f / pi, ATTITUDE.Epitch * 180.0f / pi,
			ATTITUDE.Dyaw * 180.0f / pi);
	}
	if (1 < _kineCTX.dmpLvl_Attitude) {
		SPrintF(sizeof(buffer), cnt, buffer, "\nPOS xyz(%+10.3f%+10.3f%+10.3f)m",
			ATTITUDE.Xcur, ATTITUDE.Ycur, ATTITUDE.Zcur);
		SPrintF(sizeof(buffer), cnt, buffer, "\nSPD xyz(%+10.3f%+10.3f%+10.3f)m/s",
			ATTITUDE.lin_Vx, ATTITUDE.lin_Vy, ATTITUDE.lin_Vz);
		SPrintF(sizeof(buffer), cnt, buffer, "\nLin acc(%+10.3f%+10.3f%+10.3f)mm/s²",
			ATTITUDE.lin_ax * 1000.0f, ATTITUDE.lin_ay * 1000.0f, ATTITUDE.lin_az * 1000.0f);
		SPrintF(sizeof(buffer), cnt, buffer, "\nGravXYZ(%+10.3f%+10.3f%+10.3f)mg",
			1000.0f * ATTITUDE.Grav_x, 1000.0f * ATTITUDE.Grav_y, 1000.0f * ATTITUDE.Grav_z);
	}
	return String(buffer);
}
//---------------------------------------------------------------------
String Kine_1::dumpPush_KEYFRAME(const s_kineKeyframe &KF){
	char buffer[512];
	buffer[0] = 0;
	int16_t cnt = 0;
	//SPrintF(sizeof(buffer), cnt, buffer,""==== KEYFRAME %llu µs\nDxyz=[",	KF.isrMicroTS);
	if (0 < _kineCTX.dmpLvl_Keyframe) {
		SPrintF(sizeof(buffer), cnt, buffer, "kf Dxyz=[");
		for (size_t i = 0; i < 3; i++)
			SPrintF(sizeof(buffer), cnt, buffer, "\%+10.3f", KF.DXYZ[i]); //\%+.3e
		SPrintF(sizeof(buffer), cnt, buffer, "]m %+10.3e<ε<%+10.3e", KF.qualityMin, KF.qualityMax);
	}
	if (1 < _kineCTX.dmpLvl_Keyframe) {
		for (size_t l = 0; l < 4; l++) {
			SPrintF(sizeof(buffer), cnt, buffer, "\nMext |");
			for (size_t r = 0; r < 4; r++)
				SPrintF(sizeof(buffer), cnt, buffer, "\%+10.3f", KF.MATEXT[l][r]);
			SPrintF(sizeof(buffer), cnt, buffer, " |");
		}
	}
	return String(buffer);
}
//-------------------------------------------------------
void Kine_1::Predict(){
	//integrateAcc(K, *_roverFifo.rq, _roverFifo._da);
}
//---------------------------------------------------------------------
void Kine_1::push(t_rqKF &RQ, const s_kineKeyframe &KF){
	RQ.enqueue(KF);
}
//-------------------------------------------------------
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
//-------------------------------------------------------
void Kine_1::dump_KINE(t_rqKF &rqKF){
	int debut = rqKF.debut;
	int fin = rqKF.fin;
	int cnt = rqKF.cnt();
	_DBG_DRV_KINE_("\n *** rqKF(cnt,debut,fin) = (%i,%i,%i)  *******", cnt, debut, fin);
	_DBG_DRV_KINE_(
		"\n--T(µs)------  I * +---------Acceleration----------+-------------Gyroscope---------+---------Magnetometer----------+--------------Speed------------+-MSG----");
	for (int i = 0; i <= DEF_KINE_RQSIZE; i++) {
//		RQ_L.clrX(i); //effacer le message
//		RQ_R.clrX(i); //effacer le message
		/*
		 s_kineAttitude frame = rqKF.getX(i);
		 _DBG_DRV_KINE_( "\n%s",
		 S_Kine(frame.isrMicroTS, i,
		 ((i == debut) && (i == fin)) ?
		 'X' : ((i == debut) ? '>' : ((i == fin) ? '<' : ' ')),
		 frame.Acc, frame.Gyr, frame.Mag, frame.Spd, frame.MSG).c_str());
		 */
	}
	_DBG_DRV_KINE_(
		"\n-------------------+-------------------------------+-------------------------------+-------------------------------+-------------------------------+--------");
	/*
	 _DBG_DRV_KINE_( "\n%s",
	 S_Kine(milli_TS(), 0, '*', pid_L._kp, pid_R._kp, pid_L._ki, pid_R._ki, pid_L._kd,
	 pid_R._kd, 0, 0, ".....").c_str());
	 */
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
//-------------------------_last_Attitude------------------------------
//  INTEGRATION
/*
 TMS in µicro-secondes
 Retou//---------------------------------------------------------------------
 void Kine_1::updateRoverKine(const s_fusensors & FUSENS, const float LMD) {
 calc_ATTITUDE(_last_Attitude, FUSENS, LMD);
 calc_KEYFRAME(_last_Keyframe, _last_Attitude, FUSENS);
 _roverRQ.enqueue(_last_Keyframe); // recoppie le KF ????????????????
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
 _savMts = inK.isrMicroTS;
 _savAcc = inK.Acc;
 }
 void operator()(kineModelData & inK) {
 _count++;s_fusensors
 float dt = (_savMts - inK.isrMicroTS) / 2000000.0 / _depth; // seconds !!
 std::transform(_savAcc.begin(), _savAcc.end(), inK.Acc.begin(), dSpd.begin(),
 std::plus<float>());

 // std::transform(_savAcc.begin(), _savAcc.end(), dSpd.begin(),			 [&dSpd](auto& c) {return c*dt;});

 for (auto i = 0; i < 3; i++) {
 dSpd[i] = dSpd[i] * dt;
 }
 _savMts = inK.isrMicroTS;
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
