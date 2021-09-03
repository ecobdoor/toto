#include "rov-Hard_IMU_000_WS3.h"
#include "000_I2C.h"
#include "esp32_LOGS.h"
using namespace std;
////////////////////////////////////////////////////////////////
H_AccGyrMag::H_AccGyrMag(const s_IMU IMU_CFG, s_ximuCTX *CONTEXT, const String NAME,
	int8_t *DBGMAX, const bool ON)
:
	H_Module(NAME.c_str(), DBGMAX, ON), _ximuCTX(*CONTEXT){
	//saveIMU();
	//loadIMU();
	_DBG_DRV_XIMU("\nIMU9250 calibration:/n%s", dumpCAL().c_str());
	_LMD = IMU_CFG.LMD;
	sampleRate = IMU_CFG.SAMPLERATE;
//	return;

	/*
	 // Set up the interrupt pin, it's set as active high, push-pull
	 pinMode(myLed, OUTPUT);
	 digitalWrite(myLed, HIGH); // start with orange led on (active HIGH)

	 pinMode(intPin, INPUT);

	 pinMode(csPin, OUTPUT);
	 digitalWrite(csPin, HIGH);
	 */
	/* Configure the MPU9250 */
	// Read the WHO_AM_I register, this is a good test of communication
	_DBG_DRV_XIMU("\n\t====================");
	_DBG_DRV_XIMU("\n\tMPU9250 9-axis motion sensor...");
	uint8_t c = _mpu9250.getMPU9250ID();
	_DBG_DRV_XIMU("\n\tMPU9250 I AM %#x (should be 0x71)", c);

	if (c == 0x71) // WHO_AM_I should always be 0x71 for MPU9250, 0x73 for MPU9255
		{
		_DBG_DRV_XIMU("\n\tMPU9250 is online...");

		_mpu9250.resetMPU9250(); // start by resetting MPU9250

		_mpu9250.SelfTest(SelfTest); // Start by performing self test and reporting values
		_DBG_DRV_XIMU("\n\tMPU9250 self test results:");
		_DBG_DRV_XIMU("\n\t--------------------");
		_DBG_DRV_XIMU(
			"\n\tAcceleration xyz-axis (%6.3f,%6.3f,%6.3f) %% trim within of factory value",
			SelfTest[0], SelfTest[1], SelfTest[2]);
		_DBG_DRV_XIMU(
			"\n\tGyration     xyz-axis (%6.3f,%6.3f,%6.3f) %% trim within of factory value",
			SelfTest[3], SelfTest[4], SelfTest[5]);

		// get sensor resolutions, only need to do this once
		aRes = _mpu9250.getAres(Ascale);
		gRes = _mpu9250.getGres(Gscale);
		mRes = _mpu9250.getMres(Mscale);
		_DBG_DRV_XIMU("\n\tSensors resolution Acc Gyr Mag=(%6.3f,%6.3f,%6.3f) per LSB",
			aRes, gRes, mRes);

		// Comment out if using pre-measured, pre-stored offset biases
		//calACCGYR();

		_DBG_DRV_XIMU("\n\tAccel biases xyz-axis (%6.3f%%,%6.3f%%,%6.3f%%) mg",
			1000. * _ximuCTX.accelBias[0], 1000. * _ximuCTX.accelBias[1],
			1000. * _ximuCTX.accelBias[2]);
		_DBG_DRV_XIMU("\n\tGyro  biases xyz-axis(%6.3f,%6.3f,%6.3f) °/s",
			_ximuCTX.gyroBias[0], _ximuCTX.gyroBias[1], _ximuCTX.gyroBias[2]);

		// Initialize device for active mode read of acclerometer, gyroscope, and temperature
		_mpu9250.initMPU9250(Ascale, Gscale, sampleRate);
		_DBG_DRV_XIMU("\n\tMPU9250 initialized for active data mode....");

		_DBG_DRV_XIMU("\n\t--------------------");
		// Read the WHO_AM_I register of the magnetometer, this is a good test of communication
		byte d = _mpu9250.getAK8963CID(); // Read WHO_AM_I register for AK8963
		_DBG_DRV_XIMU("\n\tAK8963  I AM %#x (should be 0x48)", d);
		delay(1000);

		// Get magnetometer calibration from AK8963 ROM
		// Initialize device for active mode read of magnetometer
		_mpu9250.initAK8963Slave(Mscale, Mmode, magCalibration);
		_DBG_DRV_XIMU("\n\tAK8963 initialized for active data mode....");

		// Comment out if using pre-measured, pre-stored offset biases
		//		calMAGNETO();

		_DBG_DRV_XIMU("\n\tMAG   scales XYZ-axis(%6.3f,%6.3f,%6.3f) mG",
			_ximuCTX.magScale[0], _ximuCTX.magScale[1], _ximuCTX.magScale[2]);
		_DBG_DRV_XIMU(
			"\n\tMAG sensitivity XYZ-axis(%6.3f,%6.3f,%6.3f)  adjustment value",
			magCalibration[0], magCalibration[1], magCalibration[2]);
		_DBG_DRV_XIMU("\n\t====================");
		delay(2000); // add delay to see results before serial spew of data

//			attachInterrupt(intPin, myinthandler, RISING); // define interrupt for intPin output of MPU9250
	} else {
		_DEBUG_ERR("Could not connect to MPU9250 at %#x", c);
	}

}
//---------------------------------------------------------------------
void H_AccGyrMag::telemetry(const s_fusensors &FSI){
	if (_ximuCTX.telemetry) {
		if (!_CSV_("@wx:%llu;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f",
			FSI.isrMicroTS,
			FSI.ax, FSI.ay, FSI.az,
			FSI.gx, FSI.gy, FSI.gz,
			FSI.mx, FSI.my, FSI.mz,
			FSI.q[0], FSI.q[1], FSI.q[2], FSI.q[3])
			)
			_ximuCTX.telemetry = false;
	}
}
//---------------------------------------------------------------------
void H_AccGyrMag::Clear_IMU_quaternion(){
	_microTS = 0;
	//microTSoffset)=micro_TS();
	q[0] = 1.0;
	q[1] = 0.0;
	q[2] = 0.0;
	q[3] = 0.0;
}
void H_AccGyrMag::get_IMU(s_fusensors &FSI){
	// 100 loops cost is about 1 ms !!!!!!!!!
	lastUpdate = _microTS;
	for (uint8_t i = 0; i < 100; i++) { // iterate a fixed number of times per data read cycle
		Now = micro_TS();
		deltat = ((Now - lastUpdate) / 1000000.0f); // set integration time by time elapsed since last filter update
		lastUpdate = Now;

		sum += deltat; // sum for averaging filter update rate
		sumCount++;
		/*
		 MadgwickQuaternionUpdate(-ax, +ay, +az, gx * pi / 180.0f, -gy * pi / 180.0f,
		 -gz * pi / 180.0f, my, -mx, mz);
		 */
		MadgwickQuaternionUpdate(+ax, +ay, +az,
			+gx * pi / 180.0f, +gy * pi / 180.0f, +gz * pi / 180.0f,
			+mx, +my, +mz);
		//MahonyQuaternionUpdate(-ax, +ay, +az, gx * pi / 180.0f, -gy * pi / 180.0f,-gz * pi / 180.0f, my, -mx, mz);
	}
	FSI.isrMicroTS = _microTS;
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

}

