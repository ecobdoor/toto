#include "pix_t.h"
///////////////////////////////////////////////////////////////////////
String buf_Info(const uint8_t *BUFFER, const size_t COUNT, const char *LABEL){
	char buffer[128];
	int16_t cnt = 0;
	cnt += sprintf((char*)&buffer[cnt], "%s[%i]@%p->", LABEL ? LABEL : "Buf", COUNT, BUFFER);
	uint32_t Max = 16;
	if (COUNT < 16)
		Max = COUNT;
	for (size_t i = 0; i < Max; i++)
		cnt += sprintf((char*)&buffer[cnt], " %2X", BUFFER[i]);
	if (Max < COUNT)
		cnt += sprintf((char*)&buffer[cnt], "...");
	return String(buffer);
}
//---------------------------------------------------------------------
#ifndef DEF_TEST_PIXL_T
void TEST_MONO8(){
}
void TEST_RGB565(){
}
void TEST_RGB888(){
}
void TEST_RGBcast(){
}
#else
void TEST_INIT_BUF(uint8_t *BUF, const uint8_t LEN = 128){
	for (size_t i = 0; i <= LEN; i++)
		BUF[i] = 0xFF;
	BUF[1] = 1;
	BUF[2] = 2;
	BUF[3] = 3;
	BUF[4] = 4;
}
//---------------------------------------------------------------------
template<typename PIXL>
void dump_PIXL(const char *COMMENT, const PIXL X = nullptr, const uint8_t *BUFFER = nullptr){
	Serial.printf("\n%s", X.dump(COMMENT, BUFFER).c_str());
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void TEST_MONO8(){
	uint8_t BUF[256];
	TEST_INIT_BUF((uint8_t*)&BUF);
	Serial.printf("\n%s\n", buf_Info(BUF,8, "\n#### DEBUG MONO 8  ####").c_str());
	MONO8_t x;
	x = MONO8_t(0x76543210);
	dump_PIXL("x = MONO8_t(0x76543210);        ", x);
	x = *(MONO8_t*)BUF;
	dump_PIXL("x = *(MONO8_t*)BUF;             ", x);
	x = MONO8_t(*(uint8_t*)&BUF[1]);
	dump_PIXL("x = MONO8_t(*(uint8_t*)&BUF[1]);", x);
	for (size_t i = 0; i < 8; i++) {
		x = x * 2;
		dump_PIXL("x = x * 2;                      ", x);
	}
	Serial.printf("\n");

	MONO8_t a = 0x1A01E6;
	dump_PIXL("MONO8_t a = 0x1A01E6;             ", a);
	MONO8_t b = 0x16FF16;
	dump_PIXL("MONO8_t a = 0x16FF16;             ", b);
	MONO8_t c = a + b;
	dump_PIXL("MONO8_t c = a + b;                ", c);
	MONO8_t d = a - b;
	dump_PIXL("MONO8_t d = a - b;                ", d);

	Serial.printf("\n");

	Serial.printf("\n %2X  %2X  %2X  %2X  %2X  %2X  %2X",
		a.bin(), a.bin(), a.bin(), a.bin(), a.bin(), a.bin(), a.bin());
	Serial.printf("\n+%2X -%2X *%2X /%2X &%2X |%2X ^%2X",
		b.bin(), b.bin(), 2, 2, b.bin(), b.bin(), b.bin());
	Serial.printf("\n=== === === === === === ===");
	Serial.printf("\n=%2X =%2X =%2X =%2X =%2X =%2X =%2X",
		c.bin(), d.bin(), (a * 2).bin(), (a / 2).bin(), (a & b).bin(), (a | b).bin(),
		(a ^ b).bin());
	Serial.printf("\n");
	a = 0;
	Serial.printf("\n     norm(%02X)='%3i(dec)'", a.bin(), a.norm());
	a = 0x0F;
	Serial.printf("\n     norm(%02X)='%3i(dec)'", a.bin(), a.norm());
	a = 0xF0;
	Serial.printf("\n     norm(%02X)='%3i(dec)'", a.bin(), a.norm());
	a = 0xFF;
	Serial.printf("\n     norm(%02X)='%3i(dec)'", a.bin(), a.norm());
	Serial.printf("\n");

	Serial.printf("\n");
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void TEST_RGB888(){
	uint8_t BUF[256];
	TEST_INIT_BUF((uint8_t*)&BUF);
	Serial.printf("\n%s\n", buf_Info(BUF,8, "\n#### DEBUG RGB 888 ####").c_str());
	RGB888_t x;
	x = RGB888_t(0x76543210);
	dump_PIXL("x = RGB888_t(0x76543210);                            ", x);
	Serial.printf("Easy to set a RGB color as 0x123456 (Red=12, Green=34, Blue=56)...");
	x = RGB888_t(__builtin_bswap32(0x76543210));
	dump_PIXL("x = RGB888_t(__builtin_bswap32(0x76543210));         ", x);
	Serial.printf("!!! FALSE don't use  __builtin_bswap32!!!");
	x = RGB888_t(__builtin_bswap32(*(uint32_t*)&BUF[1]));
	dump_PIXL("x = RGB888_t(__builtin_bswap32(*(uint32_t*)&BUF[1]));", x);
	Serial.printf("!!!! FALSE don't use  __builtin_bswap32!!!");
	x = RGB888_t(*(uint32_t*)&BUF[1]);
	dump_PIXL("x = RGB888_t(*(uint32_t*)&BUF[1]);                   ", x);
	Serial.printf("Ok (Buf[4] is ignored)");
	x = *(RGB888_t*)&BUF[1];
	dump_PIXL("x = *(RGB888_t*)&BUF[1];                             ", x);
	RGB888_t *p = &x;
	dump_PIXL("RGB888_t *p = &x;                                    ", *p);
	Serial.printf(" p=%p", p);
	TEST_INIT_BUF((uint8_t*)&BUF);
	*(RGB888_t*)&BUF[1] = 16 * x.bin();
	dump_PIXL("*(RGB888_t*)&BUF[1] = 16 * x.bin();                  ", *(RGB888_t*)&BUF[1], BUF);
	TEST_INIT_BUF((uint8_t*)&BUF);
	*(RGB888_t*)&BUF[1] = 16 * x;
	dump_PIXL("*(RGB888_t*)&BUF[1] = 16 * x;                        ", *(RGB888_t*)&BUF[1], BUF);
	Serial.printf(
		"\n*(uint32_t*)&BUF[1] = 16 * x.bin();                   !!! NOT POSSIBLE cannot convert 'RGB888_t' to 'uint32_t' {aka 'unsigned int'} in assignment");
	Serial.printf(
		"\n*(uint32_t*)&BUF[1] = 16 * x;                         !!! NOT POSSIBLE cannot convert 'RGB888_t' to 'uint32_t' {aka 'unsigned int'} in assignment");
	Serial.printf("\n      to get the uint32_t value of a RGB888_t, use .bin()");
	Serial.printf("\n");

	RGB888_t a = 0x1A01E6;
	dump_PIXL("RGB888_t a = 0x1A01E6;                               ", a);
	RGB888_t b = 0x16FF16;
	dump_PIXL("RGB888_t b = 0x16FF16;                               ", b);
	RGB888_t c = a + b;
	dump_PIXL("RGB888_t c = a + b;                                  ", c);
	RGB888_t d = a - b;
	dump_PIXL("RGB888_t d = a - b;                                  ", d);

	Serial.printf("\n");

	Serial.printf("\n %6X  %6X  %6X  %6X  %6X  %6X  %6X",
		a.bin(), a.bin(), a.bin(), a.bin(), a.bin(), a.bin(), a.bin());
	Serial.printf("\n+%6X -%6X     *%2X     /%2X &%6X |%6X ^%6X",
		b.bin(), b.bin(), 2, 2, b.bin(), b.bin(), b.bin());
	Serial.printf("\n======= ======= ======= ======= ======= ======= =======");
	Serial.printf("\n=%6X =%6X =%6X =%6X =%6X =%6X =%6X",
		c.bin(), d.bin(), (a * 2).bin(), (a / 2).bin(), (a & b).bin(), (a | b).bin(),
		(a ^ b).bin());

	Serial.printf("\n");

	a = 0;
	Serial.printf("\n     norm(%06X)='%3i(dec)'", a.bin(), a.norm());
	a = 0xFF0000;
	Serial.printf("\n     norm(%06X)='%3i(dec)'", a.bin(), a.norm());
	a = 0x00FF00;
	Serial.printf("\n     norm(%06X)='%3i(dec)'", a.bin(), a.norm());
	a = 0x0000FF;
	Serial.printf("\n     norm(%06X)='%3i(dec)'", a.bin(), a.norm());
	a = 0xFFFFFF;
	Serial.printf("\n     norm(%06X)='%3i(dec)'", a.bin(), a.norm());

	Serial.printf("\n");
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void TEST_RGB565(){
	uint8_t BUF[256];
	TEST_INIT_BUF((uint8_t*)&BUF);
	Serial.printf("\n%s\n", buf_Info(BUF,8, "\n#### DEBUG RGB 565 ####").c_str());
	RGB565_t x;
	x = RGB565_t(0x76543210);
	dump_PIXL("x = RGB565_t(0x76543210);               ", x);
	x = *(RGB565_t*)&BUF[1];
	dump_PIXL("x = *(RGB565_t*)&BUF[1];                ", x, BUF);
	x = RGB565_t(*(uint16_t*)&BUF[1]);
	dump_PIXL("x = RGB565_t(*(uint16_t*)&BUF[1]);      ", x, BUF);
	x = RGB565_t(*(uint16_t*)&BUF[1]);
	dump_PIXL("x = RGB565_t(*(uint16_t*)&BUF[1]));     ", x, BUF);
	TEST_INIT_BUF((uint8_t*)&BUF);
	*(RGB565_t*)&BUF[1] = 16 * x.bin();
	dump_PIXL("*(RGB565_t*)&BUF[1] = 16 * x.bin();     ", *(RGB565_t*)BUF, BUF);
	TEST_INIT_BUF((uint8_t*)&BUF);
	*(RGB565_t*)&BUF[1] = 16 * x;
	dump_PIXL("*(RGB565_t*)&BUF[1] = 16 * x;           ", *(RGB565_t*)BUF, BUF);
	TEST_INIT_BUF((uint8_t*)&BUF);
	*(uint16_t*)&BUF[1] = 16 * x.bin();
	dump_PIXL("*(uint16_t*)&BUF[1] = 16 * x.bin();     ", *(RGB565_t*)BUF, BUF);
	TEST_INIT_BUF((uint8_t*)&BUF);
	*(uint16_t*)&BUF[1] = 16 * x;
	dump_PIXL("*(uint16_t*)&BUF[1] = 16 * x;           ", *(RGB565_t*)BUF, BUF);
	Serial.printf("\n");
	RGB565_t a = 0b1111100000110000; // 0b 00110000 11111000 -> 001 10000 11111 000
	dump_PIXL("RGB565_t a = 0b1111100000110000;        ", a);
	RGB565_t b = 0b0011100100000100; // 0b 00000100 00111001 -> 000 00100 00111 001
	dump_PIXL("RGB565_t b = 0b0011100100000100;        ", b);
	RGB565_t c = a + b;
	dump_PIXL("RGB565_t c = a + b;                     ", c);
	RGB565_t d = a - b;
	dump_PIXL("RGB565_t d = a - b;                     ", d);

	Serial.printf("\n");

	Serial.printf(
		"\n "B565_PAT"  "B565_PAT"  "B565_PAT"  "B565_PAT"  "B565_PAT"  "B565_PAT"  "B565_PAT"",
		BIN16(a.bin()), BIN16(a.bin()), BIN16(a.bin()), BIN16(a.bin()), BIN16(a.bin()),
		BIN16(a.bin()),
		BIN16(a.bin()));
	Serial.printf(
		"\n+"B565_PAT" -"B565_PAT"                   *%2X                    /%2X  &"B565_PAT" |"B565_PAT" ^"B565_PAT"",
		BIN16(b.bin()), BIN16(b.bin()), 2, 2, BIN16(b.bin()), BIN16(b.bin()), BIN16(b.bin()));
	Serial.printf(
		"\n---------------------- ---------------------- ---------------------- ---------------------- ---------------------- ---------------------- ----------------------");
	Serial.printf(
		"\n="B565_PAT" ="B565_PAT" ="B565_PAT" ="B565_PAT" ="B565_PAT" ="B565_PAT" ="B565_PAT"",
		BIN16(c.bin()), BIN16(d.bin()), BIN16((a * 2).bin()), BIN16((a / 2).bin()),
		BIN16((a & b).bin()),
		BIN16((a | b).bin()), BIN16((a ^ b).bin()));

	Serial.printf("\n");

	Serial.printf(
		"\n "H565_PAT"  "H565_PAT"  "H565_PAT"  "H565_PAT"  "H565_PAT"  "H565_PAT"  "H565_PAT"",
		H565(a.bin()), H565(a.bin()), H565(a.bin()), H565(a.bin()), H565(a.bin()),
		H565(a.bin()),
		H565(a.bin()));
	Serial.printf(
		"\n+"H565_PAT" -"H565_PAT"        *%2X         /%2X  &"H565_PAT" |"H565_PAT" ^"H565_PAT"",
		H565(b.bin()), H565(b.bin()), 2, 2, H565(b.bin()), H565(b.bin()), H565(b.bin()));
	Serial.printf(
		"\n----------- ----------- ----------- ----------- ----------- ----------- -----------");
	Serial.printf(
		"\n="H565_PAT" ="H565_PAT" ="H565_PAT" ="H565_PAT" ="H565_PAT" ="H565_PAT" ="H565_PAT"",
		H565(c.bin()), H565(d.bin()), H565((a * 2).bin()), H565((a / 2).bin()),
		H565((a & b).bin()), H565((a | b).bin()), H565((a ^ b).bin()));

	Serial.printf("\n");

	dump_PIXL("__builtin_bswap16(0b0000000000000000)    ", RGB565_t(__builtin_bswap16(0b0000000000000000)));
	dump_PIXL("__builtin_bswap16(0b1111100000000000)    ", RGB565_t(__builtin_bswap16(0b1111100000000000)));
	dump_PIXL("__builtin_bswap16(0b0000011111100000)    ", RGB565_t(__builtin_bswap16(0b0000011111100000)));
	dump_PIXL("__builtin_bswap16(0b0000000000011111)    ", RGB565_t(__builtin_bswap16(0b0000000000011111)));
	dump_PIXL("__builtin_bswap16(0b1111111111111111)    ", RGB565_t(__builtin_bswap16(0b1111111111111111)));
	Serial.printf("\n");
	dump_PIXL("__builtin_bswap16(0b0000100000000000)    ", RGB565_t(__builtin_bswap16(0b0000100000000000)));
	dump_PIXL("__builtin_bswap16(0b0001000000000000)    ", RGB565_t(__builtin_bswap16(0b0001000000000000)));
	dump_PIXL("__builtin_bswap16(0b0001100000000000)    ", RGB565_t(__builtin_bswap16(0b0001100000000000)));
	dump_PIXL("__builtin_bswap16(0b0010000000000000)    ", RGB565_t(__builtin_bswap16(0b0010000000000000)));

	Serial.printf("\n");
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template<typename A, typename B>
void dump_RGBcast(const char *COMMENT, const A a, const char *SENS, const B b){
	Serial.printf("\n%s%s%s", a.dump(COMMENT).c_str(), SENS, b.dump().c_str());
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void TEST_RGBcast(){
	uint8_t BUF[256];
	TEST_INIT_BUF((uint8_t*)&BUF);
	Serial.printf("\n%s\n", buf_Info(BUF,8, "\n#### DEBUG RGB CASTING ####").c_str());
	Serial.printf("\n");
	uint16_t bits = 1;
	for (size_t i = 0; i < 16; i++) {
		RGB555_t a = RGB555_t(bits);
		RGB565_t b = rgb565(a); //a=>b
		RGB555_t c = rgb555(b); //c<=b
		uint16_t cnt = 0;
		char buffer[256];
		sprintf(buffer, "555=>565; `"BIN16_PAT"`", BIN16(bits));
		dump_RGBcast(buffer, a, " =>", b);
		sprintf(buffer, "555<=565; `"BIN16_PAT"`", BIN16(bits));
		dump_RGBcast(buffer, c, " <=", b);
		Serial.printf("\n");
		bits <<= 1;
	}
	Serial.printf("\n");
	Serial.printf("\n");
}
#endif
///////////////////////////////////////////////////////////////////////
