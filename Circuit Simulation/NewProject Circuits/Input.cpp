#include "Input.h"

Input::Input(const string S, vector<Node*>& Nodes, vector<CircuitElement*>& Element, int& TempCounter, vector<CircuitElement*>& VS, vector<CircuitElement*>& CS, vector<complex<float>*>& VSV)
{
	FileName = S;
	// some variables for the dependant sources
	complex<float>Factor[100];
	int vsindex = 1;
	int csindex = 1;
	FileInput.open("../Input Circuits/" + FileName);
	if (FileInput.is_open())
	{
		// temoporary variables to read the input from the file
		std::string elename;
		float val;
		while (true)
		{
			if (FileInput.eof())
			{
				FileInput.close();
				break;
			}
			FileInput >> elename;
			if ((toupper(elename[0]) == 'C' && toupper(elename[2]) == 'C') || (toupper(elename[0]) == 'C' && toupper(elename[2]) == 'V') || (toupper(elename[0]) == 'V' && toupper(elename[2]) == 'V') || (toupper(elename[0]) == 'V' && toupper(elename[2]) == 'C'))
			{
				Read_Dependant(elename, Factor[CircuitElement::NumDep], Nodes, Element, TempCounter);
				CircuitElement::NumDep++;
				continue;
			}
			if (elename[0] == 'w' || elename[0] == 'W')
			{
				FileInput >> val;
				CircuitElement::W = val;
			}
			else if (elename[0] == 'V' || elename[0] == 'v' || (elename[0] == 'I' || elename[0] == 'i'))
			{
				Read_Voltage_Source_Current_Source(vsindex, csindex, elename, Nodes, Element, TempCounter, VS, CS, VSV);
			}
			else
			{
				Read_Resistor_Capacitor_Inductor(elename, Nodes, Element, TempCounter);
			}
		}

	}
	else
	{
		std::cout << "cannot open the file\nPlease, check the name of the file is writtern correctly\n\n";
		exit(-1);
	}

}


void Input::Read_Dependant(const string elename,complex<float>& Factor, vector<Node*>& Nodes, vector<CircuitElement*>& Element, int& TempCounter)
{
	int n1 , n2 , M , N, re, ima;
	FileInput >> n1 >> n2 >> re >> ima>> M >> N;
	Factor= complex<float> (re,ima);
	if (!Nodes[n1])
		Nodes[n1] = new Node;
	if (!Nodes[n2])
		Nodes[n2] = new Node;
	Nodes[n1]->SetName(n1);
	Nodes[n2]->SetName(n2);
	Nodes[M]->SetName(M);
	Nodes[N]->SetName(N);
	Element[CircuitElement::TempCounter - 1] = new CircuitElement;
	CircuitElement::TempCounter--;
	Element[CircuitElement::TempCounter]->Setid(CircuitElement::TempCounter);
	Element[CircuitElement::TempCounter]->SetElementName(elename);
	Element[CircuitElement::TempCounter]->SetNode1(Nodes[n1]);
	Element[CircuitElement::TempCounter]->GetNode1()->Connect(n2);
	Element[CircuitElement::TempCounter]->GetNode1()->IncrementFrequency();
	Element[CircuitElement::TempCounter]->SetNode2(Nodes[n2]);
	Element[CircuitElement::TempCounter]->GetNode2()->Connect(n1);
	Element[CircuitElement::TempCounter]->GetNode2()->IncrementFrequency();
	Element[CircuitElement::TempCounter]->SetNode3(Nodes[M]);
	Element[CircuitElement::TempCounter]->SetNode4(Nodes[N]);
	Element[CircuitElement::TempCounter]->SetFactor(Factor);
	CircuitElement::TempCounter++;

}

void Input::Read_Voltage_Source_Current_Source( int& vsindex , int& csindex ,const string elename, vector<Node*>& Nodes, vector<CircuitElement*>& Element,int &TempCounter, vector<CircuitElement*>&VS, vector<CircuitElement*>&CS, vector<complex<float>*>&VSV)
{

	int n1 , n2 , M ,val,Phase;
	FileInput >> n1 >> n2 >> val>> Phase;
	if (!Nodes[n1])
		Nodes[n1] = new Node;
	if (!Nodes[n2])
		Nodes[n2] = new Node;
	Nodes[n1]->SetName(n1);
	Nodes[n2]->SetName(n2);
	Element[CircuitElement::TempCounter - 1] = new CircuitElement;
	CircuitElement::TempCounter--;
	Element[CircuitElement::TempCounter]->Setid(CircuitElement::TempCounter);
	Element[CircuitElement::TempCounter]->SetElementName(elename);
	Element[CircuitElement::TempCounter]->SetNode1(Nodes[n1]);
	Element[CircuitElement::TempCounter]->GetNode1()->Connect(n2);
	Element[CircuitElement::TempCounter]->GetNode1()->IncrementFrequency();
	Element[CircuitElement::TempCounter]->SetNode2(Nodes[n2]);
	Element[CircuitElement::TempCounter]->GetNode2()->Connect(n1);
	Element[CircuitElement::TempCounter]->GetNode2()->IncrementFrequency();
	Element[CircuitElement::TempCounter]->SetValueOfVoltageandCurrent(val,Phase);
	//to store the voltage sources in an array of elements
	if(Element[CircuitElement::TempCounter]->IsVoltageSource())
	{
		VS[vsindex]=Element[CircuitElement::TempCounter];
		VSV[vsindex] = new complex<float>;
		*VSV[vsindex]=VS[vsindex]->GetValue();
		vsindex++;
	}
	if(Element[CircuitElement::TempCounter]->IsCurrentSource())
	{
		CS[csindex]=Element[CircuitElement::TempCounter];
		csindex++;
	}
	Node::NodeCount = (Node::NodeCount < n1 )? n1 : Node::NodeCount;  
	Node::NodeCount = (Node::NodeCount < n2 )? n2 : Node::NodeCount; 
	CircuitElement::TempCounter++;

}

void Input::Read_Resistor_Capacitor_Inductor(const string elename, vector<Node*>& Nodes, vector<CircuitElement*>& Element,int &TempCounter)
{
	int n1,n2,val;
		FileInput >> n1 >> n2 >> val;
		if (!Nodes[n1])
			Nodes[n1] = new Node;
		if (!Nodes[n2])
			Nodes[n2] = new Node;
		Nodes[n1]->SetName(n1);
		Nodes[n2]->SetName(n2);
		Element[CircuitElement::TempCounter - 1] = new CircuitElement;
		CircuitElement::TempCounter--;
		Element[CircuitElement::TempCounter]->Setid(CircuitElement::TempCounter);
		Element[CircuitElement::TempCounter]->SetElementName(elename);
		Element[CircuitElement::TempCounter]->SetNode1(Nodes[n1]);
		Element[CircuitElement::TempCounter]->GetNode1()->Connect(n2);
		Element[CircuitElement::TempCounter]->GetNode1()->IncrementFrequency();
		Element[CircuitElement::TempCounter]->SetNode2(Nodes[n2]);
		Element[CircuitElement::TempCounter]->GetNode2()->Connect(n1);
		Element[CircuitElement::TempCounter]->GetNode2()->IncrementFrequency();
		Element[CircuitElement::TempCounter]->SetValue(val);
		Node::NodeCount = (Node::NodeCount < n1 )? n1 : Node::NodeCount;  
		Node::NodeCount = (Node::NodeCount < n2 )? n2 : Node::NodeCount; 
		CircuitElement::TempCounter++;

}



Input::~Input(void)
{
}
