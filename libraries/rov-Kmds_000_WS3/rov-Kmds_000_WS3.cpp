/** @file */
#include "rov-Kmds_000_WS3.h"
#include "esp32_GOODIES.h"
#include "esp32_TASKS.h"
#include "esp32_LOGS.h"
#include "esp32_SPRINT.h"
#include "esp32_FSTR.h"
#include "esp32_WIFI.h"
#include "000_WSCHK.h"
#include "rov-Kmds_000_WS3.h"
#include "000_I2C.h"
//#include "rov-Hard_IMU_000_WS3.h"
#include "../rov.CONTEXT/rov.CONTEXT.h"
#include "rov-Mod_Driver_000_WS3.h"
#include "rov-Mod_Motors_000_WS3.h"
c_myKMDS myKMDS;
int8_t c_myKMDS_dbglvl = 0;
static int8_t &this_DebugLVL = CTX.SOFTctx.FCT_DBGMAX_KMDS;
///////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------
c_myKMDS::c_myKMDS() :
	Core("KMD", &c_myKMDS_dbglvl){
	_inKMDSqueue = xQueueCreate(10, sizeof(s_kmdITM));
}
//---------------------------------------------------------------------
/**
 * @fn char* c_myKMDS::has_KMD()
 * @brief Pulls a command from _inKMDSqueue (if any) and delays the next one if command is Zmmmm in milliseconds
 * @return a low level command or nullptr if none
 */
char* c_myKMDS::has_KMD(){
	static s_kmdITM item; ///< To keep item during KMD processing !!!
	static uint64_t when = 0;///< Delay to wait for command execution
	if (when < milli_TS())
		when = 0;
	if ((!when) && (xQueueReceive(_inKMDSqueue, &item, 0) == pdPASS)) {
		if (CTX.SOFTctx.trace_UDP)
			_SERIAL_0("\nUDP item.kmdsMSG=`%s`", item.kmdsMSG);
		if (item.kmdsMSG[0] == 'Z') {
			when = milli_TS() + atoi(1 + item.kmdsMSG);
		}
		return item.kmdsMSG;
	}
	return nullptr;
}
//---------------------------------------------------------------------
/**
 * @fn void c_myKMDS::reset_ISR()
 * @brief restart the IMU in case of problem (I²C, recoverable error, etc...)
 */
