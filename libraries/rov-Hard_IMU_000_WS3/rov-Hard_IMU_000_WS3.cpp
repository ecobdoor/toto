#include "rov-Hard_IMU_000_WS3.h"
#include "000_I2C.h"
#include "esp32_LOGS.h"
#include "esp32_SPRINT.h"
#include "esp32_FSTR.h"
using namespace std;
////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------
/**
 * Calibrate gyro and accelerometers, computes biases
 */
/*
 bool savePCD_MagCal(const char *FILENAME, const uint32_t COLOR){
 File file = SPIFFS.open(FILENAME, FILE_WRITE);
 if (!file) {
 _SERIAL_0(" FAILS to create '%s'", FILENAME);
 return false;
 }
 file.println("# .PCD v.7 - Point Cloud Data file format");
 file.println("VERSION .7");
 file.println("FIELDS x y z rgb");
 file.println("SIZE 4 4 4 4");
 file.println("TYPE F F F F");
 file.println("COUNT 1 1 1 1");
 file.println("WIDTH 213");
 file.println("HEIGHT 1");
 file.println("VIEWPOINT 0 0 0 1 0 0 0");
 file.println("POINTS 213");
 file.println("DATA ascii");
 file.println("");
 while (false) {
 file.println("");
 }
 file.close();
 return true;
 }
 */
//---------------------------------------------------------------------
/**
 * Calibrate gyro and accelerometers, computes biases
 */
void H_AccGyrMag::calibrate_ACCGYR(){
	_DBG_DRV_XIMU("\n     ------------------------------------------------------");
	_DBG_DRV_XIMU("\n%s", dumpCALintertial("     CURRENT ").c_str());
	_DBG_DRV_XIMU("\n     Place the device in a very stable position");
	uint16_t measuresCnt = _mpu9250.calibrateMPU9250(_ximuCTX.gyrBias, _ximuCTX.accBias);
	_DBG_DRV_XIMU("\n     ACC/GYR calibration done (%i measures)", measuresCnt);
	/*
	 _mpu9250.calibrateMPU9250(_ximuCTX.gyrBias, _ximuCTX.accBias);
	 _mpu9250.calibrateMPU9250(_ximuCTX.gyrBias, _ximuCTX.accBias);
	 _mpu9250.calibrateMPU9250(_ximuCTX.gyrBias, _ximuCTX.accBias);
	 */
	_DBG_DRV_XIMU("\n%s", dumpCALintertial("     RESULT  ").c_str());
	_DBG_DRV_XIMU("\n     ------------------------------------------------------");
}
//---------------------------------------------------------------------
/**
 * Calibrate magnetometer, computes bias & scale
 *
 * Duration has been increase because first data was false at 100 Hz (min,max)
 */
