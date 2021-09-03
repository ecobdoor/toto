#include "rov-Mod_Driver_000_WS3.h"
#include "esp32_LOGS.h"
extern c_myUDP myUDP;
///////////////////////////////////////////////////////////////////////
Driver::Driver(Flow *FLOW, const s_MODULE MOD_CFG, s_DRVctx *CONTEXT,
	Kine_1 *KINE, H_AccGyrMag *IMU, H_ultraSounds *USD, int8_t *DBGMAX)
:
	Module(FLOW, MOD_CFG, DBGMAX),
		_Kine(*KINE),
		_Ximu(*IMU),
		_usdDrv(*USD),
		_Ctx(*CONTEXT){
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
	clear_Cinematique_Rover();
	_isRunning = true; // ALWAYS !!!
}
//---------------------------------------------------------------------
void Driver::clear_Cinematique_Rover(){
	if (_Simimu != nullptr)
		_Simimu->Reset();
	_Ximu.Clear_IMU_quaternion();
	_Kine.clear_Kine_Rover();
}
//---------------------------------------------------------------------
void Driver::json_Cinematic(){
	JsonObject kmd = ((*_jsonDoc).as<JsonObject>())["KMD"];
	kmd["MID"] = "DRV";
	kmd["OP"] = DEF_OP_INFO; // go straight on
	kmd[DEF_KF_PMTS] = _Kine._last_Attitude.isrMicroTS;
	kmd[DEF_KF_DIST] = _Kine._last_Attitude.colDist;
	kmd[DEF_KF_IMUT] = _Kine._last_Attitude.imuTik;
	kmd[DEF_KF_XC0] = _Kine._last_Attitude.Xcur;
	kmd[DEF_KF_YC0] = _Kine._last_Attitude.Ycur;
	kmd[DEF_KF_ZC0] = _Kine._last_Attitude.Zcur;
	kmd[DEF_KF_SPX] = _Kine._last_Attitude.lin_Vx * 1000.0;
	kmd[DEF_KF_SPY] = _Kine._last_Attitude.lin_Vy * 1000.0;
	kmd[DEF_KF_SPZ] = _Kine._last_Attitude.lin_Vz * 1000.0;
	kmd[DEF_KF_LAX] = _Kine._last_Attitude.lin_ax * 1000.0;
	kmd[DEF_KF_LAY] = _Kine._last_Attitude.lin_ay * 1000.0;
	kmd[DEF_KF_LAZ] = _Kine._last_Attitude.lin_az * 1000.0;
	kmd[DEF_KF_ROLL] = _Kine._last_Attitude.Nroll * 180.0 / pi;
	kmd[DEF_KF_PITCH] = _Kine._last_Attitude.Epitch * 180.0 / pi;
	kmd[DEF_KF_YAW] = _Kine._last_Attitude.Dyaw * 180.0 / pi;
	kmd[DEF_KF_GRX] = _Kine._last_Attitude.Grav_x * 1000.0;
	kmd[DEF_KF_GRY] = _Kine._last_Attitude.Grav_y * 1000.0;
	kmd[DEF_KF_GRZ] = _Kine._last_Attitude.Grav_z * 1000.0;
	kmd[DEF_KF_Iax] = _last_rtFusensors.ax;
	kmd[DEF_KF_Iay] = _last_rtFusensors.ay;
	kmd[DEF_KF_Iaz] = _last_rtFusensors.az;
	kmd[DEF_KF_Igx] = _last_rtFusensors.gx;
	kmd[DEF_KF_Igy] = _last_rtFusensors.gy;
	kmd[DEF_KF_Igz] = _last_rtFusensors.gz;
	kmd[DEF_KF_Imx] = _last_rtFusensors.mx;
	kmd[DEF_KF_Imy] = _last_rtFusensors.my;
	kmd[DEF_KF_Imz] = _last_rtFusensors.mz;
}
//---------------------------------------------------------------------
/**
 * \fn void updateTrajectory(kineModelData & K)
 * \brief Updates X,Y,Z,A,B,C trajectory of rover
 * \param kineModelData & K
 */
