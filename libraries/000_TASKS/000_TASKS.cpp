#include "000_TASKS.h"
#include "000_DEBUG.h"
#include "esp_task_wdt.h"
t_tskMON tskMON;
/**
 * @fn void iimu_ISR()
 * @brief IRAM_ATTR Handler for I²C ISR harware interrupt (running at 100 Hz)
 *
 * Notify the \ref imu_HDL task to which \ref iimu_TASK is attached
 */
void IRAM_ATTR iimu_ISR(){
	extern bool imuFlag;
	extern uint64_t isr_microTimeStamp;
	extern TaskHandle_t ximu_HDL;
	isr_microTimeStamp = micro_TS();
	imuFlag = true;
	BaseType_t xHigherPriorityTaskWoken = pdTRUE;
	if (pdTRUE
		!= xTaskNotifyFromISR(ximu_HDL, 0, eSetValueWithoutOverwrite, &xHigherPriorityTaskWoken)) {
		/**< @ todo ERROR !!! Tragic blocks I²C for the next ESP restart maybe detach ISR pin */
	}
}
//---------------------------------------------------------------------
bool _imuActivated = false; // static member bool is always initialized to false!!!
const uint8_t _hardPIN_isr = 4; // INTERRUPT PIN !!!
const uint8_t _trigger_isr = RISING; // RISING UP !!!
uint8_t c_linkISR::pin(){
	return _hardPIN_isr;
}
void c_linkISR::Begin(){
	_imuActivated = true;
}
void c_linkISR::attach_ISR(){
	if (_imuActivated) {
		Serial.printf("\nATTACHING ISR...");
		attachInterrupt(_hardPIN_isr, iimu_ISR, _trigger_isr); // define interrupt for intPin output of MPU9250
	}
}
void c_linkISR::detach_ISR(){
	if (_imuActivated) {
		detachInterrupt(_hardPIN_isr); // define interrupt for intPin output of MPU9250
		Serial.printf("\nDETACHING ISR...");
	}
}
///////////////////////////////////////////////////////////////////////

/**
 * @fn void kill_tasks()
 * @brief Stops all rover tasks to go in OTA mode
 */
void kill_tasks(){
//	tasks_running = false;
}
///////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------
int8_t* registerFCT(const e_tasks tskIdx, const char *NAME, int8_t *THIS_DBGLVL,
	const int PRIORITY){
	s_tskInfo_X ti;
	ti.type = 'F';
	ti.name = NAME;
	ti.DebugLVL = THIS_DBGLVL;
	ti.TH = (TaskHandle_t*)xTaskGetCurrentTaskHandle();
	ti.core = xPortGetCoreID();
	ti.oldPrio = uxTaskPriorityGet(ti.TH);
	if (PRIORITY != 0)
		vTaskPrioritySet(ti.TH, PRIORITY);
	ti.curPrio = uxTaskPriorityGet(ti.TH);
	tskMON[static_cast<std::underlying_type<e_tasks>::type>(tskIdx)] = ti;
	return THIS_DBGLVL;
}
//---------------------------------------------------------------------
void launchTASK(const e_tasks tskIdx, TaskHandle_t &TSKHDL, const char *NAME, TaskFunction_t F,
	void *PARAM, int8_t *THIS_DBGLVL, const uint32_t STACK, const int CORE, const int PRIORITY){
	s_tskInfo_X ti;
	ti.type = 'T';
	ti.name = NAME;
	ti.DebugLVL = THIS_DBGLVL;
	ti.core = CORE;
	ti.stack = STACK;
	ti.curPrio = PRIORITY;
	ti.oldPrio = PRIORITY;
	ti.tFct = (TaskFunction_t*)F;
	ti.Param = PARAM;
	ti.TH = (TaskHandle_t*)TSKHDL;
	tskMON[static_cast<std::underlying_type<e_tasks>::type>(tskIdx)] = ti;
	xTaskCreatePinnedToCore(F, // Task function.
		NAME, // name of task.
		STACK, // Stack size
		&tskMON[static_cast<std::underlying_type<e_tasks>::type>(tskIdx)], // parameter of the task
		PRIORITY, //ESP_TASK_PRIO_MAX - 14, // priority of the task from (tskIDLE_PRIORITY + 1) to (configMAX_PRIORITIES -1 )
		&TSKHDL, // Task handle to keep track of created task
		CORE); // pin task to core
	tskMON[static_cast<std::underlying_type<e_tasks>::type>(tskIdx)].TH = (TaskHandle_t*)TSKHDL;
}
//---------------------------------------------------------------------
String dump_ATASK(const e_tasks tskIdx){
	char buffer[512];
	int16_t cnt = 0;
	buffer[0] = 0;
	s_tskInfo_X ti = tskMON[static_cast<std::underlying_type<e_tasks>::type>(tskIdx)];
	if (ti.TH != nullptr)
		ti.stackLeft = uxTaskGetStackHighWaterMark(ti.TH);

	SPrintF(sizeof(buffer), cnt, buffer,
		"%c %s\t%2i   %2i/%2i  %2i  %3s %5lu/%5lu %-10p %-10p %-10p %-10p",
		ti.type,
		ti.name,
		(ti.DebugLVL != nullptr) ? (*ti.DebugLVL) : (-1),
		ti.curPrio, // current get priority
		ti.oldPrio, // initial get priority
		ti.core,
		((!ti.TH) ? "---" : ((ESP_OK == esp_task_wdt_status(ti.TH)) ? "ON" : "off")),
		ti.stackLeft,
		ti.stack,
		ti.TH,
		ti.Param,
		ti.tFct,
		ti.scratch
		);
	if (ti.scratch != nullptr)
		SPrintF(sizeof(buffer), cnt, buffer, "\n%s", ti.scratch->buffer);
	return String(buffer);
}
//---------------------------------------------------------------------
String dump_TASKS(){
	char buffer[DEF_DBGMSG_BUFSIZE];
	int16_t cnt = 0;
	buffer[0] = 0; // if no task...
	SPrintF(sizeof(buffer), cnt, buffer,
		"N° * name\tDbg pri/old cor wdt left/stack   TH         Param      tFct       Scratch (%i)",
		uxTaskGetNumberOfTasks());
	for (int e = 0; e < static_cast<int>(e_tasks::END); e++) {
		SPrintF(sizeof(buffer), cnt, buffer, "\n%2i %s", e, dump_ATASK(e_tasks(e)).c_str());
	}
//	vTaskGetRunTimeStats(buffer);
	return String(buffer);
}
//---------------------------------------------------------------------
s_tskInfo_X* tskFind(const char CC){
	for (int e = 0; e < static_cast<int>(e_tasks::END); e++) {
		s_tskInfo_X *ti = &tskMON[static_cast<std::underlying_type<e_tasks>::type>(e)];
		if (CC == ti->name[0])
			return ti;
	}
	return nullptr;
}
///////////////////////////////////////////////////////////////////////
