#include "rov-Kmds_000_WS3.h"
#include "esp32_TASKS.h"
#include "esp32_LOGS.h"
#include "000_WSCHK.h"
#include "rov-Kmds_000_WS3.h"

#include "../rov.CONTEXT/rov.CONTEXT.h"
#include "rov-Mod_Driver_000_WS3.h"
/** @file */
/**
 * @fn void helpMenu()
 * @brief serial display of commands syntax
 */
String helpMenu(){
	String hMenu;
	hMenu += (" ========= MENU ===============");
	hMenu += ("\n '?' Display this help (no case sensitive)");
	hMenu += ("\n 'n' n € [0..9] TRUNCATE serial debug leve");
	hMenu += ("\n 'A' APPEND on a CSV file");
	hMenu += ("\n 'B' BEGIN a new CSV file");
	hMenu += ("\n 'C' CLOSE the CSV file");
	hMenu += ("\n 'D' DATA write in CSV file");
	hMenu += ("\n 'E' EPROM context for [B(oot),H(ard),S(oft)]");
	hMenu += ("\n      E[[B[L|S]]|[H[xn]]|[S[xn]]] x € [L(oad),S(ave)]");
	hMenu += ("\n      Load: n € [0,1,2], 0 reinits to default values");
	hMenu += ("\n      Save: n € [1,2], ex: EHL0 reinit hard context");
	hMenu += ("\n 'F'                           ");
	hMenu += ("\n 'G'                           ");
	hMenu += ("\n 'H' HIBERNATE to admin rover");
	hMenu += ("\n 'I' IMU management");
	hMenu += ("\n      I[[A]|[D][t]] Without options swaps IMU simul<->i²c");
	hMenu += ("\n      IA[t] attachs, ID[t] detachs IMU after delay of t ms.");
	hMenu += ("\n 'J' JUNCTIONS dump");
	hMenu += ("\n 'L' Toggle log (serial <=> webSocket)");
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
	hMenu += ("\n 'R'                           ");
	hMenu += ("\n 'S'                           ");
	hMenu += ("\n 'T' TASKS, dump tasks if no l option or set debug level");
	hMenu += ("\n      T[l[t]] l € [0...9] for all tasks if no t option");
	hMenu += ("\n      or for one t € [A(pp),I(mu),M(od),R(cv),S(nd)]");
	hMenu += ("\n 'U' Toggle csvActive");
	hMenu += ("\n 'V' Toggle txtActive");
	hMenu += ("\n 'W' Toggle show or toggle WSX Ctrl/Data/Event");
	hMenu += ("\n      W[x] x € [C(trl),D(ata),E(vent)] (else toggle all)");
	hMenu += ("\n 'X'                           ");
	hMenu += ("\n 'Y' webSokSrv.cleanupClients()");
	hMenu += ("\n 'Z' List STA clients of AP");
	hMenu += ("\n 		'+' Loop delay more");
	hMenu += ("\n 		'-' Loop delay less");
	hMenu += ("\n '@' ESP.restart() !!!");
	hMenu += ("\n ------------------------------");
	hMenu += ("\n -Hit a choice or '?' to quit wizard menu");
	hMenu += ("\n ==============================");
	return hMenu;
}
///////////////////////////////////////////////////////////////////////
/** < @todo KMDS() comme onMESSAGE vers fifo modules or KMDS() for packet from serial, udp or ws? JSON => Fifo modules pour mission eh eh input commands */
/*
 bool DATAGreceive ( const String  KMD ){
 // if JSON goto fifo modules else
 return KMDS(KMD.c_str());
 }
 */
/**
 * @fn void KMDS(const char *  KMD = nullptr)
 * @brief serial or udp or ws? JSON => Fifo modules pour mission eh eh input commands
 * @param KMD command string
 */
