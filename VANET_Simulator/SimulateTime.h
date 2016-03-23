#ifndef _SIMULATE_TIME_H_
#define _SIMULATE_TIME_H_


class SimulateTime
{
public:
	SimulateTime();
public:

	//ʱ�䲽����ms��
	static void TimeStepForward(void);
	static double Get_CurrentTime(void);
	static int GetTimeStep_ms(void);

	static void Reset_NS_time(void);

	//ʱ�䲽����s��.
	static void TimeStepForward_s();
	static void GetTimeStep_s();

private:
	static double m_dCurrentTime;
	static const int m_scnNS_TimeStep_ms;
};


#endif