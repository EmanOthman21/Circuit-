#include <complex>
#include <vector>
#include <cmath>
#include"MatrixFill.h"
#define MaxElements 1000
int CircuitElement::id = 0;
float CircuitElement::W = 0;
int CircuitElement::TempCounter = 0;
int CircuitElement::NumDep = 0;
//The total number of nodes	in the entered circuit 
int Node::NodeCount = 0;
int Node::JointNum = 0;
//The elements counters
int CircuitElement:: VoltageCounter=0;
int CircuitElement:: ResistanceCounter=0;
int CircuitElement::  inductorCounter=0;
int CircuitElement:: CapacitorCounter=0;
int CircuitElement:: CurrentCounter=0;
int main()
{
	// a parameter to take the input file and output file names
	string FileName;
	//array of the different elements of the circuit
	vector<CircuitElement*> Element;
	//array of Nodes
	vector<Node*> Nodes;
	Nodes.resize(MaxElements);
	// to store the joints in an array of nodesir
	Node* Joints;
	//array of voltage sources
	vector<CircuitElement*> VS;
	//array of Current sources
	vector<CircuitElement*> CS;
	//array of voltage sources values
	vector<complex<float>> VSV;
	cout << "Please, write the input file name : ";
	std::getline(std::cin, FileName);
	Input In(FileName + ".txt", Nodes, Element, CircuitElement::TempCounter, VS, CS, VSV);
	MatrixFill M(Nodes,Element,VS,VSV);
	M.SetOut(Nodes,Element);
	Joints = Nodes[0]->ArrayOfJoints(Nodes);
	complex<float> *nodevoltage = new complex<float> [CircuitElement::id];
	nodevoltage[0] = complex<float>(0,0);


	ofstream FileOutput;
	FileOutput.open("Circuit Solutions/" + FileName + " Solution.txt");
	if(toupper(In.OutMethod[0]) == 'C')
	{
		for (int i=1;i<=Node::NodeCount;i++)
		{
			FileOutput << "V("<< i<< ")   "<<Nodes[i]->GetVoltage() << std::endl;
		}
		for (int i=0;i<CircuitElement::TempCounter;i++)
		{
			FileOutput << "I("<< (Element[i]->GetNode1())->GetName()<<","<<(Element[i]->GetNode2())->GetName()<< ")   "<<Element[i]->GetCurrent()<< std::endl;
		}
	}
	else
	{
		for (int i=1;i<=Node::NodeCount;i++)
		{
			FileOutput << "V("<< i<< ")   "<< sqrt(Nodes[i]->GetVoltage().real() * Nodes[i]->GetVoltage().real() + Nodes[i]->GetVoltage().imag() * Nodes[i]->GetVoltage().imag()) << '<' << atan2(Nodes[i]->GetVoltage().imag(),Nodes[i]->GetVoltage().real()) * 180 / pi << std::endl;
		}
		for (int i=0;i<CircuitElement::TempCounter;i++)
		{
			FileOutput << "I("<< (Element[i]->GetNode1())->GetName()<<","<<(Element[i]->GetNode2())->GetName()<< ")   "<< sqrt(Element[i]->GetCurrent().real() * Element[i]->GetCurrent().real() + Element[i]->GetCurrent().imag() * Element[i]->GetCurrent().imag()) << '<' << atan2(Element[i]->GetCurrent().imag(),Element[i]->GetCurrent().real()) * 180 / pi << std::endl;
		}
	}
}

	
