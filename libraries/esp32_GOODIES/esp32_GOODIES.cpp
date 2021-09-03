#include "esp32_GOODIES.h"
//#include "cores/esp32/Esp.h"
#include "freertos/FreeRTOS.h" // uxTaskGetStackHighWaterMark
#include "ctime" // tm
///////////////////////////////////////////////////////////////////////
void printPartitions(const esp_partition_type_t PARTYPE){
	esp_partition_iterator_t partit = esp_partition_find(PARTYPE, ESP_PARTITION_SUBTYPE_ANY, NULL);
	if (partit)
		do {
			const esp_partition_t *_mypart = esp_partition_get(partit);
			Serial.printf("\n\t%2x   %2x   %06x %6x  %8.3f  %8s  %i", _mypart->type,
				_mypart->subtype,
				_mypart->address, _mypart->size, _mypart->size / 1000.0, _mypart->label,
				_mypart->encrypted);
		} while (partit = esp_partition_next(partit));
	esp_partition_iterator_release(partit);
//	Serial.printf("\n\t---------------------------------------");

}
bool SETUP_ESPINFO(){
	esp_chip_info_t chip_info;
	esp_chip_info(&chip_info);

	Serial.printf("\nHardware info");
	Serial.printf("\n\tModel: %i", chip_info.model);
	Serial.printf("\n\t%d cores, Wifi %s%s",
		chip_info.cores, (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
		(chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");
	Serial.printf("\n\tSilicon revision: %d", chip_info.revision);
	Serial.printf("\n\t%dMB %s flash", spi_flash_get_chip_size() / (1024 * 1024),
		(chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embeded" : "external");

	Serial.printf("\n\tChip id '%016X'", (uint64_t)ESP.getEfuseMac());
	Serial.printf("\n\tFlash size: %8.6f MB", spi_flash_get_chip_size() / 1000000.0);

	Serial.printf("\n");
	Serial.printf("\nPartiton table (APP/DATA/FR)");
	Serial.printf("\n\tTYP SUB ADRS-Hex  SIZE-Hex  SIZE-kB     LABEL CRYPT");
	for (uint8_t p = 0; p < 0xff; p++)
		printPartitions(esp_partition_type_t(p));
	/*
	 printPartitions (ESP_PARTITION_TYPE_APP);
	 printPartitions (ESP_PARTITION_TYPE_DATA);
	 printPartitions(esp_partition_type_t(32));
	 */
	//for (size_t i = 0; i < 255; i++)		Serial.printf("\n%3i %02X '%c'", i, i, (char)i);
	return true;
}

//---------------------------------------------------------------------
void sprint(const char *FORMAT, ...){
	char buffer[512];
	va_list args;
	va_start(args, FORMAT);
	vsprintf(buffer, FORMAT, args);
	va_end(args);
	Serial.printf("%s", buffer);
}
//---------------------------------------------------------------------
void halt(const char *REASON, ...){
	char buffer[128];
	va_list args;
	va_start(args, REASON);
	vsprintf(buffer, REASON, args);
	va_end(args);
	Serial.printf("\n");
	Serial.printf("\n");
	Serial.printf("\n");
	Serial.printf("\n@@@@ ESP halts!!! %s @@@@", buffer);
	Serial.printf("\n");
	Serial.printf("\n");
	Serial.printf("\n");
	Serial.printf("\n####Halt(deep_sleep). %s\n", buffer);
	esp_deep_sleep_start(); //(SLEEP_MODE_PWR_DOWN);
	/*
	 */
}
//---------------------------------------------------------------------
void reboot(const char *REASON, ...){
	char buffer[128];
	va_list args;
	va_start(args, REASON);
	vsprintf(buffer, REASON, args);
	va_end(args);
	Serial.printf("\n");
	Serial.printf("\n");
	Serial.printf("\n");
	Serial.printf("\n@@@@ ESP restarting in 5 seconds... %s @@@@", buffer);
	Serial.printf("\n");
	Serial.printf("\n");
	Serial.printf("\n");
	delay(5000);
	ESP.restart();
	/*
	 Serial.printf("\n####Halt(deep_sleep). %s\n", buffer);
	 esp_deep_sleep_start(); //(SLEEP_MODE_PWR_DOWN);
	 */
}
//---------------------------------------------------------------------
void printStackLeft(const char *COMMENT){
	uint32_t StackHighWaterMark = uxTaskGetStackHighWaterMark(nullptr);
	Serial.printf("\n%6.3f kB stack left %s", StackHighWaterMark / 1000.0, COMMENT ? COMMENT : "");
}
void printSysState(){
	return;
	// plantouillait avec les appels à ESP... dans le Serial.printf ??? avant context ça marchait ???
	heap_caps_check_integrity_all(true);
	heap_caps_print_heap_info (MALLOC_CAP_DEFAULT);
	Serial.printf("Hello world!\n");

	/* Print chip information */
	esp_chip_info_t chip_info;
	esp_chip_info(&chip_info);
	Serial.printf("This is ESP32 chip with %d CPU cores, WiFi%s%s, ", chip_info.cores,
		(chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
		(chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");
	Serial.printf("\n     | PSRAM(mB) | HEAP(kB)| STACK(kB)");
	uint32_t MinFreePsram = ESP.getMinFreePsram();
	uint32_t MinFreeHeap = ESP.getMinFreeHeap();
	uint32_t StackHighWaterMark = uxTaskGetStackHighWaterMark(nullptr);
	Serial.printf("\nMIN  | %8.6f  |%8.3f |%6.3f", MinFreePsram / 1000000.0, MinFreeHeap / 1000.0,
		StackHighWaterMark / 1000.0);
	uint32_t PsramSize = ESP.getPsramSize();
	uint32_t HeapSize = ESP.getHeapSize();
	Serial.printf("\nSIZE | %8.6f  |%8.3f |", PsramSize / 1000000.0, HeapSize / 1000.0);
	uint32_t FreePsram = ESP.getFreePsram();
	uint32_t FreeHeap = ESP.getFreeHeap();
	Serial.printf("\nFREE | %8.6f  |%8.3f |", FreePsram / 1000000.0, FreeHeap / 1000.0);
	uint32_t MaxAllocPsram = ESP.getMaxAllocPsram();
	uint32_t MaxAllocHeap = ESP.getMaxAllocHeap();
	Serial.printf("\nMAX  | %8.6f  |%8.3f |", MaxAllocPsram / 1000000.0, MaxAllocHeap / 1000.0);
}
//---------------------------------------------------------------------
//uint64_t IRAM_ATTR micro_TS(){	return (uint64_t)(esp_timer_get_time()); // (uint64_t)µicroseconds();}
//---------------------------------------------------------------------
/**
 * @fn String datim(uint64_t TMS, const int SHIFTHOURS = 1)
 * @param TMS timestamp in micro seconds
 * @param SHIFTHOURS from GMT to country meridian
 * @return human readable date & time
 */
String datim(uint64_t TMS, const int SHIFTHOURS){
	const time_t rawtime = (const time_t)(TMS / 1000000);

	struct tm *dt;
	char timestr[30];
	char buffer[30];

	dt = gmtime(&rawtime);
	dt->tm_hour = (dt->tm_hour + SHIFTHOURS) % 24;
	// use any strftime format spec here
	size_t cnt = strftime(timestr, sizeof(timestr), "%d/%m/%Y %H:%M:%S", dt);
	sprintf(buffer, "%s.%03i", timestr, (uint16_t)(TMS % 1000000));
	//std::string stdBuffer(buffer);
	return String(buffer);
}
///////////////////////////////////////////////////////////////////////
/*
 typedef enum {
 CHIP_ESP32  = 1:ESP32, 2:ESP32-S2, 4:ESP32-S3, 5:ESP32-C3
 } esp_chip_model_t;


 ESP32 Dev Module
 Hardware info
 Model: 1
 2 cores, Wifi /BT/BLE
 Silicon revision: 1
 4MB external flash
 Chip id ' DC40A24'
 Flash size: 4.194304 MB

 Partiton table (APP/DATA)
 TYP SUB ADRS-Hex  SIZE-kB      LABEL CRYPT
 0   10   010000 1310.720      app0  0
 0   11   150000 1310.720      app1  0
 ---------------------------------------
 1    2   009000   20.480       nvs  0
 1    0   00e000    8.192   otadata  0
 1   82   290000 1507.328    spiffs  0
 ---------------------------------------


 AI Thinker ESP32-CAM
 Hardware info
 Model: 1
 2 cores, Wifi /BT/BLE
 Silicon revision: 1
 4MB external flash
 Chip id '  C4F5FC'
 Flash size: 4.194304 MB

 Partiton table (APP/DATA)
 TYP SUB ADRS-Hex  SIZE-kB     LABEL CRYPT
 0   10   010000 3145.728      app0  0
 ---------------------------------------
 1    2   009000   20.480       nvs  0
 1    0   00e000    8.192   otadata  0
 1   82   310000  983.040    spiffs  0
 ---------------------------------------

 */