/*
 void Driver::updateTrajectory(kineModelData & K) {
 float dMts = (K.isrMicroTS - _prevMTS) / 1000000.0f; // micro seconds time;
 _prevMTS = K.isrMicroTS;
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
void Driver::acq_trueIMU(const uint64_t ISR_MICRO_TS){
	_Ximu.acq_trueIMU(ISR_MICRO_TS);
}
//---------------------------------------------------------------------
/**
 * \fn void Driver::push_RTdata()
 * \brief Every hardware interrupt or simulated, push one \ref s_fusensors: IMU data & USD distance if (_needsUSD) in inter-tasks RT fifo
 *
 * WARNING: At each IMU interrupt, the serial line (115000 baud) can output 100 characters
 * every 10 ms at most with nothing else to do. In practice only a short message of a few
 * characters is only possible (no more than 2 ms). Detailed information is only visible in UDP.
 */
void Driver::push_RTdata(){
	_imuTIK++;
	s_fusensors &item = _last_rtFusensors;
	item.tik = _imuTIK;
	extern QueueHandle_t _IMUqueue;
	_Ximu.get_IMU(item);
	_Ximu.telemetry(item);

	//static int tst=0;	tst=(tst+1)%10;	_needsUSD = tst==0;
	if (_needsUSD) {
		_needsUSD = false;
		item.distUSD = _usdDrv.Get();
//		_last_rtFusensors.USD = true;
		item.USD = true;
		_DBG_DRV_FIFO("\n>%i %f", uxQueueMessagesWaiting(_IMUqueue), item.distUSD);
	} else {
		_DBG_DRV_FIFO("\n>%i ...", uxQueueMessagesWaiting(_IMUqueue));
	}
	item.fid = uxQueueMessagesWaiting(_IMUqueue);
	try {
		BaseType_t xStatus = pdPASS; // xQueueSend(_IMUqueue, &item, 0);
		if (xStatus != pdPASS) {
			//throw String("FATAL_IMUqueue OVERFLOW !");
		}
		if (pdPASS != xQueueSend(_IMUqueue, &item, 0)) {
			THROWERR(FIFO_OVERFLOW, _imuTIK, item.isrMicroTS);
		}
		_Kine.updateRoverKine(item);

//		_SERIAL_1("\nIMU>%i H_%i", uxQueueMessagesWaiting(_IMUqueue), ESP.getMaxAllocHeap()); en cas de souci...
		_SERIAL_1("\nIMU>%i", uxQueueMessagesWaiting(_IMUqueue));
		if (myUDP.isOpen()) {
			_DBG_DRV_PUSH("\n%s\n%s\n%s",
				_Ximu.dumpPush_FUSENSOR(item).c_str(),
				_Kine.dumpPush_ATTITUDE(_Kine._last_Attitude).c_str(),
				_Kine.dumpPush_KEYFRAME(_Kine._last_Keyframe).c_str());
		}
	} catch (EXCEPT const &e) {
		FIX_ERROR();
	}
}
//---------------------------------------------------------------------
/**
 * \fn void Driver::pull_RTdata()
 * \brief Every driver time out, pull all \ref s_fusensors: IMU data & USD distance if (_needsUSD) from inter-tasks RT fifo
 * \return USD distance
 */
uint8_t
Driver::pull_RTdata()
{
	//return;
	float dist = -1;
	extern QueueHandle_t _IMUqueue;
	uint8_t cnt = 0;
	_last_rtFusensors.isrMicroTS = micro_TS(); // to prevent too big dt start with first empty _IMUqueue
	while (xQueueReceive(_IMUqueue, &_last_rtFusensors, 0) == pdPASS) {
		/*
		 kineModelData K;
		 K.Acc = { {_last_rtFusensors.ax,_last_rtFusensors.ay,_last_rtFusensors.az}};
		 K.Mag = { {_last_rtFusensors.mx,_last_rtFusensors.my,_last_rtFusensors.mz}};
		 K.Gyr = { {_last_rtFusensors.gx,_last_rtFusensors.gy,_last_rtFusensors.gz}};
		 K.isrMicroTS = _last_rtFusensors.isrMicroTS;
		 _Kine.Predict(K);
		 _Kine.push(K);
		 updateTrajectory(K);
		 */
		_DBG_DRV_FIFO("\n<%i %f %s", _last_rtFusensors.fid, _last_rtFusensors.ax,
			_last_rtFusensors.USD ? String(_last_rtFusensors.distUSD) : "");
		if (_last_rtFusensors.USD) {
			dist = _last_rtFusensors.distUSD; // to pass on the true _last_rtFusensors
		}
		cnt++;
	}
	if (dist != -1) {
		_last_rtFusensors.USD = true;
		_last_rtFusensors.distUSD = dist;
	}
	return cnt;
}
//---------------------------------------------------------------------
/**
 * @fn bool Driver::complete_MSG(JsonObject &KMD)
 * @brief explore (frq 10 Hz)
 * Every 100 ms acquire USD distance & pull RT interrupt I²C IMU data
 * To test motors & Proportional–Integral–Derivative regulation behaviour we do that:
 * If ultrasonic sensor measures a distance less than 0.35 meter, an avoidance strategy is chosen
 * Either the rover rolls back
 * Or the rover turns right
 * Or the rover turns left
 * As soon as there are no more obstacles, the rover starts moving forward again
 * @param KMD
 * @return true to send a client JSON
 */
