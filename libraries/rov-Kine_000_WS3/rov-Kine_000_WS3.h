#ifndef _KINE_000_WS3_
#define _KINE_000_WS3_
//#include <esp32_LOGS.h>
#include "rov-Hard_IMU_000_WS3.h"
//	------------------------------------------------------
typedef struct {
	//To be initialized by clear_lastKine(s_kineAttitude & KP)
	float Xcur = 0, Ycur = 0, Zcur = 0;
	float lin_Vx = 0.0, lin_Vy = 0.0, lin_Vz = 0.0;
	//To be initialized later by fusensoors
	uint32_t imuTik = 0; //???????????? ussefull ??
	uint64_t isrMicroTS = 0;
	float Nroll = 0.0, Epitch = 0.0, Dyaw = 0.0;
	float lin_ax = 0.0, lin_ay = 0.0, lin_az = 0.0;
	float Grav_x = 0.0, Grav_y = 0.0, Grav_z = 0.0;
	float Dx = 0.0, Dy = 0.0, D = 0.0;
	float colDist = 0.0;
} s_kineAttitude;
//	------------------------------------------------------

typedef std::array<float, 3> t_vct3F;
typedef std::array<float, 4> t_vct4F;
typedef std::array<t_vct4F, 4> t_mat4F;
/*
 typedef float t_vct3F[3];
 typedef float t_vct4F[4];
 typedef float t_mat4F[4][4];
 */
/**
 *
 * s_kineKeyframe KF; //trajectory Key frame (repère clé)
 */
typedef struct {
	uint64_t isrMicroTS; // micro seconds time;
	float qualityMin;
	float qualityMax;
	t_vct4F DXYZ; // from linear accelerations
	t_mat4F MATEXT; //NED attitude from quaternion relative to RefPoint (at T0 & Offset)
	char MSG[36]; // for ring queue messaging
} s_kineKeyframe;
typedef ringQueue<s_kineKeyframe> t_rqKF;

String S_Kine(const uint64_t MTS, const uint8_t IDX, const char CAR,
	const t_vct3F &ACC, const t_vct3F &GYR, const t_vct3F &MAG,
	const t_vct3F &SPD, const char *MSG);
///////////////////////////////////////////////////////////////////////
class Kine_1: public Core {
private:
	s_kineCTX &_kineCTX;
	t_rqKF &_roverRQ;
	t_rqKF &_driveRQ;
	String csv_Keykf(const s_kineKeyframe &KF);
	/**
	 * compute s_kineKeyframe Position & external Attitude matrix (EXTMAT like)
	 * Cf. https://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/index.htm
	 * Cf. https://fr.wikipedia.org/wiki/Quaternions_et_rotation_dans_l%27espace
	 */
	void calc_KEYFRAME(s_kineKeyframe &KEYFRAME,
		const float Xcur, const float Ycur, const float Zcur,
		const s_fusensors &FUSENS);
	void calc_ATTITUDE(s_kineAttitude &ATTITUDE,
		float &DX, float &DY, float &DZ,
		const s_fusensors &FUSENS, const float LMD);
	void telemetry();
	public:
	s_kineAttitude _last_Attitude;
	s_kineKeyframe _last_Keyframe = {
		.isrMicroTS = 0,
		.qualityMin = +1.0e+4,
		.qualityMax = -1.0e+4,
		.DXYZ = { { (float)0.0, (float)0.0, (float)0.0, (float)1.0 } },
		.MATEXT = { {
			{ { (float)1.0, (float)0.0, (float)0.0, (float)0.0 } },
			{ { (float)0.0, (float)1.0, (float)0.0, (float)0.0 } },
			{ { (float)0.0, (float)0.0, (float)1.0, (float)0.0 } },
			{ { (float)0.0, (float)0.0, (float)0.0, (float)1.0 } }
		} },
		.MSG = { '\0' }
	};
	void dump_KINE(t_rqKF &rqKF);
	void push(t_rqKF &RQ, const s_kineKeyframe &KF);
	Kine_1(s_kineCTX *KINECTX, const String NAME, int8_t *DBGMAX);
	void Predict();
	void clear_Kine_Rover();
	void updateRoverKine(const s_fusensors &FUSENS, const float LMD = 0.0);
	String dumpPush_ATTITUDE(const s_kineAttitude &ATTITUDE);
	String dumpPush_KEYFRAME(const s_kineKeyframe &KEYFRAME);
};

///////////////////////////////////////////////////////////////////////
#endif
///////////////////////////////////////////////////////////////////////