void H_AccGyrMag::calibrate_MAGNETO(const int16_t samples){
	_DBG_DRV_XIMU("\n     ------------------------------------------------------");
	_DBG_DRV_XIMU("\n%s", dumpCALcompass("     CURRENT ").c_str());
	_DBG_DRV_XIMU("\n     Move the device in a figure-of-eight motion until you are done");
	_DBG_DRV_XIMU("\n     starts in 5s (during 15s) !!!");
	delay(5000);
	int16_t mag_min[3];
	int16_t mag_max[3];
//
// if there is an errror, magcalMPU9250 return without finishing (bias & scale not modified)
//
#ifdef DEF_WEBGL
	if (!calMagDialog.calMag_START(&browserReady, _mpu9250.magcalSampleCount(samples)))
		return calMagDialog.calMag_STOP(&browserReady);
	if (!calMagDialog.openWrite())
		return calMagDialog.calMag_STOP(&browserReady);
	using namespace std::placeholders;
	_mpu9250.magcalMPU9250(
		_ximuCTX.magBias, _ximuCTX.magScale, mag_min, mag_max, calMagDialog._expectedPoints,
		std::bind(&c_calMagDialog::calMag_CHECKIN, std::ref(calMagDialog), _1, _2, _3, _4, _5, _6),
		&browserReady);
	if (!calMagDialog._status) {
		_DBG_CALMAG("\nH_AccGyrMag::calibrate_MAGNETO ABORT & EXIT !!!!");
		return calMagDialog.calMag_STOP(&browserReady);
	}
	calMagDialog.close();
	_DBG_CALMAG("\nWaiting for the last point updated");
	while (!browserReady) {
		_DBG_CALMAG("-");
		delay(5);
	}
#else
	_mpu9250.magcalMPU9250(_ximuCTX.magBias, _ximuCTX.magScale, mag_min, mag_max);
#endif
	_DBG_DRV_XIMU("\n     ------------------------------------------------------");
	_DBG_DRV_XIMU("\n     MAG calibration done !!!");
	_DBG_DRV_XIMU("\n\trawData   Min    XYZ-axis(%6i,%6i,%6i)",
		mag_min[0], mag_min[1], mag_min[2]);
	_DBG_DRV_XIMU("\n\trawData   Max    XYZ-axis(%6i,%6i,%6i)",
		mag_max[0], mag_max[1], mag_max[2]);
	_DBG_DRV_XIMU("\n%s", dumpCALcompass("     RESULT  ").c_str());
	_DBG_DRV_XIMU("\n     ------------------------------------------------------");
#ifdef DEF_WEBGL
	if (!calMagDialog.calMag_SHIFT(&browserReady, &mag_min[0], &mag_max[0])) {
		return calMagDialog.calMag_STOP(&browserReady);
	}
	_DBG_CALMAG("\nWaiting for the SHIFT points updated");
	while (!browserReady) {
		_DBG_CALMAG("+");
		delay(5);
	}
	if ((calMagDialog.openRead())
		&& (calMagDialog._processedPoints == calMagDialog._expectedPoints)) {
		_DBG_CALMAG("\n$$$$ OK, begin verifying for %i pts", calMagDialog._processedPoints);
		calMagDialog._processedPoints = 0;
		while (calMagDialog.calMag_CHECKOUT(&browserReady, *this))
			;
	}
	calMagDialog.calMag_STOP(&browserReady);
#endif
}
//---------------------------------------------------------------------
e_hasMsg H_AccGyrMag::auto_MSG(const uint64_t DTMS, JsonObject &KMD, uint64_t *_milliLastTS){
#ifdef DEF_WEBGL
	e_hasMsg hasMsg = e_hasMsg::Full;
	switch(calMagDialog._OP){
		case c_calMagDialog::e_OP::STOP:
			KMD["MSG"] = calMagDialog._Msg;
			break;
		case c_calMagDialog::e_OP::START:
			KMD["MSG"] = calMagDialog._Msg;
			KMD["CNT"] = calMagDialog._expectedPoints;
			break;
		case c_calMagDialog::e_OP::CHECKIN:
			KMD[DEF_KF_Imx] = calMagDialog._rawX;
			KMD[DEF_KF_Imy] = calMagDialog._rawY;
			KMD[DEF_KF_Imz] = calMagDialog._rawZ;
			break;
		case c_calMagDialog::e_OP::SHIFT:
			KMD[DEF_KF_Imx] = -calMagDialog._dX;
			KMD[DEF_KF_Imy] = -calMagDialog._dY;
			KMD[DEF_KF_Imz] = -calMagDialog._dZ;
			break;
		case c_calMagDialog::e_OP::CHECKOUT:
			KMD[DEF_KF_Imx] = calMagDialog._calX;
			KMD[DEF_KF_Imy] = calMagDialog._calY;
			KMD[DEF_KF_Imz] = calMagDialog._calZ;
			break;
		default:
			hasMsg = e_hasMsg::No;
			break;
	}
	if (hasMsg != e_hasMsg::No) {
		*_milliLastTS = milli_TS(); // Avoid the following Time Out message
		KMD["DT"] = DTMS;
		KMD["IDX"] = calMagDialog._processedPoints;
		KMD["OP"] = calMagDialog.c_OP[calMagDialog._OP];
		calMagDialog._OP = c_calMagDialog::e_OP::NOP;
		_DBG_CALMAG("\n calMagDialog message [%s]", dumpJSON().c_str()); // just to debug
	}
	return hasMsg;
#endif
	return e_hasMsg::No;
}
//---------------------------------------------------------------------
void H_AccGyrMag::init_IMU(const bool CALIB_ACCGYR, const bool CALIB_MAGNET, const int16_t samples){
//saveIMU();
//loadIMU();
// Read the WHO_AM_I register, this is a good test of communication
//_SERIAL_0("\n\tI²C busy: %s", SBOOL(Wire.busy()));
	delay(100);
	_SERIAL_0(Lang.CST(I2C_CLOCK), Wire.getClock());
	delay(50);
	_mpu9250.resetMPU9250(); // start by resetting MPU9250
	_DBG_DRV_XIMU("\n     Init MPU 9-axis motion sensor ===");
	uint8_t c = _mpu9250.getMPU9250ID();
	try {
		if (c != 0x71)
			THROWERR(IMU_I2CERR, c);
	} catch (EXCEPT const &e) {
		FIX_ERROR();
	}
	_DBG_DRV_XIMU("\n     Imu9250 is online at i²c %#x", c);

	/* Configure the MPU9250 */
// get sensor resolutions, only need to do this once
	aRes = _mpu9250.getAres(Ascale);
	gRes = _mpu9250.getGres(Gscale);
	mRes = _mpu9250.getMres(Mscale);
	_DBG_DRV_XIMU("\n\tMPU9250 resolution Acc-Gyr-Mag(%12.8f,%12.8f,%12.8f) per LSB",
		aRes, gRes, mRes);

	_DBG_DRV_XIMU("\n     Imu9250 self test results %% trim within of factory value:");
	_mpu9250.SelfTest(SelfTest); // Start by performing self test and reporting values
	_DBG_DRV_XIMU("\n\tImu9250 %% deviation Acc xyz-axis (%12.6f%%,%12.6f%%,%12.6f%%)",
		SelfTest[0], SelfTest[1], SelfTest[2]);
	_DBG_DRV_XIMU("\n\tImu9250 %% deviation Gyr xyz-axis (%12.6f%%,%12.6f%%,%12.6f%%)",
		SelfTest[3], SelfTest[4], SelfTest[5]);

	if (CALIB_ACCGYR) { // Calibrate or use pre-measured, pre-stored offset biases
		calibrate_ACCGYR();
	}

// Initialize device for active mode read of acclerometer, gyroscope, and temperature
	_mpu9250.initMPU9250(Ascale, Gscale, sampleRate);
	_DBG_DRV_XIMU("\n\tMPU9250 initialized for active data mode....");

// Read the WHO_AM_I register of the magnetometer, this is a good test of communication
	uint8_t d = _mpu9250.getAK8963CID(); // Read WHO_AM_I register for AK8963
	_DBG_DRV_XIMU("\n     AK_8963 is online at i²c %#x", d);
	delay(1000);

// Get magnetometer calibration from AK8963 ROM
// Initialize device for active mode read of magnetometer
	_mpu9250.initAK8963Slave(Mscale, Mmode, magCalibration);
	_DBG_DRV_XIMU("\n\tAK_8963 initialized for active data %s bits at %s Hz",
		Mscale ? "16" : "14", (Mmode == 6) ? "100" : "8");

	if (CALIB_MAGNET) { // Calibrate or use pre-measured, pre-stored offset biases
		calibrate_MAGNETO(samples);
	}
	/*
	 _DBG_DRV_XIMU("\n\tMAG   scales XYZ-axis(%6.3f,%6.3f,%6.3f) mG",
	 _ximuCTX.magScale[0], _ximuCTX.magScale[1], _ximuCTX.magScale[2]);
	 _DBG_DRV_XIMU(
	 "\n\tMAG sensitivity XYZ-axis(%6.3f,%6.3f,%6.3f) calibration adjustment value",
	 magCalibration[0], magCalibration[1], magCalibration[2]);*/
	_DBG_DRV_XIMU("\n%s\n%s",
		dumpCALintertial("     RUNNING ").c_str(),
		dumpCALcompass("     RUNNING ").c_str());
	delay(2000); // add delay to see results before serial spew of data

//			attachInterrupt(intPin, myinthandler, RISING); // define interrupt for intPin output of MPU9250
}
//---------------------------------------------------------------------
H_AccGyrMag::H_AccGyrMag(const s_IMU IMU_CFG, s_ximuCTX *CONTEXT, const String NAME,
	int8_t *DBGMAX, const bool ON)
