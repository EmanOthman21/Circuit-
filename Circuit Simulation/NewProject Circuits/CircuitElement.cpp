#include "CircuitElement.h"

CircuitElement::CircuitElement(void)
{
	TempCounter++;
	Current = 0;
	Voltage = 0;
}
CircuitElement::CircuitElement(string EleName, Node* n1, Node* n2)
{
	TempCounter++;
	Setid(TempCounter);
	Current = 0;
	Voltage = 0;
	SetNode1(n1);
	SetNode2(n2);
	SetElementName(EleName);
	n1->Connect(n2->GetName());
	n1->IncrementFrequency();
	n2->Connect(n1->GetName());
	n2->IncrementFrequency();
}
void CircuitElement::SetNode1(Node* n)
{
	Node1 = n;
}
void CircuitElement::SetNode2(Node* n)
{
	Node2 = n;
}
void CircuitElement::SetFactor(complex <float> n)
{
	FactorValue = n;
}
void CircuitElement::SetNode3(Node* n)
{
	Node3 = n;
}
void CircuitElement::SetNode4(Node* n)
{
	Node4 = n;
}
bool CircuitElement::IsDepSource()
{
	if (ElementName.length() < 3)
		return false;
	if ((toupper(ElementName[0]) == 'V' || toupper(ElementName[0]) == 'C') && (toupper(ElementName[2]) == 'V' || toupper(ElementName[2]) == 'C'))
		return true;
	return false;
}


void CircuitElement::SetValue(float val)
{
	if (IsResistance())
		Value = complex<float>(val, 0);
	else if (IsCapacitor())
		Value = complex<float>(0, -1 / (W * val));
	else if (IsInductor())
		Value = complex<float>(0, W * val);
}
void CircuitElement::SetValueOfVoltageandCurrent (float val,float Phase)
{ 
	 
	Value=complex<float> (val*(cos((Phase*pi/180))-cos(acos(0))),val*(sin((Phase*pi/180))-sin(asin(0))));
}


void CircuitElement::SetElementName(std::string s)
{
	ElementName = s;
	SetCounter();
}
void CircuitElement::Setid(int i)
{
	id = i; 
}

void CircuitElement::SetVoltage(complex<float> v)
{
	Voltage = v;	
}
void CircuitElement::SetCurrent(complex<float> x)
{
	Current = x;
}
void CircuitElement::SetCounter()
{
	if(toupper(ElementName[0])=='V' && !IsDepSource())
	{
		VoltageCounter++;
	}
	if(IsResistance())
	{
		ResistanceCounter++;
	}
	if(IsInductor())
	{
		inductorCounter++;
	}
	if(IsCapacitor())
	{
		CapacitorCounter++;
	}
	if(toupper(ElementName[0]) == 'I')
	{
		CurrentCounter++;
	}
}
bool CircuitElement::IsCapacitor()
{
	if(toupper(ElementName[0])=='C' && !IsDepSource())
	{
		return true;
	}
		return false;
	
}

bool CircuitElement::IsCurrentSource()
{
	if(toupper(ElementName[0])=='I')
	{ 
		SetCurrent(Value);
		return true;
	}
	return false;
}


bool CircuitElement::IsInductor ()
{
	if(toupper(ElementName[0]) == 'L')
		return true;
	return false;
}


bool CircuitElement::IsResistance ()
{
	if(toupper(ElementName[0]) == 'R')
		return true;
	return false;
}


bool CircuitElement::IsVoltageSource ()
{
	if(toupper(ElementName[0]) == 'V' && !IsDepSource())
		return true;
	return false;
}


Node* CircuitElement::GetNode1() 
{
	return Node1;
}
Node* CircuitElement::GetNode2()
{
	return Node2;
}
Node* CircuitElement::GetNode3() 
{
	return Node3;
}
Node* CircuitElement::GetNode4()
{
	return Node4;
}

complex <float> CircuitElement::GetValue() const
{
	return Value;
}



complex <float> CircuitElement::GetFactor() const
{
	return FactorValue;
}


std::string CircuitElement::GetElementName() const
{
	return ElementName;
}
int CircuitElement::Getid() const
{
	return id;
}
complex<float> CircuitElement::GetCurrent() const
{
	return Current;
}
complex<float> CircuitElement::GetVoltage() const
{
	return Voltage;
}

vector<CircuitElement*> CircuitElement::ElementBetweenNodes(int n1, int n2, int& NumOfElemBetweenNodes, vector<CircuitElement*> Element)
{	
	NumOfElemBetweenNodes = 0;
	int node1 = 0;
	int node2 = 0;
	vector<CircuitElement*> parallelresistances;
	for (int i = 0; i < CircuitElement::id; i++)
	{
		node1=Element[i]->GetNode1()->GetName();
		node2=Element[i]->GetNode2()->GetName();
		if( (node1== n1 && node2 == n2) || (node1 == n2 && node2 == n1))
		{ 
			parallelresistances.push_back(Element[i]);
			NumOfElemBetweenNodes++;
		}
	}
	return parallelresistances;
}

CircuitElement::~CircuitElement(void)
{
}