bool KMDS(const char *KMD){
	static int8_t &this_DebugLVL = *registerFCT(e_tasks::KMDS, "Kmds",
		&CTX.SOFTctx.FCT_DBGMAX_KMDS);
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
					char *kmd = myUDP.has_KMD();
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
			_DBG_APP_KMDS("`%s` *********************************\n", str.c_str());
			cmd = splitString(args, str);
		}
		if ((32 <= cmd) && (cmd <= 122)) {
			switch(toupper(cmd)){
				case '0' ... '9': {
					CTX.SOFTctx.ALL_DBGMAX_TRUNC = d09(args[0][0]); // fault with d09(cmd)???
					break;
				}
				case '?': {
					if (!Menu) {
						(*(c_linkISR::detach_ISR))();
						vTaskSuspend(findCORE()->TH);
						//vTaskSuspend(findYLOG()->TH);// maybe nullptr!!!
						_DBG_APP_KMDS("\n%s", helpMenu().c_str());
						Menu = true;
					} else {
						_DBG_APP_KMDS("\nLEAVING MENU");
						//vTaskResume(findYLOG()->TH);
						vTaskResume(findCORE()->TH);
						(*(c_linkISR::attach_ISR))(); // define interrupt for intPin output of MPU9250
						Menu = false;
					}
					break;
				}
				case '@': {
					ESP.restart();
					break;
				}
					/*
					 case '+':
					 loop_Delay *= 2;
					 _DBG_APP_KMDS("\nMENU:Set loop_Delay to %i ms", loop_Delay);
					 break;
					 case '-': {
					 loop_Delay /= 2;
					 if (loop_Delay == 0)
					 loop_Delay = 1;
					 _DBG_APP_KMDS("\nMENU:Set loop_Delay to %i ms", loop_Delay);
					 break;
					 }
					 */
				case 'A': {
					if (_CSV_("@APPND:telemetry.csv"))
						//CTX.HARDctx.ximuCTX.telemetry = true;
						CTX.HARDctx.kineCTX.telemetry = true;
					break;
				}
				case 'B': {
					//CTX.SOFTctx.DRVctx.kmdIdx = args[0][1] - 48;
					CTX.SOFTctx.DRVctx.clear_CineRover = true;
					/*
					 Module * mod = findModule("DRV");
					 Driver * _driver = reinterpret_cast<Driver *>(mod);
					 _driver->clear_Cinematique_Rover();
					 */
					if ((_CSV_("@BEGIN:telemetry.csv"))
						//&& (_CSV_("@What;MTS;ax;ay;az;gx;gy;gz;mx;my;mz;Q0;Q1;Q2;Q3")))
						&& (_CSV_(
							"@_KMD;MTS;m00;m01;m02;m03;m10;m11;m12;m13;m20;m21;m22;m23;m30;m31;32;m33")))
						//CTX.HARDctx.ximuCTX.telemetry = true;
						CTX.HARDctx.kineCTX.telemetry = true;
					break;
				}
				case 'C': {
					_CSV_("@CLOSE:");
//					CTX.HARDctx.ximuCTX.telemetry = false;
					CTX.HARDctx.kineCTX.telemetry = false;
					break;
				}
				case 'D': {
					_CSV_("@%llu;%i;%i", 9999, 9999, 9999);
					break;
				}
				case 'E': {
					if (args[0].length() == 1) { // E
						_DBG_APP_KMDS("\n%s", CTX.dumpBOOTctx().c_str());
						_DBG_APP_KMDS("\n%s", CTX.dumpSOFTctx().c_str());
						_DBG_APP_KMDS("\n%s", CTX.dumpHARDctx().c_str());
					} else if (args[0].length() == 2) { // EB | EH | ES
						if (toupper(args[0][1]) == 'B')
							_DBG_APP_KMDS("\n%s", CTX.dumpBOOTctx().c_str());
						else if (toupper(args[0][1]) == 'H')
							_DBG_APP_KMDS("\n%s", CTX.dumpHARDctx().c_str());
						else if (toupper(args[0][1]) == 'S')
							_DBG_APP_KMDS("\n%s", CTX.dumpSOFTctx().c_str());
						else
							syntax = false;
					} else if ((toupper(args[0][1]) == 'B') && (args[0].length() == 3)) {
						if (toupper(args[0][2]) == 'L') // EBL load
							CTX.LOAD();
						else if (toupper(args[0][2]) == 'S') // EBS save
							CTX.SAVE();
						else
							syntax = false;
					} else if ((args[0].length() == 4) && (isDigit(args[0][3]))
						&& ((toupper(args[0][2]) == 'L') || (toupper(args[0][2]) == 'S'))) {
						if (toupper(args[0][1]) == 'H') {
							if (toupper(args[0][2]) == 'L') { // EHL0 EHL1 EHL2 load hard
								if (toupper(args[0][3]) == '0')
									CTX.HARDctx=s_HARDctx{};
								else if (toupper(args[0][3]) == '1')
									Eload(CTX.E_HARD1, CTX.HARDctx);
								else if (toupper(args[0][3]) == '2')
									Eload(CTX.E_HARD2, CTX.HARDctx);
								else
									syntax = false;
							} else if (toupper(args[0][2]) == 'S') { // EHS1 EHS2 save hard
								if (toupper(args[0][3]) == '1')
									Esave(CTX.E_HARD1, CTX.HARDctx, '1');
								else if (toupper(args[0][3]) == '2')
									Esave(CTX.E_HARD2, CTX.HARDctx, '2');
								else
									syntax = false;
							} else
								syntax = false;
							if (syntax)
								CTX.BOOTctx.HARD = d09(args[0][3]);
						} else if (toupper(args[0][1]) == 'S') {
							if (toupper(args[0][2]) == 'L') { // ESL0 ESL1 ESL2 load soft
								if (toupper(args[0][3]) == '0')
									CTX.SOFTctx=s_SOFTctx{};
								else if (toupper(args[0][3]) == '1')
									Eload(CTX.E_SOFT1, CTX.SOFTctx);
								else if (toupper(args[0][3]) == '2')
									Eload(CTX.E_SOFT2, CTX.SOFTctx);
								else
									syntax = false;
							} else if (toupper(args[0][2]) == 'S') { // ESS1 ESS2 save soft
								if (toupper(args[0][3]) == '1')
									Esave(CTX.E_SOFT1, CTX.SOFTctx, '1');
								else if (toupper(args[0][3]) == '2')
									Esave(CTX.E_SOFT2, CTX.SOFTctx, '2');
								else
									syntax = false;
							} else
								syntax = false;
							if (syntax)
								CTX.BOOTctx.SOFT = d09(args[0][3]);
						}
						_DBG_APP_KMDS("\n%s", CTX.dumpBOOTctx().c_str());
					} else
						syntax = false;
					break;
				}
				case 'H': {
					_DBG_APP_KMDS("\nHibernate to admin rover");
					kill_tasks();
					break;
				}
				case 'I': {
					if (args[0].length() == 1) {
						CTX.SOFTctx.DRVctx.simul_IMU = !CTX.SOFTctx.DRVctx.simul_IMU;
					} else {
						const char *tms = 2 + args[0].c_str();
						int wait = atoi(tms);
						if (0 < wait)
							vTaskDelay(wait / portTICK_PERIOD_MS);
						if ('D' == toupper(args[0][1])) {
							c_linkISR::detach_ISR();
						} else if ('A' == toupper(args[0][1])) {
							c_linkISR::attach_ISR();
						} else
							syntax = false; // unknown
					}
					//kill_tasks();
					break;
				}
				case 'J': {
					_DBG_APP_KMDS("\n%s", dumpAllJUNCTIONS().c_str());
					break;
				}
				case 'L': {
					CTX.SOFTctx.logWebsocket = !CTX.SOFTctx.logWebsocket;
					break;
				}
				case 'M': {
					if (args[0].length() == 1) {
						_DBG_APP_KMDS("\nMODULES");
						_DBG_APP_KMDS("\n%s", dumpAllMODULES().c_str());
					} else if (isDigit(args[0][1])) {
						if (args[0].length() == 2) {
							for (auto it = modulesMap.begin(); it != modulesMap.end(); ++it) {
								_DBG_APP_KMDS("\nMENU:Module %s Set DebugLVL from %i to %i",
									it->second->_Mid, it->second->get_dbgLVL(), d09(args[0][1]));
								it->second->set_dbgLVL(d09(args[0][1]));
							}
						} else if (args[0].length() >= 3) {
							Module *mod = findModule0(toupper(args[0][2]));
							if (mod != nullptr) {
								if (args[0].length() == 3) {
									_DBG_APP_KMDS("\nMENU:Module '%s' Set DebugLVL from %i to %i",
										mod->_Mid, mod->get_dbgLVL(), d09(args[0][1]));
									mod->set_dbgLVL(d09(args[0][1]));
								} else if (args[0].length() == 4) {
									if (mod->_Mid.equals("DRV")) {
										Driver &driver = *reinterpret_cast<Driver*>(mod);
										switch(toupper(args[0][3])){
											case 'K': {
												_DBG_APP_KMDS(
													"\nSetting debugLevel(%s:KINE) from %i to %i",
													mod->_Mid.c_str(), CTX.SOFTctx.KINE___DBGMAX_,
													d09(args[0][1]));
												CTX.SOFTctx.KINE___DBGMAX_ = d09(args[0][1]);
												driver.setKINEdbgLvl(d09(args[0][1]));
												break;
											}
											default:
												syntax = false; // DRV has no this SUB module
										}
									}
								} else
									syntax = false; // args[0] too long
							} else
								syntax = false; // Module no found
						}
					} else
						syntax = false; // args[0][1] !isDigit
					break;
				}
				case 'P': {
					CTX.HARDctx.ximuCTX.beta *= 2;
					break;
				}
				case 'Q': {
					CTX.HARDctx.ximuCTX.beta *= 0.5;
					break;
				}
				case 'T': {
					if (args[0].length() == 1) {
						_DBG_APP_KMDS("\n%s", dump_TASKS().c_str());
					} else if (isDigit(args[0][1])) {
						if (args[0].length() == 2) {
							for (size_t i = 0; i < static_cast<int>(e_tasks::END); i++) {
								//_DBG_APP_KMDS("\n%i %s", i, dump_ATASK(e_tasks(i)).c_str());
								s_tskInfo_X *ti =
									&tskMON[static_cast<std::underlying_type<e_tasks>::type>(i)];
								_DBG_APP_KMDS("\nMENU:Task %s Set DebugLVL from %i to %i",
									ti->name, *(ti->DebugLVL), d09(args[0][1]));
								if (ti->DebugLVL != nullptr)
									*ti->DebugLVL = d09(args[0][1]);
								//_DBG_APP_KMDS("\n%i %s", i, dump_ATASK(e_tasks(i)).c_str());
							}
						} else if (args[0].length() == 3) {
							s_tskInfo_X *ti = tskFind(toupper(args[0][2]));
							if (ti != nullptr) {
								_DBG_APP_KMDS("\nMENU:Task %s Set DebugLVL from %i to %i",
									ti->name, *(ti->DebugLVL), d09(args[0][1]));
								*ti->DebugLVL = d09(args[0][1]);
							} else
								syntax = false; // s_tskInfo_X not found
						} else
							syntax = false; // args[0] too long
					} else
						syntax = false; // args[0][1] !isDigit
					break;
				}
				case 'U': {
					CTX.SOFTctx.csvActive = !CTX.SOFTctx.csvActive;
					_DBG_APP_KMDS("\nMENU:Toggle csvActive to %i", CTX.SOFTctx.csvActive);
					break;
				}
				case 'V': {
					CTX.SOFTctx.txtActive = !CTX.SOFTctx.txtActive;
					_DBG_APP_KMDS("\nMENU:Toggle txtActive to %i", CTX.SOFTctx.txtActive);
					break;
				}
				case 'W': {
					if (args[0].length() == 1) {
						CTX.SOFTctx.withCtrlBlock = !CTX.SOFTctx.withCtrlBlock;
						CTX.SOFTctx.showWSX_data = !CTX.SOFTctx.showWSX_data;
						CTX.SOFTctx.showWSX_event = !CTX.SOFTctx.showWSX_event;
					} else if (args[0].length() == 2) {
						if (toupper(args[0][1]) == 'C')
							CTX.SOFTctx.withCtrlBlock = !CTX.SOFTctx.withCtrlBlock;
						else if (toupper(args[0][1]) == 'D')
							CTX.SOFTctx.showWSX_data = !CTX.SOFTctx.showWSX_data;
						else if (toupper(args[0][1]) == 'E')
							CTX.SOFTctx.showWSX_event = !CTX.SOFTctx.showWSX_event;
						else
							syntax = false; // not C|D|E
						bool Menu = false;
					}
					else
						syntax = false; // too long
					_DBG_APP_KMDS("\nToogle (Ctrl,Data,Event) to (%i,%i,%i)",
						CTX.SOFTctx.withCtrlBlock, CTX.SOFTctx.showWSX_data,
						CTX.SOFTctx.showWSX_event);
					break;
				}
				case 'X': {
					_DBG_APP_KMDS("\nMENU:webSokSrv.cleanupClients");
					webSokSrv.cleanupClients();
					break;
				}
				case 'Z': {
					_DBG_APP_KMDS(listStaClients().c_str());
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
			_DBG_APP_KMDS("\n\t Heap(%i/%i bytes)", ESP.getMaxAllocHeap(), ESP.getFreeHeap());
		}
	} while (Menu);
	return syntax && carOK;
}
///////////////////////////////////////////////////////////////////////
