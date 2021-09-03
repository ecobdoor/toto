#include "rov-Mod_Driver_000_WS3.h"
#include "esp32_LOGS.h"
#include "esp32_SPRINT.h"
#include "esp32_GOODIES.h"
extern c_myUDP myUDP;
///////////////////////////////////////////////////////////////////////
Driver::Driver(c_myFlow *FLOW, const s_MODULE MOD_CFG, s_DRVctx *CONTEXT, Kine_1 *IMUKINE,
	Kine_1 *DRVKINE, H_AccGyrMag *IMU, H_ultraSounds *USD, int8_t *DBGMAX) :
	Module(FLOW, MOD_CFG, DBGMAX),
		_imu_Kine(*IMUKINE),
		_drv_Kine(*DRVKINE),
		_Ximu(*IMU),
		_usdDrv(*USD),
		_Ctx(*CONTEXT),
		_motors(*reinterpret_cast<Motors*>(findModule("MOT"))),
		_jobgoal(*reinterpret_cast<Jobgoal*>(findModule("JOB")))
/*
 //	static Module *mod = findModule("MOT");
 static Motors &motors = *reinterpret_cast<Motors*>(findModule("MOT"));
 //	static Module *mod = findModule("MOT");
 static Jobgoal &jobgoal = *reinterpret_cast<Jobgoal*>(findModule("JOB"));

 */
{
	map_OPdriver = {
		{ "setPARAM", &Driver::mOP_Param },
		{ "calMagnet", &Driver::mOP_calMagnet },
		{ "calAccGyr", &Driver::mOP_calAccGyr }
	};

	int imuDATAqueueSIZE = 40;
	_IMUqueue = xQueueCreate(imuDATAqueueSIZE, sizeof(s_kineFusensor));
	delete _Simimu;
	_Simimu = nullptr;
	String CHN;
	if (_Ctx.simul_IMU) {
		_Simimu = new imuDsp();
		if (_Simimu->Begin(_Ctx.drvKmd[_Ctx.kmdIdx], CHN)) {
			_SERIAL_0("\n_Simimu->Begin %i:'%s' OK (%s)",
				_Ctx.kmdIdx, _Ctx.drvKmd[_Ctx.kmdIdx].c_str(), CHN.c_str());
		} else {
			_SERIAL_0("\n_Simimu->Begin %i:'%s' ERROR (%s)",
				_Ctx.kmdIdx, _Ctx.drvKmd[_Ctx.kmdIdx].c_str(), CHN.c_str());
			delete _Simimu;
			_Simimu = nullptr;
		}
	}
	clear_CineRover();
	_isRunning = true; // ALWAYS !!!
}
//---------------------------------------------------------------------
void Driver::clear_CineRover(){
	if (_Simimu != nullptr)
		_Simimu->Reset();
	_mustClearDRV = true;
	_mustClearIMU = true;
	_mustClearOGL = true;
}
//---------------------------------------------------------------------
/**
 * @fn e_hasMsg Driver::auto_MSG(const uint64_t DTMS, JsonObject &KMD)
 * @brief generates or not an auto message from Driver itself or children soft or hard modules
 *
 * Works at core task varying speed loop. Must run faster than this module timeout
 *
 * @param DTMS time elapsed since last update of @ref _milliLastTS
 * @param KMD json message pre built by popup_MSG
 * @return @ref e_hasMsg
 */
e_hasMsg Driver::auto_MSG(const uint64_t DTMS, JsonObject &KMD){
	e_hasMsg hasMsg;
	hasMsg = _Ximu.auto_MSG(DTMS, KMD, &_milliLastTS);
	/*
	 if(hasMsg == e_hasMsg::No)
	 hasMsg = anOtherOne.auto_Msg( DTMS, KMD);// never _milliLastTS update
	 if(hasMsg == e_hasMsg::No)
	 hasMsg = anOtherSecond.auto_Msg( DTMS, KMD,&_milliLastTS);// sometimes _milliLastTS update or not...
	 ...
	 */
	if (hasMsg != e_hasMsg::No)
		_DBG_CALMAG("\n Driver::auto_MSG [%s]", dumpJSON().c_str()); // just to debug
	return hasMsg;
}
//---------------------------------------------------------------------
/**
 * @fn bool Driver::updateATreply(JsonObject &JOBJ)
 * @brief update module depending of reply content
 *
 * In case of compass calibration, the pilot browser has to wait for webGL to be achieved. So \n
 * First javascript answers a first reply to the @ref _Ximu query with KMD["endCalPt"]==false \n
 * Then @ref _Ximu repeat new queries until javascript reply with KMD["endCalPt"]==true
 *
 * @param JOBJ complete binary reply form as {CTL:{...},KMD:{...}}
 * @return true if a message is to send (query | info}, false otherwise
 */