void c_myKMDS::reset_ISR(){
	extern H_AccGyrMag *imu9250;

	_SERIAL_3(Lang.CST(I2C_), Lang.CST(____LINE_40_EQUAL____));
	DETACH(P_imuISR_);
	try {
		//_SERIAL_3("\n\t I²C busy: %s", SBOOL(Wire.busy()));
		if (!Wire.begin()) // set master mode, default on SDA/SCL for Ladybug
			THROWERR(I2C_DONTSTARTS);

		//_SERIAL_3("\n\t I²C busy: %s", SBOOL(Wire.busy()));
		delay(100);
//Clock Stretching by SFR02
		Wire.setClock(100000); // I2C frequency at 100 kHz
		_SERIAL_3(Lang.CST(I2C_CLOCK), Wire.getClock());
		delay(50);
		Wire.setClock(382000); // I2C frequency at 400 kHz
		_SERIAL_3(Lang.CST(I2C_CLOCK), Wire.getClock());
		delay(50);
#define PWR_MGMT_1 0x6B
#define MPU9250_1_ADDRESS 0x68
#define USER_CTRL        0x6A  // Bit 7 enable DMP, bit 3 reset DMP
#define  I2C_MST_EN	0x20
#define WHO_AM_I_MPU9250 0x75 // Should return 0x71
		_SERIAL_3("\n\tWHOAMI:'%i'", readByte(MPU9250_1_ADDRESS, WHO_AM_I_MPU9250));
		writeByte(MPU9250_1_ADDRESS, USER_CTRL, 0); // disable internal I2C bus
		writeByte(MPU9250_1_ADDRESS, PWR_MGMT_1, 0x80);
		//< @todo Find configured values MPU9250_?_ADDRESS & PWR_MGMT_? in 000_MPU9250
		delay(100); // Wait for all registers to reset
		writeByte(MPU9250_1_ADDRESS, USER_CTRL, I2C_MST_EN); // re-enable internal I2C bus
		delay(100); // Wait for all registers to reset
		_SERIAL_3("\n\tWHOAMI 100000:'%i'", readByte(MPU9250_1_ADDRESS, WHO_AM_I_MPU9250));
		Wire.setClock(382000); // I2C frequency at 400 kHz
		delay(50);
		_SERIAL_3("\n\tWHOAMI 400000:'%i'", readByte(MPU9250_1_ADDRESS, WHO_AM_I_MPU9250));
		delay(50);
		MPU9250 *mpu = new MPU9250(MPU9250_1_ADDRESS);
		mpu->resetMPU9250();
		delay(50);
		float SelfTest[6]; // holds results of gyro and accelerometer self test
		mpu->SelfTest(SelfTest); // Start by performing self test and reporting values
		_DBG_DRV_XIMU(
			"\n\tAcceleration xyz-axis (%6.3f,%6.3f,%6.3f) %% trim within of factory value",
			SelfTest[0], SelfTest[1], SelfTest[2]);
		_DBG_DRV_XIMU(
			"\n\tGyration     xyz-axis (%6.3f,%6.3f,%6.3f) %% trim within of factory value",
			SelfTest[3], SelfTest[4], SelfTest[5]);
		delay(50);
		_SERIAL_3("\n\tWHOAMI==>'%i'", mpu->getMPU9250ID());
		delay(50);
		delete mpu;
		/*
		 */
		int ni2c = I2Cscan();
		if (2 > ni2c)
			THROWERR(I2C_MISSING, ni2c);
	}
	catch (EXCEPT const &e) {
		FIX_ERROR();
	}
	if (imu9250)
		imu9250->init_IMU();
	ATTACH(P_imuISR_);
}
//---------------------------------------------------------------------
void plot(){
	for (size_t i = 0; i < 100; i++) {
		int v1 = 10;
		int v2 = i;
		int v3 = -i;
		Serial.printf("%i\t%i\t%i\r\n", v1, v2, v3);
		delay(1000);
	}
}
//---------------------------------------------------------------------
static int8_t sav_udpLvl_Fusensor = 0;///< Safeguard of udpLvl_Fusensor during telemetry sequence
static int8_t sav_udpLvl_Attitude = 0;///< Safeguard of udpLvl_Attitude during telemetry sequence
static int8_t sav_udpLvl_keyFrame = 0;///< Safeguard of udpLvl_keyFrame during telemetry sequence
//---------------------------------------------------------------------
/**
 * @fn void SET_outputLvl_(const int8_t VALUE, int8_t &WHOSE, const char * NAME)
 * @brief Changes the debug level after displaying the old value
 * @param VALUE new value
 * @param WHOSE address of debug level variable
 * @param NAME label to be human readable
 */
void SET_outputLvl_(const int8_t VALUE, int8_t &WHOSE, const char *NAME){
	_DBG_APP_KMDS("\ndebugLevel(%s) from %i to %i", NAME, WHOSE, VALUE);
	WHOSE = VALUE;
}
//---------------------------------------------------------------------
/**
 * @fn void SLEEP_udpLvl_Kine()
 * @brief Safeguards the UDP Kine levels during a telemetry sequence
 */
