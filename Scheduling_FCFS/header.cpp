#include "header.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <algorithm>
using namespace std;



/* ------ ���μ��� ������ �����ϴ� Ŭ���� �Լ�------*/
// ���μ���Ŭ���� ������
ProcessClass::ProcessClass(int _id, double _BurstTime, double _ArrivalTime) {
	id = _id;		// ���μ��� ���̵�
	BurstTime = _BurstTime;		 // �۾��ð�
	ArrivalTime = _ArrivalTime;		// �����ð�
}//end ProcessClass Constructor

int ProcessClass::get_id() { return id; }		// ���μ��� ���̵� get�ϴ� �Լ� (return : id)
double ProcessClass::get_BurstTime() { return BurstTime; }		// ���μ��� �۾��ð� get�ϴ� �Լ� (return : BurstTime)
double ProcessClass::get_ArrivalTime() { return ArrivalTime; }		// ���μ��� �����ð� get�ϴ� �Լ� (return : ArrivalTime)



/* ------ �����ٸ� ������ �����ϴ� Ŭ���� �Լ�------*/
// �����ٸ� ���� Ŭ���� ������ (�Ķ���� : ���� ������ ���μ��� �ִ� ����)
FCFS_Scheduling::FCFS_Scheduling(int _processMAXNumber) {

	// ���� ������ ���μ��� �ִ� ������ŭ ���������� ����
	ptrExecutionProcess = new double*[_processMAXNumber];

	// �����ٸ� ����(���μ��� ���̵�, ���۽ð�, ����ð�, �ش� ���μ����� ���ð�)�� ���� [_processMAXNumber][4] ũ���� �迭 �Ҵ�
	for (int i = 0; i < _processMAXNumber; i++) {
		ptrExecutionProcess[i] = new double[4];
	}//end for

	// ������ ���μ��� ���� ����
	executionIndex = 0;
}//end RRScheduling Constructor

// ���μ��� ������� �����ٸ� ������ Ŭ���� ��ü�� �����ϴ� �Լ�
// �Ķ���� : (���μ��� ���̵�, ���۽ð�, ����ð�, �ش� ���μ����� ���ð�)
void FCFS_Scheduling::set_Scheduling(int _process_id, double _startTime, double _endTime, double _waitTime) {
	// executionIndex : ���� �����ٸ� �ε���
	ptrExecutionProcess[executionIndex][0] = _process_id;		// ���μ��� ���̵�
	ptrExecutionProcess[executionIndex][1] = _startTime;		// ���۽ð�
	ptrExecutionProcess[executionIndex][2] = _endTime;		// ����ð�
	ptrExecutionProcess[executionIndex][3] = _waitTime;		// ���ð�
	
	// ���� �ε�����
	executionIndex++;
}

// �����ٸ� ���� ��, �غ� ť ����Ű��(set) �Լ�
// �Ķ���� : (���μ������� ��ü, ���μ��� ����)
void FCFS_Scheduling::set_ReadyQueue(ProcessClass **_pArray, int* _pArray_Index) {
	// ���� �ܰ��� ���μ������� �����ð� �� ���μ��� ���̵�  pair vector<�����ð�, ���μ��� ���̵�>�� ����
	for (int i = 0; i < *_pArray_Index; i++) {
		produceVec.push_back(pair<double, int>(_pArray[i]->get_ArrivalTime(), _pArray[i]->get_id()));
	}
	// �����ð� �������� ������������ vector ����
	sort(produceVec.begin(), produceVec.end());

	// �غ� ť�� �� ó���� ���� �ܰ��� ���μ��� ���̵� push �� vector���� �����
	auto start = produceVec.begin();
	readyQueue.push(start->second);		// start->second : vecotor ù��° ����� ���μ��� ���̵�
	startIndex = start->second;
	produceVec.erase(start);

}

// Ư�� �����ٸ� �ε���(_index)�� �����ٸ� ���� get�ϴ� �Լ� (return : ptrExecutionProcess[_index])
double * FCFS_Scheduling::get_ExecutionProcess(int _index) { return ptrExecutionProcess[_index]; };
// �����ٸ� Ƚ���� get�ϴ� �Լ� (return : executionIndex)
int FCFS_Scheduling::get_executionIndex() { return executionIndex; }		
// �����ٸ� ������ �� �� ó�� �����Ű�� ���μ��� ���̵� get�ϴ� �Լ� (return : startIndex)
int FCFS_Scheduling::get_startIndex() { return startIndex; }
// �غ� ť�� �� �տ��� ��ٸ��� ���μ���(front) get�ϰ� pop���ִ� �Լ� (return : readyQueue.front())
int FCFS_Scheduling::get_readyQueueFront() {
	if (!readyQueue.empty()) {
		int exec = readyQueue.front();
		readyQueue.pop();
		return exec;
	}
};