//-------------------------------------------------------
/**
 * http://arduinolearning.com/code/arduino-mpu-9250-example.php
 * \fn void H_AccGyrMag::acq_simuIMU(imuDsp &SIMIMU)
 * \brief Gets I²C IMU data (400 kHz)
 * \param imuDsp &SIMIMU signal processor @ref imuDsp to simulate IMU
 */

void H_AccGyrMag::acq_simuIMU(imuDsp &SIMIMU){
	static uint64_t oldMTS = 0;
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
	_microTS = data.MTS;
	temperature = 20;
	_SERIAL_5("\n# Simu %6.2f ms n°%i", (data.MTS - oldMTS) / 1000.0, data.SAMPLE);
	oldMTS = data.MTS;
}
//-------------------------------------------------------
/**
 * http://arduinolearning.com/code/arduino-mpu-9250-example.php
 * \fn void H_AccGyrMag::acq_trueIMU(const uint64_t MICROTS)
 * \brief Gets I²C (clock 400 kHz) IMU data every 10ms (ISR 100 Hz)
 * \param const uint64_t MICROTS esp time of interrupt in µicro seconds
 */
void H_AccGyrMag::acq_trueIMU(const uint64_t MICROTS){
	static uint64_t oldMTS = 0;
	static uint32_t SAMPLE = 0;
// If intPin goes high, either all data registers have new data
	// On interrupt, read data
	//intFlag = false; // reset newData flag
//	uint16_t ii, packet_count, fifo_count;
//	fifo_count = _mpu9250.getFIFOCount();
////	vTaskSuspendAll();
	_mpu9250.readMPU9250Data(MPU9250Data); // INT cleared on any read
////	xTaskResumeAll();
	// Now we'll calculate the accleration value into actual g's
	ax = (float)MPU9250Data[0] * aRes - _ximuCTX.accelBias[0]; // get actual g value, this depends on scale being set
	ay = (float)MPU9250Data[1] * aRes - _ximuCTX.accelBias[1];
	az = (float)MPU9250Data[2] * aRes - _ximuCTX.accelBias[2];
	//ax = 0;		ay = 0;		az = 1;
	// Calculate the gyro value into actual degrees per second
	gx = (float)MPU9250Data[4] * gRes - _ximuCTX.gyroBias[0]; // get actual gyro value, this depends on scale being set
	gy = (float)MPU9250Data[5] * gRes - _ximuCTX.gyroBias[1];
	gz = (float)MPU9250Data[6] * gRes - _ximuCTX.gyroBias[2];
	//gx = 0;		gy = 0;		gz = 0;
	//    if( _mpu9250.checkNewMagData() == true) { // wait for magnetometer data ready bit to be set
	_mpu9250.readMagData(magCount); // Read the x/y/z adc values

	// Calculate the magnetometer values in milliGauss
	// Include factory calibration per data sheet and user environmental corrections
	mx = (float)magCount[0] * mRes * magCalibration[0] - _ximuCTX.magBias[0]; // get actual magnetometer value, this depends on scale being set
	my = (float)magCount[1] * mRes * magCalibration[1] - _ximuCTX.magBias[1];
	mz = (float)magCount[2] * mRes * magCalibration[2] - _ximuCTX.magBias[2];
	mx *= _ximuCTX.magScale[0];
	my *= _ximuCTX.magScale[1];
	mz *= _ximuCTX.magScale[2];
	_microTS = MICROTS;

	temperature = ((float)MPU9250Data[3]) / 333.87f + 21.0f; // Gyro chip temperature in degrees Centigrade
	float dt = (MICROTS - oldMTS) / 1000.0;
	if (abs(dt - 10) > 2)/** < @todo detach isr and stop!!! */
		_SERIAL_5("\n# Iimu %6.2fms n°%i", dt, SAMPLE);
	//else 		_SERIAL_5("\n# Iimu %i", SAMPLE);

//	_SERIAL_5( "\n#### IMU <I2C %+10.3f ms, FIFO=%i", (MICROTS - oldMTS) / 1000.0, fifo_count);
	oldMTS = MICROTS;
	SAMPLE++;
}
//---------------------------------------------------------------------
/**
 *
 */
