#pragma once
#include <iostream>
#include <fstream>
#include "Node.h"
#include <complex>
#include <cmath>
using namespace std;
class CircuitElement
{
	Node *Node1, *Node2;
	complex <float>  Value;
	std::string ElementName;
	complex <float> Current;
	complex <float> Voltage;
public:
	static float W;
	static int VoltageCounter;
	static int ResistanceCounter;
	static int  inductorCounter;
	static int CapacitorCounter;
	static int CurrentCounter;
	static int id;
	static int TempCounter;
	void SetCounter();
	void SetNode1(Node*);
	void SetNode2(Node*);
	void SetValue(float);
	void SetValueOfVoltageandCurrent(float ,float);
	void SetElementName(std::string);
	void Setid(int);
	void SetCurrent(complex <float>);
	bool IsVoltageSource();
	bool IsCurrentSource();
	bool IsResistance();
	bool IsCapacitor();
	bool IsDepSource();
	bool IsInductor();
	void SetVoltage(complex<float>);
	Node* GetNode1() ;
	Node* GetNode2() ;
	int Getid() const;
	complex<float> GetValue() const;

	complex<float> GetCurrent() const;
	complex<float> GetVoltage() const;
	std::string GetElementName() const;
	CircuitElement** ElementBetweenNodes(int , int, int& NumOfElemBetweenNodes, CircuitElement*);
	CircuitElement(void);
	~CircuitElement(void);
};