void SLEEP_udpLvl_Kine(){
	if (!sav_udpLvl_Fusensor)
		sav_udpLvl_Fusensor = CTX.HARDctx.kineCTX.udpLvl_Fusensor;
	if (!sav_udpLvl_Attitude)
		sav_udpLvl_Attitude = CTX.HARDctx.kineCTX.udpLvl_Attitude;
	if (!sav_udpLvl_keyFrame)
		sav_udpLvl_keyFrame = CTX.HARDctx.kineCTX.udpLvl_keyFrame;
	CTX.HARDctx.kineCTX.udpLvl_Fusensor = 0;
	CTX.HARDctx.kineCTX.udpLvl_Attitude = 0;
	CTX.HARDctx.kineCTX.udpLvl_keyFrame = 0;
}
//---------------------------------------------------------------------
/**
 * @fn void AWAKE_udpLvl_Kine()
 * @brief Restores the UDP Kine levels at end of a telemetry sequence
 */
void AWAKE_udpLvl_Kine(){
	if (sav_udpLvl_Fusensor)
		CTX.HARDctx.kineCTX.udpLvl_Fusensor = sav_udpLvl_Fusensor;
	if (sav_udpLvl_Attitude)
		CTX.HARDctx.kineCTX.udpLvl_Attitude = sav_udpLvl_Attitude;
	if (sav_udpLvl_keyFrame)
		CTX.HARDctx.kineCTX.udpLvl_keyFrame = sav_udpLvl_keyFrame;
}
//---------------------------------------------------------------------
/**
 * @fn void start_telemetry(const e_telemetry WHICH)
 * @brief Saves the UDP Kine levels & sets CTX.SOFTctx.DRVctx.telemetry
 * @param WHICH 1:10Hz driver telemetry, 2:100Hz isr telemetry
 */
void start_telemetry(const e_telemetry WHICH){
	SLEEP_udpLvl_Kine();
	CTX.SOFTctx.DRVctx.telemetry = WHICH;
}
//---------------------------------------------------------------------
/**
 * @fn void close_telemetry()
 * @brief Restores the UDP Kine levels & clear CTX.SOFTctx.DRVctx.telemetry
 * @param WHICH 1:10Hz driver telemetry, 2:100Hz isr telemetry
 */
void close_telemetry(){
	CTX.SOFTctx.DRVctx.telemetry = e_telemetry::no;
	AWAKE_udpLvl_Kine();
}
///////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------
/**
 * @fn String helpMenu()
 * @brief serial or UDP display of commands syntax
 * @return String below
 */
