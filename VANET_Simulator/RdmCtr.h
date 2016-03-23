#ifndef _RDM_CTR_H_
#define _RDM_CTR_H_

#include <time.h>

const unsigned long maxshort = 65535L;
const unsigned long multiplier = 1194211693L;
const unsigned long adder = 12345L;

class RdmCtr
{
public:
	RdmCtr(unsigned long s = 0);
// 	RdmCtr() {
// 		m_lSeed = (unsigned long)time(NULL);
// 	}

	unsigned int GetRdmIntNum( unsigned int nMinNum, unsigned int nMaxNum);

	double GetRdmRealNum( double dMinNum, double dMaxNum);

	unsigned int GetRdmIntNumNotEqual(unsigned int nMinNum, unsigned int nMaxNum);

private:
	// ����[0 ~ n-1) ֮����������
	unsigned int Random(unsigned long n);

	// ����[0, 1) ֮������ʵ��
	double fRandom();

private:
	unsigned long m_lSeed;
};


#endif