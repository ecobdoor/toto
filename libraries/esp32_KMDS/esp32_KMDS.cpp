#include "esp32_KMDS.h"
#include "esp32_TASKS.h"
#include "esp32_LOGS.h"
/** @file */
/**
 * @fn void helpMenu()
 * @brief serial display of commands syntax
 */
String helpMenu(){
	String hMenu;
	hMenu += (" ========= MENU ===============");
	hMenu += ("\n '?' Display this help (no case sensitive)");
	hMenu += ("\n 'A' APPEND on a CSV file");
	hMenu += ("\n 'B' BEGIN a new CSV file");
	hMenu += ("\n 'C' CLOSE the CSV file");
	/*

	 hMenu += ("\n 'D' DATA write in CSV file");
	 hMenu += ("\n 'T' TASKS, dump tasks if no l option or set debug level");
	 hMenu += ("\n      T[l[t]] l € [0...9] for all tasks if no t option");
	 hMenu += ("\n      or for one t € [A(pp),I(mu),M(od),R(cv),S(nd)]");
	 hMenu += ("\n 'U' Toggle csvActive");
	 hMenu += ("\n 'V' Toggle txtActive");
	 hMenu += ("\n '@' ESP.restart() !!!");
	 hMenu += ("\n ------------------------------");
	 hMenu += ("\n -Hit a choice or '?' to quit wizard menu");
	 */
	hMenu += ("\n ==============================");
	return hMenu;
}
///////////////////////////////////////////////////////////////////////
/**
 * @fn void KMDS( char *  KMD = nullptr)
 * @brief serial or udp or ws? JSON => Fifo modules pour mission eh eh input commands
 * @param KMD command string
 */
bool KMDS(char *KMD){
	static uint32_t cntest = 0;
	static int8_t debug_KMDS = 5;
	static int8_t &this_DebugLVL = *registerFCT(e_tasks::KMDS, "Kmds", &debug_KMDS);
	static bool Menu = false;
	//extern AsyncWebSocket webSokSrv;
	bool syntax;
	bool carOK;
	do {
		syntax = true;
		carOK = true;
		String str;
		if (KMD != nullptr) {
			_DBG_APP_KMDS("\n\n>>UDP: ");
			str = String(KMD);
			KMD = nullptr; // reset for next do loop task
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
				case '?': {
					if (!Menu) {
						_DBG_APP_KMDS("\n%s", helpMenu().c_str());
						Menu = true;
					} else {
						_DBG_APP_KMDS("\nLEAVING MENU");
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
					_CSV_("@APPND:telemetry.csv");
					/*
					 if (_CSV_("@APPND:telemetry.csv"))
					 //CTX.HARDctx.ximuCTX.telemetry = true;
					 CTX.HARDctx.kineCTX.telemetry = true;
					 */
					break;
				}
				case 'B': {
					_CSV_("@BEGIN:telemetry.csv");
					/*
					 //CTX.SOFTctx.DRVctx.kmdIdx = args[0][1] - 48;
					 CTX.SOFTctx.DRVctx.clear_CineRover = true;
					 if ((_CSV_("@BEGIN:telemetry.csv"))
					 //&& (_CSV_("@What;MTS;ax;ay;az;gx;gy;gz;mx;my;mz;Q0;Q1;Q2;Q3")))
					 && (_CSV_(
					 "@_KMD;MTS;m00;m01;m02;m03;m10;m11;m12;m13;m20;m21;m22;m23;m30;m31;32;m33")))
					 //CTX.HARDctx.ximuCTX.telemetry = true;
					 CTX.HARDctx.kineCTX.telemetry = true;
					 */
					break;
				}
				case 'C': {
					_CSV_("@CLOSE:");
					/*
					 //					CTX.HARDctx.ximuCTX.telemetry = false;
					 CTX.HARDctx.kineCTX.telemetry = false;
					 */
					break;
				}
				case 'D': {
					_CSV_("@%llu;%i;%i", 9999, 9999, 9999);
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
			//_DBG_APP_KMDS("\n\t Heap(%i/%i bytes)", ESP.getMaxAllocHeap(), ESP.getFreeHeap());
			_DBG_APP_KMDS("\n\t Heap max:%i>free:%i>size:%i (min:%i)",
				ESP.getMaxAllocHeap(), ESP.getFreeHeap(), ESP.getHeapSize(), ESP.getMinFreeHeap());
		}
	} while (Menu);
	if (cntest % 3 == 0)
		_DBG_APP_KMDS(
			"\n\t************************************************************************** %i %i",
			cntest, Menu);
	cntest++;
	return syntax && carOK;
}
///////////////////////////////////////////////////////////////////////