String helpMenu(){
	String hMenu;
	/**
	 * @code
	 */
	hMenu += (" ========= MENU ===============");
	hMenu += ("\n '?' Display this help (no case sensitive)");
	hMenu += ("\n 'n' n € [0..9] TRUNCATE serial debug level");
	hMenu += ("\n 'A' APPEND on a CSV file");
	hMenu += ("\n 'B' BEGIN a new CSV file");
	hMenu += ("\n 'C' CLOSE the CSV file");
	hMenu += ("\n 'D' DASHBOARD control (no option toggle dump or no dump)");
	hMenu += ("\n      D[1|2|3] 1:10Hz driver only, 2:100Hz isr only, 3:twice");
	hMenu += ("\n 'E' EPROM context for [B(oot),H(ard),S(oft)]");
	hMenu += ("\n      E[[B[L|S]]|[H[xn]]|[S[xn]]] x € [L(oad),S(ave)]");
	hMenu += ("\n      Load: n € [0,1,2], 0 reinits to default values");
	hMenu += ("\n      Save: n € [1,2], ex: EHL0 reinit hard context");
	hMenu += ("\n 'F'                           ");
	hMenu += ("\n 'G'                           ");
	hMenu += ("\n 'H' HIBERNATE to admin rover");
	hMenu += ("\n 'I' IMU management");
	hMenu += ("\n      I[A|D|R|I|M[:ssss]] Without option swaps IMU simul<->i²c");
	hMenu += ("\n      IA attaches, ID detaches IMU, IR restart");
	hMenu += ("\n      II Inertial calibrate, IM Magneto calibrate for ssss samples");
	hMenu += ("\n 'J' JUNCTIONS dump");
	hMenu += ("\n 'K'                           ");
	hMenu += ("\n 'L'                           ");
	hMenu += ("\n 'M' MODULES dump or set debug level");
	hMenu += ("\n      M[l[m[s]]] l € [0...9] for all modules if no m option");
	hMenu += ("\n      or for one m € [D(rv),J(ob),M(ot),T(ms),W(at)]");
	hMenu += ("\n      if [s] for one sub module ex: m5dk Driver/Kine");
	hMenu += ("\n 'N'                           ");
	hMenu += ("\n 'O'                           ");
	hMenu += ("\n 'P'                           ");
	hMenu += ("\n 'Q'                           ");
	hMenu += ("\n 'R' ROVER mode control management");
	hMenu += ("\n      R[[D|L|R|W]] No option reset position & velocity, else");
	hMenu += ("\n      toggle (RD:Driving, RL:Learning, RR:Roaming, RW:Working)");
	hMenu += ("\n 'S'                           ");
	hMenu += ("\n 'T' TASKS, dump tasks if no l option or set debug level");
	hMenu += ("\n      T[l[t]] l € [0...9] for all tasks if no t option");
	hMenu += ("\n      or for one t € [A(pp),I(mu),M(od),R(cv),S(nd)]");
	hMenu += ("\n 'U' Toggle serial udp fifo, send & keep-alive");
	hMenu += ("\n 'V' View serial openGL attitude 0:no > 1:driver > 2:isr > 0");
	hMenu += ("\n 'W' Toggle to display web socket Data/Event/Fifo");
	hMenu += ("\n      W[x] x € [D(ata),E(vent),F(ifo)] (else toggle all)");
	hMenu += ("\n 'X' webSokSrv.cleanupClients()");
	hMenu += ("\n 'Y' List STA clients of AP");
	hMenu += ("\n 'Z' Reserved for delayed has_KMD()");
	hMenu += ("\n '@' ESP.restart() !!!");
	hMenu += ("\n ------------------------------");
	hMenu += ("\n -Hit a choice or '?' to quit wizard menu");
	hMenu += ("\n ==============================");
	/**
	 * @endcode
	 */
	return hMenu;
}
///////////////////////////////////////////////////////////////////////
/**
 * @todo Receive command from web socket (ex: onMESSAGE to KMDS fifo as stop mission or others...)
 */
/*
 bool DATAGreceive ( const String  KMD ){
 // if JSON goto fifo modules else
 return KMDS(KMD.c_str());
 }
 */