:
	H_Module(NAME.c_str(), DBGMAX, ON), _ximuCTX(*CONTEXT)
{
	_LMD = IMU_CFG.LMD;
	sampleRate = IMU_CFG.SAMPLERATE;
	init_IMU();
}
//---------------------------------------------------------------------
void H_AccGyrMag::get_IMU(const uint64_t PREVMICROTS, s_kineFusensor &FSI){
// 100 loops cost is about 1 ms !!!!!!!!!
	/*
	 lastUpdate = _micro TS;
	 for (uint8_t i = 0; i < 50; i++) { // iterate a fixed number of times per data read cycle
	 Now = micro_TS();
	 deltat = ((Now - lastUpdate) / 1000000.0f); // set integration time by time elapsed since last filter update
	 lastUpdate = Now;

	 sum += deltat; // sum for averaging filter update rate
	 sumCount++;
	 //MadgwickEXE(-ax, +ay, +az, gx * pi / 180.0f, -gy * pi / 180.0f, -gz * pi / 180.0f, my, -mx, mz);
	 //MadgwickEXE(+ax, +ay, +az,	 +gx * pi / 180.0f, +gy * pi / 180.0f, +gz * pi / 180.0f, +mx, +my, +mz);
	 //MahonyQuaternionUpdate(-ax, +ay, +az, gx * pi / 180.0f, -gy * pi / 180.0f,-gz * pi / 180.0f, my, -mx, mz);
	 }
	 */
	float pi_180 = pi / 180.0f;
	deltat = ((_microTS - PREVMICROTS) / 1000000.0f / _ximuCTX.cntInt); // set integration time by time elapsed since last filter update
	for (uint8_t i = 0; i < _ximuCTX.cntInt; i++) { // iterate a fixed number of times per data read cycle
		sum += deltat; // sum for averaging filter update rate
		sumCount++;
		//MadgwickEXE(-ax, +ay, +az, gx * pi / 180.0f, -gy * pi / 180.0f, -gz * pi / 180.0f, my, -mx, mz);
		// 2ième calcul
		MadgwickEXE(+ax, +ay, +az, +gx * pi_180, +gy * pi_180, +gz * pi_180, +mx, +my, +mz);
		// Premier calcul
		//MadgwickEXE(+ax, +ay, +az, +gx * pi / 180, +gy * pi / 180, +gz * pi / 180, +mx, +my, +mz);
		// Premier calcul
		//MadgwickEXE(+ax, +ay, +az, +gx * pi / 180.0f, +gy * pi / 180.0f, +gz * pi / 180.0f, +mx, +my, +mz);
		// Tentative NED
		//MadgwickEXE(-ax, +ay, +az,	+gx * pi / 180.0f, -gy * pi / 180.0f, -gz * pi / 180.0f,+my, -mx, +mz);
		//MahonyQuaternionUpdate(-ax, +ay, +az, gx * pi / 180.0f, -gy * pi / 180.0f,-gz * pi / 180.0f, my, -mx, mz);
	}
	FSI.ax = ax;
	FSI.ay = ay;
	FSI.az = az;
	FSI.gx = gx;
	FSI.gy = gy;
	FSI.gz = gz;
	FSI.mx = mx;
	FSI.my = my;
	FSI.mz = mz;
	FSI.q[0] = q[0];
	FSI.q[1] = q[1];
	FSI.q[2] = q[2];
	FSI.q[3] = q[3];
	FSI.temp = temperature;
	FSI.microTS = _microTS;
}
//---------------------------------------------------------------------
float invSqrt(const float VALUE){
	return 1.0 / sqrtf(VALUE);
}
/**
 *
 */
