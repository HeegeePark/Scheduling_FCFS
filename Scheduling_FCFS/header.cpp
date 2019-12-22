#include "header.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <algorithm>
using namespace std;



/* ------ 프로세스 정보를 저장하는 클래스 함수------*/
// 프로세스클래스 생성자
ProcessClass::ProcessClass(int _id, double _BurstTime, double _ArrivalTime) {
	id = _id;		// 프로세스 아이디
	BurstTime = _BurstTime;		 // 작업시간
	ArrivalTime = _ArrivalTime;		// 도착시간
}//end ProcessClass Constructor

int ProcessClass::get_id() { return id; }		// 프로세스 아이디 get하는 함수 (return : id)
double ProcessClass::get_BurstTime() { return BurstTime; }		// 프로세스 작업시간 get하는 함수 (return : BurstTime)
double ProcessClass::get_ArrivalTime() { return ArrivalTime; }		// 프로세스 도착시간 get하는 함수 (return : ArrivalTime)



/* ------ 스케줄링 정보를 저장하는 클래스 함수------*/
// 스케줄링 정보 클래스 생성자 (파라미터 : 생성 가능한 프로세스 최대 개수)
FCFS_Scheduling::FCFS_Scheduling(int _processMAXNumber) {

	// 생성 가능한 프로세스 최대 개수만큼 더블포인터 선언
	ptrExecutionProcess = new double*[_processMAXNumber];

	// 스케줄링 정보(프로세스 아이디, 시작시간, 종료시간, 해당 프로세스의 대기시간)를 담을 [_processMAXNumber][4] 크기의 배열 할당
	for (int i = 0; i < _processMAXNumber; i++) {
		ptrExecutionProcess[i] = new double[4];
	}//end for

	// 실행한 프로세스 개수 선언
	executionIndex = 0;
}//end RRScheduling Constructor

// 프로세스 실행시켜 스케줄링 정보를 클래스 객체에 저장하는 함수
// 파라미터 : (프로세스 아이디, 시작시간, 종료시간, 해당 프로세스의 대기시간)
void FCFS_Scheduling::set_Scheduling(int _process_id, double _startTime, double _endTime, double _waitTime) {
	// executionIndex : 현재 스케줄링 인덱스
	ptrExecutionProcess[executionIndex][0] = _process_id;		// 프로세스 아이디
	ptrExecutionProcess[executionIndex][1] = _startTime;		// 시작시간
	ptrExecutionProcess[executionIndex][2] = _endTime;		// 종료시간
	ptrExecutionProcess[executionIndex][3] = _waitTime;		// 대기시간
	
	// 다음 인덱스로
	executionIndex++;
}

// 스케줄링 시작 전, 준비 큐 대기시키는(set) 함수
// 파라미터 : (프로세스정보 객체, 프로세스 개수)
void FCFS_Scheduling::set_ReadyQueue(ProcessClass **_pArray, int* _pArray_Index) {
	// 생성 단계의 프로세스들의 도착시간 및 프로세스 아이디를  pair vector<도착시간, 프로세스 아이디>에 저장
	for (int i = 0; i < *_pArray_Index; i++) {
		produceVec.push_back(pair<double, int>(_pArray[i]->get_ArrivalTime(), _pArray[i]->get_id()));
	}
	// 도착시간 기준으로 오름차순으로 vector 정렬
	sort(produceVec.begin(), produceVec.end());

	// 준비 큐에 맨 처음의 생성 단계의 프로세스 아이디 push 후 vector에서 지우기
	auto start = produceVec.begin();
	readyQueue.push(start->second);		// start->second : vecotor 첫번째 요소의 프로세스 아이디
	startIndex = start->second;
	produceVec.erase(start);

}

// 특정 스케줄링 인덱스(_index)의 스케줄링 정보 get하는 함수 (return : ptrExecutionProcess[_index])
double * FCFS_Scheduling::get_ExecutionProcess(int _index) { return ptrExecutionProcess[_index]; };
// 스케줄링 횟수를 get하는 함수 (return : executionIndex)
int FCFS_Scheduling::get_executionIndex() { return executionIndex; }		
// 스케줄링 시작할 때 맨 처음 실행시키는 프로세스 아이디 get하는 함수 (return : startIndex)
int FCFS_Scheduling::get_startIndex() { return startIndex; }
// 준비 큐의 맨 앞에서 기다리는 프로세스(front) get하고 pop해주는 함수 (return : readyQueue.front())
int FCFS_Scheduling::get_readyQueueFront() {
	if (!readyQueue.empty()) {
		int exec = readyQueue.front();
		readyQueue.pop();
		return exec;
	}
};

