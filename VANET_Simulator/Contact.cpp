#include "Contact.h"

// �ж������ڵ��Ƿ��ڴ��䷶Χ��
bool Contact::Judge_Two_Point_Whether_Within_Transmission_Range(CBusNode *pBus, CCarNode *pCar)
{
	double dBusTransR = pBus->GetBusTransRange();
	//double dCarTransR = pCar->GetCarTransRange();
	//double dTransRange = dBusTransR + dCarTransR;

	double dBusLocX = pBus->GetLoc_X();
	double dBusLocY = pBus->GetLoc_Y();

	double dCarLocX = pCar->GetLoc_X();
	double dCarLocY = pCar->GetLoc_Y();

	return The_Two_Point_Meet_each_Other_with_TR(dBusLocX, dBusLocY, dCarLocX, dCarLocY, dBusTransR);
}


// �����ڵ�����
bool Contact::The_Two_Point_Meet_each_Other_with_TR( double x1, double y1, double x2, double y2, double dTr)
{
	//double dTr1 = dTr * 0.1;

	double dDist = sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) );
	if ( dDist <= dTr )
	{
		return true;
	}
	else	
		return false;
}

// �����ڵ�����
bool Contact::The_Two_Point_Meet_each_Other( double x1, double y1, double x2, double y2)
{
	double dDist = sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) );
	if ( dDist <= m_dBusCarTransRange )
	{
		return true;
	}
	else	
		return false;
}


// ������֮��Ľ���
void Contact::Transmission_Between_Tow_Vehicles(CBusNode* pBus, CCarNode *pCar, double dCurTime, int nCase)
{
	assert(NULL != pBus);
	assert(NULL != pCar);
	assert(dCurTime > 0);
	
	double dJudgeOutTime = 0.0;

	// �ж��������Ƿ��ڴ��䷶Χ��
	if (Judge_Two_Point_Whether_Within_Transmission_Range(pBus, pCar)) {

		//std::cout << "bingo" << std::endl;

		
		
		
		// �жϵ�ǰ�������Ƿ񳬳������ӳ����̶Ȼ�ȡ��Դ�ɹ�
		if ( false == pCar->isCarOutofTime() && false == pCar->GetSuccess()) {
			//�õ�Car���������Դ����
			char type = pCar->GetCarRequestResourceType();
			//��bus��Ѱ���Ƿ��и���Դ, ����У������.
			if (pBus->FindCarRequestRes_IfAtBus(type)) {

				//���������Ĵ���. + 1
				pCar->SetContactNum();
				
				int busID = pBus->GetBusId();
				pCar->SetBusID(busID);

				// ��¼bus��car���������
				//Record_Bus_Car_Contact_Situation(pBus, pCar, nCase);

				// �����������Ϊ��һ��
				if (1 == pCar->GetContactNum()) {
					//����car������Դ��ʱ��Ϊ��ǰʱ��
					pCar->SetCarRequestTime(dCurTime);
					// ���ݸ������ҵ�����Ӧ����Դ
					Resource *pRes = pBus->GetResource_viaResType(type);
					// ��ȡ����Դ�Ĵ�С��ʱ��.
					unsigned int nSize = pRes->GetResSize();
					double dTime = pRes->GetResTime();
					//������Դ��ʱ��ʹ�С����car
					pCar->SetCarRequestResSize(nSize);
					pCar->SetCarResTime(dTime);

					//���������Ƿ�ʹ�С��ȣ�����˵��ȡ��Դ�ɹ�.
					if (pCar->GetContactNum() == pCar->GetCarResSize()) {
						//������Դ�ɹ���ȡ�ı�־
						pCar->SetSuccess(true);

						std::cout << "ȡ��Դ�ɹ���\n";
						//���ý���ʱ��
						pCar->SetCarReceiveTime(dCurTime);

						// ��¼bus��car���������
						Record_Bus_Car_Contact_Situation(pBus, pCar,nCase, dJudgeOutTime);
					}
				}
				// ���粻�ǵ�һ������
				else {
					//�����Ƿ�ʱ
					double dCarRTime = pCar->GetCarRequestTime();
					double dDelayTime = pCar->GetCarResTime();
					dJudgeOutTime = dCurTime - dCarRTime;
					//�ж��Ƿ�ʱ
					if (dJudgeOutTime > dDelayTime) {
						//���ó�ʱ�ͻ�ȡ��Դʧ��
						pCar->SetCarisOutofTime(true);
						pCar->SetSuccess(false);

						std::cout << "��ʱ\n";

						// ��¼bus��car���������
						Record_Bus_Car_Contact_Situation(pBus, pCar, nCase, dJudgeOutTime);
					}
					else {
						//���������Ƿ�ʹ�С��ȣ�����˵��ȡ��Դ�ɹ�.
						if (pCar->GetContactNum() == pCar->GetCarResSize()) {
						//������Դ�ɹ���ȡ�ı�־
						pCar->SetSuccess(true);

						std::cout << "ȡ��Դ�ɹ���\n";
						//���ý���ʱ��
						pCar->SetCarReceiveTime(dCurTime);

						// ��¼bus��car���������
						Record_Bus_Car_Contact_Situation(pBus, pCar,nCase, dJudgeOutTime);
						}
					}
				}
			}
		}
	}
}

// ��¼bus��car���������
void Contact::Record_Bus_Car_Contact_Situation(CBusNode *pBus, CCarNode *pCar, int nCase, double dJudgeTime)
{
	int carID = pCar->GetCarID();
	std::vector<int> vecID = pCar->m_vecBusID;

	char cType = pCar->GetCarRequestResourceType();

	Resource *pRes = pBus->GetResource_viaResType(cType);

	unsigned int nSize = pRes->GetResSize();
	double dPop = pRes->GetResPop();
	double dTime = pRes->GetResTime();

	int dContact = pCar->GetContactNum();
	bool isOutTime = pCar->isCarOutofTime();
	bool isSuccess = pCar->GetSuccess();

	double dRequestTime = pCar->GetCarRequestTime();
	double dReceiveTime = pCar->GetCarReceiveTime();

	//std::cout << "hello11111111111111\n"; 
	

	LogFile::instance()->m_ofContact
		<< nCase << "\t";
	LogFile::instance()->m_ofContact << " ( ";
	for (std::vector<int>::iterator it = vecID.begin(); it != vecID.end(); ++ it) {
		LogFile::instance()->m_ofContact << *it << " ";
	}
	LogFile::instance()->m_ofContact << " ) \t"
		<< carID << "\t"
		<< cType << "\t"
		<< nSize << "\t"
		<< dPop << "\t"
		<< dTime << "\t"
		<< dContact << "\t"
		<< isOutTime << "\t"
		<< isSuccess << "\t"
		<< dRequestTime << "\t"
		<< dReceiveTime << "\t"
		<< dJudgeTime << "\n\n";
}