// Implementation of Sebastian Madgwick's "...efficient orientation filter for... inertial/magnetic sensor arrays"
// (see http://www.x-io.co.uk/category/open-source/ for examples and more details)
// which fuses acceleration, rotation rate, and magnetic moments to produce a quaternion-based estimate of absolute
// device orientation -- which can be converted to yaw, pitch, and roll. Useful for stabilizing quadcopters, etc.
// The performance of the orientation filter is at least as good as conventional Kalman-based filtering algorithms
// but is much less computationally intensive---it can be performed on a 3.3 V Pro Mini operating at 8 MHz!
__attribute__((optimize("O3"))) void H_AccGyrMag::MadgwickEXE(
	float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float mz){
//float beta = 0.1;// 20 secondes pour converger si on reset nord à 90°
//float beta = 0.5;//  5 secondes pour converger si on reset nord à 90°
	float beta = _ximuCTX.beta; //  secondes pour converger si on reset nord à 90°
	float invSampleFreq = deltat; // 0.01;
	bool anglesComputed = false;
	float q0 = q[0], q1 = q[1], q2 = q[2], q3 = q[3];
	float recipNorm;
	float s0, s1, s2, s3;
	float qDot1, qDot2, qDot3, qDot4;
	float hx, hy;
	float _2q0mx, _2q0my, _2q0mz, _2q1mx, _2bx, _2bz, _4bx, _4bz, _2q0, _2q1, _2q2, _2q3,
		_2q0q2,
		_2q2q3, q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;
	/*
	 // Use IMU algorithm if magnetometer measurement invalid (avoids NaN in magnetometer normalisation)
	 if ((mx == 0.0f) && (my == 0.0f) && (mz == 0.0f)) {
	 updateIMU(gx, gy, gz, ax, ay, az);
	 return;
	 }

	 // Convert gyroscope degrees/sec to radians/sec
	 gx *= 0.0174533f;
	 gy *= 0.0174533f;
	 gz *= 0.0174533f;
	 */
// Rate of change of quaternion from gyroscope
	qDot1 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz);
	qDot2 = 0.5f * (q0 * gx + q2 * gz - q3 * gy);
	qDot3 = 0.5f * (q0 * gy - q1 * gz + q3 * gx);
	qDot4 = 0.5f * (q0 * gz + q1 * gy - q2 * gx);

// Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
	if (!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

		// Normalise accelerometer measurement
		recipNorm = invSqrt(ax * ax + ay * ay + az * az);
		ax *= recipNorm;
		ay *= recipNorm;
		az *= recipNorm;

		// Normalise magnetometer measurement
		recipNorm = invSqrt(mx * mx + my * my + mz * mz);
		mx *= recipNorm;
		my *= recipNorm;
		mz *= recipNorm;

		// Auxiliary variables to avoid repeated arithmetic
		_2q0mx = 2.0f * q0 * mx;
		_2q0my = 2.0f * q0 * my;
		_2q0mz = 2.0f * q0 * mz;
		_2q1mx = 2.0f * q1 * mx;
		_2q0 = 2.0f * q0;
		_2q1 = 2.0f * q1;
		_2q2 = 2.0f * q2;
		_2q3 = 2.0f * q3;
		_2q0q2 = 2.0f * q0 * q2;
		_2q2q3 = 2.0f * q2 * q3;
		q0q0 = q0 * q0;
		q0q1 = q0 * q1;
		q0q2 = q0 * q2;
		q0q3 = q0 * q3;
		q1q1 = q1 * q1;
		q1q2 = q1 * q2;
		q1q3 = q1 * q3;
		q2q2 = q2 * q2;
		q2q3 = q2 * q3;
		q3q3 = q3 * q3;

		// Reference direction of Earth's magnetic field
		hx = mx * q0q0 - _2q0my * q3 + _2q0mz * q2 + mx * q1q1 + _2q1 * my * q2 + _2q1 * mz * q3
			- mx * q2q2 - mx * q3q3;
		hy = _2q0mx * q3 + my * q0q0 - _2q0mz * q1 + _2q1mx * q2 - my * q1q1 + my * q2q2
			+ _2q2 * mz * q3 - my * q3q3;
		_2bx = sqrtf(hx * hx + hy * hy);
		_2bz = -_2q0mx * q2 + _2q0my * q1 + mz * q0q0 + _2q1mx * q3 - mz * q1q1 + _2q2 * my * q3
			- mz * q2q2 + mz * q3q3;
		_4bx = 2.0f * _2bx;
		_4bz = 2.0f * _2bz;

		// Gradient decent algorithm corrective step
		s0 = -_2q2 * (2.0f * q1q3 - _2q0q2 - ax) + _2q1 * (2.0f * q0q1 + _2q2q3 - ay)
			- _2bz * q2 * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx)
			+ (-_2bx * q3 + _2bz * q1) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my)
			+ _2bx * q2 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
		s1 = _2q3 * (2.0f * q1q3 - _2q0q2 - ax) + _2q0 * (2.0f * q0q1 + _2q2q3 - ay)
			- 4.0f * q1 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - az)
			+ _2bz * q3 * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx)
			+ (_2bx * q2 + _2bz * q0) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my)
			+ (_2bx * q3 - _4bz * q1)
				* (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
		s2 = -_2q0 * (2.0f * q1q3 - _2q0q2 - ax) + _2q3 * (2.0f * q0q1 + _2q2q3 - ay)
			- 4.0f * q2 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - az)
			+ (-_4bx * q2 - _2bz * q0)
				* (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx)
			+ (_2bx * q1 + _2bz * q3) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my)
			+ (_2bx * q0 - _4bz * q2)
				* (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
		s3 = _2q1 * (2.0f * q1q3 - _2q0q2 - ax) + _2q2 * (2.0f * q0q1 + _2q2q3 - ay)
			+ (-_4bx * q3 + _2bz * q1)
				* (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx)
			+ (-_2bx * q0 + _2bz * q2) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my)
			+ _2bx * q1 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
		recipNorm = invSqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
		s0 *= recipNorm;
		s1 *= recipNorm;
		s2 *= recipNorm;
		s3 *= recipNorm;

		// Apply feedback step
		qDot1 -= beta * s0;
		qDot2 -= beta * s1;
		qDot3 -= beta * s2;
		qDot4 -= beta * s3;
	}