bool Driver::updateATreply(JsonObject &JOBJ){
	if (_Ximu.calibrating()) {
		JsonObject CTL = JOBJ["CTL"];
		JsonObject KMD = JOBJ["KMD"];
		_Ximu.browserReady = KMD["endCalPt"];
		_DBG_CALMAG("\n [%s] Driver::updateATreply", _Ximu.browserReady ? "VRAI" : "FAUX");
		if (_Ximu.browserReady)
			return false;
		CTL["TYP"] = "rQp";
		KMD["DT"] = 0;
		KMD["endCalPt"] = "???";
		_DBG_CALMAG(" [%s]", dumpJSON().c_str()); // just to debug
		CTL["TIK"] = _Flow.tag(milli_TS(), CTL["TYP"].as<String>());
		return true;
	}
	return false;
}
//---------------------------------------------------------------------
bool Driver::mOP_calMagnet(JsonObject &JOBJ){
	bool ok = true;
	DETACH(P_imuISR_);
	calib_Compas();
	JOBJ["KMD"]["OP"] = "calAccGyr";
	JOBJ["KMD"]["OP"] = "calMagnet";
	ATTACH(P_imuISR_);
	JOBJ["KMD"][DEF_KF_EXE] = ok;
	return ok;
}
//---------------------------------------------------------------------
bool Driver::mOP_calAccGyr(JsonObject &JOBJ){
	bool ok = true;
	DETACH(P_imuISR_);
	calib_AccGyr();
	JOBJ["KMD"][DEF_KF_EXE] = true;
	ATTACH(P_imuISR_);
	return ok;
}
//---------------------------------------------------------------------
void Driver::calib_AccGyr(const int16_t samples){
	_Ximu.init_IMU(true, false, samples);
}
//---------------------------------------------------------------------
void Driver::calib_Compas(const int16_t samples){
	_Ximu.init_IMU(false, true, samples);
}
//---------------------------------------------------------------------
bool Driver::mOP_Param(JsonObject &JOBJ){
	const char *param = JOBJ["KMD"]["PARAM"].as<string>().c_str();
	_SERIAL_9("\nmOP_Param '%s'", param);
	if (strcmp(param, "thr_linAcc") == 0) {
		float value = JOBJ["KMD"]["VALUE"].as<float>();
		CTX.HARDctx.kineCTX.thr_linAcc = value / 1000.0; // mm/s² -> m/s²
		_SERIAL_9("='%10.4f'", value);
	}
	else if (strcmp(param, "beta") == 0) {
		float value = JOBJ["KMD"]["VALUE"].as<float>();
		CTX.HARDctx.ximuCTX.beta = value;
		_SERIAL_9("='%10.4f'", value);
	}
	else if (strcmp(param, "cntInt") == 0) {
		float value = JOBJ["KMD"]["VALUE"].as<uint8_t>();
		CTX.HARDctx.ximuCTX.cntInt = value;
		_SERIAL_9("='%i'", value);
	}
	else if (strcmp(param, "show_drvInfos") == 0) {
		bool value = JOBJ["KMD"]["VALUE"].as<bool>();
		CTX.SOFTctx.DRVctx.show_drvInfos = value;
		_SERIAL_9("='%s'", value ? "oui" : "non");
	}
	else if (strcmp(param, "MAX_DBG_DRV_XIMU") == 0) {
		int8_t value = JOBJ["KMD"]["VALUE"].as<int8_t>();
		CTX.SOFTctx.MAX_DBG_DRV_XIMU = value;
		_SERIAL_9("='%i'", value);
	}
	else if (strcmp(param, "ALL_DBGMAX_TRUNC") == 0) {
		int8_t value = JOBJ["KMD"]["VALUE"].as<int8_t>();
		CTX.SOFTctx.ALL_DBGMAX_TRUNC = value;
		_SERIAL_9("='%i'", value);
	} else {
		_SERIAL_0("\nmOP_Param '%s'", param);
		_SERIAL_0(" INCONNU !!!");
		JOBJ["KMD"][DEF_KF_EXE] = false;
		return true;
	}
	JOBJ["KMD"][DEF_KF_EXE] = true;
	JOBJ["KMD"].remove("VALUE");
	/*
	 JsonObject crew = JOBJ["KMD"];
	 crew["VALUE"].remove("VALUE");
	 */
	return true;
}
//---------------------------------------------------------------------
bool Driver::reply2pilotQ_OP(JsonObject &JOBJ){
	const char *x = JOBJ["KMD"]["OP"].as<string>().c_str();
//_SERIAL_0(" looking for driver OP '%s'", x); //esp32_DEBUG.h
	(this->*(findOP<OPdrv>(map_OPdriver, x)))(JOBJ);
	return true;
}
//---------------------------------------------------------------------
/**
 * \fn void updateTrajectory(kineModelData & K)
 * \brief Updates X,Y,Z,A,B,C trajectory of rover
 * \param kineModelData & K
 */