bool c_myKMDS::KMDS(const char *KMD){
	this_DebugLVL = *registerFCT(e_tasks::KMDS, "Kmds", &CTX.SOFTctx.FCT_DBGMAX_KMDS);
	static bool Menu = false;
	extern AsyncWebSocket webSokSrv;
	bool syntax;
	bool carOK;
	do {
		syntax = true;
		carOK = true;
		String str;
		if (KMD != nullptr) {
			_DBG_APP_KMDS("\n\n>>UDP: ");
			str = String(KMD);
			KMD = nullptr; // reset for next loop task
		} else {
			if (Menu) {
				_DBG_APP_KMDS("\n\n>>KMD? ");
				do {
					str = "";
					vTaskDelay(500 / portTICK_PERIOD_MS); // to give time to others tasks
//					delay(500);
					char *kmd = has_KMD();
					if (kmd)
						str = String(kmd);
					else
						str = inString();
				} while (str.length() == 0);
			}
			else
				str = inString();
		}
		vector<String> args;
		char cmd = '\0';
		if (str.length() > 0) {
			if (!Menu)
				_DBG_APP_KMDS("\n\n>>KMD= ");
			_DBG_APP_KMDS("`%s` *** ", str.c_str());
			_DBG_APP_KMDS(Lang.CST(HEAP_STAT), heapState().c_str());
			_DBG_APP_KMDS(" ***\n", str.c_str());
			myWIFI.Status();
			cmd = splitString(args, str);
		}
		if ((32 <= cmd) && (cmd <= 122)) {
			String ARG = args[0];
			ARG.toUpperCase();
			switch(toupper(cmd)){
				case '0' ... '9': {
					CTX.SOFTctx.ALL_DBGMAX_TRUNC = d09(ARG[0]); // fault with d09(cmd)???
					break;
				}
				case '?': {
					if (!Menu) {
						DETACH(P_imuISR_);
						vTaskSuspend(findCORE()->TH);
						//vTaskSuspend(findYLOG()->TH);// maybe nullptr!!!
						_DBG_APP_KMDS("\n%s", helpMenu().c_str());
						Menu = true;
					} else {
						_DBG_APP_KMDS("\nLEAVING MENU");
						//vTaskResume(findYLOG()->TH);
						vTaskResume(findCORE()->TH);
						ATTACH(P_imuISR_); // define interrupt for intPin output of MPU9250
						Menu = false;
					}
					break;
				}
				case '@': {
					ESP.restart();
					break;
				}
				case '_': {
					CTX.HARDctx.ximuCTX.adjust_Bias = true;
					break;
				}
				case 'A': {
					if ((ARG.length() == 2) && (isDigit(ARG[1]))
						&& (1 <= d09(ARG[1])) && (d09(ARG[1] <= 2))) {
						_DBG_APP_KMDS("\n@APPND:telemetry.csv %c", ARG[1]);
						int16_t cnt;
						char *buffer = allocBuf(128, &cnt);
						SPrintF(128, cnt, buffer, "@APPND:telemetry.csv");
						if (myLOGS._UDP_(buffer)) {
							start_telemetry((e_telemetry)d09(ARG[1]));
						}
					} else
						syntax = false;
					break;
				}
				case 'B': {
					if ((ARG.length() == 2) && (isDigit(ARG[1]))
						&& (1 <= d09(ARG[1])) && (d09(ARG[1] <= 2))) {
						_DBG_APP_KMDS("\n@BEGIN:telemetry.csv %c", ARG[1]);
						int16_t cnt;
						char *buffer = allocBuf(128, &cnt);
						SPrintF(128, cnt, buffer, "@BEGIN:telemetry.csv");
						if (myLOGS._UDP_(buffer)) {
							start_telemetry((e_telemetry)d09(ARG[1]));
							buffer = allocBuf(256, &cnt); // an other buffer !!!
							SPrintF(256, cnt, buffer,
								"@TITLE:Call;MTS;Ax(m/s²);Ay(m/s²);Az(m/s²);Roll(°);Pitch(°);Yaw(°);grX(mg);grY(mg);grZ(mg);Q0;Q1;Q2;Q3;Speed;Dir;Left;Right\n");
//							"@TITLE:Call;MTS;Ax;Ay;Az;gx;gy;gz;mx;my;mz;Q0;Q1;Q2;Q3;Speed;Dir;Left;Right\n");
//							"@TITLE:Call;MTS;ax;ay;az;gx;gy;gz;mx;my;mz;Q0;Q1;Q2;Q3;m00;m01;m02;m03;m10;m11;m12;m13;m20;m21;m22;m23;m30;m31;m32;m33\n");
							myLOGS._UDP_(buffer);
						}
					} else
						syntax = false;
					break;
				}
				case 'C': {
					_DBG_APP_KMDS("\n@CLOSE:telemetry.csv");
					int16_t cnt;
					char *buffer = allocBuf(128, &cnt);
					SPrintF(128, cnt, buffer, "@CLOSE:telemetry.csv");
					if (myLOGS._UDP_(buffer)) {
						close_telemetry();
					}
					break;
				}
				case 'D': {
					if ((ARG.length() == 2) && (isDigit(ARG[1]))) {
						CTX.SOFTctx.DRVctx.udpLvl_Kine = d09(ARG[1]);
						_DBG_APP_KMDS("\nPassing udpLvl_Kine to %i", CTX.SOFTctx.DRVctx.udpLvl_Kine);
					} else
						syntax = false;
					break;
				}
				case 'E': {
					if (ARG.length() == 1) { // E
						_DBG_APP_KMDS("\n%s", CTX.dumpBOOTctx().c_str());
						_DBG_APP_KMDS("\n%s", CTX.dumpSOFTctx().c_str());
						_DBG_APP_KMDS("\n%s", CTX.dumpHARDctx().c_str());
					} else if (ARG.length() == 2) { // EB | EH | ES
						if (ARG[1] == 'B')
							_DBG_APP_KMDS("\n%s", CTX.dumpBOOTctx().c_str());
						else if (ARG[1] == 'H')
							_DBG_APP_KMDS("\n%s", CTX.dumpHARDctx().c_str());
						else if (ARG[1] == 'S')
							_DBG_APP_KMDS("\n%s", CTX.dumpSOFTctx().c_str());
						else
							syntax = false;
					} else if ((ARG[1] == 'B') && (ARG.length() == 3)) {
						if (ARG[2] == 'L') // EBL load
							CTX.LOAD();
						else if (ARG[2] == 'S') // EBS save
							CTX.SAVE();
						else
							syntax = false;
					} else if ((ARG.length() == 4) && (isDigit(ARG[3]))
						&& ((ARG[2] == 'L') || (ARG[2] == 'S'))) {
						if (ARG[1] == 'H') {
							if (ARG[2] == 'L') { // EHL0 EHL1 EHL2 load hard
								if (ARG[3] == '0')
									CTX.HARDctx = s_HARDctx { };
								else if (ARG[3] == '1')
									Eload(CTX.E_HARD1, CTX.HARDctx);
								else if (ARG[3] == '2')
									Eload(CTX.E_HARD2, CTX.HARDctx);
								else
									syntax = false;
							} else if (ARG[2] == 'S') { // EHS1 EHS2 save hard
								if (ARG[3] == '1')
									Esave(CTX.E_HARD1, CTX.HARDctx, '1');
								else if (ARG[3] == '2')
									Esave(CTX.E_HARD2, CTX.HARDctx, '2');
								else
									syntax = false;
							} else
								syntax = false;
							if (syntax)
								CTX.setBootHARD(d09(ARG[3]));
						} else if (ARG[1] == 'S') {
							if (ARG[2] == 'L') { // ESL0 ESL1 ESL2 load soft
								if (ARG[3] == '0')
									CTX.SOFTctx = s_SOFTctx { };
								else if (ARG[3] == '1')
									Eload(CTX.E_SOFT1, CTX.SOFTctx);
								else if (ARG[3] == '2')
									Eload(CTX.E_SOFT2, CTX.SOFTctx);
								else
									syntax = false;
							} else if (ARG[2] == 'S') { // ESS1 ESS2 save soft
								if (ARG[3] == '1')
									Esave(CTX.E_SOFT1, CTX.SOFTctx, '1');
								else if (ARG[3] == '2')
									Esave(CTX.E_SOFT2, CTX.SOFTctx, '2');
								else
									syntax = false;
							} else
								syntax = false;
							if (syntax)
								CTX.setBootSOFT(d09(ARG[3]));
						}
						_DBG_APP_KMDS("\n%s", CTX.dumpBOOTctx().c_str());
					} else
						syntax = false;
					break;
				}
				case 'H': {
					_DBG_APP_KMDS("\nHibernate to admin rover");
					break;
				}
				case 'I': {
					if (ARG.length() == 1) {
						CTX.SOFTctx.DRVctx.simul_IMU = !CTX.SOFTctx.DRVctx.simul_IMU;
					} else {
						if ('D' == ARG[1]) {
							DETACH(P_imuISR_);
						} else if ('A' == ARG[1]) {
							ATTACH(P_imuISR_);
						} else if ('R' == ARG[1]) {
							reset_ISR();
						} else if ('I' == ARG[1]) {
							DETACH(P_imuISR_);

							Module *mod = findModule("DRV");
							Driver &driver = *reinterpret_cast<Driver*>(mod);
							driver.calib_AccGyr();
							if (!Menu)
								ATTACH(P_imuISR_);
						} else if ('M' == ARG[1]) {
							int16_t samples=0;
							if(args.size()==2)// number of samples
								samples=Int16(args[1].c_str());
							_DBG_APP_KMDS("\nMAGNETO calibration for %i samples",samples);
							DETACH(P_imuISR_);

							Module *mod = findModule("DRV");
							Driver &driver = *reinterpret_cast<Driver*>(mod);
							driver.calib_Compas(samples);
							if (!Menu)
								ATTACH(P_imuISR_);
						} else
							syntax = false; // unknown
					}
					break;
				}
				case 'J': {
					_DBG_APP_KMDS("\n%s", dumpAllJUNCTIONS().c_str());
					break;
				}
				case 'M': {
					if (ARG.length() == 1) {
						_DBG_APP_KMDS("\n%s", dumpAllMODULES().c_str());
					} else if (isDigit(ARG[1])) {
						int8_t lvl = d09(ARG[1]);
						if (ARG.length() == 2) {
							for (auto it = modulesMap.begin(); it != modulesMap.end(); ++it) {
								SET_outputLvl_(lvl, it->second->adr_dbgLVL(),
									it->second->_Mid.c_str());
							}
						} else if (ARG.length() >= 3) {
							Module *mod = findModule0(ARG[2]);
							if (mod != nullptr) {
								if (ARG.length() == 3) {
									SET_outputLvl_(lvl, mod->adr_dbgLVL(), mod->_Mid.c_str());
								} else if (ARG.length() == 4) {
									if (mod->_Mid.equals("DRV")) {
										Driver &driver = *reinterpret_cast<Driver*>(mod);
										switch(ARG[3]){
											case 'Q': {
												SET_outputLvl_(lvl,
													CTX.HARDctx.kineCTX.udpLvl_Fusensor,
													"DRV:IMUsensor");
												break;
											}
											case 'A': {
												SET_outputLvl_(lvl,
													CTX.HARDctx.kineCTX.udpLvl_Attitude,
													"DRV:Attitude");
												break;
											}
											case 'K': {
												SET_outputLvl_(lvl,
													CTX.HARDctx.kineCTX.udpLvl_keyFrame,
													"DRV:kfMatrix");
												break;
											}
											default:
												syntax = false; // DRV has no this SUB module
										}
									} else if (mod->_Mid.equals("MOT")) {
										switch(ARG[3]){
											case 'Q': {
												SET_outputLvl_(lvl,
													CTX.HARDctx.xpidCTX.udpLvl_PIDrq,
													"MOT:ringQueue");
												break;
											}
											default:
												syntax = false; // MOT has no this SUB module
										}
									}
								} else
									syntax = false; // ARG too long
							} else
								syntax = false; // Module no found
						}
					} else
						syntax = false; // ARG[1] !isDigit
					break;
				}
				case 'P': {
					CTX.HARDctx.ximuCTX.beta += 0.0001;
					_DBG_APP_KMDS("\nbeta=%10.5f", CTX.HARDctx.ximuCTX.beta);
					break;
				}
				case 'Q': {
					CTX.HARDctx.ximuCTX.beta -= 0.0001;
					_DBG_APP_KMDS("\nbeta=%10.5f", CTX.HARDctx.ximuCTX.beta);
					break;
				}
				case 'R': {
					if (ARG.length() == 1) { // R
						CTX.SOFTctx.DRVctx.clear_CineRover = true; // Done in Driver::complete_MSG
					} else if (ARG.length() == 2) { // RD | RI | RL | RR | RW
						if (CTX.getRoverMode() == e_roverMode::error) {
							_DBG_APP_KMDS("\nDriver is in \"%s\" mode ERROR !!!",
								CTX.strRoverMode());
							//break;
						} //else
						if (ARG[1] == 'D') { // Driving
							CTX.setRoverMode(e_roverMode::driving);
						} else if (ARG[1] == 'I') { // Idle
							CTX.setRoverMode(e_roverMode::idle);
						} else if (ARG[1] == 'L') { // Learning
							CTX.setRoverMode(e_roverMode::learning);
						} else if (ARG[1] == 'R') { // Roaming
							CTX.setRoverMode(e_roverMode::roaming);
						} else if (ARG[1] == 'W') { // Working
							CTX.setRoverMode(e_roverMode::working);
						} else {
							syntax = false;
							CTX.setRoverMode(e_roverMode::error);
						}
						_DBG_APP_KMDS("\nDriver passing in \"%s\" mode", CTX.strRoverMode());
					} else
						syntax = false;
					break;
				}
				case 'T': {
					if (ARG.length() == 1) {
						_DBG_APP_KMDS("\n%s", dump_TASKS().c_str());
					} else if (isDigit(ARG[1])) {
						if (ARG.length() == 2) {
							for (size_t i = 0; i < static_cast<int>(e_tasks::END); i++) {
								s_tskInfo_X *ti =
									&tskMON[static_cast<std::underlying_type<e_tasks>::type>(i)];
								if (ti->TH)
									SET_outputLvl_(d09(ARG[1]), *(ti->DebugLVL), ti->name);
							}
						} else if (ARG.length() == 3) {
							s_tskInfo_X *ti = tskFind(ARG[2]);
							if (ti->TH) {
								SET_outputLvl_(d09(ARG[1]), *(ti->DebugLVL), ti->name);
							} else
								syntax = false; // s_tskInfo_X not found
						} else
							syntax = false; // ARG too long
					} else
						syntax = false; // ARG[1] !isDigit
					break;
				}
				case 'U': {
					CTX.SOFTctx.trace_UDP = !CTX.SOFTctx.trace_UDP;
					break;
				}
				case 'V': {
					CTX.SOFTctx.DRVctx.openGL += 1;
					if (2 < CTX.SOFTctx.DRVctx.openGL)
						CTX.SOFTctx.DRVctx.openGL = 0;
					break;
				}
				case 'W': {
					if (ARG.length() == 1) {
						CTX.SOFTctx.showWSX_data = !CTX.SOFTctx.showWSX_data;
						CTX.SOFTctx.showWSX_event = !CTX.SOFTctx.showWSX_event;
						CTX.SOFTctx.showWSX_fifo = !CTX.SOFTctx.showWSX_fifo;
					} else if (ARG.length() == 2) {
						if (ARG[1] == 'D')
							CTX.SOFTctx.showWSX_data = !CTX.SOFTctx.showWSX_data;
						else if (ARG[1] == 'E')
							CTX.SOFTctx.showWSX_event = !CTX.SOFTctx.showWSX_event;
						else if (ARG[1] == 'F')
							CTX.SOFTctx.showWSX_fifo = !CTX.SOFTctx.showWSX_fifo;
						else
							syntax = false; // not W[|D|E|F]
						//bool Menu = false;
					} else
						syntax = false; // too long
					_DBG_APP_KMDS("\nSwitching (Data,Event,Fifo) to (%i,%i,%i)",
						CTX.SOFTctx.showWSX_data, CTX.SOFTctx.showWSX_event,
						CTX.SOFTctx.showWSX_fifo);
					break;
				}
				case 'X': {
					_DBG_APP_KMDS("\nMENU:webSokSrv.cleanupClients");
					webSokSrv.cleanupClients();
					break;
				}
				case 'Y': {
					_DBG_APP_KMDS(listStaClients().c_str());
					break;
				}
				case 'Z': {
					// Reserved for delayed has_KMD()
					break;
				}
				default: {
					carOK = false;
					break;
				}
			}
			if (!syntax)
				_DBG_APP_KMDS("\nMENU:Syntax error `%s`", str.c_str());
			if (Menu) {
				if (!carOK)
					_DBG_APP_KMDS("\nMENU: Commande inconnue(%s)", str.c_str());
			}
		}
	} while (Menu);
	return syntax && carOK;
}
///////////////////////////////////////////////////////////////////////