// Integrate rate of change of quaternion to yield quaternion
	q0 += qDot1 * invSampleFreq;
	q1 += qDot2 * invSampleFreq;
	q2 += qDot3 * invSampleFreq;
	q3 += qDot4 * invSampleFreq;

// Normalise quaternion
	recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	q0 *= recipNorm;
	q1 *= recipNorm;
	q2 *= recipNorm;
	q3 *= recipNorm;
	anglesComputed = 0;
	q[0] = q0;
	q[1] = q1;
	q[2] = q2;
	q[3] = q3;
} //---------------------------------------------------------------------
void H_AccGyrMag::Clear_IMU_quaternion(){
	q[0] = 1.0;
	q[1] = 0.0;
	q[2] = 0.0;
	q[3] = 0.0;
}
//-------------------------------------------------------
/**
 * http://arduinolearning.com/code/arduino-mpu-9250-example.php
 * \fn void H_AccGyrMag::acq_simuIMU(imuDsp &SIMIMU)
 * \brief Gets I²C IMU data (400 kHz)
 * \param imuDsp &SIMIMU signal processor @ref imuDsp to simulate IMU
 */

void H_AccGyrMag::acq_simuIMU(imuDsp &SIMIMU){
	s_imuDsp data;
	SIMIMU.next(data); // ok for first sample also
	ax = data.AX;
	ay = data.AY;
	az = data.AZ;
	gx = data.GX;
	gy = data.GY;
	gz = data.GZ;
	mx = data.MX;
	my = data.MY;
	mz = data.MZ;
	temperature = 20;
	_microTS = data.MTS;
//_SERIAL_5("\n# Simu %6.2f ms n°%i", (data.MTS - _prev MicroTS) / 1000.0, data.SAMPLE);
}
//-------------------------------------------------------
/**
 * @fn void H_AccGyrMag::adjust_Bias()
 * @brief Adjusts Acc & Gyr bias to obtain on an horizontal plane ACCxyz=(0,0,1) & GYRxyz=(0,0,0)
 */
void H_AccGyrMag::adjust_Bias(){

	const size_t bloop = 1000; // ie 10 sec at 100 Hz
	static double sumAcc_X, sumAcc_Y, sumAcc_Z;// double to sum about 1000 samples
	static double sumGyr_X, sumGyr_Y, sumGyr_Z;
	static size_t cnt = 0;
	if (_ximuCTX.adjust_Bias) {
		//init
		if (!cnt) {
			_SERIAL_0("\n%s",dumpCALintertial("CURRENT").c_str());
			_SERIAL_0("\nADJUSTING BIAS STARTS");
			sumAcc_X = 0.0;
			sumAcc_Y = 0.0;
			sumAcc_Z = 0.0;
			sumGyr_X = 0.0;
			sumGyr_Y = 0.0;
			sumGyr_Z = 0.0;
		}
// sum
		sumAcc_X += ax;
		sumAcc_Y += ay;
		sumAcc_Z += az -1.0;// !!!
		sumGyr_X += gx;
		sumGyr_Y += gy;
		sumGyr_Z += gz;
		cnt++;
		//  average
		if (cnt == bloop) {
			_ximuCTX.accBias[0] += sumAcc_X / bloop;
			_ximuCTX.accBias[1] += sumAcc_Y / bloop;
			_ximuCTX.accBias[2] += sumAcc_Z / bloop;
			_ximuCTX.gyrBias[0] += sumGyr_X / bloop;
			_ximuCTX.gyrBias[1] += sumGyr_Y / bloop;
			_ximuCTX.gyrBias[2] += sumGyr_Z / bloop;
			_ximuCTX.adjust_Bias = false;
			_SERIAL_0("\nADJUSTING BIAS ENDS");
			_SERIAL_0("\n%s",dumpCALintertial("ADJUSTED").c_str());
			cnt = 0;
		}
	}
}
//-------------------------------------------------------
/**
 * http://arduinolearning.com/code/arduino-mpu-9250-example.php
 * \fn void H_AccGyrMag::acq_trueIMU(const uint64_t MICROTS)
 * \brief Gets I²C (clock 400 kHz) IMU data every 10ms (ISR 100 Hz)
 * \param const uint64_t MICROTS esp time of interrupt in µicro seconds
 */
