#include "header.h"
#include "iostream"
using namespace std;

// CONST
const char READ_FILE_NAME[] = "FCFS_test.txt";
const char WRITE_FILE_NAME[] = "FCFS_schedulingResult.txt";
const int MAX_PROCESS_NUMBER = 100;

// Variable
ProcessClass *processArray[MAX_PROCESS_NUMBER];		//ProcessClass point Process[]
int processArray_Index = 0;
FCFS_Scheduling *FCFSch = new FCFS_Scheduling(MAX_PROCESS_NUMBER);

void main() {

	cout << "Main Start" << endl;

	// txt 파일에 있는 프로세드 아이디, 프로세스별 작업시간, 퀀텀 정보를 읽어 ProcessClass에 저장
	cout << "\nStep1 : Read File" << endl;
	cout << "Only Accept File Name = " << READ_FILE_NAME << endl;
	ReadProcessInfo(READ_FILE_NAME, processArray, &processArray_Index, MAX_PROCESS_NUMBER);

	// FCFS 스케줄링
	cout << "\nStep2 : FCFS Scheduling" << endl;
	Scheduling(processArray, &processArray_Index, FCFSch);

	// 스케줄링 실행 결과를 cmd 및 txt 파일에 출력
	cout << "\nStep3 : Write File" << endl;
	WriteExecutionResult(FCFSch, &processArray_Index, READ_FILE_NAME, WRITE_FILE_NAME);
	cout << "\nResult File Name = " << WRITE_FILE_NAME << endl;

	system("pause");
}//end main

