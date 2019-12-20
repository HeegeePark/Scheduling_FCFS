#include <string>
#include <queue>
#include <vector>
using namespace std;



/* ���μ��� ������ �����ϴ� Ŭ����*/
class ProcessClass {
private:
	int id;		// ���μ��� id
	double BurstTime;		// �۾� �ð�
	double ArrivalTime;		// ���� �ð�
public:
	// Ŭ���� ���� �Լ�
	ProcessClass(int _id, double _BurstTime, double _ArrivalTime);		// ������
	int get_id();		// ���μ��� ���̵� get�ϴ� �Լ� (return : id)
	double get_BurstTime();		// ���μ��� �۾��ð� get�ϴ� �Լ� (return : BurstTime)
	double get_ArrivalTime();		// ���μ��� �����ð� get�ϴ� �Լ� (return : ArrivalTime)
};



/* FCFS �����ٸ� ������ �����ϴ� Ŭ����*/
class FCFS_Scheduling {
private:
	double **ptrExecutionProcess;		// �����Ų ���μ����� ����Ű�� ���������� (�����Ͱ� ����Ű�� �迭�� �����ٸ� ���� ����)
	int executionIndex;		// �����ٸ� �ε���
	queue<int> readyQueue;		// �غ� ť
	// ������ ���μ������� �غ� ť�� �����ϱ� �� �����ð� ������ �����Ͽ� ��� ����
	vector<pair<double, int>> produceVec;		// <�����ð�, ���μ��� ���̵�>
	int startIndex;		// �����ٸ� ������ �� �� ó�� ���μ��� ���̵�
public:
	// Ŭ���� ���� �Լ�
	FCFS_Scheduling(int _proccessMAXNumber);		// ������
	// ���μ��� ������� �����ٸ� ������ Ŭ���� ��ü�� �����ϴ� �Լ�
	// �Ķ���� : (���μ��� ���̵�, ���۽ð�, ����ð�, �ش� ���μ����� ���ð�)
	void set_Scheduling(int _process_id, double _startTime, double _endTime, double _waitTime);		

	// �����ٸ� ���� ��, �غ� ť ����Ű��(set) �Լ�
	// �Ķ���� : (���μ������� ��ü, ���μ��� ����)
	void set_ReadyQueue(ProcessClass **_pArray, int* _pArray_Index);		

	// Ư�� �����ٸ� �ε���(_index)�� �����ٸ� ���� get�ϴ� �Լ� (return : ptrExecutionProcess[_index])
	/* ��� ����)
		get_ExecutionProcess(i)[0]; : i��° �����ٸ� �ε����� ���μ��� ���̵� get
		get_ExecutionProcess(i)[1]; : i��° �����ٸ� �ε����� �����ٸ� ���۽ð� get
		get_ExecutionProcess(i)[2]; : i��° �����ٸ� �ε����� �����ٸ� ����ð� get
		get_ExecutionProcess(i)[3]; : i��° �����ٸ� �ε����� �ش� ���μ��� ���ð� get
	*/
	double * get_ExecutionProcess(int _index);

	int get_executionIndex();		// �����ٸ� Ƚ���� get�ϴ� �Լ� (return : executionIndex)
	int get_startIndex();		// �����ٸ� ������ �� �� ó�� �����Ű�� ���μ��� ���̵� get�ϴ� �Լ� (return : startIndex)
	int get_readyQueueFront();		// �غ� ť�� �� �տ��� ��ٸ��� ���μ���(front) get�ϰ� pop���ִ� �Լ� (return : readyQueue.front())
	bool is_produceVecEmpty();		// ���� ���Ͱ� ����ִ��� boolean���� �������ִ� �Լ� (return : produceMap.empty())
	void update_ReadyQueue(double _time);		// ���μ������� �����ð��� ���� �����ߴٸ�, �غ� ť�� push���ִ� �Լ� (�Ķ���� : ����ð�)
};



/* �� �� �Լ�*/
// txt������ �ҷ��� ���μ��� ���� �о���� �Լ�
// �Ķ���� : (READ�� txt���� �̸�, ���μ������� ��ü, �� ���μ��� ����, ���� ������ ���μ��� �ִ� ����)
void ReadProcessInfo(const char *_FileName, ProcessClass **_processArray, int *_proceeArray_Index, const int MAX_PROCESS_NUMBER);

// �����ٸ� �Լ�
// �Ķ���� : (���μ������� ��ü, ���μ��� ����, �����ٸ����� ��ü)
void Scheduling(ProcessClass **_processArray, int*_processArray_Index, FCFS_Scheduling *_FCFSch);

// �����ָ� �������� cmd â�� ���ο� txt���Ϸ� ������ִ� �Լ�
// �Ķ���� : (�����ٸ����� ��ü, �� ���μ��� ����, READ�� txt���� �̸�, WRITE�� txt���� �̸�)
void WirteExecutionResult(FCFS_Scheduling *_FCFSch, int* _processArray_Index, const char *_READ_FILE_NAME, const char *_WRITE_FILE_NAME);

// �迭 ��� �Լ� (���� �� Ȯ�ο�)
void showCharArray(char *, int);
// �迭 �ʱ�ȭ �Լ�
void resetCharArray(char *, int);