void H_AccGyrMag::acq_trueIMU(const uint64_t MICROTS){
	static uint32_t SAMPLE = 0;
// If intPin goes high, either all data registers have new data
// On interrupt, read data
//intFlag = false; // reset newData flag
//	uint16_t ii, packet_count, fifo_count;
//	fifo_count = _mpu9250.getFIFOCount();mRes
////	vTaskSuspendAll();
	_mpu9250.readMPU9250Data(MPU9250Data); // INT cleared on any read
////	xTaskResumeAll();
		// Calculate the accleration value into actual g's
	ax = (float)MPU9250Data[0] * aRes - _ximuCTX.accBias[0]; // get actual g value, this depends on scale being set
	ay = (float)MPU9250Data[1] * aRes - _ximuCTX.accBias[1];
	az = (float)MPU9250Data[2] * aRes - _ximuCTX.accBias[2];
	temperature = ((float)MPU9250Data[3]) / 333.87f + 21.0f; // Gyro chip temperature in degrees Centigrade
//ax = 0;		ay = 0;		az = 1;
// Calculate the gyro value into actual degrees per second
	gx = (float)MPU9250Data[4] * gRes - _ximuCTX.gyrBias[0]; // get actual gyro value, this depends on scale being set
	gy = (float)MPU9250Data[5] * gRes - _ximuCTX.gyrBias[1];
	gz = (float)MPU9250Data[6] * gRes - _ximuCTX.gyrBias[2];

	if (_ximuCTX.adjust_Bias)
		adjust_Bias();
//gx = 0;		gy = 0;		gz = 0;
//    if( _mpu9250.checkNewMagData() == true) { // wait for magnetometer data ready bit to be set
	_mpu9250.readMagData(magCount); // Read the x/y/z adc values
	calMagData(magCount, mx, my, mz); // Calculate the compass values in milliGauss
// Tag timing
	_microTS = MICROTS;
// Adapt to rover local frame....
	/*
	 ax = ax;
	 ay = ay;
	 az = az;


	 gx = gx;
	 gy = gy;
	 gz = gz;
	 */
	float mm = mx;
	mx = my;
	my = mm;
	mz = -mz;

//	_SERIAL_5( "\n#### IMU <I2C %+10.3f ms, FIFO=%i", (MICROTS - oldMTS) / 1000.0, fifo_count);
	SAMPLE++;
}
//-------------------------------------------------------
/**
 * @fn void H_AccGyrMag::calMagData(int16_t *rawMag,float & mx,float & my,float & mz)
 * @brief Computes calibrated value from magneto raw data
 * @param rawMag array of 3 int16_t raw mag data
 * @param mx return mx
 * @param my return my
 * @param mz return mz
 */
void H_AccGyrMag::calMagData(int16_t *rawMag, float &mx, float &my, float &mz){
// Include factory calibration per data sheet and user environmental corrections
// get actual magnetometer value, this depends on scale being set
	mx = (float)rawMag[0] * mRes * magCalibration[0] - _ximuCTX.magBias[0];
	my = (float)rawMag[1] * mRes * magCalibration[1] - _ximuCTX.magBias[1];
	mz = (float)rawMag[2] * mRes * magCalibration[2] - _ximuCTX.magBias[2];
	mx *= _ximuCTX.magScale[0];
	my *= _ximuCTX.magScale[1];
	mz *= _ximuCTX.magScale[2];
}
//-------------------------------------------------------
__attribute__((optimize("O3"))) void H_AccGyrMag::MadgwickEXE2(
	float ax, float ay, float az,
	float gx, float gy, float gz,
	float mx, float my, float mz){
	float q1 = q[0], q2 = q[1], q3 = q[2], q4 = q[3]; // short name local variable for readability
	float norm;
	float hx, hy, _2bx, _2bz;
	float s1, s2, s3, s4;
	float qDot1, qDot2, qDot3, qDot4;

// Auxiliary variables to avoid repeated arithmetic
	float _2q1mx;
	float _2q1my;
	float _2q1mz;
	float _2q2mx;
	float _4bx;
	float _4bz;
	float _2q1 = 2.0f * q1;
	float _2q2 = 2.0f * q2;
	float _2q3 = 2.0f * q3;
	float _2q4 = 2.0f * q4;
	float _2q1q3 = 2.0f * q1 * q3;
	float _2q3q4 = 2.0f * q3 * q4;
	float q1q1 = q1 * q1;
	float q1q2 = q1 * q2;
	float q1q3 = q1 * q3;
	float q1q4 = q1 * q4;
	float q2q2 = q2 * q2;
	float q2q3 = q2 * q3;
	float q2q4 = q2 * q4;
	float q3q3 = q3 * q3;
	float q3q4 = q3 * q4;
	float q4q4 = q4 * q4;

// Normalise accelerometer measurement
	norm = sqrtf(ax * ax + ay * ay + az * az);
	if (norm == 0.0f)
		return; // handle NaN
	norm = 1.0f / norm;
	ax *= norm;
	ay *= norm;
	az *= norm;

// Normalise magnetometer measurement
	norm = sqrtf(mx * mx + my * my + mz * mz);
	if (norm == 0.0f)
		return; // handle NaN
	norm = 1.0f / norm;
	mx *= norm;
	my *= norm;
	mz *= norm;

// Reference direction of Earth's magnetic field
	_2q1mx = 2.0f * q1 * mx;
	_2q1my = 2.0f * q1 * my;
	_2q1mz = 2.0f * q1 * mz;
	_2q2mx = 2.0f * q2 * mx;
	hx = mx * q1q1 - _2q1my * q4 + _2q1mz * q3 + mx * q2q2 + _2q2 * my * q3
		+ _2q2 * mz * q4 - mx * q3q3 - mx * q4q4;
	hy = _2q1mx * q4 + my * q1q1 - _2q1mz * q2 + _2q2mx * q3 - my * q2q2 + my * q3q3
		+ _2q3 * mz * q4 - my * q4q4;
	_2bx = sqrtf(hx * hx + hy * hy);
	_2bz = -_2q1mx * q3 + _2q1my * q2 + mz * q1q1 + _2q2mx * q4 - mz * q2q2
		+ _2q3 * my * q4 - mz * q3q3 + mz * q4q4;
	_4bx = 2.0f * _2bx;
	_4bz = 2.0f * _2bz;

// Gradient decent algorithm corrective step
	s1 = -_2q3 * (2.0f * q2q4 - _2q1q3 - ax) + _2q2 * (2.0f * q1q2 + _2q3q4 - ay)
		- _2bz * q3 * (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx)
		+ (-_2bx * q4 + _2bz * q2) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my)
		+ _2bx * q3 * (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mz);

	s2 = _2q4 * (2.0f * q2q4 - _2q1q3 - ax) + _2q1 * (2.0f * q1q2 + _2q3q4 - ay)
		- 4.0f * q2 * (1.0f - 2.0f * q2q2 - 2.0f * q3q3 - az)
		+ _2bz * q4 * (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx)
		+ (_2bx * q3 + _2bz * q1) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my)
		+ (_2bx * q4 - _4bz * q2) * (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mz);

	s3 = -_2q1 * (2.0f * q2q4 - _2q1q3 - ax) + _2q4 * (2.0f * q1q2 + _2q3q4 - ay)
		- 4.0f * q3 * (1.0f - 2.0f * q2q2 - 2.0f * q3q3 - az)
		+ (-_4bx * q3 - _2bz * q1)
			* (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx)
		+ (_2bx * q2 + _2bz * q4) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my)
		+ (_2bx * q1 - _4bz * q3) * (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mz);

	s4 = _2q2 * (2.0f * q2q4 - _2q1q3 - ax) + _2q3 * (2.0f * q1q2 + _2q3q4 - ay)
		+ (-_4bx * q4 + _2bz * q2) * (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx)
		+ (-_2bx * q1 + _2bz * q3) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my)
		+ _2bx * q2 * (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mz);

	norm = sqrtf(s1 * s1 + s2 * s2 + s3 * s3 + s4 * s4); // normalise step magnitude
	norm = 1.0f / norm;
	s1 *= norm;
	s2 *= norm;
	s3 *= norm;
	s4 *= norm;

