#ifndef __000_TASKS__
#define __000_TASKS__
#include <array>
#include <functional>
#include "000_EXCEPT.h"
///////////////////////////////////////////////////////////////////////
//ESP_TASK_PRIO_MAX - 14, // priority of the task from (tskIDLE_PRIORITY + 1) to (configMAX_PRIORITIES -1 )
//tskIDLE_PRIORITY		is		1+tskIDLE_PRIORITY
#define DEF_PRIO_LOGS			1+tskIDLE_PRIORITY // TSK logs
#define DEF_PRIO_CORE			2+tskIDLE_PRIORITY // TSK root 
#define DEF_PRIO_VRCV			3+tskIDLE_PRIORITY // FCT onWSMessage WS reveive
#define DEF_PRIO_RUDP			3+tskIDLE_PRIORITY // FCT onPacket UDP reveive
#define DEF_PRIO_SUDP			4+tskIDLE_PRIORITY // TSK UPD send
#define DEF_PRIO_WSND			4+tskIDLE_PRIORITY // TSK WS send
#define DEF_PRIO_XIMU		   10+tskIDLE_PRIORITY // TSK IMU was good with 5 before udp installed ????

/**
 * @brief enum class e_tasks : unsigned int
 *
 *
 */
enum class e_tasks : unsigned int {
	MAIN,
	BOOT,
	SERV,
	LOOP,
	KMDS,
	CORE,
	RUDP,
	SUDP,
	WRCV,
	WSND,
	XIMU,
	YLOG,
	ZLOG,
	END,
};
static e_tasks& operator ++(e_tasks &e){
	if (e == e_tasks::END) {
		throw std::out_of_range("for e_tasks& operator ++ (e_tasks&)");
	}
	e = e_tasks(static_cast<std::underlying_type<e_tasks>::type>(e) + 1);
	return e;
}
//typedef std::function<void(void *pvParameters)> f_taskFCT;
typedef struct {
	char type = '?';
	const char *name = "....";
	int8_t *DebugLVL = nullptr;
	int core = -1;
	int curPrio = -1;
	int oldPrio = -1;
	uint32_t stackLeft = 0;
	uint32_t stack = 0;
	TaskFunction_t *tFct = nullptr;
	void *Param = nullptr;
	TaskHandle_t *TH = nullptr;
	Scratch *scratch = nullptr;
} s_tskInfo_X;
typedef std::array<s_tskInfo_X, static_cast<int>(e_tasks::END)> t_tskMON;

extern t_tskMON tskMON;
///////////////////////////////////////////////////////////////////////

s_tskInfo_X* tskFind(const char CC);
inline s_tskInfo_X* findTask(const TaskHandle_t *TSKHDL){
	for (int e = 0; e < static_cast<int>(e_tasks::END); e++) {
		s_tskInfo_X *ti = &tskMON[static_cast<int>(e)];
		if (ti->TH == TSKHDL)
			return ti;
	}
	return nullptr;
}
inline s_tskInfo_X* findSendUDP(){
	return &tskMON[static_cast<int>(e_tasks::SUDP)];
}
inline s_tskInfo_X* findRecvUDP(){
	return &tskMON[static_cast<int>(e_tasks::RUDP)];
}
inline s_tskInfo_X* findRecvWS(){
	return &tskMON[static_cast<int>(e_tasks::WRCV)];
}
inline s_tskInfo_X* findCORE(){
	return &tskMON[static_cast<int>(e_tasks::CORE)];
}
inline s_tskInfo_X* findYLOG(){
	return &tskMON[static_cast<int>(e_tasks::YLOG)];
}
String dump_ATASK(const e_tasks tskIdx);
int8_t* registerFCT(const e_tasks tskIdx, const char *NAME, int8_t *THIS_DBGLVL,
	const int PRIORITY = 0);
/**
 * @fn void launchTASK(const e_tasks tskIdx, TaskHandle_t &TSKHDL, const char *NAME, TaskFunction_t F,
 void *PARAM, int8_t *THIS_DBGLVL, const uint32_t STACK, const int CORE const int PRIORITY=0, const bool LAUNCH=true)
 * @param tskIdx
 * @param TSKHDL task handler
 * @param NAME task name
 * @param F task functional
 * @param PARAM task user parameters
 * @param THIS_DBGLVL task debug level
 * @param STACK size
 * @param CORE number
 * @param PRIORITY unused if LAUNCH is false, get the priority of existing task
 */
void launchTASK(const e_tasks tskIdx, TaskHandle_t &TSKHDL, const char *NAME, TaskFunction_t F,
	void *PARAM, int8_t *THIS_DBGLVL, const uint32_t STACK, const int CORE, const int PRIORITY);
String dump_TASKS();
void kill_tasks();
///////////////////////////////////////////////////////////////////////
class c_linkISR {
//	typedef std::function<void(void)> f_link_ISR; pour passer void IRAM_ATTR iimu_ISR() mais marche pas !!!
private: // Disallow creating an instance of this class
	c_linkISR(){
	}
public:
	static void Begin();
	static void attach_ISR();
	static void detach_ISR();
	static uint8_t pin();
};
///////////////////////////////////////////////////////////////////////
#endif //__000_TASK__
