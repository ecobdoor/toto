/** @file */
#pragma once
//#include <esp32_LOGS.h>
#include "rov-Hard_IMU_000_WS3.h"
//#include <esp32_STATS.h>
//	------------------------------------------------------
typedef struct {
	//To be initialized by clear_lastKine(s_kineAttitude & KP)
	double Xcur = 0.0, Ycur = 0.0, Zcur = 0.0;
	double Ax = 0.0, Ay = 0.0, Az = 0.0;
	double Vx = 0.0, Vy = 0.0, Vz = 0.0;
	double Dx = 0.0, Dy = 0.0, Dz = 0.0;
	//To be initialized later by fusensors
	uint32_t imuTik = 0; //???????????? ussefull ??
	float Nroll = 0.0, Epitch = 0.0, Dyaw = 0.0;
	float Grav_x = 0.0, Grav_y = 0.0, Grav_z = 0.0;
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
	float qualityMin = +1.0e+4;
	float qualityMax = -1.0e+4;
	t_vct4F DXYZ = { { (float)0.0, (float)0.0, (float)0.0, (float)1.0 } }; // from linear accelerations
	t_mat4F MATEXT = { {
		{ { (float)1.0, (float)0.0, (float)0.0, (float)0.0 } },
		{ { (float)0.0, (float)1.0, (float)0.0, (float)0.0 } },
		{ { (float)0.0, (float)0.0, (float)1.0, (float)0.0 } },
		{ { (float)0.0, (float)0.0, (float)0.0, (float)1.0 } }
	} };
	char MSG[36] = { '\0' }; // for ring queue messaging
} s_kineKeyframe;
String S_Kine(const uint64_t MTS, const uint8_t IDX, const char CAR,
	const t_vct3F &ACC, const t_vct3F &GYR, const t_vct3F &MAG,
	const t_vct3F &SPD, const char *MSG);
///////////////////////////////////////////////////////////////////////
class Kine_1: public Core {
private:
	s_kineCTX &_kineCTX;
	c_stat_3F stat_As, /*stat_Vs,*/ stat_Ds;
	c_stat_3F stat_An, stat_Gn,stat_Grav, stat_Mn;
	/**
	 * compute s_kineKeyframe Position & external Attitude matrix (EXTMAT like)
	 * Cf. https://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/index.htm
	 * Cf. https://fr.wikipedia.org/wiki/Quaternions_et_rotation_dans_l%27espace
	 */
	void calc_KEYFRAME();
	void calc_ATTITUDE(const float LMD);
	void init_statistics();

public:
	uint64_t curMicroTS=0;/**< microseconds time current kinematic's update (@ref updateRoverKine) */
	uint64_t deltaMicroTS=0;/**< microseconds time since the previous kinematic's update (@ref updateRoverKine) */
	s_kineFusensor Fusensor;// µTS == prev MicroTS after updateRoverKine !!!
	s_kineAttitude Attitude;// µTS == prev MicroTS after updateRoverKine !!!
	s_kineKeyframe Keyframe;// µTS == prev MicroTS after updateRoverKine !!!
	Kine_1(s_kineCTX *KINECTX, const String NAME, int8_t *DBGMAX);
	void Predict();
	void Clear();
	/**
	 * @fn void updateRoverKine(uint8_t &TELEMETRY, const bool TELEM, bool &CLROPNGL, const bool OPNGL,	const char *NL0, const char *NLN, const float LMD)
	 * @brief updates the attitude, acceleration,velocity, position... and the frame matrix
	 *
	 *
	 * @param TELEMETRY returns 0 & stops CSV telemetry if UDP error when sending else unchanged
	 * @param TELEM activates CSV telemetry if true
	 * @param CLROPNGL if true, returns false after clearing openGL
	 * @param OPNGL activates openGL serial telemetry
	 * @param NL0 prefix for the first line on UDP dashboard (permits to interlace 2 Kine timings on dashboard)
	 * @param NLN prefix for the next lines on the UDP dashboard (permits to interlace 2 Kine timings on dashboard)
	 * @param LMD
	 */
	void updateRoverKine( const float LMD = 0.0);
	void dump_FUSENSOR(const size_t SIZ, int16_t &CNT, char *BUF,
		const char *NL0 = "\n", const char *NLN = "\n");
	void dump_ATTITUDE(const size_t SIZ, int16_t &CNT, char *BUF,
		const char *NL0 = "\n", const char *NLN = "\n");
	void dump_KFMATRIX(const size_t SIZ, int16_t &CNT, char *BUF,
		const char *NL0 = "\n", const char *NLN = "\n");
	void telemetry_KEYFRAME(const size_t SIZ, int16_t &CNT, char *BUF)const;
	void telemetry_FUSENSOR(const size_t SIZ, int16_t &CNT, char *BUF)const;
	void telemetry_ATTITUDE(const size_t SIZ, int16_t &CNT, char *BUF)const;
};
inline void Kine_1::init_statistics(){
	stat_As.init();
//	stat_Vs.init();
	stat_Ds.init();
	stat_An.init(-1);
	stat_Gn.init();
	stat_Mn.init();
	stat_Grav.init();
}
///////////////////////////////////////////////////////////////////////
