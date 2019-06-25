#pragma once
#include <string>
#include <set>
#include <complex>
using namespace std;
#define MaxElements 1000
class Node
{
	int name;
	int frequency;
	 complex <float> Voltage;
	std::set <int> connection;
public:
	Node(void);
	~Node(void);
	bool IsJoint(Node*);
	void SetName(int);
	void SetVoltage(complex <float>);
	complex <float> GetVoltage();
	int GetName();
	static int NodeCount;
	static int JointNum;
	std::set<int>& GetConnection();
	void Connect (int);
	void IncrementFrequency();
	Node* ArrayOfJoints(Node **);
};