/*
 void Driver::updateTrajectory(kineModelData & K) {
 float dMts = (K.microTS - _prev MTS) / 1000000.0f; // micro seconds time;
 _prev MTS = K.microTS;
 //compute displacement
 t_vct3F deltaXYZ;
 for (auto i = 0; i < 3; i++) {
 deltaXYZ[i] = K.Spd[i] * dMts;
 }
 std::transform(_Trajectory.XYZ.begin(), _Trajectory.XYZ.end(), deltaXYZ.begin(),
 _Trajectory.XYZ.begin(), std::plus<float>());
 }
 */
//---------------------------------------------------------------------
void Driver::acq_simuIMU(){
	if (_Simimu != nullptr)
		_Ximu.acq_simuIMU(*_Simimu);
}
//---------------------------------------------------------------------
//#define DEEP_DBG
#ifdef DEEP_DBG
String SmicroTS(const uint64_t MICROTS) {
	char buffer[64];
	uint32_t second = (uint32_t)(MICROTS / 1000000);
	uint32_t milli = (uint32_t)((MICROTS - second * 1000000) / 1000);
	uint32_t micro = (uint32_t)(MICROTS - second * 1000000 - milli * 1000);
	sprintf(buffer, "%5i_%03i.%03i", second, milli, micro);
	return String(buffer);
}
#endif
//---------------------------------------------------------------------
void Driver::acq_trueIMU(const uint64_t ISR_MICRO_TS){
	_Ximu.acq_trueIMU(ISR_MICRO_TS);
}
///---------------------------------------------------------------------
void Driver::telemetry_MOTION(const size_t SIZ, int16_t &CNT, char *BUF){
	SPrintF(SIZ, CNT, BUF, "%f;%f;%f;%f", Motion.Speed, Motion.Dir, Motion.Left,
		Motion.Right);
}
//---------------------------------------------------------------------
/**
 * @fn void Driver::dashBoard(Kine_1 &KINE, e_telemetry &TELEMETRY, const bool TELEM, bool &CLROPNGL,
 const bool OPNGL, const char *NL0, const char *NLN)
 * @brief Sends UDP dashboard data, telemetry data (if asked) & serial data for openGL rover gui
 *
 * dashboard dump depends of s_DRVctx.udpLvl_Kine. UDP output 0:none, 1:driver 10Hz, 2:isr 100Hz, 3:twice together
 * NL0 & NLN are to interlace driver & isr data. They could contain an ANSI sequence to color data, typically:
 * - for example for udpLvl_Kine==3
 *  - driver (NL0,NLN)=("\n\n\033[31m","\n\n") starting at line 2 with yellow color, then 1 line out of 2
 *  - isr (NL0,NLN)=("\n\033[0m","\n\n") starting at line 1 with red color, then 1 line out of 2
 *
 * @param KINE @ref Kine_1 kinematic's data to dump on dashboard
 * @param TELEMETRY type for @ref e_telemetry (reseted if an UDP error occurs). When TELEMETRY==e_telemetry::drv, motion data are inserted.
 * @param TELEM flag for UDP telemetry (dashboard dump is not available during telemetry transfert)
 * @param CLROPNGL flag to clear openGL positionning (reseted by this call)
 * @param OPNGL ///< Serial output 0:none, 1:driver 10Hz, 2:isr 100Hz
 * @param NL0 line break for first line of attitude dump
 * @param NLN line break for following lines of attitude dump
 */
