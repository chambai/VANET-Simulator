#include <iostream>
#include <fstream>
#include <Windows.h>

using namespace std;

#include "SimulateTime.h"
#include "NSRunner.h"
#include "BaseDef.h"


unsigned int TOTAL_RUN_TIMES = 5;
unsigned int NS_LOGICAL_LAST_TIME = 600;
const int STEPCNT = 200;
const int NBUFFER = 2;


void SetNSRunTime(unsigned int nTimes)
{
	if (nTimes == 1) {
		NS_LOGICAL_LAST_TIME = 200;
	}
	else if (nTimes == 2) {
		NS_LOGICAL_LAST_TIME = 400;
	}
	else if (nTimes == 3) {
		NS_LOGICAL_LAST_TIME = 600;
	}
	else if (nTimes == 4) {
		NS_LOGICAL_LAST_TIME = 800;
	}
	else if (nTimes == 5) {
		NS_LOGICAL_LAST_TIME = 2000;
	}
}

int main(void) 
{
	char *filename1 = "Data\\NS_BUSPATH.txt";
	char *filename2 = "Data\\NS_ALL_RESOURCES.txt";
	char *filename3 = "Data\\NS_CAR_RESOURCES.txt";

	ifstream in, in2, in3;
	ofstream out;

	NSRunner* pNSRunner = new NSRunner();

	//LogFile::instance()->m_ofRunLog << "--------------------------------------Topology--------------------------------------\n\n";
	// ����topology
	pNSRunner->SetTheTopology(TOPO_WIDTH, TOPO_HEIGHT);

	//LogFile::instance()->m_ofRunLog << "--------------------------------------Bus Path--------------------------------------\n\n";
	// ����bus path
	//pNSRunner->Create_Bus_Path_at_Random(BUS_PATH);

	//pNSRunner->WriteFromFile_path(out, filename1);

	// ����֪�ļ��ж�
	pNSRunner->ReadFromFile_path(in, filename1);

	//LogFile::instance()->m_ofRunLog << "--------------------------------------All Resource--------------------------------------\n\n";
	// �������е���Դ
// 	pNSRunner->Create_All_Resources(RES_NUM);
// 	pNSRunner->WriteFromFile_resources(out, filename2);

	pNSRunner->ReadFromFile_resources(in2, filename2);

	//LogFile::instance()->m_ofRunLog << "--------------------------------------CarResources--------------------------------------\n\n";
	
// 	pNSRunner->Create_All_Car_Request_Resources(CAR_NUM);
// 	pNSRunner->WriteFromFile_carRes(out, filename3);
	
	pNSRunner->ReadFromFile_carRes(in3, filename3);
	// ����ģ�����
	unsigned int nCurRunTimes = 1;
	//int nCase = 0;

	// ���û�����䷽���Ĳ���
	
	int nStepsCnt = 0;
	bool fInput = true;

	while( nCurRunTimes <= TOTAL_RUN_TIMES ) {

		LogFile::instance()->m_ofRunLog << " -------------------- Run times: ----- [ " << nCurRunTimes << " ] running.\n";
		//LogFile::instance()->m_ofSIRLog << "\n  ----------------- MN_TR: " << MN_TR << " -------------------- [ " << nCurrentRunTimes << " ] runnin  ----------------- MN_TR: " << MN_TR << " -------------------- [ " << nCurrentRunTimes << " ] rung.\n";
		//LogFile::instance()->m_ofForwardLog << "\nning.\n";

		//std::cout << "\n  ----------------- MN_TR: " << MN_TR << " ------------------- [ " << nCurrentRunTimes << " ] running.\n";
		std::cout << "\nTimes:" << nCurRunTimes << "\n";
		SetNSRunTime(nCurRunTimes);
		// set NS's running last interval.
		pNSRunner->SetNSLastRunTime(NS_LOGICAL_LAST_TIME);

		pNSRunner->Record_Other_Information(nCurRunTimes, NS_LOGICAL_LAST_TIME);
		
		int nBuffer = 1;
		
		while (nBuffer < NBUFFER) {
		//	LogFile::instance()->m_ofRunLog << "--------------------------------------���Ե� " << nBuffer << "�ֻ�����䷽��-----------------------------------\n\n";
			// Reset time and start this current time's running.
			SimulateTime::Reset_NS_time();
			

		//	LogFile::instance()->m_ofRunLog << "--------------------------------------BusNode--------------------------------------\n\n";
			// ����������bus path�ϴ���bus�ڵ�.
			pNSRunner->Create_Bus_At_All_Path_BeginPoint();

			
			// set all buses' velocity.
			pNSRunner->Set_All_Buses_Velocity(BUS_VEL);


		//	LogFile::instance()->m_ofRunLog << "--------------------------------------CarNode--------------------------------------\n\n";
			// �������е�car�ڵ�
			pNSRunner->Create_Cars_At_Random_Location(CAR_NUM, TOPO_MIN_X, TOPO_MAX_X, TOPO_MIN_Y, TOPO_MAX_Y);

			// set all cars' velocity.
			pNSRunner->Set_All_Cars_Velocity(CAR_VEL);


		//	LogFile::instance()->m_ofRunLog << "--------------------------------------BusBufferAllocation--------------------------------------\n\n";
			// ���ݻ�����䷽��Ϊbus���仺��
			// ��������0-1�����ķ�ʽ
			pNSRunner->Set_Bus_Buffer_rely_on_Buffer_Allocation(nBuffer);

			pNSRunner->Record_pdf_Results(nBuffer);

			// Ϊcar��ʼ������Ҫ�������Դ
			pNSRunner->Create_Cars_Resource_at_Random();

			LogFile::instance()->m_ofContact << "------------------------------------------------------------- " << nBuffer + 1 << " ---------------------------------------------------------------------------\n\n";
			while (true) {
				int nNS_State = pNSRunner->NS_Runs_for_a_tick();
				if (NS_RUNS_OVER == nNS_State ) {
					break;
				}

				// �ж�bus�ڵ��Ƿ�ﵽ�յ�.
				if (pNSRunner->Judge_Whether_All_buses_Arrival_Path_End()) {
					break;
				}

				//LogFile::instance()->m_ofRunLog << "--------------------------------------BusMoveAStep--------------------------------------\n\n";
				// let all node forward a step 
				pNSRunner->Let_All_Buses_Forward_a_Step();

				//LogFile::instance()->m_ofRunLog << "--------------------------------------CarMoveAStep--------------------------------------\n\n";
				pNSRunner->Let_All_Cars_Forward_a_Step(RWALK);

				nStepsCnt ++;


				if ( 0 == nStepsCnt % STEPCNT) {
					if (fInput) {
						LogFile::instance()->m_ofContact << "nCase:\t"
							<< "BusID:\t" 
							<< "CarID:\t"
							<< "ResType:\t"
							<< "ResSize:\t"
							<< "ResPop:\t"
							<< "ResTime:\t"
							<< "ContactNum:\t"
							<< "isOutTiem:\t"
							<< "isSuccess:\t"
							<< "RequestTime:\t"
							<< "ReceiveTime:\t"
							<< "ReceiveFullMessTime:\t\n\n";
					}
					fInput = false;
// 					for (int iB = 0; iB < nBufferAllocation; ++ iB) {
// 						pNSRunner->Let_All_Node_Check_Contact_rely_on_Buffer_Allocation(iB);
// 					}
					
				//	LogFile::instance()->m_ofRunLog << "--------------------------------------Check Transmission--------------------------------------\n\n";
					// let all node check transmission chance.
					pNSRunner->Let_All_Node_Check_Transmission_Chance(nBuffer);
					nStepsCnt = 0;
				}
			}

			// ��¼ȡ��Դ�ɹ��ĸ���
			
			// ��¼��ʱ�ĸ���
			pNSRunner->Record_TestCase_Results();

			nBuffer ++;
			

			// ��bus������Ϊ��ʶ״̬.
			
			pNSRunner->Reset_Bus_To_Begin();
			pNSRunner->Reset_Cars_To_Begin();
		}
		
		//int nFlag = true;
		nCurRunTimes++;
	}
	SAFELY_DELETE_PTR(pNSRunner);
	return 0;
}