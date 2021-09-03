#ifndef __esp32_TASKS__
#define __esp32_TASKS__
//#include <array>
#include <functional>
#include "esp32_EXCEPT.h"
#include "rov-Hard_MOT_000_WS3.h"
///////////////////////////////////////////////////////////////////////
// @formatter:off
const uint16_t P_imuISR_=	0b00000000000000001;
const uint16_t P_hardPWM_=	0b00000000000000010;
// @formatter:on

void DETACH(const uint16_t WHICHES);
void ATTACH(const uint16_t WHICHES);
void vTaskGetRunTimeStats(char *buffer);

///////////////////////////////////////////////////////////////////////
//ESP_TASK_PRIO_MAX - 14, // priority of the task from (tskIDLE_PRIORITY + 1) to (configMAX_PRIORITIES -1 )
//tskIDLE_PRIORITY		is		  tskIDLE_PRIORITY
#define DEF_PRIO_LOGS			1+tskIDLE_PRIORITY // TSK logs
#define DEF_PRIO_CORE			2+tskIDLE_PRIORITY // TSK root 
#define DEF_PRIO_VRCV			3+tskIDLE_PRIORITY // FCT onWSMessage WS reveive
#define DEF_PRIO_RUDP			3+tskIDLE_PRIORITY // FCT onPacket UDP reveive
#define DEF_PRIO_SUDP			4+tskIDLE_PRIORITY // TSK UPD send
#define DEF_PRIO_WSND			4+tskIDLE_PRIORITY // TSK WS send
#define DEF_PRIO_XIMU		   20+tskIDLE_PRIORITY // TSK IMU was good with 5 before udp installed ????

/**
 * @brief enum class e_tasks : unsigned int
 *
 *
 */
enum class e_tasks : unsigned int {
	MAIN,///< Main program
	BOOT,///< Setup function (Arduino)
	SERV,///< Async web server (html & ws)
	LOOP,///< Loop task (Arduino)
	KMDS,///< serial or udp keyboard commands
	CORE,///< Kern web sockets & modules task (application's kern)
	RUDP,///< UDP receiver
	SUDP,///< UDP sender
	WRCV,///< WS reciever
	WSND,///< WS sender
	XIMU,///< IMU task
	YLOG,///< LOG task for udp tests
	ZLOG,///< LOG task for udp tests
	END,///< just for for loops
};
static e_tasks& operator ++(e_tasks &e){
	if (e == e_tasks::END) {
		throw std::out_of_range("for e_tasks& operator ++ (e_tasks&)");
	}
	e = e_tasks(static_cast<std::underlying_type<e_tasks>::type>(e) + 1);
	return e;
}
class c_myTASKS: public Core {
private:
	int8_t *dbgLvlMax = 0;

public:
	c_myTASKS() :
	Core("TASKS",dbgLvlMax) {
	}
};
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
///////////////////////////////////////////////////////////////////////
class c_linkISR {
//	typedef std::function<void(void)> f_link_ISR; pour passer void IRAM_ATTR iimu_ISR() mais marche pas !!!
private: ///< private constructor, disallow creating an instance of this class
	c_linkISR(){
	}
public:
	static void Begin();
	static void attach_ISR();
	static void detach_ISR();
	static uint8_t pin();
};
///////////////////////////////////////////////////////////////////////
extern bool imuFlag;
extern uint64_t isr_microTimeStamp;
///////////////////////////////////////////////////////////////////////
extern c_myTASKS myTASKS;
extern H_pwmMotors *pwmMotors;
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
#endif //__000_TASK__
