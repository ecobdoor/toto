#include "esp32_GOODIES.h"
#include "esp32_SYS_basic.h"
#include "esp32_SPRINT.h"
//#include "cores/esp32/Esp.h"
#include "freertos/FreeRTOS.h" // uxTaskGetStackHighWaterMark
#include "ctime" // tm
///////////////////////////////////////////////////////////////////////
void printPartitions(const size_t SIZE, int16_t &CNT, char *BUFFER,const esp_partition_type_t PARTYPE){
	esp_partition_iterator_t partit = esp_partition_find(PARTYPE, ESP_PARTITION_SUBTYPE_ANY, NULL);
	if (partit)
		do {
			const esp_partition_t *_mypart = esp_partition_get(partit);
			SPrintF(SIZE, CNT, BUFFER,"\n\t%2x   %2x   %06x %6x  %8.3f  %8s  %i", _mypart->type,
				_mypart->subtype,
				_mypart->address, _mypart->size, _mypart->size / 1000.0, _mypart->label,
				_mypart->encrypted);
		} while (partit = esp_partition_next(partit));
	esp_partition_iterator_release(partit);
//	SPrintF(SIZE, CNT, BUFFER,"\n\t---------------------------------------");

}
bool SETUP_ESPINFO(const size_t SIZE, int16_t &CNT, char *BUFFER){
	esp_chip_info_t chip_info;
	esp_chip_info(&chip_info);

	SPrintF_tilte(SIZE, CNT, BUFFER,"#### ESP32 hardware info");
	SPrintF(SIZE, CNT, BUFFER,"\n\tModel: %i", chip_info.model);
	SPrintF(SIZE, CNT, BUFFER,"\n\t%d cores, Wifi %s%s",
		chip_info.cores, (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
		(chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");
	SPrintF(SIZE, CNT, BUFFER,"\n\tSilicon revision: %d", chip_info.revision);
	SPrintF(SIZE, CNT, BUFFER,"\n\t%dMB %s flash", spi_flash_get_chip_size() / (1024 * 1024),
		(chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embeded" : "external");

	SPrintF(SIZE, CNT, BUFFER,"\n\tChip id '%016X'", (uint64_t)ESP.getEfuseMac());
	SPrintF(SIZE, CNT, BUFFER,"\n\tFlash size: %8.6f MB", spi_flash_get_chip_size() / 1000000.0);
	SPrintF(SIZE, CNT, BUFFER,"\n");

	SPrintF(SIZE, CNT, BUFFER,"\n\tPartiton table (APP/DATA...) --------------------");
	SPrintF(SIZE, CNT, BUFFER,"\n\tTYP SUB   0xADRS 0xSIZE  SIZE(kB)     LABEL CRYPT");
	for (uint8_t p = 0; p < 0xff; p++)
		printPartitions(SIZE, CNT, BUFFER,esp_partition_type_t(p));
	SPrintF(SIZE, CNT, BUFFER,"\n");
	/*
	 printPartitions (ESP_PARTITION_TYPE_APP);
	 printPartitions (ESP_PARTITION_TYPE_DATA);
	 printPartitions(esp_partition_type_t(32));
	 */
	//for (size_t i = 0; i < 255; i++)		SPrintF(SIZE, CNT, BUFFER,"\n%3i %02X '%c'", i, i, (char)i);
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
	char buffer[256];
	va_list args;
	va_start(args, REASON);
	vsprintf(buffer, REASON, args);
	va_end(args);
	Serial.printf("\n\n\n#### ESP halts!!!\n\n%s\n\n#### Halt(deep_sleep)", buffer);
	esp_deep_sleep_start(); //(SLEEP_MODE_PWR_DOWN);
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
/**
 * able:32756<free:71175<size:324011>left:67431
 */
String heapState(){
	int16_t cnt = 0;
	char buffer[64];
	SPrintF(64, cnt, buffer, "able:%i<free:%i<size:%i>left:%i",
		heap_caps_get_largest_free_block(MALLOC_CAP_8BIT),
		heap_caps_get_free_size(MALLOC_CAP_8BIT),
		heap_caps_get_total_size(MALLOC_CAP_8BIT),
		heap_caps_get_minimum_free_size(MALLOC_CAP_8BIT)
			);
	return String(buffer);
}
void printSysState(){
//	return;
	// plantouillait avec les appels à ESP... dans le Serial.printf ??? avant context ça marchait ???
	Serial.printf("\n*****************************************\n");
	heap_caps_check_integrity_all(true);
	Serial.printf("\n*****************************************\n");
	heap_caps_print_heap_info(MALLOC_CAP_DEFAULT);
	Serial.printf("*****************************************\n");
	heap_caps_dump(MALLOC_CAP_8BIT);
	Serial.printf("*****************************************\n");
	heap_caps_dump_all();
	Serial.printf("*****************************************\n");

	/* Print chip information */
	esp_chip_info_t chip_info;
	esp_chip_info(&chip_info);
	Serial.printf("This is ESP32 chip with %d CPU cores, WiFi%s%s, ", chip_info.cores,
		(chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
		(chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");
	Serial.printf("\n     | PSRAM(mB) | HEAP(kB)| STACK(kB)");
	uint32_t MinFreePsram = ESP.getMinFreePsram();
	uint32_t MinFreeHeap = heap_caps_get_minimum_free_size(MALLOC_CAP_8BIT);
	uint32_t StackHighWaterMark = uxTaskGetStackHighWaterMark(nullptr);
	Serial.printf("\nLEFT | %8.6f  |%8.3f |%6.3f", MinFreePsram / 1000000.0, MinFreeHeap / 1000.0,
		StackHighWaterMark / 1000.0);
	uint32_t PsramSize = ESP.getPsramSize();
	uint32_t HeapSize = heap_caps_get_total_size(MALLOC_CAP_8BIT);
	Serial.printf("\nSIZE | %8.6f  |%8.3f |", PsramSize / 1000000.0, HeapSize / 1000.0);
	uint32_t FreePsram = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
	uint32_t FreeHeap = heap_caps_get_free_size(MALLOC_CAP_8BIT);
	Serial.printf("\nFREE | %8.6f  |%8.3f |", FreePsram / 1000000.0, FreeHeap / 1000.0);
	uint32_t MaxAllocPsram = ESP.getMaxAllocPsram();
	uint32_t MaxAllocHeap = heap_caps_get_largest_free_block(MALLOC_CAP_8BIT);
	Serial.printf("\nABLE | %8.6f  |%8.3f |", MaxAllocPsram / 1000000.0, MaxAllocHeap / 1000.0);
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
//---------------------------------------------------------------------
#define benchLoop 100000
template<typename T>
uint64_t add(const T dummy){
	volatile T a = 123, b = 894, c;
	uint64_t start = esp_timer_get_time();
	for (size_t loop = 0; loop < benchLoop; loop++)
		c = a + b;
	return esp_timer_get_time() - start;
}
//---------------------------------------------------------------------
template<typename T>
uint64_t mul(const T dummy){
	volatile T a = 123, b = 894, c;
	uint64_t start = esp_timer_get_time();
	for (size_t loop = 0; loop < benchLoop; loop++)
		c = a * b;
	return esp_timer_get_time() - start;
}
//---------------------------------------------------------------------
template<typename T>
uint64_t div(const T dummy){
	volatile T a = 123, b = 894, c;
	uint64_t start = esp_timer_get_time();
	for (size_t loop = 0; loop < benchLoop; loop++)
		c = a / b;
	return esp_timer_get_time() - start;
}
//---------------------------------------------------------------------
template<typename T>
uint64_t tri(const T dummy){
	volatile T a = 123, b = 894, c;
	uint64_t start = esp_timer_get_time();
	for (size_t loop = 0; loop < benchLoop; loop++)
		c = sin(a);
	return esp_timer_get_time() - start;
}
//---------------------------------------------------------------------
template<typename T>
uint64_t sqr(const T dummy){
	volatile T a = 123, b = 894, c;
	uint64_t start = esp_timer_get_time();
	if (sizeof(T) <= 4)
		for (size_t loop = 0; loop < benchLoop; loop++)
			c = sqrtf(a);
	else if (sizeof(T) == 8)
		for (size_t loop = 0; loop < benchLoop; loop++)
			c = sqrt(a);
	else
		for (size_t loop = 0; loop < benchLoop; loop++)
			c = sqrtl(a);
	return esp_timer_get_time() - start;
}
//---------------------------------------------------------------------
void benchMark(const size_t SIZE, int16_t &CNT, char *BUFFER){
	enum _typ {
		i16,
		i32,
		flt,
		i64,
		dbl,
		ldb,
		_typend
	};
	enum _op {
		_add,
		_mul,
		_div,
		_sqr,
		_sin,
		_opend
	};
	char *OP[_opend] = { "add", "mul", "div", "sqrt", "sinus" };
	char *TYP[_typend] = { "           int16_t", "   int32_t", "   int64_t", "     float", "    double",
		"longdouble" };
	uint8_t SIZ[_typend] = { sizeof(int16_t), sizeof(int32_t), sizeof(int64_t), sizeof(float),
		sizeof(double), sizeof(long double) };
	uint64_t bm[_opend][_typend];
	int16_t a16;
	int32_t a32;
	int64_t a64;
	float af;
	double ad;
	long double ld;
	for (size_t typ = 0; typ < (size_t)_typend; typ++) {
		SPrintF(SIZE, CNT, BUFFER,"%s(%i)|", TYP[typ], SIZ[typ]);
	}
	_op op;
// @formatter:off
	op=_add; SPrintF(SIZE, CNT, BUFFER,"\n%s\t", OP[op]);
	bm[op][i16]=add(a16),bm[op][i32]=add(a32),bm[op][flt]= add(af),bm[op][i64]=add(a64),bm[op][dbl]=add(ad),bm[op][ldb]=add(ld);
	for (size_t typ = 0; typ < (size_t)_typend; typ++) SPrintF(SIZE, CNT, BUFFER,"%12.1f |", 0.001/((bm[op][typ]/1000000.0)/benchLoop));

	op=_mul; SPrintF(SIZE, CNT, BUFFER,"\n%s\t", OP[op]);
	bm[op][i16]=mul(a16),bm[op][i32]=mul(a32),bm[op][flt]=mul(af),bm[op][i64]=mul(a64),bm[op][dbl]=mul(ad),bm[op][ldb]=mul(ld);
	for (size_t typ = 0; typ < (size_t)_typend; typ++) SPrintF(SIZE, CNT, BUFFER,"%12.1f |", 0.001/((bm[op][typ]/1000000.0)/benchLoop));

	op=_div; SPrintF(SIZE, CNT, BUFFER,"\n%s\t", OP[op]);
	bm[op][i16]=div(a16),bm[op][i32]=div(a32),bm[op][flt]=div(af),bm[op][i64]=div(a64),bm[op][dbl]=div(ad),bm[op][ldb]=div(ld);
	for (size_t typ = 0; typ < (size_t)_typend; typ++) SPrintF(SIZE, CNT, BUFFER,"%12.1f |", 0.001/((bm[op][typ]/1000000.0)/benchLoop));

	op=_sqr; SPrintF(SIZE, CNT, BUFFER,"\n%s\t", OP[op]);
	bm[op][i16]=sqr(a16),bm[op][i32]=sqr(a32),bm[op][flt]=sqr(af),bm[op][i64]=sqr(a64),bm[op][dbl]=sqr(ad),bm[op][ldb]=sqr(ld);
	for (size_t typ = 0; typ < (size_t)_typend; typ++) SPrintF(SIZE, CNT, BUFFER,"%12.1f |", 0.001/((bm[op][typ]/1000000.0)/benchLoop));

	op=_sin; SPrintF(SIZE, CNT, BUFFER,"\n%s\t", OP[op]);
	bm[op][i16]=tri(a16),bm[op][i32]=tri(a32),bm[op][flt]=tri(af),bm[op][i64]=tri(a64),bm[op][dbl]=tri(ad),bm[op][ldb]=tri(ld);
	for (size_t typ = 0; typ < (size_t)_typend; typ++) SPrintF(SIZE, CNT, BUFFER,"%12.1f |", 0.001/((bm[op][typ]/1000000.0)/benchLoop));
// @formatter:on

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
 Silicon revision: 1void benchMark();

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