void Driver::dashBoard(Kine_1 &KINE, e_telemetry &TELEMETRY, const bool TELEM,
	bool &CLROPNGL,
	const bool OPNGL, const char *NL0, const char *NLN){
	static size_t BUF_SIZ = 1024; //don't use BUFSIZ of stdio.h !!!!!
	if (OPNGL) {
		char c;
		if (CLROPNGL) {
			c = 'D';
			CLROPNGL = false;
		} else
			c = '@';
		_SERIAL_0("%c %6llu", c, (KINE.Fusensor.microTS) / 1000);
		_SERIAL_0("  %12.5f  %12.5f  %12.5f", KINE.Fusensor.ax, KINE.Fusensor.ay,
			KINE.Fusensor.az);
		_SERIAL_0("  %12.5f  %12.5f  %12.5f", KINE.Fusensor.gx, KINE.Fusensor.gy,
			KINE.Fusensor.gz); //°/s
		_SERIAL_0("  %10.3f  %10.3f  %10.3f", KINE.Fusensor.mx, KINE.Fusensor.my,
			KINE.Fusensor.mz);
		if (c == '@')
			_SERIAL_0("  %12.5f  %12.5f  %12.5f",
				KINE.Attitude.Nroll* 180./pi, KINE.Attitude.Epitch*180./pi,
				KINE.Attitude.Dyaw*180./pi);
		_SERIAL_0(" \n");

	}
	if (myUDP.isOpen()) {
		int16_t cnt = 0;
		if (TELEM) {
			char *buffer = allocBuf(BUF_SIZ, &cnt);
			buffer[0] = 0;
			SPrintF(BUF_SIZ, cnt, buffer, "@Telem:;%llu;", KINE.Fusensor.microTS);
			KINE.telemetry_ATTITUDE(BUF_SIZ, cnt, buffer);
			SPrintF(BUF_SIZ, cnt, buffer, ";");
			if (TELEMETRY == e_telemetry::drv) {
				telemetry_MOTION(BUF_SIZ, cnt, buffer);
				SPrintF(BUF_SIZ, cnt, buffer, "\n");
				if (!myLOGS._UDP_(buffer))
					TELEMETRY = e_telemetry::no;
			}
			/*
			 telemetry_KEYFRAME(BUF_SIZ, cnt, buffer);
			 SPrintF(BUF_SIZ, cnt, buffer, "\n");
			 */
		} else {
			char *imuBuf = allocBuf(BUF_SIZ, &cnt);
			imuBuf[0] = 0;
			KINE.dump_FUSENSOR(BUF_SIZ, cnt, imuBuf, NL0, NLN);
			KINE.dump_KFMATRIX(BUF_SIZ, cnt, imuBuf, NLN, NLN);
			myLOGS._UDP_(imuBuf);

			char *attBuf = allocBuf(BUF_SIZ, &cnt);
			attBuf[0] = 0;
			KINE.dump_ATTITUDE(BUF_SIZ, cnt, attBuf, NL0, NLN);
			myLOGS._UDP_(attBuf);
		}
	}
}
//---------------------------------------------------------------------
/**
 * \fn void Driver::push_RTdata()
 * \brief Every hardware interrupt or simulated, push one \ref s_kineFusensor: IMU data & USD distance if (_needsUSD) in inter-tasks RT fifo
 *
 * WARNING: COULD REENTER BECAUSE OF HARDWARE INTERRUPT (_pushDone is here to forbid it!!!)
 * At each IMU interrupt, the serial line (115000 baud) can output 100 characters
 * every 10 ms at most with nothing else to do. In practice only a short message of a few
 * characters is only possible (no more than 2 ms). Detailed information is only visible in UDP.
 */
void Driver::push_RTdata(){
	_pushDone = false;
	if (_mustClearIMU) {
//		_Ximu.Clear_IMU_quaternion();
		_imu_Kine.Clear();
		_mustClearIMU = false;
		_Ctx.clear_CineRover = _mustClearIMU || _mustClearDRV;

	}
	if (_needsUSD) {
		_needsUSD = false;
		_imu_Kine.Fusensor.distUSD = _usdDrv.Get(); // Always out of interrupt _Ximu.acq_trueIMU !!!!
//		_imu_Kine.Fusensor.USD = true;
		_DBG_DRV_FIFO("\n>%i %f", uxQueueMessagesWaiting(_IMUqueue),
			_imu_Kine.Fusensor.distUSD);
	} else {
		_DBG_DRV_FIFO("\n>%i ...", uxQueueMessagesWaiting(_IMUqueue));
	}
	_imuTIK++;

	_Ximu.get_IMU(_imu_Kine.curMicroTS, _imu_Kine.Fusensor); // update quaternion

	_imu_Kine.Fusensor.tik = _imuTIK;
	_imu_Kine.Fusensor.fid = _imuTIK % 100; //uxQueueMessagesWaiting(_IMUqueue);
	/*
	 */
	try {
		if (pdPASS != xQueueSend(_IMUqueue, &_imu_Kine.Fusensor, 0)) {
			THROWERR(FIFO_OVERFLOW, _imuTIK, _imu_Kine.Fusensor.microTS);
		}
		//_SERIAL_0("\n %4i %12llu",_imuTIK, _imu_Kine.Fusensor.microTS);
#ifdef DEEP_DBG
		_SERIAL_1("\n⇉%02i %s:%i %s %s", uxQueueMessagesWaiting(_IMUqueue),
			SmicroTS(_imu_Kine.Fusensor.microTS).c_str(),_imu_Kine.Fusensor.fid,
			SmicroTS(_imu_Kine.prev MicroTS).c_str(),
			((_Ximu.microTS()==_imu_Kine.Fusensor.microTS)?"ok":"ERROR"));
#endif

		//_SERIAL_0("\n_imu(%i)", _Ctx.telemetry);
		_imu_Kine.updateRoverKine();

		if (_Ctx.udpLvl_Kine & 2)
			dashBoard(_imu_Kine, _Ctx.telemetry, e_telemetry::imu == _Ctx.telemetry,
				_mustClearOGL,
				2 == _Ctx.openGL,
				"\n\n\033[1;31m",
				"\n\n");
//		(3 != _Ctx.udpLvl_Kine) ? "\n" : "\n\n");

		//_SERIAL_0("\n_imu(%i)", _Ctx.telemetry);

#ifdef DEEP_DBG
		_SERIAL_1(" %s",((_imu_Kine.prev MicroTS==_imu_Kine.Fusensor.microTS)?"ok":"ERROR"));
#endif
	} catch (EXCEPT const &e) {
		FIX_ERROR();
	}
	_pushDone = true;
}
//---------------------------------------------------------------------
/**
 * \fn void Driver::pull_RTdata()
 * \brief extracts real-time IMU data from the @ref _IMUqueue fifo and prepares its smoothing
 *
 * Every driver time out, pull all \ref s_kineFusensor: IMU data & USD distance if (_needsUSD) from inter-tasks RT fifo
 * \return count of pulled IMU records
 */
