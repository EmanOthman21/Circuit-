#pragma once
#include <iostream>
#include "CircuitElement.h"
#include <fstream>
#include "Node.h"
class Input
{
	string FileName;
	ifstream FileInput;
public:
	Input(const string, Node**, CircuitElement**,int&, CircuitElement**, CircuitElement**, complex<float>**);
	void Read_Dependant(const string,complex<float>&, Node**&, CircuitElement**&,int &);
	void Read_Resistor_Capacitor_Inductor(const string, Node**&, CircuitElement**&,int &);
	void Read_Voltage_Source_Current_Source(int&,int&,const string, Node**&, CircuitElement**&,int &, CircuitElement**&, CircuitElement**&, complex<float>**&);
	~Input(void);
};

