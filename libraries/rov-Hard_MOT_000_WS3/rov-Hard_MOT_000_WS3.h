#ifndef _HARD_MOT_000_WS3_
#define _HARD_MOT_000_WS3_
#include "rov-Hard_000_WS3.h"
#include "rov.CONTEXT.h"
///////////////////////////////////////////////////////////////////////
/**
 * \class H_pwmMotors :public H_Module
 */
class H_pwmMotors: public H_Module {
private:
	asmPWMs &data;
	void analogWrite(const t_Aint32Axes VALUES);
	t_Aint32Axes analogRead();

public:
	H_pwmMotors(asmPWMs *PWM, const s_pwm leftCFG, const s_pwm rightCFG,
		const String NAME, int8_t *DBGMAX, const bool ON = true);
	void attachPWMpins();
	void detachPWMpins();
	/**
	 * \fn void Set(const t_AfloatAxes & COMMAND);
	 * \brief thresholds commands on [-1,+1] & scales values in [-pwm._valueMax,+pwm._valueMax]
	 * \\n commands should be in [-1,+1] but pid runs without thresholding (in & out)
	 * \param const t_AfloatAxes & COMMAND values of pwm commands for motors € ...[-1.0,+1.0]...
	 * \return t_Aint32Axes values of pwm setup of motors € [-pwm._valueMax,+pwm._valueMax]
	 */
	t_Aint32Axes Set(const t_AfloatAxes &COMMAND);
	/**
	 * \fn t_Aint32Axes Reset()
	 * \brief emergency stop
	 * \return t_Aint32Axes (={0,0})
	 */
	/*t_Aint32Axes*/
	void Reset();
	t_Aint32Axes Get_valueCur(); // for debug
};
///////////////////////////////////////////////////////////////////////
String S_flow(const uint64_t TMS, const uint8_t IDX, const char CAR, const float L_ERR,
	const float R_ERR, const float L_INT, const float R_INT, const float L_DER,
	const float R_DER, const float L_OUT, const float R_OUT, const char *MSG);
void addMSG(pidQueueData &item, const String MSG);
///////////////////////////////////////////////////////////////////////
/**
 * \class H_pidMotors :public H_Module
 */
class H_pidMotors: public H_Module {
private:
	asmPIDs &data;
	s_xpidCTX &_xpidCTX;
	ringQueue<pidQueueData> *_qu[DEF_MOTORS_NBAXIS];
	double Test = 0;
	float prorata(pidQueueData &x, ringQueue<pidQueueData> &RQ, const uint8_t DEPTH);
	void integrated(pidQueueData &x, ringQueue<pidQueueData> &RQ, const uint8_t DEPTH);
	void derived(pidQueueData &x, ringQueue<pidQueueData> &RQ, const uint8_t DEPTH);
	//	t_AfloatAxes order(t_AfloatAxes CONSIGNE, t_AfloatAxes SENSOR);
	String dump_RQ();
	void dumpRQ(ringQueue<pidQueueData> &RQ);
	void dump2();
	void Reset();
	public:
	H_pidMotors(asmPIDs *PIDS, const s_pid linCFG, const s_pid rotCFG, s_xpidCTX *CONTEXT,
		const String NAME, int8_t *DBGMAX, const bool ON = true);
	/**
	 * \fn t_AfloatAxes H_pidMotors::Run(const t_AfloatAxes & ERROR,const uint64_t MICROTS)
	 * \brief inputs errors signals, computes on the ringQueue and outputs commands signals
	 * \\n signals should be in [-1,+1] but pid runs without thresholding (in & out)
	 * \\n If !isON() return ERROR as next commands
	 * \param const t_AfloatAxes & ERROR
	 * @param MICROTS for ERROR
	 * @return Motors setpoints € [-1.0,+1.0]
	 */
	void Tune(const uint8_t AX, const float KP, const int DP, const float KI, const int DI,
		const float KD, const int DD);
	/**
	 * \fn void Reset()
	 * \brief empties the ringqueue
	 */

	t_AfloatAxes Run(const t_AfloatAxes &ERROR, const uint64_t MICROTS);
	/**
	 * \fn t_AfloatAxes getLastCMD()
	 * \brief gets the lasr command s sent to pwm module
	 * \\n used to simulate speed sensors or to inhibit pid. If !isON() return (0.0,0.0)
	 * \return last commands
	 */
	t_AfloatAxes getLastCMD(); // T simulate speed sensor...
};
///////////////////////////////////////////////////////////////////////
/**
 * \class H_setpointJoystickMotors :public H_Module
 */
class H_setpointSensorMotors: public H_Module {
private:
	void Reset(){
	}
public:
	H_setpointSensorMotors(const String NAME, int8_t *DBGMAX, const bool ON = true);
	/**
	 * \fn t_AfloatAxes Get(int16_t MEASURE[DEF_MOTORS_NBAXIS]);
	 * \brief gets joystick expected data in [-255,+255],thresholds them & scales values on [-1,+1]
	 * \param int16_t MEASURE[DEF_MOTORS_NBAXIS]
	 * \return t_AfloatAxes setpoint speeds
	 */
	t_AfloatAxes Get(int16_t MEASURE[DEF_MOTORS_NBAXIS]);
};
///////////////////////////////////////////////////////////////////////
/**
 * \class H_speedSensorMotors :public H_Module
 */
class H_speedSensorMotors: public H_Module {
private:
	void Reset(){
	}
public:
	H_speedSensorMotors(const String NAME, int8_t *DBGMAX, const bool ON = true);
	/**
	 * \fn t_AfloatAxes Get(H_pidMotors & PIDS);
	 * \brief simulate by last pdata in ringQueues & thresholds values on [-1,+1]
	 * \param H_pidMotors & PIDS
	 * \return t_AfloatAxes current speeds
	 */
	t_AfloatAxes Get(H_pidMotors &PIDS); // simulate by last dat in fifo
};
///////////////////////////////////////////////////////////////////////
#endif
///////////////////////////////////////////////////////////////////////