uint8_t
Driver::pull_RTdata()
{
	s_kineFusensor _pull_rtFusensors;
	_mustClearMAP = false;
	_drv_Kine.Fusensor = { };
	if (_Ctx.clear_CineRover)
		clear_CineRover();
	if (_mustClearDRV) {
		_drv_Kine.Clear();
		_mustClearMAP = true;
		_mustClearDRV = false;
		_Ctx.clear_CineRover = _mustClearIMU || _mustClearDRV;
	}
//return;
#ifdef DEEP_DBG
	String prvStr = SUI64(milli_TS() - _milliLastTS) + String(" ms ");
	String fusStr = SUI64(milli_TS() - _milliLastTS) + String(" ms ");
#endif

	float dist = -1;
	uint8_t cnt = 0;
	while (xQueueReceive(_IMUqueue, &_pull_rtFusensors, 0) == pdPASS) {
#ifdef DEEP_DBG
		prvStr += SmicroTS(_drv_Kine.prev MicroTS)+ String(":") + String(_pull_rtFusensors.fid) + String(" ");
		fusStr += SmicroTS(_pull_rtFusensors.microTS)+ String(":") + String(_pull_rtFusensors.fid) + String(" ");
#endif
		/*
		 * Compute ponderate average of Fusensors of fifo (not equidistant in time !!!)
		 */
		_drv_Kine.Fusensor.temp += _pull_rtFusensors.temp;
		_drv_Kine.Fusensor.ax += _pull_rtFusensors.ax;
		_drv_Kine.Fusensor.ay += _pull_rtFusensors.ay;
		_drv_Kine.Fusensor.az += _pull_rtFusensors.az;
		_drv_Kine.Fusensor.gx += _pull_rtFusensors.gx;
		_drv_Kine.Fusensor.gy += _pull_rtFusensors.gy;
		_drv_Kine.Fusensor.gz += _pull_rtFusensors.gz;
		_drv_Kine.Fusensor.mx += _pull_rtFusensors.mx;
		_drv_Kine.Fusensor.my += _pull_rtFusensors.my;
		_drv_Kine.Fusensor.mz += _pull_rtFusensors.mz;
		_drv_Kine.Fusensor.q[0] += _pull_rtFusensors.q[0];
		_drv_Kine.Fusensor.q[1] += _pull_rtFusensors.q[1];
		_drv_Kine.Fusensor.q[2] += _pull_rtFusensors.q[2];
		_drv_Kine.Fusensor.q[3] += _pull_rtFusensors.q[3];
		_drv_Kine.Fusensor.microTS = _pull_rtFusensors.microTS; // keep the newest last microTS
		if (_pull_rtFusensors.distUSD)
			_drv_Kine.Fusensor.distUSD = _pull_rtFusensors.distUSD;
		/*
		 kineModelData K;
		 K.Acc = { {Fusensor.ax,Fusensor.ay,Fusensor.az}};
		 K.Mag = { {Fusensor.mx,Fusensor.my,Fusensor.mz}};
		 K.Gyr = { {Fusensor.gx,Fusensor.gy,Fusensor.gz}};
		 K.microTS = Fusensor.microTS;
		 _Kine.Predict(K);
		 _Kine.push(K);
		 updateTrajectory(K);
		 */
		/*
		 _DBG_DRV_FIFO("\n<%i %s", _drv_Kine.Fusensor.fid,
		 _drv_Kine.Fusensor.USD ? String(_drv_Kine.Fusensor.distUSD) : "...");
		 if (_drv_Kine.Fusensor.USD) {
		 dist = _drv_Kine.Fusensor.distUSD; // to pass on the true Fusensor
		 }
		 */
		cnt++;
	}
	/*
	 if (dist != -1) {
	 _drv_Kine.Fusensor.USD = true;
	 _drv_Kine.Fusensor.distUSD = dist;
	 }
	 */
	String comment;
	if (cnt < 5)
		comment += "Poor pull_RTdata " + String(cnt);
	else
		_needsUSD = true; // for next push_RTdata isr time...
//if (!_drv_Kine.Fusensor.USD)		comment += ". Radar not found";
#ifdef DEEP_DBG
	_LOG_(0, 0, "\n%i %10.3f ms %s", cnt,(_drv_Kine.Fusensor.microTS - _drv_Kine.prev MicroTS) / 1000.0, prvStr.c_str());
	_LOG_(0, 0, "\n%i %10.3f ms %s", cnt,(_drv_Kine.Fusensor.microTS - _drv_Kine.prev MicroTS) / 1000.0, fusStr.c_str());
#endif
	if ((CTX.SOFTctx.showWSX_fifo) && (comment.length()))
		myLOGS._LOG_(0, 0, "\n%s", comment.c_str());
//myLOGS._LOG_(0, 0, "\n%i", cnt);
	return cnt;
}
//---------------------------------------------------------------------
/**
 * @fn bool Driver::complete_MSG(JsonObject &KMD)
 * @brief roaming (frq 10 Hz)
 * Every 100 ms acquire USD distance, pull RT interrupt I²C IMU data & finish their smoothing process
 * To test motors & Proportional–Integral–Derivative regulation behaviour we do that:
 * If ultrasonic sensor measures a distance less than 0.35 meter, an avoidance strategy is chosen
 * Either the rover rolls back
 * Or the rover turns right
 * Or the rover turns left
 * As soon as there are no more obstacles, the rover starts moving forward again
 * @param KMD JsonObject
 * @return e_hasMsg::Ack if (_Ctx.jsonDRVinfos) to send a client JSON, otherwise e_hasMsg::No
 */
