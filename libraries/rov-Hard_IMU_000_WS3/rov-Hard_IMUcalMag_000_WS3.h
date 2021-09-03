/*
 #pragma once
 #include "SPIFFS.h"
 #include <esp32_DEBUG.h>
 */
///////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////
/**
 * \class Profiler
 */
class Profiler {
private:
	File file;
	public:
	bool open(const char *PATH, const char *MODE = FILE_READ){
		file = SPIFFS.open(PATH, MODE);
		if (!file)
			return false;
		return true;
	}
	bool close(){
		file.flush();
		file.close();
		return true;
	}
	bool writeln(const char *CHN){
		file.println(CHN);
		return true;
	}
	bool readln(char *BUFFER, size_t SIZE){
		int len = file.readBytesUntil('\n', BUFFER, SIZE);
		if (len)
			BUFFER[len - 1] = 0;
		return 0 != len;
	}
};

////////////////////////////////////////////////////////////////
/**
 * @class c_calMagDialog (enclosing class)
 * @brief Manages dialog with pilot browser if @ref DEF_WEBGL is defined
 *
 * During magnetometer calibration acquisition, writes raw data in ptsFile (which is openWrite)\n
 * After during verification, reads raw data and apply bias & scale to find again better compass behavior
 */
class c_calMagDialog {
public:
	typedef enum : int {
		START,
		STOP,
		CHECKIN,
		SHIFT,
		CHECKOUT,
		NOP
	} e_OP;
	// BE WARRY WITH dECLARATION ORDER !!!
	const char *c_OP[c_calMagDialog::e_OP::NOP] = {
		"calMag_START",
		"calMag_STOP",
		"calMag_CHECKIN",
		"calMag_SHIFT",
		"calMag_CHECKOUT" };
	Profiler ptsFile;
	uint16_t _expectedPoints = 0;
	uint16_t _processedPoints = 0;
	e_OP _OP = e_OP::NOP;
	bool _status = true;
	bool _running = false;
	float _dX = 0.0;
	float _dY = 0.0;
	float _dZ = 0.0;
	float _calX = 0.0;
	float _calY = 0.0;
	float _calZ = 0.0;
	int16_t _rawX = 0;
	int16_t _rawY = 0;
	int16_t _rawZ = 0;
	String _Msg = "";
	void init(){
		_expectedPoints = 0;
		_processedPoints = 0;
		_OP = e_OP::NOP;
		_running = false;
		_status = true;
	}
	bool openWrite(){
//		init(); // Reset the class !!!
		if (ptsFile.open("/lastMagCal.txt", "w")) {
			return true;
		}
		ERROR("ERREUR ouverture fichier points");
		return false;
	}
	bool openRead(){
		if (ptsFile.open("/lastMagCal.txt", "r")) {
			return true;
		}
		_Msg = "ERREUR ouverture fichier points";
		_DBG_CALMAG("\n@@@@ %s", _Msg.c_str());
		_status = false;
		return false;
	}
	void close(){
		ptsFile.close();
	}
	/**
	 * @fn void ERROR(const String MSG)
	 * @brief Sets _status to false an gets the error message
	 * @param MSG error message
	 */
	void ERROR(const String MSG){
		_Msg = MSG;
		Serial.printf("\n@@@@ %s @@@@@@@@@@@@@@@@@@@@@@@@@@", _Msg.c_str());
		_status = false;
	}
	/**
	 * @fn void waitFor_then_clear(bool *FLAG, const uint16_t TIMEOUT = 1000)
	 * @brief Waits for the FLAG becomes true and resets it to false
	 *
	 * waitFor_then_clear is intended to wait for responses from the pilot. In the event of a web \n
	 * socket problem, stop requested by the driver, etc... waitFor_then_clear returns false \n
	 * to possibly abort the rover processing in progress.
	 *
	 * @param FLAG pointer
	 * @param TIMEOUT in milliseconds < 65535 (1 minute...). if zero waits indefinitely
	 * @return true if TIMEOUT has not been passed, false otherwise
	 */
	bool waitFor_then_clear(volatile bool *FLAG, const uint16_t TIMEOUT = 1000){
		uint16_t tms = 0;
		while ((!*FLAG) && (tms <= TIMEOUT)) {
			_DBG_CALMAG("%%");
			delay(5);
			if (TIMEOUT)
				tms += 5;
		}
		*FLAG = false;
		return tms <= TIMEOUT;
	}
	bool clear_then_waitFor(volatile bool *FLAG, const uint16_t TIMEOUT = 1000){
		*FLAG = false;
		uint16_t tms = 0;
		while ((!*FLAG) && (tms <= TIMEOUT)) {
			_DBG_CALMAG("%%");
			delay(5);
			if (TIMEOUT)
				tms += 5;
		}
		return tms <= TIMEOUT;
	}
	/**
	 * @fn void calMag_STOP(bool *browserReady)
	 * @brief Stops the magneto calibration (normal end or on error)
	 *
	 * Asks an @ref H_AccGyrMag::auto_MSG query to wait for the alert OK from pilot indefinitely !!!
	 *
	 * @param browserReady flag true when browser is ready
	 */
	void calMag_STOP(volatile bool *browserReady){
		// if !_status keep the current error message reason !!!
		if (_status)
			_Msg = "Calibration compas terminée";
		_DBG_CALMAG("\n$$$$ calMag_STOP: %s", _Msg.c_str());
		waitFor_then_clear(browserReady);
		_OP = e_OP::STOP;
		// ... To finish properly
		clear_then_waitFor(browserReady,0);// Indefinitely !!!
		_running = false;
	}
	/**
	 * @fn bool calMag_START(bool *browserReady,const uint16_t expectedPoints)
	 * @brief reinit this and ask for an auto_MSG to erase webGL cal mag data
	 * @param browserReady flag true when browser is ready
	 * @param expectedPoints from compass calibration
	 * @return true if associated file is correctly open, false otherwise
	 */
	bool calMag_START(volatile bool *browserReady, const uint16_t expectedPoints){
		init();
		_expectedPoints = expectedPoints;
		_processedPoints = 0;
		_Msg = "Calibration compas démarrée pour "+String(_expectedPoints)+" points";
		_DBG_CALMAG("\n$$$$ calMag_START: %s", _Msg.c_str());
		_running = true;
		_OP = e_OP::START;
		// ... Before beginning the first raw data acquisition
		return clear_then_waitFor(browserReady);
	}
	/**
	 * @fn void calMag_CHECKIN(bool *browserReady,bool &acqOK, const int16_t x, const int16_t y, const int16_t z)
	 * @brief Saves a magneto acquisition set in file & set flag for a ws auto message
	 * @param browserReady to  wait for auto_MSG confirms the latest OP has been updapted by browser
	 * @param acqOK input:report of data acquisition, output report of data storage
	 * @param x magneto value of IMU x axis
	 * @param y magneto value of IMU y axis
	 * @param z magneto value of IMU z axis
	 */
	void calMag_CHECKIN(volatile bool *browserReady, const bool acqOK,
		const int16_t x, const int16_t y, const int16_t z, const String &MSG){
		_status = acqOK;
		_Msg = MSG;
		if (!_status) { // MPU9250 error reading raw data
			_DBG_CALMAG("\nMPU9250 error, closing ptsFile & return '%s'", MSG.c_str());
			close();
			return ERROR(MSG);
		}
		char buffer[200];
		int16_t cnt = 0;
		SPrintF(sizeof(buffer), cnt, buffer, "%4i/%i %3i %6i %6i %6i %s",
			_processedPoints, _expectedPoints, acqOK, x, y, z, MSG.c_str());
		_DBG_CALMAG("\n%s", buffer);
		if (!ptsFile.writeln((char*)&buffer)) {
			close();
			return ERROR("ERREUR écriture fichier points");
		}
		*browserReady = false; // To block next mag acquisition until the complete browser update without wasting time of the stepped delay in MPU9250::magcalMPU9250
		_processedPoints++;
		//if (remaining == 1200)		status = false;
		_rawX = x;
		_rawY = y;
		_rawZ = z;
		_OP = e_OP::CHECKIN; // last statement !!!
	}
	/**
	 * @fn void calMag_SHIFT(bool *browserReady,int16_t *mag_min, int16_t *mag_max)
	 * @brief Centers acquired raw datas around (0,0,0) iron effect & set flag for a web socket auto message
	 * @param browserReady to  wait for auto_MSG confirms the latest OP has been updapted by browser
	 * @param mag_min magneto min raw values found (3 values xyz axis)
	 * @param mag_max magneto max raw values found (3 values xyz axis)
	 */
	bool calMag_SHIFT(volatile bool *browserReady, int16_t *mag_min, int16_t *mag_max){
		if (!_status) {
			_DBG_CALMAG("\nBEURK calMag_SHIFT !!!!!!!!!!!!!!!!!!");
			return false;
		}
		waitFor_then_clear(browserReady); // Block this checkOUT until the previous one was updated by browser
		_dX = (mag_max[0] + mag_min[0]) / 2.0; // get average x mag bias in counts
		_dY = (mag_max[1] + mag_min[1]) / 2.0; // get average y mag bias in counts
		_dZ = (mag_max[2] + mag_min[2]) / 2.0; // get average z mag bias in counts
		_DBG_CALMAG("\n$$$$ Shifting raw points (%10.3f,%10.3f,%10.3f)", -_dX, -_dY, -_dZ);
		_OP = e_OP::SHIFT;
		return true;
	}
	/**
	 * @fn void calMag_CHECKOUT(bool *browserReady,int16_t *mag_min, int16_t *mag_max,H_AccGyrMag &PARENT)
	 * @brief Loads a magneto acquisition set from file, calibrates values & set flag for a ws auto message
	 * @param browserReady to  wait for auto_MSG confirms the latest OP has been updapted by browser
	 * @param PARENT IMU parent class of this nested class
	 */
	bool calMag_CHECKOUT(volatile bool *browserReady, H_AccGyrMag &PARENT){
		waitFor_then_clear(browserReady); // Block this checkOUT until the previous one was updated by browser
		char buffer[200];
		if (ptsFile.readln((char*)&buffer, sizeof(buffer))) {
			_processedPoints++;
			_DBG_CALMAG("\n Verifying point read [%s]", buffer);
			vector<String> args;
			splitString(args, String(buffer), " ");
			int16_t rawData[3];
			rawData[0] = Float(args[2].c_str());
			rawData[1] = Float(args[3].c_str());
			rawData[2] = Float(args[4].c_str());
			PARENT.calMagData(rawData, _calX, _calY, _calZ); // in the enclosing class !!!
			_DBG_CALMAG("(%5i,%5i,%5i)->(%10.3f,%10.3f,%10.3f)",
				rawData[0], rawData[1], rawData[2], _calX, _calY, _calZ);
			_OP = e_OP::CHECKOUT;
			return true;
		}
		close();
		if (_processedPoints != _expectedPoints)
			ERROR("ERREUR lecture fichier points");
		return false;
	}
};
///////////////////////////////////////////////////////////////////////
