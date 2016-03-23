#ifndef _NODE_H_
#define _NODE_H_

//#include "Point.h"
#include "Path.h"
#include "Resource.h"
#include "NSMath.h"
#include "BaseControl.h"

#include <utility>
#include <vector>
#include <iostream>
#include <cassert>

using namespace std;


typedef vector< Resource* > BUS_RESOURCE;
typedef vector< Resource* >::iterator BUS_RESOURCE_ITER;

// =============== the base class of Node ==================
class CNode 
{
public:
	CNode(double dLoc_x = 0.0, double dLoc_y = 0.0, double dVelocity = 0.0, bool isStart = false):
		m_dLoc_X(dLoc_x), 
		m_dLoc_Y(dLoc_y),
		m_dVelocity(dVelocity), 
		m_bStartMove(isStart){}
	//	CNode(Point &p, double dVelocity = 0.0);
		virtual ~CNode() {}

	// ------------------ A. location and velocity. -------------
public:
	//	void SetLocation(Point &p);
	//	Point& GetPoint();

	void SetLocation(double dLoc_x, double dLoc_y);
	void SetLoc_X(double dLoc_x);
	void SetLoc_Y(double dloc_y);
	double GetLoc_X();
	double GetLoc_Y();

	void SetVelocity(double dVelocity);
	double GetVelocity();
	double GetTrueVelocity();

	// --------------------- B.mobility. ---------------------
public:
	bool Get_isStartMove();
	void Set_isStartMove(bool bBeginMove);


private:
	double	m_dLoc_X;     // X-axis
	double	m_dLoc_Y;     // Y-axis
	//	Point	m_dLoc_Point; // the point signing the location
	double	m_dVelocity;  //Velocity

private:
	bool	m_bStartMove; // the flag of starting move;
};

// ==================== Bus Node ==========================
class CBusNode: public CNode
{
public:
	//	CBusNode(Point &p, double dVelocity = 0.0);
	CBusNode(double dlocx = 0.0, double dlocy = 0.0, double dv = 0.0, bool isStart = false)
		:CNode(dlocx, dlocy, dv, isStart)
		, m_pBusPath( NULL )
		, m_dBusCurDirection( 0.0 )
		, m_dBusTransRange( BUS_TR )
		, m_nBusID (0)
		, nCount_Record_Bus(0){}

	//CBusNode()
	virtual ~CBusNode(){
		SAFELY_DELETE_PTR( m_pBusPath );
		m_dBusCurDirection = 0.0;

		Set_isStartMove(false);
	}

	// ------------------ A. Path and Direction -------------------
public:
	// get the bus's path.
	void Get_TheBus_path(CBusNode * pBusNode);
	// get a path;
	Path* GetAPath();

	//set the path.
	void SetAPath(Path *p);

	//set the current direction.
	void SetBusCurDirection(double bCurDir);

	//get the current direction.
	double GetBusCurDirection();

	// reset the bus direction rely on the next point of path.
	void Reset_Direction_rely_on_path(double dLocB_x, double dLocB_y, double dLoc_x, double dLoc_y );

	// ����bus��λ��ȷ��������·�εĿ�ʼ�ڵ�
	Point *Get_PathBeginPoint_Of_Bus_Location(double dLoc_x, Path *pPath);

	// ����bus��λ��ȷ��������·�ε���ֹ�ڵ�
	Point *Get_PathEndPoint_Of_Bus_Location(double dLoc_x, Path *pPath);
	
private:
	// every bus correspond to a path;	
	Path	*m_pBusPath;  

	// the curDrirection of bus.
	double	m_dBusCurDirection;

	// -------------------B. Resource. --------------
public:
	// buffer the  resources.
	void SetTheResources(std::vector<Resource> R);

	// Get the resources of the buffer.
	std::vector<Resource>* GetTheResources_ofBuffer();

	// ��bus�����Ƿ���car���������Դ
	bool FindCarRequestRes_IfAtBus(char type);

	// �õ��������������Դ.
	BUS_RESOURCE * get_BusContainer_Resource();

	// ���ù������������Դ.
	void SetBusAllocationResource(BUS_RESOURCE pBusRes);

	// ������Դ���͵õ�����Ӧ�ĵ���Դ.
	Resource * GetResource_viaResType(char type);

	// ��bus����Դ��Ϊ��.
	void ResetBusBufferToEmpty();

public:
	BUS_RESOURCE	m_pVecResource; // ��Դ����.

// -----------------------C. Contact ------------------------
public:
	//get the id of contact car.
//	int GetTheContactCar_ID();
//	int GetBusSelf_ID();

	// �õ�bus�Ĵ��䷶Χ
	double GetBusTransRange();

	void SetBusId(int id);
	int GetBusId();

private:

	// the bus transmission range;
	double m_dBusTransRange;
	int m_nBusID;

public:
	void SetBusRecordCount(unsigned int nC);
	unsigned int GetBusRecordCount();
	void SetBusRecordCount_IN();

private:
	unsigned int nCount_Record_Bus;

};