e_hasMsg Driver::complete_MSG(JsonObject &KMD){
	try {
#ifdef DEEP_DBG
		String before_pull=SmicroTS(_drv_Kine.prev MicroTS);
#endif
		uint8_t cnt = pull_RTdata();
		if (cnt < 4)
			return e_hasMsg::No;
#ifdef DEEP_DBG
		String after_pull=SmicroTS(_drv_Kine.prev MicroTS);
#endif
		_drv_Kine.Fusensor.temp /= cnt;
		_drv_Kine.Fusensor.ax /= cnt;
		_drv_Kine.Fusensor.ay /= cnt;
		_drv_Kine.Fusensor.az /= cnt;
		_drv_Kine.Fusensor.gx /= cnt;
		_drv_Kine.Fusensor.gy /= cnt;
		_drv_Kine.Fusensor.gz /= cnt;
		_drv_Kine.Fusensor.mx /= cnt;
		_drv_Kine.Fusensor.my /= cnt;
		_drv_Kine.Fusensor.mz /= cnt;
		double norm = 0;
		norm += _drv_Kine.Fusensor.q[0] * _drv_Kine.Fusensor.q[0];
		norm += _drv_Kine.Fusensor.q[1] * _drv_Kine.Fusensor.q[1];
		norm += _drv_Kine.Fusensor.q[2] * _drv_Kine.Fusensor.q[2];
		norm += _drv_Kine.Fusensor.q[3] * _drv_Kine.Fusensor.q[3];
		norm = sqrt(norm);
		_drv_Kine.Fusensor.q[0] /= norm;
		_drv_Kine.Fusensor.q[1] /= norm;
		_drv_Kine.Fusensor.q[2] /= norm;
		_drv_Kine.Fusensor.q[3] /= norm;

		//_SERIAL_0("\n_drv(%i)", _Ctx.telemetry);
		_drv_Kine.updateRoverKine();
		//_SERIAL_0("\n_drv(%i)", _Ctx.telemetry);
		if (_Ctx.udpLvl_Kine & 1)
			dashBoard(_drv_Kine, _Ctx.telemetry, e_telemetry::drv == _Ctx.telemetry,
				_mustClearOGL,
				1 == _Ctx.openGL,
				"\n\033[0m",
				"\n\n");
//		(3 != _Ctx.udpLvl_Kine) ? "\n\033[32m" : "\n\n\033[31m",
		//(3 != _Ctx.udpLvl_Kine) ? "\n" : "\n\n");

#ifdef DEEP_DBG
		String after_update=SmicroTS(_drv_Kine.prev MicroTS);
		_SERIAL_1("\n prev %s %s %s",before_pull,after_pull,after_update);
#endif

//
// UPDATE ANTI COLLIDING
		/*
		 _speed = 200;
		 _dir = 0;
		 */
//
// UPDATE following e_roverMode
//
		JsonObject root = (*_jsonDoc).as<JsonObject>();
		if (KMD["OP"] == "setPARAM")
			reply2query_PILOT(root);
		switch(CTX.getRoverMode()){
			///////////////////////////////////////////
			/// IDLE: 	call stop motors if needs
			/// 		close learn file if needs
			case e_roverMode::idle: {
				if (_motors.isLearning()) {
					_SERIAL_0("\n IDLE motors.isLearning()   => CLOSE");
					KMD["MID"] = "MOT";
					KMD["OP"] = DEF_OP_CLOSE;
					KMD[DEF_KF_FIL] = "Learn";
					reply2query_PILOT(root);
				}

				if (_jobgoal.isWorking()) {
					_SERIAL_0("\n IDLE jobgoal.isWorking()   => CLOSE");
					KMD["MID"] = "JOB";
					KMD["OP"] = DEF_OP_CLOSE;
					KMD[DEF_KF_FIL] = "Learn";
					reply2query_PILOT(root);
				}
				if (!_motors.areStopped()) {
					_SERIAL_0("\n IDLE !motors.areStopped() => STOP");
					KMD["MID"] = "MOT"; // Force the tmsout_MSG to be a motors query
					KMD[DEF_KF_SPD] = 0;
					KMD[DEF_KF_DIR] = 0;
					KMD[DEF_KF_PMTS] = _drv_Kine.Fusensor.microTS;
					KMD["OP"] = DEF_OP_MOT; // go straight on
					reply2query_PILOT(root); // Adds fields S D L R X MTS !!!!!!!!!!!!!!!
				}
				break;
			}
				///////////////////////////////////////////
				/// DRIVING: 	nothing to do, just jsonDRVinfos updates pilot
			case e_roverMode::driving: {
				break;
			}
				///////////////////////////////////////////
				/// LEARNING: 	just open file on the first time
			case e_roverMode::learning: {
				if (!_motors.isLearning()) {
					_SERIAL_0("\n LEARNING !motors.isLearning() => OPEN");
					KMD["MID"] = "MOT";
					KMD["OP"] = DEF_OP_OPEN;
					KMD[DEF_KF_FIL] = "Learn";
					reply2query_PILOT(root);
				}
				break;
			}
				///////////////////////////////////////////
				/// WORKING: 	just open file on the first time
			case e_roverMode::working: {
				if (!_jobgoal.isWorking()) {
					_SERIAL_0("\n WORKING !jobgoal.isWorking() => OPEN");
					KMD["MID"] = "JOB";
					KMD["OP"] = DEF_OP_OPEN;
					KMD[DEF_KF_FIL] = "Learn";
					reply2query_PILOT(root);
				} else if (_jobgoal._qryLine < 0) {
					_SERIAL_0("\n WORKING _jobgoal._qryLine < 0 => CLOSE");
					KMD["MID"] = "JOB";
					KMD["OP"] = DEF_OP_CLOSE;
					reply2query_PILOT(root);
				}
				break;
			}
				///////////////////////////////////////////
				/// ROAMING: 	check collision & run motors
			case e_roverMode::roaming: {
				//_SERIAL_4( "\n+++++++++++++ %f m", Fusensor.distUSD);
				if (_drv_Kine.Fusensor.distUSD < 0.35) {
					if (!_colliding) {
						_colliding = true;
						_dir = -(rand() % 3 - 1) * 50;
						if (_dir == 0) {
							_speed = -100;
						} else {
							_speed = 0;
						}
					}
				} else {
					_colliding = false;
					_speed = 100;
					_dir = 0;
				}
				KMD["MID"] = "MOT"; // Force the tmsout_MSG to be a motors query
				//_SERIAL_3( "\n[%s]", dumpJSON().c_str());
				KMD["OP"] = DEF_OP_MOT; // go straight on

				KMD[DEF_KF_SPD] = _speed;
				KMD[DEF_KF_DIR] = _dir;
				KMD[DEF_KF_PMTS] = _drv_Kine.Fusensor.microTS;
				//_SERIAL_3( "\n[%s]", dumpJSON().c_str());
				// Execute the OP and get back the reply
				reply2query_PILOT(root); // Adds fields S D L R X MTS !!!!!!!!!!!!!!!
				//roverMode = e_roverMode::roaming; // because reply2query_PILOT will overload roverMode to e_roverMode::driving
				// ack the local motors query
//			_SERIAL_3( "\n[%s]", dumpJSON().c_str());
				//
				// UPDATE CLIENT DRIVER
				//
//			_SERIAL_3( "\n[%s]", dumpJSON().c_str());
//		_Flow.ack(root["CTL"]["TIK"].as<uint32_t>());
				//root["CTL"]["TYP"] = DEF_REPLY_ROVER_TO_PILOT;

//			tmsout_MSG(DTMS); //a DRIVER query !!! return true no matter... no send yet
//		externValues(Fusensor);
				break;
			}
			case e_roverMode::error: {
				break;
			}
			default: { // _UNKNOWN???
				;
			}
		}
	}
	catch (EXCEPT const &e) {
		THROW_UP();
	}

	Motion.Speed = _motors.getMotSpeed();
	Motion.Dir = _motors.getMotDir();
//kmd["MTS"] = 54411073;
	Motion.Left = _motors.getMotLeft();
	Motion.Right = _motors.getMotRight();
	return jsonDRVinfos(); // e_hasMsg::Ack else if!_Ctx.jsonDRVinfos, returns e_hasMsg::No
}
//---------------------------------------------------------------------
/**
 * @fn void Driver::jsonDRVinfos()
 * @brief if _Ctx.jsonDRVinfos, builds JSON data for sending to pilot browser
 * @return if _Ctx.jsonDRVinfos returns e_hasMsg::Ack else e_hasMsg::No
 */