// ���� ���Ͱ� ����ִ��� boolean���� �������ִ� �Լ� (return : produceMap.empty())
bool FCFS_Scheduling::is_produceVecEmpty() { return produceVec.empty(); }
// ���μ������� �����ð��� ���� �����ߴٸ�, �غ� ť�� push���ִ� �Լ� (�Ķ���� : ����ð�)
void FCFS_Scheduling::update_ReadyQueue(double _time) {
	// vector �� ���� �ܰ��� ���μ����� ��, �����ð��� ���� �ð����� ���� ��(���μ����� �����ߴٸ�), �غ� ť�� �̵�
	// �̵��ϴ°��̹Ƿ�, ���� vector������ ���� ����, value�� ���μ��� ���̵� �غ� ť�� push
	for (auto iter = produceVec.begin(); iter != produceVec.end();) {
		if (iter->first <= _time) {
			readyQueue.push(iter->second);
			iter = produceVec.erase(iter);
		}
		else {		// ��Ÿ�ӿ��� ����
			iter++;
		}
	}
}



/* ------ �� �� �Լ��� ------*/
// txt������ �ҷ��� ���μ��� ���� �о���� �Լ�
// �Ķ���� : (READ�� txt���� �̸�, ���μ������� ��ü, �� ���μ��� ����, ���� ������ ���μ��� �ִ� ����)
void ReadProcessInfo(const char *_FileName, ProcessClass **_processArray, int *_proceeArray_Index, const int MAX_PROCESS_NUMBER) {
	//Open File
	try {
		fstream fin;
		fin.open(_FileName, ios::in);
		if (fin.fail()) {		// ���� ���� ���� �� �ý��� ����
			fin.close();
			std::cout << "\n#01-Read file opening failed.\n";
			system("pause");
			exit(1);
		}//end if

		cout << "\n#01-Read File Success" << endl;

		// ���μ��� ������ ����
		const int BUFFER_SIZE = 20;
		char buffer[BUFFER_SIZE] = { NULL };
		char process_id[BUFFER_SIZE] = { NULL };
		char burst_time[BUFFER_SIZE] = { NULL };
		char arrival_time[BUFFER_SIZE] = { NULL };
		char quantum[BUFFER_SIZE] = { NULL };

		while (*_proceeArray_Index < MAX_PROCESS_NUMBER) {		// �� ���μ����� ������ �ִ� ���μ��� �������� ���� �� ���� ����
			fin.getline(buffer, BUFFER_SIZE, ' ');		// �� ��ū�� ���ۿ� txt �о����

			// ���ۿ� ��� ���μ��� ���̵�, �۾� �ð�, �����ð� �ɰ��� ���
			for (int i = 0; buffer[i] != NULL; i++) {
				process_id[i] = buffer[i];
			}//end for
			fin.getline(buffer, BUFFER_SIZE, ' ');
			for (int i = 0; buffer[i] != NULL; i++) {
				burst_time[i] = buffer[i];
			}//end for
			fin.getline(arrival_time, BUFFER_SIZE, '\n');

			// ������ ���ۿ� ���� ���μ��� �������� �̿��Ͽ� ���ο� ���μ���Ŭ���� ��ü ����
			_processArray[*_proceeArray_Index] = new ProcessClass(atoi(process_id), atof(burst_time), atof(arrival_time));

			// ���μ��� ������ �� ������ cmd�� ����Ͽ� Ȯ��
			cout << "Process Id = " << _processArray[*_proceeArray_Index]->get_id();
			cout << ", Burst Time = " << _processArray[*_proceeArray_Index]->get_BurstTime();
			cout << ", Arrival Time = " << _processArray[*_proceeArray_Index]->get_ArrivalTime();
			cout << endl;

			// �迭 �ʱ�ȭ
			resetCharArray(process_id, BUFFER_SIZE);
			resetCharArray(burst_time, BUFFER_SIZE);
			resetCharArray(quantum, BUFFER_SIZE);

			// ���μ��� ���� ++
			*_proceeArray_Index += 1;
			
			// txt ������ �����̶�� �� ���μ��� ������ �� �о����� �б� ����
			if (fin.eof()) break;
		}//end while

		// ���� ����
		fin.close();
	}
	catch (exception e) {		// ���� ó��
		cout << "\n#01-ReadProcessInfo : " << e.what() << endl;
	}//end try

}//end ReadProcessInfo