// ===================== Car Node ===========================
class CCarNode: public CNode
{
public:
	CCarNode(double dlocx = 0.0, double dlocy = 0.0, double dv = 0.0, bool isStart = false, double carCurDirection = 0.0, unsigned int nNum = 0, bool isSuccess = false, char type = ' ', unsigned int nsize = 0, double dTime = 0.0, double dReqT = 0.0, double dRecT = 0.0, bool isOutTime = false, double dCarTransR = CAR_TR, bool first = false, int id = 0)
		:CNode(dlocx, dlocy, dv, isStart)
		, m_dCarCurDirection(carCurDirection)
		, m_nNumContant_WithBus(nNum)
		, m_isSuccess_to_GetTheFullResource(isSuccess)
		, m_cType(type)
		, m_nSize(nsize)
		, m_dTime(dTime)
		, m_dRequestTime(dReqT)
		, m_dReceiveTime(dRecT)
		, m_isOutofTime(isOutTime)
		, m_dCarTransRange(dCarTransR)
		, nCount_Record_Car(0)
		, m_nCarID(id){
			m_pObjRC = new RdmCtr();
		}


	virtual ~CCarNode(){
		SAFELY_DELETE_PTR( m_pObjRC );
		m_dCarCurDirection = 0.0;
		Set_isStartMove(false);
	}



private:

// ------------------- A.Direction -------------------
public:
	// get next direction via random.
	double GetCurDirection_ByRandom();

	// get current direction.
	double GetCurDirection_RWALK();

	// Set current direction.
	void SetCurDirection_RWALK(double dCarCurDirection);

	// ���÷�����һ��ȷ����ֵ.
	// Reset the Direction with an determined value of radian Angle.
	void Reset_Direction_RWALK(double dRadianValue);

	// ���õ�ǰ���� for RWALK.
	// Reset the Direction randomly with an radian Angle in [dLowerBound, dUpperBound).
	void Reset_Direction_random_RWALK( double dLowerBound, double dUpperBound );

private:
	double m_dCarCurDirection;

// --------------------B.Mobility -------------------
private:
private:
	// Return an Random Radian Angle in [dLowerBound, dUpperBound).
	double Create_a_new_random_Direction(double dLowerBound, double dUpperBound);


//---------------------C.Resources ---------------------
public:
// 	// Generate car's resource via random.(the request resource)
// 	void SetCarResource_ByRandom();
// 	
// 	//get car's resource(one resource)
// 	Resource* GetCarResource();
// 
// 	bool FindCarRequestRes_IfAtBus(char type);
// 
// private:
// 	Resource * CarResource;


// --------------------D.Contact ---------------------

public:
	std::vector<int> m_vecBusID;

	void SetBusID_toEmpty();
	void SetBusID(int id);

public:

	// �õ�Car�Ĵ��䷶Χ
	double GetCarTransRange();

	// �õ�Car���ӳ����̶�(���Ƿ�ʱ��û�б�Ҫ�ڽ�������)
	bool isCarOutofTime();

	// ����Car�Ƿ�ʱ����Ϣ
	void SetCarisOutofTime(bool isOutOftime);
	
	// ����Car��������Դ�Ĵ�Сֵ
	void SetCarRequestResSize(unsigned int nSize);

	// �õ���Դ�Ĵ�С
	unsigned int GetCarResSize();

	// ����Car���������Դ
	void SetCarRequestResourceType(char cType);

	// �õ�car���������Դ
	char GetCarRequestResourceType();

	// ���������Ĵ���
	void SetContactNum();
	void SetContactNum_toNum(int nNum);

	// �õ������Ĵ���
	unsigned int GetContactNum();

	// �õ���һ����bus�����ı�־
	bool GetFirstContactWithBus();

	// ���õ�һ����bus�����ı�־
	void SetFirstContactWithBus(bool isFirst);

	// �õ�ȡ���ݳɹ��ı�־
	bool GetSuccess();

	// ����ȡ���ݳɹ��ı�־
	void SetSuccess(bool isSuccess);

	// ����car������Դ��ʱ��
	void SetCarRequestTime(double dRTime);

	// �õ�car������Դ��ʱ��
	double GetCarRequestTime();

	// ����car������Դ��ʱ��
	void SetCarReceiveTime(double dRTime);

	// �õ�car������Դ��ʱ��
	double GetCarReceiveTime();

	// ������Դ��ʱ��
	void SetCarResTime(double dTime);
	
	// �õ���Դ��ʱ��
	double GetCarResTime();

	int GetCarID();
	void SetCarID(int id);

private:

	int m_nCarID;
	// the car tansmission range.
	double m_dCarTransRange;
	
	// is success to get full resource for car?
	bool m_isSuccess_to_GetTheFullResource;

	// car request type of resource.
	char m_cType; 

	// size of the resource.
	unsigned int m_nSize;

	// time of the resource.
	double m_dTime;

	// judge whether out of time.
	bool m_isOutofTime;

	// the contact number with bus.
	unsigned int m_nNumContant_WithBus;

	// car ������Դ��ʱ��
	double m_dRequestTime;

	// car �õ���Դ��ʱ��
	double m_dReceiveTime;

	// �Ƿ��һ����bus����
	//bool m_isFirstContactBus;

public:
	void SetCarRecordCount(unsigned int nC);
	unsigned int GetCarRecordCount();
	void SetCarRecordCount_IN();

private:
	unsigned int nCount_Record_Car;

private:
	RdmCtr		*m_pObjRC;


};

#endif