e_hasMsg Driver::jsonDRVinfos(){
	static float xx=0;
	static float yy=0;
	static float zz=50;
	static float aa=0;
	static float rr=10;
	if (!_Ctx.jsonDRVinfos)
		return e_hasMsg::No;
	/*
	 char buffer[512];
	 int lenJson = serializeJson(*_jsonDoc, buffer, 512);
	 _SERIAL_0("\n%i %s", lenJson, buffer);
	 */
	JsonObject kmd = ((*_jsonDoc).as<JsonObject>())["KMD"];
	kmd["MID"] = "DRV";
	kmd[DEF_KF_EXE] = CTX.strRoverMode();
	kmd["OP"] = DEF_OP_INFO; // go straight on

	kmd[DEF_KF_SPD] = Motion.Speed;
	kmd[DEF_KF_DIR] = Motion.Dir;
//kmd["MTS"] = 54411073;
	kmd[DEF_KF_LFT] = Motion.Left;
	kmd[DEF_KF_RGT] = Motion.Right;
	if (_mustClearMAP)
		_mustClearMAP = false;
	else
		kmd[DEF_KF_PMTS] = _drv_Kine.Fusensor.microTS;
	kmd[DEF_KF_DIST] = _drv_Kine.Attitude.colDist;
	kmd[DEF_KF_IMUT] = _drv_Kine.Attitude.imuTik;
	kmd[DEF_KF_XC0] = _drv_Kine.Attitude.Xcur;
	kmd[DEF_KF_YC0] = _drv_Kine.Attitude.Ycur;
	kmd[DEF_KF_ZC0] = _drv_Kine.Attitude.Zcur;

	kmd[DEF_KF_XC0] = xx;
	kmd[DEF_KF_YC0] = yy;
	kmd[DEF_KF_ZC0] = zz;

	aa+=1;//1;
	rr+=.021;
	xx=rr*cos(PI*aa/180);
	yy=rr*sin(PI*aa/180);

	kmd[DEF_KF_SPX] = _drv_Kine.Attitude.Vx * 1000.0;
	kmd[DEF_KF_SPY] = _drv_Kine.Attitude.Vy * 1000.0;
	kmd[DEF_KF_SPZ] = _drv_Kine.Attitude.Vz * 1000.0;
	kmd[DEF_KF_LAX] = _drv_Kine.Attitude.Ax * 1000.0;
	kmd[DEF_KF_LAY] = _drv_Kine.Attitude.Ay * 1000.0;
	kmd[DEF_KF_LAZ] = _drv_Kine.Attitude.Az * 1000.0;
	kmd[DEF_KF_ROLL] = _drv_Kine.Attitude.Nroll * 180.0 / pi;
	kmd[DEF_KF_PITCH] = _drv_Kine.Attitude.Epitch * 180.0 / pi;
	kmd[DEF_KF_YAW] = _drv_Kine.Attitude.Dyaw * 180.0 / pi;
	kmd[DEF_KF_GRX] = _drv_Kine.Attitude.Grav_x * 1000.0;
	kmd[DEF_KF_GRY] = _drv_Kine.Attitude.Grav_y * 1000.0;
	kmd[DEF_KF_GRZ] = _drv_Kine.Attitude.Grav_z * 1000.0;
	kmd[DEF_KF_Iax] = _drv_Kine.Fusensor.ax;
	kmd[DEF_KF_Iay] = _drv_Kine.Fusensor.ay;
	kmd[DEF_KF_Iaz] = _drv_Kine.Fusensor.az;
	kmd[DEF_KF_Igx] = _drv_Kine.Fusensor.gx;
	kmd[DEF_KF_Igy] = _drv_Kine.Fusensor.gy;
	kmd[DEF_KF_Igz] = _drv_Kine.Fusensor.gz;
	kmd[DEF_KF_Imx] = _drv_Kine.Fusensor.mx;
	kmd[DEF_KF_Imy] = _drv_Kine.Fusensor.my;
	kmd[DEF_KF_Imz] = _drv_Kine.Fusensor.mz;
	kmd["temp"] = _drv_Kine.Fusensor.temp;
	kmd["Q0"] = _drv_Kine.Fusensor.q[0];
	kmd["Q1"] = _drv_Kine.Fusensor.q[1];
	kmd["Q2"] = _drv_Kine.Fusensor.q[2];
	kmd["Q3"] = _drv_Kine.Fusensor.q[3];
	/*
	 lenJson = serializeJson(*_jsonDoc, buffer, 512);
	 Serial.printf("\n%i %s", lenJson, buffer);
	 */
	return e_hasMsg::Ack; // just an info
}
///////////////////////////////////////////////////////////////////////
