#include <esp32_STATS.h>
#include "esp32_SPRINT.h"
using namespace std;
//---------------------------------------------------------------------
void c_stat_3F::calc_Norm(const float A, const float B, const float C){
	if (ptr == 0) {
		N = 0;
		for (uint8_t i = 0; i < dimAvg; i++) {
			N += _rqN[i];
		}
		N /= dimAvg;
	}
	float Norm = sqrtf(A * A + B * B + C * C) + _offset;
	if (Norm < Min)
		Min = Norm;
	if (Max < Norm)
		Max = Norm;
	_rqN[ptr] = Norm;
	ptr = (ptr + 1) % dimAvg;
}
//---------------------------------------------------------------------
String c_stat_3F::norm(const char *TITLE, const float COEFF){
	int16_t cnt = 0;
	char buf[64];
	SPrintF(sizeof(buf), cnt, buf, "%s%+9.3f<%+9.3f<%+9.3f %i", TITLE,
		COEFF * Min, COEFF * N, COEFF * Max,ptr);
	return String(buf);
}
//---------------------------------------------------------------------
void c_stat_3F::calc_Cent(const float A, const float B, const float C){
	if (ptr == 0) {
		R = 0, S = 0, T = 0;
		for (uint8_t i = 0; i < dimAvg; i++) {
			R += _rqR[i];
			S += _rqS[i];
			T += _rqT[i];
		}
		R /= dimAvg, S /= dimAvg, T /= dimAvg;
	}
	_rqR[ptr] = A;
	_rqS[ptr] = B;
	_rqT[ptr] = C;
	ptr = (ptr + 1) % dimAvg;
}
//---------------------------------------------------------------------
String c_stat_3F::cent(const char *TITLE, const float COEFF){
	int16_t cnt = 0;
	char buf[64];
	SPrintF(sizeof(buf), cnt, buf, "%s%+9.3f %+9.3f %+9.3f", TITLE, COEFF * R, COEFF * S,
		COEFF * T);
	return String(buf);
}

