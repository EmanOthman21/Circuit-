#include "CircuitElement.h"

CircuitElement::CircuitElement(void)
{
	TempCounter++;
	Current = 0;
	Voltage = 0;
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
	 if((ElementName[0]=='v'||ElementName[0]=='V')&&(ElementName[2]=='C'||ElementName[2]=='c'))
		return true;
	 if((ElementName[0]=='v'||ElementName[0]=='V')&&(ElementName[2]=='V'||ElementName[2]=='v'))
		return true;
	 if((ElementName[0]=='C'||ElementName[0]=='C')&&(ElementName[2]=='C'||ElementName[2]=='c'))
		return true;
	if((ElementName[0]=='C'||ElementName[0]=='C')&&(ElementName[2]=='V'||ElementName[2]=='V'))
		return true;
		return false;
}

void CircuitElement::SetValue(float val)
{
	if(IsResistance())
		Value=complex<float>(val,0);
	else if (IsCapacitor())
		Value =complex<float>(0,-1/(W*val));
	else if (IsInductor())
		Value=complex<float>(0,W*val);
}
void CircuitElement::SetValueOfVoltageandCurrent (float val,float Phase)
{ 
	Value=complex<float> (val*cos((Phase/180)*3.1415926535897932384626433832795028841971),val*sin((Phase/180)*3.1415926535897932384626433832795028841971));
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
	Current=x;
}
void CircuitElement::SetCounter()
{
	if((ElementName[0]=='v'||ElementName[0]=='V')&&!IsDepSource())
	{
		VoltageCounter++;
	}

	if(ElementName[0]=='r'||ElementName[0]=='R')
	{
		ResistanceCounter ++;
	}
	if(ElementName[0]=='l'||ElementName[0]=='L')
	{
		inductorCounter ++;
	}
	if(ElementName[0]=='C'||ElementName[0]=='c')
	{
		CapacitorCounter ++;
	}
	if((ElementName[0]=='i'||ElementName[0]=='I'))
	
	{
		CurrentCounter++;
	}
}
bool CircuitElement::IsCapacitor()
{
	if((ElementName[0]=='C'||ElementName[0]=='c')&&!IsDepSource())
	{
		return true;
	}
		return false;
	
}


bool CircuitElement::IsCurrentSource ()
{
	if((ElementName[0]=='i'||ElementName[0]=='I')&&(ElementName[1]!='C'||ElementName[1]!='c'))
	{ 

		SetCurrent(Value);
		return true;
	}
	return false;
}


bool CircuitElement::IsInductor ()
{
	if(ElementName[0]=='l'||ElementName[0]=='L')
	{
		return true;
	}
	return false;
}


bool CircuitElement::IsResistance ()
{
	if(ElementName[0]=='r'||ElementName[0]=='R')
	{
		return true;
	}
	return false;
}


bool CircuitElement::IsVoltageSource ()
{
	if((ElementName[0]=='v'||ElementName[0]=='V')&&(ElementName[1]!='C'&&ElementName[1]!='c'))
	{
		return true;
	}
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

CircuitElement** CircuitElement::ElementBetweenNodes(int n1, int n2, int& NumOfElemBetweenNodes, CircuitElement **Element)
{	
	NumOfElemBetweenNodes = 0;
	int node1=0;
	int node2=0;
	CircuitElement **parallelresistances = new CircuitElement*[MaxElements];
	for (int i = 0; i <= CircuitElement::id; i++)
	{
		node1=Element[i]->GetNode1()->GetName();
		node2=Element[i]->GetNode2()->GetName();
		if( (node1== n1 && node2 == n2) || (node1 == n2 && node2 == n1))
		{ 
			parallelresistances[NumOfElemBetweenNodes] = Element[i];
			NumOfElemBetweenNodes++;
		}

	}
	
	return parallelresistances;
}

CircuitElement::~CircuitElement(void)
{
}

