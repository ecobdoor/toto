#include "esp32_TASKS.h"
#include "esp32_DEBUG.h"
#include "esp32_SPRINT.h"
#include "esp_task_wdt.h"
H_pwmMotors *pwmMotors = nullptr;/**< @brief @ref H_pwmMotors pointer  */
t_tskMON tskMON;
bool imuFlag = false;
uint64_t isr_microTimeStamp = 0;
c_myTASKS myTASKS;
///////////////////////////////////////////////////////////////////////
/**
 * @fn void iimu_ISR()
 * @brief IRAM_ATTR Handler for I²C ISR harware interrupt (running at 100 Hz)
 *
 * Notify the \ref imu_HDL task to which \ref iimu_TASK is attached
 */
void IRAM_ATTR iimu_ISR(){
	extern TaskHandle_t ximu_HDL;
	isr_microTimeStamp = esp_timer_get_time();//micro_TS();
	imuFlag = true;
	BaseType_t xHigherPriorityTaskWoken = pdTRUE;
	if (pdTRUE
		!= xTaskNotifyFromISR(ximu_HDL, 0, eSetValueWithoutOverwrite, &xHigherPriorityTaskWoken)) {
		/**< @ todo ERROR !!! Tragic blocks I²C for the next ESP restart maybe detach ISR pin */
	}
}
///////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------
bool _imuActivated = false; // static member bool is always initialized to false!!!
const uint8_t _hardPIN_isr = 4; // INTERRUPT PIN !!!
const uint8_t _trigger_isr = RISING; // RISING UP !!!
//---------------------------------------------------------------------
uint8_t c_linkISR::pin(){
	return _hardPIN_isr;
}
//---------------------------------------------------------------------
void c_linkISR::Begin(){
	_imuActivated = true;
}
//---------------------------------------------------------------------
void c_linkISR::attach_ISR(){
	if (_imuActivated) {
		Serial.printf("\nATTACHING ISR...");
		attachInterrupt(_hardPIN_isr, iimu_ISR, _trigger_isr); // define interrupt for intPin output of MPU9250
	}else
		Serial.printf("\nCAN'T ATTACH ISR (not activated yet)...");
}
//---------------------------------------------------------------------
void c_linkISR::detach_ISR(){
	if (_imuActivated) {
		detachInterrupt(_hardPIN_isr); // define interrupt for intPin output of MPU9250
		Serial.printf("\nDETACHING ISR...");
	}else
		Serial.printf("\nCAN'T DETACH ISR (not activated yet)...");
}
///////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------
void DETACH(uint16_t WHICHES){
	if(WHICHES & P_imuISR_)
		c_linkISR::detach_ISR();
	if(WHICHES & P_hardPWM_)
		pwmMotors->detachPWMpins();
}
//---------------------------------------------------------------------
void ATTACH(uint16_t WHICHES){
	if(WHICHES & P_imuISR_)
		c_linkISR::attach_ISR();
	if(WHICHES & P_hardPWM_)
		pwmMotors->attachPWMpins();
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
#if ( (false)||( configUSE_TRACE_FACILITY == 1 ) && ( configUSE_STATS_FORMATTING_FUNCTIONS > 0 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) )

    void vTaskList( char * pcWriteBuffer )
    {
        TaskStatus_t * pxTaskStatusArray;
        UBaseType_t uxArraySize, x;
        char cStatus;

        /*
         * PLEASE NOTE:
         *
         * This function is provided for convenience only, and is used by many
         * of the demo applications.  Do not consider it to be part of the
         * scheduler.
         *
         * vTaskList() calls uxTaskGetSystemState(), then formats part of the
         * uxTaskGetSystemState() output into a human readable table that
         * displays task: names, states, priority, stack usage and task number.
         * Stack usage specified as the number of unused StackType_t words stack can hold
         * on top of stack - not the number of bytes.
         *
         * vTaskList() has a dependency on the sprintf() C library function that
         * might bloat the code size, use a lot of stack, and provide different
         * results on different platforms.  An alternative, tiny, third party,
         * and limited functionality implementation of sprintf() is provided in
         * many of the FreeRTOS/Demo sub-directories in a file called
         * printf-stdarg.c (note printf-stdarg.c does not provide a full
         * snprintf() implementation!).
         *
         * It is recommended that production systems call uxTaskGetSystemState()
         * directly to get access to raw stats data, rather than indirectly
         * through a call to vTaskList().
         */


        /* Make sure the write buffer does not contain a string. */
        *pcWriteBuffer = ( char ) 0x00;

        /* Take a snapshot of the number of tasks in case it changes while this
         * function is executing. */
        uxArraySize = uxCurrentNumberOfTasks;

        /* Allocate an array index for each task.  NOTE!  if
         * configSUPPORT_DYNAMIC_ALLOCATION is set to 0 then pvPortMalloc() will
         * equate to NULL. */
        pxTaskStatusArray = pvPortMalloc( uxCurrentNumberOfTasks * sizeof( TaskStatus_t ) ); /*lint !e9079 All values returned by pvPortMalloc() have at least the alignment required by the MCU's stack and this allocation allocates a struct that has the alignment requirements of a pointer. */

        if( pxTaskStatusArray != NULL )
        {
            /* Generate the (binary) data. */
            uxArraySize = uxTaskGetSystemState( pxTaskStatusArray, uxArraySize, NULL );

            /* Create a human readable table from the binary data. */
            for( x = 0; x < uxArraySize; x++ )
            {
                switch( pxTaskStatusArray[ x ].eCurrentState )
                {
                    case eRunning:
                        cStatus = tskRUNNING_CHAR;
                        break;

                    case eReady:
                        cStatus = tskREADY_CHAR;
                        break;

                    case eBlocked:
                        cStatus = tskBLOCKED_CHAR;
                        break;

                    case eSuspended:
                        cStatus = tskSUSPENDED_CHAR;
                        break;

                    case eDeleted:
                        cStatus = tskDELETED_CHAR;
                        break;

                    case eInvalid: /* Fall through. */
                    default:       /* Should not get here, but it is included
                                    * to prevent static checking errors. */
                        cStatus = ( char ) 0x00;
                        break;
                }

                /* Write the task name to the string, padding with spaces so it
                 * can be printed in tabular form more easily. */
                pcWriteBuffer = prvWriteNameToBuffer( pcWriteBuffer, pxTaskStatusArray[ x ].pcTaskName );

                /* Write the rest of the string. */
                sprintf( pcWriteBuffer, "\t%c\t%u\t%u\t%u\r\n", cStatus, ( unsigned int ) pxTaskStatusArray[ x ].uxCurrentPriority, ( unsigned int ) pxTaskStatusArray[ x ].uxStackHighWaterMark, ( unsigned int ) pxTaskStatusArray[ x ].xTaskNumber ); /*lint !e586 sprintf() allowed as this is compiled with many compilers and this is a utility function only - not part of the core kernel implementation. */
                pcWriteBuffer += strlen( pcWriteBuffer );                                                                                                                                                                                                /*lint !e9016 Pointer arithmetic ok on char pointers especially as in this case where it best denotes the intent of the code. */
            }

            /* Free the array again.  NOTE!  If configSUPPORT_DYNAMIC_ALLOCATION
             * is 0 then vPortFree() will be #defined to nothing. */
            vPortFree( pxTaskStatusArray );
        }
        else
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
#else
    void vTaskList( char * pcWriteBuffer ){

    }
#endif /* ( ( configUSE_TRACE_FACILITY == 1 ) && ( configUSE_STATS_FORMATTING_FUNCTIONS > 0 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) ) */
/*----------------------------------------------------------*/

#if ((false)|| ( configGENERATE_RUN_TIME_STATS == 1 ) && ( configUSE_STATS_FORMATTING_FUNCTIONS > 0 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) )

    void vTaskGetRunTimeStats( char * pcWriteBuffer )
    {
        TaskStatus_t * pxTaskStatusArray;
        UBaseType_t uxArraySize, x;
        uint32_t ulTotalTime, ulStatsAsPercentage;

        #if ( configUSE_TRACE_FACILITY != 1 )
            {
                #error configUSE_TRACE_FACILITY must also be set to 1 in FreeRTOSConfig.h to use vTaskGetRunTimeStats().
            }
        #endif

        /*
         * PLEASE NOTE:
         *
         * This function is provided for convenience only, and is used by many
         * of the demo applications.  Do not consider it to be part of the
         * scheduler.
         *
         * vTaskGetRunTimeStats() calls uxTaskGetSystemState(), then formats part
         * of the uxTaskGetSystemState() output into a human readable table that
         * displays the amount of time each task has spent in the Running state
         * in both absolute and percentage terms.
         *
         * vTaskGetRunTimeStats() has a dependency on the sprintf() C library
         * function that might bloat the code size, use a lot of stack, and
         * provide different results on different platforms.  An alternative,
         * tiny, third party, and limited functionality implementation of
         * sprintf() is provided in many of the FreeRTOS/Demo sub-directories in
         * a file called printf-stdarg.c (note printf-stdarg.c does not provide
         * a full snprintf() implementation!).
         *
         * It is recommended that production systems call uxTaskGetSystemState()
         * directly to get access to raw stats data, rather than indirectly
         * through a call to vTaskGetRunTimeStats().
         */

        /* Make sure the write buffer does not contain a string. */
        *pcWriteBuffer = ( char ) 0x00;

        /* Take a snapshot of the number of tasks in case it changes while this
         * function is executing. */
        uxArraySize = uxCurrentNumberOfTasks;

        /* Allocate an array index for each task.  NOTE!  If
         * configSUPPORT_DYNAMIC_ALLOCATION is set to 0 then pvPortMalloc() will
         * equate to NULL. */
        pxTaskStatusArray = pvPortMalloc( uxCurrentNumberOfTasks * sizeof( TaskStatus_t ) ); /*lint !e9079 All values returned by pvPortMalloc() have at least the alignment required by the MCU's stack and this allocation allocates a struct that has the alignment requirements of a pointer. */

        if( pxTaskStatusArray != NULL )
        {
            /* Generate the (binary) data. */
            uxArraySize = uxTaskGetSystemState( pxTaskStatusArray, uxArraySize, &ulTotalTime );

            /* For percentage calculations. */
            ulTotalTime /= 100UL;

            /* Avoid divide by zero errors. */
            if( ulTotalTime > 0UL )
            {
                /* Create a human readable table from the binary data. */
                for( x = 0; x < uxArraySize; x++ )
                {
                    /* What percentage of the total run time has the task used?
                     * This will always be rounded down to the nearest integer.
                     * ulTotalRunTimeDiv100 has already been divided by 100. */
                    ulStatsAsPercentage = pxTaskStatusArray[ x ].ulRunTimeCounter / ulTotalTime;

                    /* Write the task name to the string, padding with
                     * spaces so it can be printed in tabular form more
                     * easily. */
                    pcWriteBuffer = prvWriteNameToBuffer( pcWriteBuffer, pxTaskStatusArray[ x ].pcTaskName );

                    if( ulStatsAsPercentage > 0UL )
                    {
                        #ifdef portLU_PRINTF_SPECIFIER_REQUIRED
                            {
                                sprintf( pcWriteBuffer, "\t%lu\t\t%lu%%\r\n", pxTaskStatusArray[ x ].ulRunTimeCounter, ulStatsAsPercentage );
                            }
                        #else
                            {
                                /* sizeof( int ) == sizeof( long ) so a smaller
                                 * printf() library can be used. */
                                sprintf( pcWriteBuffer, "\t%u\t\t%u%%\r\n", ( unsigned int ) pxTaskStatusArray[ x ].ulRunTimeCounter, ( unsigned int ) ulStatsAsPercentage ); /*lint !e586 sprintf() allowed as this is compiled with many compilers and this is a utility function only - not part of the core kernel implementation. */
                            }
                        #endif
                    }
                    else
                    {
                        /* If the percentage is zero here then the task has
                         * consumed less than 1% of the total run time. */
                        #ifdef portLU_PRINTF_SPECIFIER_REQUIRED
                            {
                                sprintf( pcWriteBuffer, "\t%lu\t\t<1%%\r\n", pxTaskStatusArray[ x ].ulRunTimeCounter );
                            }
                        #else
                            {
                                /* sizeof( int ) == sizeof( long ) so a smaller
                                 * printf() library can be used. */
                                sprintf( pcWriteBuffer, "\t%u\t\t<1%%\r\n", ( unsigned int ) pxTaskStatusArray[ x ].ulRunTimeCounter ); /*lint !e586 sprintf() allowed as this is compiled with many compilers and this is a utility function only - not part of the core kernel implementation. */
                            }
                        #endif
                    }

                    pcWriteBuffer += strlen( pcWriteBuffer ); /*lint !e9016 Pointer arithmetic ok on char pointers especially as in this case where it best denotes the intent of the code. */
                }
            }
            else
            {
                mtCOVERAGE_TEST_MARKER();
            }

            /* Free the array again.  NOTE!  If configSUPPORT_DYNAMIC_ALLOCATION
             * is 0 then vPortFree() will be #defined to nothing. */
            vPortFree( pxTaskStatusArray );
        }
        else
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }

#endif /* ( ( configGENERATE_RUN_TIME_STATS == 1 ) && ( configUSE_STATS_FORMATTING_FUNCTIONS > 0 ) && ( configSUPPORT_STATIC_ALLOCATION == 1 ) ) */
/*-----------------------------------------------------------*/
//---------------------------------------------------------------------
String dump_TASKS(){
	char buffer[DEF_DBGMSG_BUFSIZE];
	int16_t cnt = 0;
	buffer[0] = 0; // if no task...
	SPrintF(sizeof(buffer), cnt, buffer,
		"N° * name\tDbg pri/old cor wdt  left/stack   TH         Param      tFct       Scratch (%i)",
		uxTaskGetNumberOfTasks());
	for (int e = 0; e < static_cast<int>(e_tasks::END); e++) {
		SPrintF(sizeof(buffer), cnt, buffer, "\n%2i %s", e, dump_ATASK(e_tasks(e)).c_str());
	}
	char tasksBuffer[250];
	vTaskList(tasksBuffer);

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