// Implementation of Sebastian Madgwick's "...efficient orientation filter for... inertial/magnetic sensor arrays"
// (see http://www.x-io.co.uk/category/open-source/ for examples and more details)
// which fuses acceleration, rotation rate, and magnetic moments to produce a quaternion-based estimate of absolute
// device orientation -- which can be converted to yaw, pitch, and roll. Useful for stabilizing quadcopters, etc.
// The performance of the orientation filter is at least as good as conventional Kalman-based filtering algorithms
// but is much less computationally intensive---it can be performed on a 3.3 V Pro Mini operating at 8 MHz!
__attribute__((optimize("O3"))) void H_AccGyrMag::MadgwickQuaternionUpdate(
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
		+ (-_2bx * q4 + _2bz * q2)
			* (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my)
		+ _2bx * q3 * (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mz);
	s2 = _2q4 * (2.0f * q2q4 - _2q1q3 - ax) + _2q1 * (2.0f * q1q2 + _2q3q4 - ay)
		- 4.0f * q2 * (1.0f - 2.0f * q2q2 - 2.0f * q3q3 - az)
		+ _2bz * q4 * (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx)
		+ (_2bx * q3 + _2bz * q1) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my)
		+ (_2bx * q4 - _4bz * q2)
			* (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mz);
	s3 = -_2q1 * (2.0f * q2q4 - _2q1q3 - ax) + _2q4 * (2.0f * q1q2 + _2q3q4 - ay)
		- 4.0f * q3 * (1.0f - 2.0f * q2q2 - 2.0f * q3q3 - az)
		+ (-_4bx * q3 - _2bz * q1)
			* (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx)
		+ (_2bx * q2 + _2bz * q4) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my)
		+ (_2bx * q1 - _4bz * q3)
			* (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mz);
	s4 = _2q2 * (2.0f * q2q4 - _2q1q3 - ax) + _2q3 * (2.0f * q1q2 + _2q3q4 - ay)
		+ (-_4bx * q4 + _2bz * q2)
			* (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx)
		+ (-_2bx * q1 + _2bz * q3)
			* (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my)
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
 * @fn String H_AccGyrMag::dumpPush_FUSENSOR(s_fusensors &RTitem)
 * @param RTitem
 * @return text dump
 */
