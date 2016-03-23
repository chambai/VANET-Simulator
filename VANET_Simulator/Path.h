#ifndef _PATH_H_
#define _PATH_H_

//#include "Point.h"

#include "Topology.h"
#include "BaseControl.h"
#include "BaseDef.h"
#include "NSMath.h"
#include "RdmCtr.h"
#include <vector>

class Point 
{
public:
	Point(double dx = 0.0, double dy = 0.0):m_dLoc_X(dx),m_dLoc_Y(dy) { 

	}
	~Point() {}

public:
	double m_dLoc_X;
	double m_dLoc_Y;
};


typedef std::vector< Point* > PATHPOINT;
typedef std::vector< Point* >::iterator PATHPOINT_ITER;

class Path 
{
public:
	Path(Point *db = NULL, Point *de = NULL):m_dBeginPoint(db), m_dEndPoint(de) {
		rtr = new RdmCtr();
	}
	~Path(){}
public:
	//void SetBeginPoint(double dMinX,); // set the point of begin;

	Point* GetBeginPoint();
	Point* GetEndPoint();

	void SetBeginPoint(Point* pBp);
	void SetBeginPoint(double dx, double dy);

	void SetEndPoint(double dx, double dy);
	// type: 1->left_top
	void GetAPath_By_APoint(int type); // get a path by a point;

	std::vector<Point *>* Get_a_Path();

	//�������һ��·��
	void Create_A_Path_At_Random(RdmCtr *pObjRC);

// 	// Get begin point of the path.
// 	Point* GetBeginPointOfPath(std::vector<Point *> *p);

	void BubbleSort(std::vector<Point *> &pfp);

	//�������һ����
	Point* Get_A_Point_At_Random(RdmCtr *pObjRC);

	Point* GetNextPointRandom_viaAPoint(/*Point *pPrePoint*/);

	//Get begin point of path from four angle of topology.
	void SetPathBeginPoint_FromFourAngle(int type);

	bool isBeyondTheTopo(double dX, double dY); // Judge a path whether beyond the range of topo;
	bool isInRadiiOfPoint(double dX, double dY/*, Point *pre*/); //Judge a point whether in the radii of a point .

	void OutPutPathPoint();

	// ͨ��һ����õ���һ����
	Point * GetNextPoint_viaAPoint(Point *p);

	// ����������֮��ĳ���.
	double CalculateTwoPointLength(Point *p1, Point *p2);

	// ����������֮��ļн�.
	double CalculateTwoPointAngle(Point *p1, Point *p2);

	// ����һ����õ��õ�����·�ε���ʼ��
	Point * GetBeginPointOfAPoint(double dLoc_x, double dLoc_y);



public:
	 PATHPOINT	m_vecPath;
	

private:
	Point* m_dBeginPoint;
	Point* m_dEndPoint;

	

private:
	RdmCtr *rtr;
};
#endif//_PATH_H_