// Compute rate of change of quaternion
	qDot1 = 0.5f * (-q2 * gx - q3 * gy - q4 * gz) - _ximuCTX.beta * s1;
	qDot2 = 0.5f * (+q1 * gx + q3 * gz - q4 * gy) - _ximuCTX.beta * s2;
	qDot3 = 0.5f * (+q1 * gy - q2 * gz + q4 * gx) - _ximuCTX.beta * s3;
	qDot4 = 0.5f * (+q1 * gz + q2 * gy - q3 * gx) - _ximuCTX.beta * s4;

// Integrate to yield quaternion
	q1 += qDot1 * deltat;
	q2 += qDot2 * deltat;
	q3 += qDot3 * deltat;
	q4 += qDot4 * deltat;
// normalise quaternion
	norm = sqrtf(q1 * q1 + q2 * q2 + q3 * q3 + q4 * q4);
	norm = 1.0f / norm;
	q[0] = q1 * norm;
	q[1] = q2 * norm;
	q[2] = q3 * norm;
	q[3] = q4 * norm;

}

// Similar to Madgwick scheme but uses proportional and integral filtering on the error between estimated reference vectors and
// measured ones.
void H_AccGyrMag::MahonyQuaternionUpdate(float ax, float ay, float az, float gx,
	float gy,
	float gz,
	float mx, float my, float mz)
	{
	float q1 = q[0], q2 = q[1], q3 = q[2], q4 = q[3]; // short name local variable for readability
	float norm;
	float hx, hy, bx, bz;
	float vx, vy, vz, wx, wy, wz;
	float ex, ey, ez;
	float pa, pb, pc;

// Auxiliary variables to avoid repeated arithmetic
	float q1q1 = q1 * q1;
	float q1q2 = q1 * q2;
	float q1q3 = q1 * q3;
	float q1q4 = q1 * q4;
	float q2q2 = q2 * q2;
	float q2q3 = q2 * q3;
	float q2q4 = q2 * q4;
	float q3q3 = q3 * q3;
	float q3q4 = q3 * q4;
	float q4q4 = q4 * q4;

// Normalise accelerometer measurement
	norm = sqrtf(ax * ax + ay * ay + az * az);
	if (norm == 0.0f)
		return; // handle NaN
	norm = 1.0f / norm; // use reciprocal for division
	ax *= norm;
	ay *= norm;
	az *= norm;

// Normalise magnetometer measurement
	norm = sqrtf(mx * mx + my * my + mz * mz);
	if (norm == 0.0f)
		return; // handle NaN
	norm = 1.0f / norm; // use reciprocal for division
	mx *= norm;
	my *= norm;
	mz *= norm;

// Reference direction of Earth's magnetic field
	hx = 2.0f * mx * (0.5f - q3q3 - q4q4) + 2.0f * my * (q2q3 - q1q4)
		+ 2.0f * mz * (q2q4 + q1q3);
	hy = 2.0f * mx * (q2q3 + q1q4) + 2.0f * my * (0.5f - q2q2 - q4q4)
		+ 2.0f * mz * (q3q4 - q1q2);
	bx = sqrtf((hx * hx) + (hy * hy));
	bz = 2.0f * mx * (q2q4 - q1q3) + 2.0f * my * (q3q4 + q1q2)
		+ 2.0f * mz * (0.5f - q2q2 - q3q3);

// Estimated direction of gravity and magnetic field
	vx = 2.0f * (q2q4 - q1q3);
	vy = 2.0f * (q1q2 + q3q4);
	vz = q1q1 - q2q2 - q3q3 + q4q4;
	wx = 2.0f * bx * (0.5f - q3q3 - q4q4) + 2.0f * bz * (q2q4 - q1q3);
	wy = 2.0f * bx * (q2q3 - q1q4) + 2.0f * bz * (q1q2 + q3q4);
	wz = 2.0f * bx * (q1q3 + q2q4) + 2.0f * bz * (0.5f - q2q2 - q3q3);

// Error is cross product between estimated direction and measured direction of gravity
	ex = (ay * vz - az * vy) + (my * wz - mz * wy);
	ey = (az * vx - ax * vz) + (mz * wx - mx * wz);
	ez = (ax * vy - ay * vx) + (mx * wy - my * wx);
	if (Ki > 0.0f)
		{
		eInt[0] += ex; // accumulate integral error
		eInt[1] += ey;
		eInt[2] += ez;
	}
	else
	{
		eInt[0] = 0.0f; // prevent integral wind up
		eInt[1] = 0.0f;
		eInt[2] = 0.0f;
	}

// Apply feedback terms
	gx = gx + Kp * ex + Ki * eInt[0];
	gy = gy + Kp * ey + Ki * eInt[1];
	gz = gz + Kp * ez + Ki * eInt[2];

// Integrate rate of change of quaternion
	pa = q2;
	pb = q3;
	pc = q4;
	q1 = q1 + (-q2 * gx - q3 * gy - q4 * gz) * (0.5f * deltat);
	q2 = pa + (q1 * gx + pb * gz - pc * gy) * (0.5f * deltat);
	q3 = pb + (q1 * gy - pa * gz + pc * gx) * (0.5f * deltat);
	q4 = pc + (q1 * gz + pa * gy - pb * gx) * (0.5f * deltat);

// Normalise quaternion
	norm = sqrtf(q1 * q1 + q2 * q2 + q3 * q3 + q4 * q4);
	norm = 1.0f / norm;
	q[0] = q1 * norm;
	q[1] = q2 * norm;
	q[2] = q3 * norm;
	q[3] = q4 * norm;

}
//-------------------------------------------------------
H_ultraSounds::H_ultraSounds(const String NAME, int8_t *DBGMAX, const bool ON)
:
	H_Module(NAME.c_str(), DBGMAX, ON){
	_SERIAL_4("\n\tusd VERSION '%i'", readByte(SFR02_ADDRESS, CMD_0));
}
//-------------------------------------------------------
/**
 * \fn float H_ultraSounds::Get()
 * \brief Gets I²C USD colliding distance (master 400 kHz, slave 100 kHz) & launches for the next acquisition (>65 ms)
 * \return float distance (meters)
 */