// 생성 벡터가 비어있는지 boolean으로 리턴해주는 함수 (return : produceMap.empty())
bool FCFS_Scheduling::is_produceVecEmpty() { return produceVec.empty(); }
// 프로세스들의 도착시간에 따라 도착했다면, 준비 큐에 push해주는 함수 (파라미터 : 현재시간)
void FCFS_Scheduling::update_ReadyQueue(double _time) {
	// vector 안 생성 단계의 프로세스들 중, 도착시간이 현재 시간보다 빠를 시(프로세스가 도착했다면), 준비 큐로 이동
	// 이동하는것이므로, 생성 vector에서는 원소 삭제, value인 프로세스 아이디를 준비 큐에 push
	for (auto iter = produceVec.begin(); iter != produceVec.end();) {
		if (iter->first <= _time) {
			readyQueue.push(iter->second);
			iter = produceVec.erase(iter);
		}
		else {		// 런타임에러 방지
			iter++;
		}
	}
}



/* ------ 그 외 함수들 ------*/
// txt파일을 불러와 프로세스 정보 읽어오는 함수
// 파라미터 : (READ용 txt파일 이름, 프로세스정보 객체, 총 프로세스 개수, 생성 가능한 프로세스 최대 개수)
void ReadProcessInfo(const char *_FileName, ProcessClass **_processArray, int *_proceeArray_Index, const int MAX_PROCESS_NUMBER) {
	//Open File
	try {
		fstream fin;
		fin.open(_FileName, ios::in);
		if (fin.fail()) {		// 파일 오픈 실패 시 시스템 종료
			fin.close();
			std::cout << "\n#01-Read file opening failed.\n";
			system("pause");
			exit(1);
		}//end if

		cout << "\n#01-Read File Success" << endl;

		// 프로세스 정보별 버퍼
		const int BUFFER_SIZE = 20;
		char buffer[BUFFER_SIZE] = { NULL };
		char process_id[BUFFER_SIZE] = { NULL };
		char burst_time[BUFFER_SIZE] = { NULL };
		char arrival_time[BUFFER_SIZE] = { NULL };
		char quantum[BUFFER_SIZE] = { NULL };

		while (*_proceeArray_Index < MAX_PROCESS_NUMBER) {		// 총 프로세스의 개수가 최대 프로세스 개수보다 적을 때 실행 가능
			fin.getline(buffer, BUFFER_SIZE, ' ');		// 한 토큰씩 버퍼에 txt 읽어오기

			// 버퍼에 담긴 프로세스 아이디, 작업 시간, 도착시간 쪼개서 담기
			for (int i = 0; buffer[i] != NULL; i++) {
				process_id[i] = buffer[i];
			}//end for
			fin.getline(buffer, BUFFER_SIZE, ' ');
			for (int i = 0; buffer[i] != NULL; i++) {
				burst_time[i] = buffer[i];
			}//end for
			fin.getline(arrival_time, BUFFER_SIZE, '\n');

			// 각각의 버퍼에 담은 프로세스 정보들을 이용하여 새로운 프로세스클래스 객체 생성
			_processArray[*_proceeArray_Index] = new ProcessClass(atoi(process_id), atof(burst_time), atof(arrival_time));

			// 프로세스 정보가 잘 담겼는지 cmd에 출력하여 확인
			cout << "Process Id = " << _processArray[*_proceeArray_Index]->get_id();
			cout << ", Burst Time = " << _processArray[*_proceeArray_Index]->get_BurstTime();
			cout << ", Arrival Time = " << _processArray[*_proceeArray_Index]->get_ArrivalTime();
			cout << endl;

			// 배열 초기화
			resetCharArray(process_id, BUFFER_SIZE);
			resetCharArray(burst_time, BUFFER_SIZE);
			resetCharArray(quantum, BUFFER_SIZE);

			// 프로세스 개수 ++
			*_proceeArray_Index += 1;
			
			// txt 파일의 끝단이라면 다 프로세스 정보를 다 읽었으니 읽기 종료
			if (fin.eof()) break;
		}//end while

		// 파일 종료
		fin.close();
	}
	catch (exception e) {		// 예외 처리
		cout << "\n#01-ReadProcessInfo : " << e.what() << endl;
	}//end try

}//end ReadProcessInfo