// FCFS �����ٸ� �Լ�
// �Ķ���� : (���μ������� ��ü, ���μ��� ����, �����ٸ����� ��ü)
void Scheduling(ProcessClass **_processArray, int*_processArray_Index, FCFS_Scheduling *_FCFSch) {
	int NotFinshProcessNumber = *_processArray_Index;	//ó������ ���� process ��
	int executingIndex = 0;		// ������ ���μ��� ���̵�

	// ���μ��� �����ð��� ���缭 �غ� ť�� ���μ��� �ε��� ����
	_FCFSch->set_ReadyQueue(_processArray, _processArray_Index);
	// ���� ó������ ������ ���μ����� �ð����� ���� �ð� ����
	double currentTime = _processArray[_FCFSch->get_startIndex()]->get_ArrivalTime();

	while (NotFinshProcessNumber > 0) {		// ��� ���μ����� ó���� ������ ����
		// �غ� ť�� �� �տ��� ��ٸ��� ���μ���(front) �����Ű��
		executingIndex = _FCFSch->get_readyQueueFront();
		_FCFSch->set_Scheduling(
			_processArray[executingIndex]->get_id(),		// ���μ��� ���̵�
			currentTime,		// ���۽ð�
			currentTime + _processArray[executingIndex]->get_BurstTime(),		// ����ð�
			currentTime - _processArray[executingIndex]->get_ArrivalTime()		// ���ð� (����ð� - ���μ����� �����ð�)
		);
		currentTime += _processArray[executingIndex]->get_BurstTime();		// ���� �ð� ����
		if (!_FCFSch->is_produceVecEmpty()) { _FCFSch->update_ReadyQueue(currentTime); }		// ������ ���μ��� �غ� ť�� �̵�
		// ���μ��� �ϳ��� �Ϸ��Ͽ����� �̿Ϸ�� process ��--
		NotFinshProcessNumber -= 1;
	}//end while
}//end Scheduling



// �����ٸ� �������� cmd â�� ���ο� txt���Ϸ� ������ִ� �Լ�
// �Ķ���� : (�����ٸ����� ��ü, �� ���μ��� ����, READ�� txt���� �̸�, WRITE�� txt���� �̸�)
void WriteExecutionResult(FCFS_Scheduling *_FCFSch, int* _processArray_Index, const char *_READ_FILE_NAME, const char *_WRITE_FILE_NAME) {

	// �ϱ��, ����� ���� ���� 
	fstream fin(_READ_FILE_NAME, ios::in);
	fstream fout(_WRITE_FILE_NAME, ios::out);

	const int BUFFER_SIZE = 20;
	char buffer[BUFFER_SIZE];

	// ����ϴ� txt ���Ͽ� �Է��� txt ���� ���� �״�� ���
	while (true) {
		fin.getline(buffer, BUFFER_SIZE, '\n');
		fout << buffer << endl;
		resetCharArray(buffer, BUFFER_SIZE);
		if (fin.eof()) { break; }
	}//end while
	fout << endl;

	double meanOfWaitingTime = 0.0;
	// FCFS �����ٸ� Ÿ�Ӷ��� ���
	// �����ٸ� �ε��� ������ ���μ��� ���̵�, ���۽ð�, ����ð�, ���ð� ���
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

		// ���ð� ���ϱ�
		meanOfWaitingTime += _FCFSch->get_ExecutionProcess(i)[3];

		// ��մ��ð� ���ϰ� ���
		if (i == _FCFSch->get_executionIndex() - 1) {
			meanOfWaitingTime /= (double)*_processArray_Index;
			cout << "mean of waitingtime = " << meanOfWaitingTime << endl;
			fout << "mean of waitingtime = " << meanOfWaitingTime << endl;
		}//end if

	}//end for

	fin.close();
	fout.close();

}//end WriteExecutionResult


// Array�� ����ϴ� �Լ� (���� �� �� Ȯ�ο� �Լ�)
void showCharArray(char *_showchar, int _charsize) {
	for (int i = 0; i < _charsize; i++) {
		if (_showchar[i] == NULL) { break; }
		cout << _showchar[i];
	}//end for

}//end showCharArray



// Array�� NULL�� �ʱ�ȭ�ϴ� �Լ�
void resetCharArray(char *_resetchar, int _charsize) {
	for (int i = 0; i < _charsize; i++) {
		_resetchar[i] = NULL;
	}//end for
}//end resetCharArray