bool Driver::complete_MSG(JsonObject &KMD){
	try {
//
// PULL & UPDATE I2C SENSORS
//
		uint8_t cnt = pull_RTdata();
		if (cnt < 5)
			_LOG_(0, 0, "\nPoor pull_RTdata (cnt=%i)", cnt);
		if (!_last_rtFusensors.USD)
			_LOG_(0, 0, "\nRadar not found");
		_needsUSD = true; // to push_RTdata(const uint64_t ISR_MICRO_TS) next time...
//		_Kine.dump_KINE();
//	_DBG_DRV_PUSH("\n%s\npullCnt=%i ", _Ximu.dumpPush_FUSENSOR(_last_rtFusensors).c_str(), cnt);
		if (_Ctx.clear_CineRover) {
			clear_Cinematique_Rover();
			_Ctx.clear_CineRover = false;
		}
//	else _Kine.updateRoverKine(_last_rtFusensors, _Ximu._LMD);
//		_SERIAL_3( "%s", dumpPush_ATTITUDE().c_str());
//		return true; //no Send

//
// UPDATE ANTI COLLIDING
		/*
		 _speed = 200;
		 _dir = 0;
		 */
//_SERIAL_4( "\n+++++++++++++ %f m", _last_rtFusensors.distUSD);
		if (_last_rtFusensors.distUSD < 0.35) {
			if (!_colliding) {
				_colliding = true;
				_dir = -(rand() % 3 - 1) * 128;
				if (_dir == 0) {
					_speed = -128;
				} else {
					_speed = 0;
				}
			}
		} else {
			_colliding = false;
			_speed = 200;
			_dir = 0;
		}
//
// UPDATE PROPULSION
//
		switch(drvMode){
			case e_drvMod::explore: {
				KMD["MID"] = "MOT"; // Force the tmsout_MSG to be a motors query
				//_SERIAL_3( "\n[%s]", dumpJSON().c_str());
				JsonObject root = (*_jsonDoc).as<JsonObject>();
				root["KMD"]["OP"] = DEF_OP_MOT; // go straight on

				root["KMD"][DEF_KF_SPD] = _speed;
				root["KMD"][DEF_KF_DIR] = _dir;
				root["KMD"][DEF_KF_PMTS] = _last_rtFusensors.isrMicroTS;
				//_SERIAL_3( "\n[%s]", dumpJSON().c_str());
				// Execute the OP and get back the reply
				reply2query_PILOT(root);
				// ack the local motors query
//			_SERIAL_3( "\n[%s]", dumpJSON().c_str());
				//
				// UPDATE CLIENT DRIVER
				//
				json_Cinematic(); // Force the query to be a Driver query
//			_SERIAL_3( "\n[%s]", dumpJSON().c_str());
//		_Flow.ack(root["CTL"]["TIK"].as<uint32_t>());
				//root["CTL"]["TYP"] = DEF_REPLY_ROVER_TO_PILOT;

//			tmsout_MSG(DTMS); //a DRIVER query !!! return true no matter... no send yet
//		externValues(_last_rtFusensors);
				break;
			}
			case e_drvMod::jobgoal:
				break;
			case e_drvMod::driveMap:
				break;
			case e_drvMod::driveMot:
				break;
			case e_drvMod::learning:
				break;
			default: // _UNKNOWN???
				;
		}
	}
	catch (EXCEPT const &e) {
		//FIX_ERROR(); //
		THROW_UP();
	}
	return false;// just an info
}
//---------------------------------------------------------------------
/**
 * Works at core task varying speed loop. Use only for emergency situation
 */
bool Driver::auto_MSG(const uint64_t DTMS){
	return false;
}
//---------------------------------------------------------------------
bool Driver::reply2pilotQ_OP(JsonObject &JOBJ){
	return true;
}
///////////////////////////////////////////////////////////////////////
