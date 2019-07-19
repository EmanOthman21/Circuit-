#include "Input.h"

Input::Input(const string S, vector<Node*>& Nodes, vector<CircuitElement*>& Element, int& TempCounter, vector<CircuitElement*>& VS, vector<CircuitElement*>& CS, vector<complex<float>>& VSV)
{
	OutMethod = "Phase";
	FileName = S;
	// the next five lines are useless for now
	complex<float> t;
	CircuitElement* T;
	VSV.push_back(t);
	VS.push_back(T);
	CS.push_back(T);
	// some variables for the dependant sources
	complex<float>Factor[100];
	FileInput.open("../Input Circuits/" + FileName);
	if (FileInput.is_open())
	{
		// temoporary variables to read the input from the file
		std::string EleName;
		float val;
		while (true)
		{
			if (FileInput.eof())
			{
				FileInput.close();
				break;
			}
			FileInput >> EleName;
			if(EleName.length() > 4 )
			{
				OutMethod = EleName;
				continue;
			}

			if ((toupper(EleName[0]) == 'C' && toupper(EleName[2]) == 'C') || (toupper(EleName[0]) == 'C' && toupper(EleName[2]) == 'V') || (toupper(EleName[0]) == 'V' && toupper(EleName[2]) == 'V') || (toupper(EleName[0]) == 'V' && toupper(EleName[2]) == 'C'))
			{
				Read_Dependant(EleName, Factor[CircuitElement::NumDep], Nodes, Element, TempCounter);
				CircuitElement::NumDep++;
				continue;
			}
			if (EleName[0] == 'w' || EleName[0] == 'W')
			{
				FileInput >> val;
				CircuitElement::W = val;
			}
			else if (EleName[0] == 'V' || EleName[0] == 'v' || (EleName[0] == 'I' || EleName[0] == 'i'))
			{
				Read_Voltage_Source_Current_Source(EleName, Nodes, Element, TempCounter, VS, CS, VSV);
			}
			else
			{
				Read_Resistor_Capacitor_Inductor(EleName, Nodes, Element, TempCounter);
			}
		}

	}
	else
	{
		std::cout << "cannot open the file\nPlease, check the name of the file is written correctly\n\n";
		exit(-1);
	}
}


void Input::Read_Dependant(const string EleName,complex<float>& Factor, vector<Node*>& Nodes, vector<CircuitElement*>& Element, int& TempCounter)
{
	int n1 , n2 , M , N;
	double re, ima;
	string real , imag;
	FileInput >> n1 >> n2 >> real >> imag>> M >> N;
	re  =  stod(real);
	ima =  stod(imag);
	Factor= complex<float> (re,ima);
	if (!Nodes[n1])
		Nodes[n1] = new Node(n1);
	if (!Nodes[n2])
		Nodes[n2] = new Node(n2);
	if (!Nodes[M])
		Nodes[M] = new Node(M);
	if (!Nodes[N])
		Nodes[N] = new Node(N);
	CircuitElement* TempElem = new CircuitElement(EleName, Nodes[n1], Nodes[n2]);
	TempElem->SetNode3(Nodes[M]);
	TempElem->SetNode4(Nodes[N]);
	TempElem->SetFactor(Factor);
	Element.push_back(TempElem);

}

void Input::Read_Voltage_Source_Current_Source(const string EleName, vector<Node*>& Nodes, vector<CircuitElement*>& Element,int &TempCounter, vector<CircuitElement*>&VS, vector<CircuitElement*>&CS, vector<complex<float>>&VSV)
{

	int n1 , n2 , M ;
	double val, Phase;
	string va , Ph;
	FileInput >> n1 >> n2 >> va>> Ph;
	val = stod(va);
	Phase = stod(Ph);
	if (!Nodes[n1])
		Nodes[n1] = new Node;
	if (!Nodes[n2])
		Nodes[n2] = new Node;
	Nodes[n1]->SetName(n1);
	Nodes[n2]->SetName(n2);
	CircuitElement* TempElem = new CircuitElement(EleName, Nodes[n1], Nodes[n2]);
	TempElem->SetValueOfVoltageandCurrent(val,Phase);
	//to store the voltage sources in an array of elements
	if(TempElem->IsVoltageSource())
	{
		VS.push_back(TempElem);
		VSV.push_back(TempElem->GetValue());
	}
	if(TempElem->IsCurrentSource())
	{
		CS.push_back(TempElem);
	}
	Node::NodeCount = (Node::NodeCount < n1 )? n1 : Node::NodeCount;  
	Node::NodeCount = (Node::NodeCount < n2 )? n2 : Node::NodeCount; 
	Element.push_back(TempElem);
}

void Input::Read_Resistor_Capacitor_Inductor(const string EleName, vector<Node*>& Nodes, vector<CircuitElement*>& Element,int &TempCounter)
{
	int n1, n2;
	double val;
	string va ;
	FileInput >> n1 >> n2 >> va;
	val = stod(va);
	if (!Nodes[n1])
		Nodes[n1] = new Node;
	if (!Nodes[n2])
		Nodes[n2] = new Node;
	Nodes[n1]->SetName(n1);
	Nodes[n2]->SetName(n2);
	CircuitElement* TempElem = new CircuitElement(EleName, Nodes[n1], Nodes[n2]);
	TempElem->SetValue(val);
	Node::NodeCount = (Node::NodeCount < n1 )? n1 : Node::NodeCount;  
	Node::NodeCount = (Node::NodeCount < n2 )? n2 : Node::NodeCount; 
	Element.push_back(TempElem);
}



Input::~Input(void)
{
}
