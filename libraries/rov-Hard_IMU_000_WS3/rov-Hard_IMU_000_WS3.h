#ifndef _HARD_IMU_000_WS3_
#define _HARD_IMU_000_WS3_
#include "rov-Hard_000_WS3.h"
#include "000_MPU9250.h"
#include "rov.CONTEXT.h"
#include "000_SFR02.h"
#include "000_SIGNAL_PROCESS.h"
typedef struct {
	uint32_t tik = 0; //12×30×24×3600×100=3 110 400 000=0xB964F000
	uint8_t fid = 0;
	float ax = 0.0;
	float ay = 0.0;
	float az = 0.0;
	float gx = 0.0;
	float gy = 0.0;
	float gz = 0.0;
	float mx = 0.0;
	float my = 0.0;
	float mz = 0.0;
	float q[4]={1.0,0.0,0.0,0.0};
	float temp = 0;
	uint64_t isrMicroTS = 0;
	float distUSD = 0.0;
	bool USD = false;
	bool IMU = false; // Unused because IMU ISR => data have to be read
} s_fusensors;
////////////////////////////////////////////////////////////////
/**
 * Calculateur de déclinaison magnétique
 * https://geomag.nrcan.gc.ca/calc/mdcal-fr.php
 * 31180+Lapeyrouse-Fossat Latitude 43.7032921 Nord , Longitude 1.4927161 Est
 * Au 24/06/2020
 * Déclinaison magnétique: 0° 56,41' est sans la déclinaison du quadrillage
 * Déclinaison magnétique: 0° 56,41' est avec la déclinaison du quadrillage
 * Déclinaison du Quadrillage: 1° 58,90' est
 * Angle de convergence: 1° 2,49' ouest
 */
/**
 * \class H_AccGyrMag :public H_Module
 */
class H_AccGyrMag: public H_Module {
private:
	s_ximuCTX & _ximuCTX;

	bool SerialDebug = true;
	// MPU9250 Configuration
	// Specify sensor full scale
	/* Choices are:
	 *  Gscale: G	float qz= FUSE.q[3];
	 *  FS_250 == 250 dps, GFS_500 DPS == 500 dps, GFS_1000 == 1000 dps, and GFS_2000DPS == 2000 degrees per second gyro full scale
	 *  Ascale: AFS_2G == 2 g, AFS_4G == 4 g, AFS_8G == 8 g, and AFS_16G == 16 g accelerometer full scale
	 *  Mscale: MFS_14BITS == 0.6 mG per LSB and MFS_16BITS == 0.15 mG per LSB
	 *  Mmode: Mmode == M_8Hz for 8 Hz data rate or Mmode = M_100Hz for 100 Hz data rate
	 *  (1 + sampleRate) is a simple divisor of the fundamental 1000 kHz rate of the gyro and accel, so
	 *  sampleRate = 0x00 means 1 kHz sample rate for both accel and gyro, 0x04 means 200 Hz, etc.
	 */
	uint8_t Gscale = GFS_250DPS, Ascale = AFS_2G, Mscale = MFS_16BITS, Mmode = M_100Hz,
		sampleRate = 0x04;
	float aRes, gRes, mRes; // scale resolutions per LSB for the sensors
	float motion = 0; // check on linear acceleration to determine motion
	// global constants for 9 DoF fusion and AHRS (Attitude and Heading Reference System)
	float GyroMeasError = pi * (40.0f / 180.0f); // gyroscope measurement error in rads/s (start at 40 deg/s)
	float GyroMeasDrift = pi * (0.0f / 180.0f); // gyroscope measurement drift in rad/s/s (start at 0.0 deg/s/s)
//	float beta = 0.0588; //sqrtf(3.0f / 4.0f) * GyroMeasError; // compute beta
	float zeta = sqrtf(3.0f / 4.0f) * GyroMeasDrift; // compute zeta, the other free parameter in the Madgwick scheme usually set to a small or zero value
	//Mahony
	float Kp = 0.25;
	float Ki = 0.002;
	bool wakeup;

	// Pin definitions
//	uint8_t intPin = 9; //  MPU9250 interrupt
//	int myLed = 13; // red led

	//bool intFlag = false;
	bool newMagData = false;

	int16_t MPU9250Data[7]; // used to read all 14 bytes at once from the MPU9250 accel/gyro
	int16_t magCount[3]; // Stores the 16-bit signed magnetometer sensor output
	float magCalibration[3] = { 0, 0, 0 }; // Factory mag calibration and mag bias
	float temperature; // Stores the MPU9250 internal chip temperature in degrees Celsius
	float SelfTest[6]; // holds results of gyro and accelerometer self test

	uint32_t delt_t = 0; // used to control display output rate
	uint32_t count = 0, sumCount = 0; // used to control display output rate
	float pitch, yaw, roll; // absolute orientation
	float deltat = 0.0f, sum = 0.0f; // integration interval for both filter schemes
	uint64_t lastUpdate = 0; // used to calculate integration interval
	uint64_t Now = 0; // used to calculate integration interval

	float ax, ay, az, gx, gy, gz, mx, my, mz; // variables to hold latest sensor data values
	uint64_t _microTS;
	// linear acceleration (acceleration with gravity component subtracted)
	float q[4] = { 1.0f, 0.0f, 0.0f, 0.0f }; // vector to hold quaternion
	float eInt[3] = { 0.0f, 0.0f, 0.0f }; // vector to hold integral error for Mahony method
	float Grav_x, Grav_y, Grav_z;
	MPU9250 _mpu9250 = MPU9250(MPU9250_1_ADDRESS); // instantiate MPU9250 class;

	void loadIMU();
	void saveIMU();
	String dumpCAL();
	void Reset() {
	}
	__attribute__((optimize("O3"))) void MadgwickQuaternionUpdate(float ax, float ay,
		float az, float gx, float gy, float gz, float mx, float my, float mz);
	void MahonyQuaternionUpdate(float ax, float ay, float az, float gx, float gy,
		float gz, float mx, float my, float mz);
	void calACCGYR();
	void calMAGNETO();
public:
	float _LMD; // Local Magnetic Declination
	void acq_trueIMU( const uint64_t MICROTS); // simulate by last s_fusensors in _IMUqueue
	void acq_simuIMU( imuDsp & SIMIMU);
	void Clear_IMU_quaternion();
	void get_IMU(s_fusensors & FSI);
	void telemetry(const s_fusensors & FSI);
	String dumpPush_FUSENSOR(s_fusensors & RTitem);
	H_AccGyrMag(const s_IMU IMU_CFG, s_ximuCTX * CONTEXT, const String NAME,
		 int8_t* DBGMAX, const bool ON = true);
	/**
	 * \fn t_AfloatAxes Get(H_pidMotors & PIDS);
	 * \brief Gets Acceleration(m/s²), Gyroscope(°/s) & Magnetometer(°) data
	 * \return s_AGM struct of 3 array<float, 3>
	 */
	//String dumpCinematic(s_fusensors & RTitem);
};
///////////////////////////////////////////////////////////////////////
/**
 * \class H_ultraSounds :public H_Module
 */
class H_ultraSounds: public H_Module {
private:
	void Reset() {
	}
public:
	H_ultraSounds(const String NAME,  int8_t* DBGMAX, const bool ON = true);
	/**
	 * \fn float Get()
	 * \brief get distance detected by sensor
	 * \return float distance in meters
	 */
	SFR02 _sfr02 = SFR02(); // instantiate SFR02 class;
	float Get(); // simulate by last dat in fifo
};
///////////////////////////////////////////////////////////////////////
#endif
///////////////////////////////////////////////////////////////////////
