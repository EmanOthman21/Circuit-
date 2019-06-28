#include <complex>
#include <cmath>
#include"MatrixFill.h"
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
	//array of the different elements of the circuit
	CircuitElement* Element[MaxElements] ;
	//array of Nodes
	Node * Nodes[MaxElements];
	// to store the joints in an array of nodesir
	Node* Joints;
	//array of voltage sources
	CircuitElement *VS[MaxElements];
	//array of Current sources
	CircuitElement *CS[MaxElements];
	//array of voltage sources values
	complex<float> *VSV[MaxElements];
	for (int i = 0; i < MaxElements; i++)
	{
		Nodes[i] = new Node;
		VS[i] = new CircuitElement;
		Element[i] = new CircuitElement;
		CS[i] = new CircuitElement;
		VSV[i] = new complex<float>;
	}
	// constructing maxelement of elements made id = maxelement that's why I reinitialize it
	CircuitElement::TempCounter = 0;
	CircuitElement::NumDep = 0;
	Input In("Input.txt",Nodes,Element,CircuitElement::TempCounter,VS,CS,VSV);
	MatrixFill M(Nodes,Element,VS,VSV);
	M.SetOut(Nodes,Element);
	Joints = Nodes[0]->ArrayOfJoints(Nodes);
	complex<float> *nodevoltage = new complex<float> [CircuitElement::id];
	nodevoltage[0] = complex<float>(0,0);
	std::ofstream ofile("output.txt");
	for (int i=1;i<=Node::NodeCount;i++)
	{
		ofile << "V("<< i<< ")   "<<Nodes[i]->GetVoltage() << std::endl;
	}
	for (int i=0;i<CircuitElement::TempCounter;i++)
	{
		ofile << "I("<< (Element[i]->GetNode1())->GetName()<<","<<(Element[i]->GetNode2())->GetName()<< ")   "<<Element[i]->GetCurrent()<< std::endl;
	}
}

	