// FCFS 스케줄링 함수
// 파라미터 : (프로세스정보 객체, 프로세스 개수, 스케줄링정보 객체)
void Scheduling(ProcessClass **_processArray, int*_processArray_Index, FCFS_Scheduling *_FCFSch) {
	int NotFinshProcessNumber = *_processArray_Index;	//처리되지 않은 process 수
	int executingIndex = 0;		// 실행할 프로세스 아이디

	// 프로세스 도착시간에 맞춰서 준비 큐에 프로세스 인덱스 삽입
	_FCFSch->set_ReadyQueue(_processArray, _processArray_Index);
	// 제일 처음으로 도착한 프로세스의 시간으로 현재 시간 갱신
	double currentTime = _processArray[_FCFSch->get_startIndex()]->get_ArrivalTime();

	while (NotFinshProcessNumber > 0) {		// 모든 프로세스가 처리될 때까지 실행
		// 준비 큐의 맨 앞에서 기다리는 프로세스(front) 실행시키기
		executingIndex = _FCFSch->get_readyQueueFront();
		_FCFSch->set_Scheduling(
			_processArray[executingIndex]->get_id(),		// 프로세스 아이디
			currentTime,		// 시작시간
			currentTime + _processArray[executingIndex]->get_BurstTime(),		// 종료시간
			currentTime - _processArray[executingIndex]->get_ArrivalTime()		// 대기시간 (현재시간 - 프로세스의 도착시간)
		);
		currentTime += _processArray[executingIndex]->get_BurstTime();		// 현재 시간 갱신
		if (!_FCFSch->is_produceVecEmpty()) { _FCFSch->update_ReadyQueue(currentTime); }		// 도착한 프로세스 준비 큐로 이동
		// 프로세스 하나를 완료하였으니 미완료된 process 수--
		NotFinshProcessNumber -= 1;
	}//end while
}//end Scheduling



// 스케줄링 실행결과를 cmd 창과 새로운 txt파일로 출력해주는 함수
// 파라미터 : (스케줄링정보 객체, 총 프로세스 개수, READ용 txt파일 이름, WRITE용 txt파일 이름)
void WriteExecutionResult(FCFS_Scheduling *_FCFSch, int* _processArray_Index, const char *_READ_FILE_NAME, const char *_WRITE_FILE_NAME) {

	// 일기용, 쓰기용 파일 오픈 
	fstream fin(_READ_FILE_NAME, ios::in);
	fstream fout(_WRITE_FILE_NAME, ios::out);

	const int BUFFER_SIZE = 20;
	char buffer[BUFFER_SIZE];

	// 출력하는 txt 파일에 입력한 txt 파일 내용 그대로 출력
	while (true) {
		fin.getline(buffer, BUFFER_SIZE, '\n');
		fout << buffer << endl;
		resetCharArray(buffer, BUFFER_SIZE);
		if (fin.eof()) { break; }
	}//end while
	fout << endl;

	double meanOfWaitingTime = 0.0;
	// FCFS 스케줄링 타임라인 출력
	// 스케줄링 인덱스 순으로 프로세스 아이디, 시작시간, 종료시간, 대기시간 출력
	for (int i = 0; i < _FCFSch->get_executionIndex(); i++) {
		cout << "pId = " << _FCFSch->get_ExecutionProcess(i)[0];
		cout << ", start = " << _FCFSch->get_ExecutionProcess(i)[1];
		cout << ", end = " << _FCFSch->get_ExecutionProcess(i)[2];
		cout << ", wait = " << _FCFSch->get_ExecutionProcess(i)[3];
		cout << endl;

		fout << _FCFSch->get_ExecutionProcess(i)[0];
		fout << " " << _FCFSch->get_ExecutionProcess(i)[1];
		fout << " ~ " << _FCFSch->get_ExecutionProcess(i)[2];
		fout << " wait : " << _FCFSch->get_ExecutionProcess(i)[3];
		fout << endl;

		// 대기시간 더하기
		meanOfWaitingTime += _FCFSch->get_ExecutionProcess(i)[3];

		// 평균대기시간 구하고 출력
		if (i == _FCFSch->get_executionIndex() - 1) {
			meanOfWaitingTime /= (double)*_processArray_Index;
			cout << "mean of waitingtime = " << meanOfWaitingTime << endl;
			fout << "mean of waitingtime = " << meanOfWaitingTime << endl;
		}//end if

	}//end for

	fin.close();
	fout.close();

}//end WriteExecutionResult


// Array를 출력하는 함수 (개발 때 값 확인용 함수)
void showCharArray(char *_showchar, int _charsize) {
	for (int i = 0; i < _charsize; i++) {
		if (_showchar[i] == NULL) { break; }
		cout << _showchar[i];
	}//end for

}//end showCharArray



// Array를 NULL로 초기화하는 함수
void resetCharArray(char *_resetchar, int _charsize) {
	for (int i = 0; i < _charsize; i++) {
		_resetchar[i] = NULL;
	}//end for
}//end resetCharArray

