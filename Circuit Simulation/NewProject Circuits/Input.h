#pragma once
#include <vector>
#include <iostream>
#include "CircuitElement.h"
#include <fstream>
#include "Node.h"
class Input
{
	string FileName;
	ifstream FileInput;
public:
	Input(const string, vector<Node*>&, vector<CircuitElement*>&, int&, vector<CircuitElement*>&, vector<CircuitElement*>&, vector<complex<float>*>&);
	void Read_Dependant(const string, complex<float>&, vector<Node*>&, vector<CircuitElement*>&, int&);
	void Read_Voltage_Source_Current_Source(int&, int&, const string, vector<Node*>&, vector<CircuitElement*>&, int&, vector<CircuitElement*>&, vector<CircuitElement*>&, vector<complex<float>*>&);
	void Read_Resistor_Capacitor_Inductor(const string, vector<Node*>&, vector<CircuitElement*>&,int &);
	~Input(void);
};