float H_ultraSounds::Get(){
	while (readByte(SFR02_ADDRESS, CMD_0) == 255) {
		_SERIAL_4("\n!!!!!!!!!! usd 255");
	}
	float dist = 0;
	uint16_t d = _sfr02.readRangeCM(0);
	writeByte(SFR02_ADDRESS, CMD_0, REAL_CM);
	dist = d;
	return dist / 100;
}
//---------------------------------------------------------------------
/**
 *
 */
String
H_AccGyrMag::dumpCALintertial(const char *PREFIX){
	char buffer[256];
	int16_t cnt = 0;
	SPrintF(sizeof(buffer), cnt, buffer, "%sINERTIAL config", (PREFIX) ? PREFIX : "");
	SPrintF(sizeof(buffer), cnt, buffer,
		"\n\taccelBias[3]={%11.5f,%11.5f,%11.5f}g   aRes=%12.8f",
		_ximuCTX.accBias[0], _ximuCTX.accBias[1], _ximuCTX.accBias[2], aRes);
	SPrintF(sizeof(buffer), cnt, buffer,
		"\n\tgyroBias[3] ={%11.5f,%11.5f,%11.5f}°/s gRes=%12.8f",
		_ximuCTX.gyrBias[0], _ximuCTX.gyrBias[1], _ximuCTX.gyrBias[2], gRes);
	return String(buffer);
}
//---------------------------------------------------------------------
/**
 *
 */
String
H_AccGyrMag::dumpCALcompass(const char *PREFIX){
	char buffer[256];
	int16_t cnt = 0;
	SPrintF(sizeof(buffer), cnt, buffer, "%sCOMPASS config", (PREFIX) ? PREFIX : "");
	SPrintF(sizeof(buffer), cnt, buffer,
		"\n\tmagBias[3]  ={%11.5f,%11.5f,%11.5f}mG  mRes=%12.8f",
		_ximuCTX.magBias[0], _ximuCTX.magBias[1], _ximuCTX.magBias[2], mRes);
	SPrintF(sizeof(buffer), cnt, buffer, "\n\tmagScale[3] ={%11.5f,%11.5f,%11.5f}",
		_ximuCTX.magScale[0], _ximuCTX.magScale[1], _ximuCTX.magScale[2]);
	SPrintF(sizeof(buffer), cnt, buffer, "\n\tmagCalib[3] ={%11.5f,%11.5f,%11.5f}",
		magCalibration[0], magCalibration[1], magCalibration[2]);
	return String(buffer);
}
//-------------------------------------------------------
/**
 * Loads gyro,accelero & magneto parameters from eeprom
 */
void H_AccGyrMag::loadIMU(){
	extern EEPROMClass E_CALIMU;
	E_CALIMU.get(0, _ximuCTX);
	_SERIAL_3("\nE_CALIMU Loading...\n%s\n%s",
		dumpCALintertial("LOADED ").c_str(),
		dumpCALcompass("LOADED ").c_str());
}
//-------------------------------------------------------
/**
 * Saves gyro,accelero & magneto parameters from eeprom
 */
void H_AccGyrMag::saveIMU(){
	extern EEPROMClass E_CALIMU;
	E_CALIMU.put(0, _ximuCTX);
	E_CALIMU.commit();
	_SERIAL_3("\nE_CALIMU Saving...\n%s\n%s",
		dumpCALintertial("STORED ").c_str(),
		dumpCALcompass("STORED ").c_str());
}
///////////////////////////////////////////////////////////////////////
/*
 * https://www.rs-online.com/designspark/noise-sensor-fusion-and-lost-drones-fr
 */
