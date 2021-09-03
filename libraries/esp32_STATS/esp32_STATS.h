#ifndef _esp32_STATS_
#define _esp32_STATS_
#include "Arduino.h"
using namespace std;
#define pi  3.141592653589793238462643383279502884f
///////////////////////////////////////////////////////////////////////
const uint8_t dimAvg = 10;

class c_stat_3F {
private:
	uint8_t ptr = 0;
	float _offset, Min, Max, N, R, S, T;
	float _rqN[dimAvg] = { }, _rqR[dimAvg] = { }, _rqS[dimAvg] = { }, _rqT[dimAvg] = { };

public:
	c_stat_3F(){
	}
	;
	void init(const float OFFSET = 0.0);
	void calc_Norm(const float A, const float B, const float C);
	String norm(const char *TITLE, const float COEFF = 1.0);
	void calc_Cent(const float A, const float B, const float C);
	String cent(const char *TITLE, const float COEFF = 1.0);
};
inline void c_stat_3F::init(const float OFFSET){
	ptr = 0;
	_offset = OFFSET, Min = +1000000000000.0, Max = -1000000000000.0;
	N = 0.0, R = 0.0, S = 0.0, T = 0.0;
	;
}
///////////////////////////////////////////////////////////////////////
#endif
