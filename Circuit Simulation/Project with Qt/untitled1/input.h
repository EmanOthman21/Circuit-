#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "CircuitElement.h"
#include <fstream>
#include "Node.h"
using namespace std;
class Input
{
    std::string FileName;
    std::ifstream FileInput;
public:
    std::string OutMethod;
    Input(const std::string, vector<Node*>&, vector<CircuitElement*>&, int&, vector<CircuitElement*>&, vector<CircuitElement*>&, vector<complex<float>>&);
    void Read_Dependant(const string, complex<float>&, vector<Node*>&, vector<CircuitElement*>&, int&);
    void Read_Voltage_Source_Current_Source(const string, vector<Node*>&, vector<CircuitElement*>&, int&, vector<CircuitElement*>&, vector<CircuitElement*>&, vector<complex<float>>&);
    void Read_Resistor_Capacitor_Inductor(const string, vector<Node*>&, vector<CircuitElement*>&,int &);
    ~Input(void);
};

