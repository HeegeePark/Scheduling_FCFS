#include "header.h"
#include "iostream"
using namespace std;

// CONST
const char READ_FILE_NAME[] = "FCFS_test.txt";
const char WRITE_FILE_NAME[] = "FCFS_shcedulingResult.txt";
const int MAX_PROCESS_NUMBER = 100;

// Variable
ProcessClass *processArray[MAX_PROCESS_NUMBER];		//ProcessClass point Process[]
int processArray_Index = 0;
FCFS_Scheduling *FCFSch = new FCFS_Scheduling(MAX_PROCESS_NUMBER);

void main() {

	cout << "Main Start" << endl;

	// txt ���Ͽ� �ִ� ���μ��� ���̵�, ���μ����� �۾��ð�, ���� ������ �о� ProcessClass�� ����
	cout << "\nStep1 : Read File" << endl;
	cout << "Only Accept File Name = " << READ_FILE_NAME << endl;
	ReadProcessInfo(READ_FILE_NAME, processArray, &processArray_Index, MAX_PROCESS_NUMBER);

	// FCFS �����ٸ�
	cout << "\nStep2 : FCFS Scheduling" << endl;
	Scheduling(processArray, &processArray_Index, FCFSch);

	// �����ٸ� ���� ����� cmd �� txt ���Ͽ� ���
	cout << "\nStep3 : Write File" << endl;
	WirteExecutionResult(FCFSch, &processArray_Index, READ_FILE_NAME, WRITE_FILE_NAME);
	cout << "\nResult File Name = " << WRITE_FILE_NAME << endl;

	system("pause");
}//end main