String H_AccGyrMag::dumpPush_FUSENSOR(s_fusensors &RTitem){
	char buffer[512];
	buffer[0] = 0;
	int16_t cnt = 0;
	if (0 < _ximuCTX.dmpLvl_Fusensor) {
		//SPrintF(sizeof(buffer), cnt, buffer, "==== FUSENSOR %llu µs, radar", RTitem.isrMicroTS);
		SPrintF(sizeof(buffer), cnt, buffer, "imu QUAT(%+10.3f%+10.3f%+10.3f%+10.3f) radar",
			RTitem.q[0], RTitem.q[1], RTitem.q[2], RTitem.q[3]);
		if (RTitem.USD)
			SPrintF(sizeof(buffer), cnt, buffer, "=%8.3f m", RTitem.distUSD);
		else
			SPrintF(sizeof(buffer), cnt, buffer, " not available");
	}
	if (1 < _ximuCTX.dmpLvl_Fusensor) {
		SPrintF(sizeof(buffer), cnt, buffer, "\nAccele (%+10.3f%+10.3f%+10.3f)",
			RTitem.ax, RTitem.ay, RTitem.az);
		SPrintF(sizeof(buffer), cnt, buffer, "\nGyrosc (%+10.3f%+10.3f%+10.3f)",
			RTitem.gx, RTitem.gy, RTitem.gz);
		SPrintF(sizeof(buffer), cnt, buffer, "\nMagnet (%+10.3f%+10.3f%+10.3f)",
			RTitem.mx, RTitem.my, RTitem.mz);
	}
	return String(buffer);
}
//---------------------------------------------------------------------
/**
 * Loads gyro,accelero & magneto parameters from eeprom
 */
String H_AccGyrMag::dumpCAL(){
	char buffer[512];
	int16_t cnt = 0;
	SPrintF(sizeof(buffer), cnt, buffer,
		"\nfloat gyroBias[3] ={%11.5f,%11.5f,%11.5f};",
		_ximuCTX.gyroBias[0], _ximuCTX.gyroBias[1], _ximuCTX.gyroBias[2]);
	SPrintF(sizeof(buffer), cnt, buffer,
		"\nfloat accelBias[3]={%11.5f,%11.5f,%11.5f};",
		_ximuCTX.accelBias[0], _ximuCTX.accelBias[1], _ximuCTX.accelBias[2]);
	SPrintF(sizeof(buffer), cnt, buffer,
		"\nfloat magBias[3]  ={%11.5f,%11.5f,%11.5f};",
		_ximuCTX.magBias[0], _ximuCTX.magBias[1], _ximuCTX.magBias[2]);
	SPrintF(sizeof(buffer), cnt, buffer,
		"\nfloat magScale[3] ={%11.5f,%11.5f,%11.5f};",
		_ximuCTX.magScale[0], _ximuCTX.magScale[1], _ximuCTX.magScale[2]);
	return String(buffer);
}
//-------------------------------------------------------
/**
 * Loads gyro,accelero & magneto parameters from eeprom
 */
void H_AccGyrMag::loadIMU(){
	extern EEPROMClass E_CALIMU;
	E_CALIMU.get(0, _ximuCTX);
	_SERIAL_3("\nE_CALIMU Loading...\n%s", dumpCAL().c_str());

}
//-------------------------------------------------------
/**
 * Saves gyro,accelero & magneto parameters from eeprom
 */
void H_AccGyrMag::saveIMU(){
	extern EEPROMClass E_CALIMU;
	E_CALIMU.put(0, _ximuCTX);
	E_CALIMU.commit();
	_SERIAL_3("\nE_CALIMU Saving...\n%s", dumpCAL().c_str());
}
//-------------------------------------------------------
/**
 * Calibrate gyro and accelerometers, computes biases
 */
void H_AccGyrMag::calACCGYR(){
	_mpu9250.calibrateMPU9250(_ximuCTX.gyroBias, _ximuCTX.accelBias);
	_SERIAL_3("\ncalACCGYR calibrating...\n%s", dumpCAL().c_str());
}
//-------------------------------------------------------
/**
 * Calibrate magnetometer, computes bias & scale
 */
void H_AccGyrMag::calMAGNETO(){
	_SERIAL_3(
		"\n\tMAG calibration. Wave device in a figure eight until done (starts in 4s, during 15s) !!!");
	delay(4000);
	int16_t mag_min[3];
	int16_t mag_max[3];
	_mpu9250.magcalMPU9250(_ximuCTX.magBias, _ximuCTX.magScale,
		mag_min,
		mag_max);
	_SERIAL_3("\n\tMAG calibration done !!!");

	_SERIAL_3("\n\tMAG   Min    XYZ-axis(%6i,%6i,%6i) mG", mag_min[0], mag_min[1],
		mag_min[2]);
	_SERIAL_3("\n\tMAG   Max    XYZ-axis(%6i,%6i,%6i) mG", mag_max[0], mag_max[1],
		mag_max[2]);
	_SERIAL_3("\ncalMAGNETO calibrating...\n%s", dumpCAL().c_str());
}
///////////////////////////////////////////////////////////////////////
/*
 * https://www.rs-online.com/designspark/noise-sensor-fusion-and-lost-drones-fr
 */
