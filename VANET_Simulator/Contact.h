#ifndef _CONTACT_H_
#define _CONTACT_H_

#include "Node.h"
#include "Log.h"
#include <cmath>

class Contact 
{
public:
	Contact(double dTR = 0.0):m_dBusCarTransRange(dTR) {}

	~Contact() { }

public:

	void SetBCTransRange(double dTransRange) {
		m_dBusCarTransRange = dTransRange;
	}

	double GetBCTransRange() {
		return m_dBusCarTransRange;
	}

	// �ж������ڵ��Ƿ��ڴ��䷶Χ��
	bool Judge_Two_Point_Whether_Within_Transmission_Range(CBusNode *pBus, CCarNode *pCar);

	// �����ڵ�����
	bool The_Two_Point_Meet_each_Other( double x1, double y1, double x2, double y2);
	
	// �����ڵ�����
	bool The_Two_Point_Meet_each_Other_with_TR( double x1, double y1, double x2, double y2, double dTr);

	// ������֮��Ľ���
	void Transmission_Between_Tow_Vehicles(CBusNode* pBus, CCarNode *pCar, double dCurTime, int nCase);

	// ��¼bus��car���������
	void Record_Bus_Car_Contact_Situation(CBusNode *pBus, CCarNode *pCar, int nCase, double dJudgeTime);

private:

	double m_dBusCarTransRange;


};
#endif//_CONTACT_H_