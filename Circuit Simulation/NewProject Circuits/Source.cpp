#include <iostream>
#include "CircuitElement.h"
#include "Node.h"
#include <fstream>
#include <Eigen\Dense>
#include <complex>
#include <cmath>
using Eigen::MatrixXd;
using Eigen::MatrixXcd;


int CircuitElement::id = 0;
float CircuitElement::W = 0;
int CircuitElement::TempCounter = 0;
//The total number of nodes	in the entered circuit 
int Node::NodeCount = 0;
int Node::JointNum = 0;
//The elements counters
int CircuitElement:: VoltageCounter=0;
int CircuitElement:: ResistanceCounter=0;
int CircuitElement::  inductorCounter=0;
int CircuitElement:: CapacitorCounter=0;
int CircuitElement:: CurrentCounter=0;
void CreateEmptyMat(complex<float>**Matrix, int N);

int main()
{
	//array of the different elements of the circuit
	CircuitElement Element[MaxElements];
	//array of Nodes
	Node Nodes[MaxElements];
	// to store the joints in an array of nodesir
	Node* Joints;
	//array of voltage sources
	CircuitElement VS[MaxElements];

	//array of Current sources
	CircuitElement CS[MaxElements];
	int vsindex =1;
	int csindex =1;
	//array of voltage sources values
	complex<float> VSV[MaxElements];	
	// constructing maxelement of elements made id = maxelement that's why I reinitialize it
	CircuitElement::TempCounter = 0;
	
	int K,L,M,N,IICounter=0,VCCounter=0;
	complex<float>FactorIV[100],FactorII[100],Factor,Factor1;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																													  //
	//					               taking the inputs from input.txt file			        						  //
	//																													  //
	//																													  //
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	std::ifstream file("input.txt");
	if(file.is_open())
	{
		// temoporary variables to read the input from the file
		std::string elename;
		int n1, n2;
		float val;
		float Phase;
		while(true)
		{
			if(file.eof())
		{			file.close();
				break;

			}
			file>>elename;
			if((elename[0]=='v'||elename[0]=='V')&&(elename[2]=='c'||elename[2]=='C'))
			{
				
				file >> n1 >> n2 >> Factor1>> M >> N;
				K=n1;
				L=n2;
				Nodes[n1].SetName(n1);
				Nodes[n2].SetName(n2);
				Element[CircuitElement::TempCounter].Setid(CircuitElement::TempCounter);
				Element[CircuitElement::TempCounter].SetElementName(elename);
				Element[CircuitElement::TempCounter].SetNode1(&Nodes[n1]);
				Element[CircuitElement::TempCounter].GetNode1()->Connect(n2);
				Element[CircuitElement::TempCounter].GetNode1()->IncrementFrequency();
				Element[CircuitElement::TempCounter].SetNode2(&Nodes[n2]);
				Element[CircuitElement::TempCounter].GetNode2()->Connect(n1);
				Element[CircuitElement::TempCounter].GetNode2()->IncrementFrequency();
				CircuitElement::TempCounter++;
				continue;
			}

			 if((elename[0]=='v'||elename[0]=='V')&&(elename[2]=='V'||elename[2]=='v'))
			{
				
				file >> n1 >> n2 >> Factor>> M >> N;
				K=n1;
				L=n2;
				Nodes[n1].SetName(n1);
				Nodes[n2].SetName(n2);
				Element[CircuitElement::TempCounter].Setid(CircuitElement::TempCounter);
				Element[CircuitElement::TempCounter].SetElementName(elename);
				Element[CircuitElement::TempCounter].SetNode1(&Nodes[n1]);
				Element[CircuitElement::TempCounter].GetNode1()->Connect(n2);
				Element[CircuitElement::TempCounter].GetNode1()->IncrementFrequency();
				Element[CircuitElement::TempCounter].SetNode2(&Nodes[n2]);
				Element[CircuitElement::TempCounter].GetNode2()->Connect(n1);
				Element[CircuitElement::TempCounter].GetNode2()->IncrementFrequency();
				CircuitElement::TempCounter++;
				continue;
			}
			if((elename[0]=='i'||elename[0]=='I')&&(elename[2]=='I'||elename[2]=='i'))
			{
				
				file >> n1 >> n2 >> FactorII[IICounter]>> M >> N;
				K=n1;
				L=n2;
				IICounter++;
				Nodes[n1].SetName(n1);
				Nodes[n2].SetName(n2);
				Element[CircuitElement::TempCounter].Setid(CircuitElement::TempCounter);
				Element[CircuitElement::TempCounter].SetElementName(elename);
				Element[CircuitElement::TempCounter].SetNode1(&Nodes[n1]);
				Element[CircuitElement::TempCounter].GetNode1()->Connect(n2);
				Element[CircuitElement::TempCounter].GetNode1()->IncrementFrequency();
				Element[CircuitElement::TempCounter].SetNode2(&Nodes[n2]);
				Element[CircuitElement::TempCounter].GetNode2()->Connect(n1);
				Element[CircuitElement::TempCounter].GetNode2()->IncrementFrequency();
				continue;
			}
			if((elename[0]=='I'||elename[0]=='I')&&(elename[2]=='c'||elename[2]=='C'))
			{
				
				file >> n1 >> n2 >> FactorIV[VCCounter]>> M >> N;
				K=n1;
				L=n2;
				VCCounter++;
				Nodes[n1].SetName(n1);
				Nodes[n2].SetName(n2);
				Element[CircuitElement::TempCounter].Setid(CircuitElement::TempCounter);
				Element[CircuitElement::TempCounter].SetElementName(elename);
				Element[CircuitElement::TempCounter].SetNode1(&Nodes[n1]);
				Element[CircuitElement::TempCounter].GetNode1()->Connect(n2);
				Element[CircuitElement::TempCounter].GetNode1()->IncrementFrequency();
				Element[CircuitElement::TempCounter].SetNode2(&Nodes[n2]);
				Element[CircuitElement::TempCounter].GetNode2()->Connect(n1);
				Element[CircuitElement::TempCounter].GetNode2()->IncrementFrequency();
				continue;
			}

			if(elename[0] == 'w' || elename[0] == 'W')
				{
					file>>val;
					CircuitElement::W=val;
				}
			

			else if(elename[0] == 'V' || elename[0] == 'v' || (elename[0] == 'I' || elename[0] == 'i')&&elename[2]!='V')
			{
				file >> n1 >> n2 >> val>> Phase;
				Nodes[n1].SetName(n1);
				Nodes[n2].SetName(n2);
				Element[CircuitElement::TempCounter].Setid(CircuitElement::TempCounter);
				Element[CircuitElement::TempCounter].SetElementName(elename);
				Element[CircuitElement::TempCounter].SetNode1(&Nodes[n1]);
				Element[CircuitElement::TempCounter].GetNode1()->Connect(n2);
				Element[CircuitElement::TempCounter].GetNode1()->IncrementFrequency();
				Element[CircuitElement::TempCounter].SetNode2(&Nodes[n2]);
				Element[CircuitElement::TempCounter].GetNode2()->Connect(n1);
				Element[CircuitElement::TempCounter].GetNode2()->IncrementFrequency();
					Element[CircuitElement::TempCounter].SetValueOfVoltageandCurrent(val,Phase);
				//to store the voltage sources in an array of elements
				if(Element[CircuitElement::TempCounter].IsVoltageSource())
				{
					VS[vsindex]=Element[CircuitElement::TempCounter];
				    VSV[vsindex]=VS[vsindex].GetValue();
					vsindex++;
				}
				if(Element[CircuitElement::TempCounter].IsCurrentSource())
				{
					CS[csindex]=Element[CircuitElement::TempCounter];
					csindex++;
				}
				Node::NodeCount = (Node::NodeCount < n1 )? n1 : Node::NodeCount;  
				Node::NodeCount = (Node::NodeCount < n2 )? n2 : Node::NodeCount; 
				CircuitElement::TempCounter++;
			}
			else 
			{
				file >> n1 >> n2 >> val;
				Nodes[n1].SetName(n1);
				Nodes[n2].SetName(n2);
				Element[CircuitElement::TempCounter].Setid(CircuitElement::TempCounter);
				Element[CircuitElement::TempCounter].SetElementName(elename);
				Element[CircuitElement::TempCounter].SetNode1(&Nodes[n1]);
				Element[CircuitElement::TempCounter].GetNode1()->Connect(n2);
				Element[CircuitElement::TempCounter].GetNode1()->IncrementFrequency();
				Element[CircuitElement::TempCounter].SetNode2(&Nodes[n2]);
				Element[CircuitElement::TempCounter].GetNode2()->Connect(n1);
				Element[CircuitElement::TempCounter].GetNode2()->IncrementFrequency();
				Element[CircuitElement::TempCounter].SetValue(val);
				Node::NodeCount = (Node::NodeCount < n1 )? n1 : Node::NodeCount;  
				Node::NodeCount = (Node::NodeCount < n2 )? n2 : Node::NodeCount; 
				CircuitElement::TempCounter++;
			}


		}

	}
	else
		std::cout<<"cannot open the file\n";

	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																													  //
	//					           declaring some variables that will be needed later									  //
	//																													  //
	//																													  //
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//the last node of the nodes entered is the number stored in nodecount but we count from zero 
	Node::NodeCount; 
	//array to store the names of the joints.. I had to call it using some node if anyone has an idea to call it directly feel free to edit 
	Joints = Nodes[0].ArrayOfJoints(Nodes);
	//id is number of elements - 1 as it is zero based
	float total_voltage = 0;
	float total_resistance= 0;
	int numofresistances = 0;
	int numofvoltagesources = 0;
	// array to store the voltage of all the nodes
	complex<float> *nodevoltage = new complex<float> [CircuitElement::id];
	// assuming the ground is zero
	nodevoltage[0] = complex<float>(0,0);
	int x = Node::NodeCount;
	// declaring the matrix that will be used in the loop analysis

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																													  //
	//									Create and fill in the G matirx													  //
	//																													  //
	//																													  //
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// here we start constructing the matrices needed for the node analysis method

	// some variable that will be used to store the number of elements in parallel between any two nodes by sending it by reference to the function (ElementsBetweenNodes) 
	int index=0;
	// will be used in various positions but its main purpose to fill the G Matrix
	complex<float> GElement=0;
	complex<float> IElement=0;
	// declaring the g matrix of size (the number of nodes in the ciruit - 1) execluding zero and node count is the num of nodes-1 as it stores the last node entered (if the maximum node entered is 5 which means we have 6 nodes so we store that 5 in Node::Nodecount
	complex<float>** G=new complex<float>*[Node::NodeCount];
	// creating the matrix dynamically and initializing every element with zero
	CreateEmptyMat(G,Node::NodeCount);
	//size is to store the number of nodes connected to a certain node
	int size=0,c=0;
	// to store an array of pointers to element between nodes we needed an array because there may be some elements between two nodes in parallel
	CircuitElement**Temp;

	//loop to iterate through every row in the G matrix i represents the node name so we started from 1 not zero as the MNA method excludes the ground
	for (int i = 1; i < Node::NodeCount+1; i++)
	{
		size=Nodes[i].GetConnection().size();
		//loops over every node connected to the node in the current row
		for (std::set<int>::iterator it = Nodes[i].GetConnection().begin(); it != Nodes[i].GetConnection().end(); ++it)
		{
			Temp=Element[i].ElementBetweenNodes(i,*it,index,Element);
			//to get the total G (conductance) connected to each node
			for (int k = 0; k < index; k++)
			{
				if(Temp[k]->IsResistance()||Temp[k]->IsCapacitor()||Temp[k]->IsInductor())
				GElement+=complex<float>(1,0)/Temp[k]->GetValue();
				
			}
		}
		// to put the diagonal elements only
		G[i-1][i-1]=GElement;
		GElement=0;

		//to fill the rest of the elements in matrix G 
		for (std::set<int>::iterator it = Nodes[i].GetConnection().begin(); it != Nodes[i].GetConnection().end(); ++it)
		{
			// to execlude node zero
			if(*it==0)
				continue;
			Temp=Element[i].ElementBetweenNodes(i,*it,index,Element);    //
			// iterate over each element in parallel between the two nodes entered in the previous line
			for (int k = 0; k < index; k++)
			{
				if(Temp[k]->IsResistance()||Temp[k]->IsCapacitor()||Temp[k]->IsInductor())
				GElement+=complex <float>(1,0)/Temp[k]->GetValue();
			}

			G[i-1][*it-1] = -GElement;
			GElement = 0;

		}
		
		GElement = 0;
		
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																													  //
	//					               create and fill in the B matirx					        						  //
	//																													  //
	//																													  //
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//create an empty matrix B N×M (part of the Bigest Matrix) 
	complex<float >**B= new complex<float>*[Node::NodeCount];
	CreateEmptyMat(B,CircuitElement::VoltageCounter);

	for (int i = 0; i < Node::NodeCount; i++)
	{
		B[i]=new complex<float> [CircuitElement::VoltageCounter];
	}
	for (int i = 0; i < Node::NodeCount; i++)
	{
		for (int j = 0; j < CircuitElement::VoltageCounter; j++)
		{
			B[i][j]=0;
		}

	}
	
	// to iterate through everynode execuling the ground
	for (int i = 1; i < Node::NodeCount+1; i++)
	{
		size=Nodes[i].GetConnection().size();
		// to iterate over every node connected to node number i
		for (std::set<int>::iterator it = Nodes[i].GetConnection().begin(); it != Nodes[i].GetConnection().end(); ++it)
		{
			Temp=Element[i].ElementBetweenNodes(i,*it,index,Element);
			// to iterate through every element in parallel between two certain nodes
			for(int x=0;x<index;x++)
			{
				if(Temp[x]->IsVoltageSource())
				{
					// starting to fill every column in the current row (ie the row of the node number i)
					for (int k = 0; k < CircuitElement::VoltageCounter; k++)
					{
						if(VS[k+1].GetNode1()==Temp[x]->GetNode1()&&VS[k+1].GetNode2()==Temp[x]->GetNode2())
							if(VS[k+1].GetNode1()->GetName()==Nodes[i].GetName())
								B[i-1][k]=-1;
							else
								B[i-1][k]=1;
					}
				}
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																													  //
	//					               create and fill in the C matirx					        						  //
	//																													  //
	//																													  //
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//creating the matrix C part of the big matrix
	complex<float> **C= new complex< float>*[CircuitElement::VoltageCounter];
	for (int i = 0; i < CircuitElement::VoltageCounter; i++)
	{
		C[i]=new complex<float> [Node::NodeCount];
	}
	for (int i = 0; i < CircuitElement::VoltageCounter; i++)
	{
		for (int j = 0; j < Node::NodeCount; j++)
		{
			C[i][j]=0;
		}

	}
	// temporarly the matrix C is the transpose of the matrix B untill we see how to add the dependent sources
	for (int i = 0; i < CircuitElement::VoltageCounter; i++)
	{
		for (int j = 0; j < Node::NodeCount; j++)
		{
			C[i][j] = B [j][i];
		}

	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																													  //
	//					               create and fill in the D matirx					        						  //
	//																													  //
	//																													  //
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// this matrix is part of the big matrix it's temporarly zero matrix but it needs to be modified when we add dependent sources
	complex<float> **D= new complex<float>*[CircuitElement::VoltageCounter];
	CreateEmptyMat(D, CircuitElement::VoltageCounter);


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																													  //
	//					               create and fill in the main matirx A					        					  //
	//																													  //
	//																													  //
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int AMatrixSize = CircuitElement::VoltageCounter + Node::NodeCount;
	complex<float> **A= new complex<float>*[AMatrixSize];
	CreateEmptyMat(A, AMatrixSize);
	
	// this for loop to fill G and B into the matrix (first nodecount rows)
	for (int i = 0; i < Node::NodeCount; i++)
	{
		int j = 0;
		for (j; j < Node::NodeCount; j++)
		{
			A[i][j] = G[i][j];
		}
		for (j ; j < AMatrixSize; j++)
		{
			A[i][j] = B[i][j - Node::NodeCount];
		}
	}
	
	// this for loop to fill C and D matrices into the matrix (remaining rows) 
	for (int i = Node::NodeCount; i < AMatrixSize; i++)
	{
		int j = 0;
		for (j; j < Node::NodeCount; j++)
		{
			A[i][j] = C[i - Node::NodeCount][j];
		}
		for (j ; j < AMatrixSize; j++)
		{
			A[i][j] = D[i - Node::NodeCount][j - Node::NodeCount];
		}
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																													  //
	//					               create and fill in the main matirx i				        				     	  //
	//																													  //
	//																													  //
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int iMatrixSize =Node::NodeCount;
	complex<float> *I= new complex<float>[iMatrixSize];
	
	for (int i = 0; i < Node::NodeCount; i++)
	{
			I[i]=0;
	}

	for (int i = 1; i < Node::NodeCount+1; i++)
	{
		size=Nodes[i].GetConnection().size();
		//loops over every node connected to the node in the current row
		for (std::set<int>::iterator it = Nodes[i].GetConnection().begin(); it != Nodes[i].GetConnection().end(); ++it)
		{
			Temp=Element[i].ElementBetweenNodes(i,*it,index,Element);
			
			for (int k = 0; k < index; k++)
			{
				if(Temp[k]->IsCurrentSource())
				{
					if (Nodes[i].GetName()==(Temp[k]->GetNode2())->GetName())
						IElement+=Temp[k]->GetValue();
					else 
						IElement-=Temp[k]->GetValue();
				}
			} 
		  }		
		I[i-1]=IElement;
		    IElement=0;

	}	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																													  //
	//					               create and fill in the E matirx					        						  //
	//																													  //
	//																													  //
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int EMatrixSize =vsindex;
	complex<float> *E= new complex<float>[EMatrixSize];
	
	for (int i = 0; i < Node::NodeCount; i++)
	{
			E[i]=0;
	}

	for(int i=0;i<EMatrixSize-1;i++)
	{
	E[i]=VSV[i+1];
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																													  //
	//					               create and fill in the Q matirx					        						  //
	//																													  //
	//																													  //
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
     int ZMatrixSize=vsindex+Node::NodeCount-1;
	 MatrixXcd Q(ZMatrixSize,1);
	for (int i = 0; i < Node::NodeCount; i++)
	{
		Q(i,0)=0;
		
	}
	for (int i = Node::NodeCount; i < ZMatrixSize; i++)
		{
		Q(i,0)=0;
	}
   for (int C=0;C<Node::NodeCount;C++)
	{
		Q(C,0)=I[C];
	}

   int m=0;
	for (int l=Node::NodeCount;l< ZMatrixSize;l++)
	{ 
   	Q(l,0)=E[m];
      m++;	
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																													  //
	//					               create and fill in the X matirx					        						  //
	//																													  //
	//																													  //
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int XMatrixSize=vsindex+Node::NodeCount-1;
	MatrixXcd XD(XMatrixSize,1);

	for (int i = 0; i < XMatrixSize; i++)
	{
		XD(i,0)=0;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																													  //
	//					               create and fill in the Y matirx					        						  //
	//																													  //
	//																													  //
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int YMatrixSize = CircuitElement::VoltageCounter + Node::NodeCount;
	complex<float> **Y= new complex<float>*[YMatrixSize];
	CreateEmptyMat(Y, YMatrixSize);
    MatrixXcd mb(AMatrixSize,AMatrixSize);
	
	for (int i = 0; i < AMatrixSize; i++)
	{
		for (int j = 0; j < AMatrixSize; j++)
		{
			mb(i,j) = A[i][j];
		}

	}
	bool flag=0;
	for (int i = 0; i < VCCounter; i++)
	{

	
	if(FactorIV[i]!=complex<float>(0,0))
	{
		flag=true;
		if(K!=0)
		{
			if(M!=0)
		mb(K-1,M-1)-=FactorIV[i];
			if(N!=0)
		mb(K-1,N-1)+=FactorIV[i];
		}
		if(L!=0)
		{
			if(M!=0)
		mb(L-1,M-1)-=FactorIV[i];
			if(N!=0)
		mb(L-1,N-1)+=FactorIV[i];
		}
	}
	}
	for (int i = 0; i < IICounter; i++)
	{

	
		if(FactorII[i]!=complex<float>(0,0))
		{

		flag=true;
			Temp=Element[i].ElementBetweenNodes(M,N,index,Element);
			for (int j = 0; j < index; j++)
			{
				if(Temp[j]->IsResistance()||Temp[j]->IsCapacitor()||Temp[j]->IsInductor())
				{
					if(K!=0)
				{
					if(M!=0)
					mb(K-1,M-1)+=FactorII[i]*complex<float>(1,0)/Temp[j]->GetValue();
					if(N!=0)
					mb(K-1,N-1)-=FactorII[i]*complex<float>(1,0)/Temp[j]->GetValue();
				}
				if(L!=0)
				{
					if(M!=0)	
					mb(L-1,M-1)-=FactorII[i]*complex<float>(1,0)/Temp[j]->GetValue();
					if(N!=0)
					mb(L-1,N-1)+=FactorII[i]*complex<float>(1,0)/Temp[j]->GetValue();
				}
				}
			}
		}
	}

	if(flag==true)
	XD= mb.inverse()*Q;
	else
	if (Factor !=complex<float>(0,0))
	{
	  MatrixXcd mb2(AMatrixSize+1,AMatrixSize+1);
	  for (int i =0;i<AMatrixSize+1;i++)
	  {
		  mb2(i,AMatrixSize)=complex<float>(0,0);
	  }
	  for (int i =0;i<AMatrixSize+1;i++)
	  {
		  mb2(AMatrixSize,i)=complex<float>(0,0);
	  }
     for (int i = 0; i < AMatrixSize; i++)
		{
			for (int j = 0; j < AMatrixSize; j++)
			{
				mb2(i,j) = A[i][j];
			}

		}
	  if(L!=0)
	   { 
	   mb2(L-1,AMatrixSize)=complex <float >(1,0);
       mb2(AMatrixSize,L-1)=complex <float> (1,0);
		}
	 if(K!=0)
		 {
	 	 mb2(K-1,AMatrixSize)=complex <float> (-1,0);
	     mb2(AMatrixSize,K-1)=complex <float>(-1,0);
		 }
	 if(M!=0)
	     mb2(AMatrixSize,M-1)=complex <float>(-1,0)*Factor;
	 if(N!=0)
	     mb2(AMatrixSize,N-1)=Factor;
      MatrixXcd S(ZMatrixSize+1,1);
	  for (int C=0;C<Node::NodeCount;C++)
	{
		S(C,0)=I[C];
	}

   int m=0;
	for (int l=Node::NodeCount;l< ZMatrixSize;l++)
	{ 
   	S(l,0)=E[m];
      m++;	
	}
	S(ZMatrixSize,0)=complex<float>(0,0);

	MatrixXcd XD1(XMatrixSize+1,1);
	for (int i = 0; i < XMatrixSize+1; i++)
	{
		XD1(i,0)=0;
	}
	 
	//result matrix
	  XD1= mb2.inverse()*S;
	  for (int i=1;i<=Node::NodeCount;i++)
	{
	Nodes[i].SetVoltage(XD1(i-1,0));
	}
	Nodes[0].SetVoltage(complex <float>(0,0));
    int o=Node::NodeCount;
	for (int i=0;i<CircuitElement::TempCounter;i++)
	{
		
			if(Element[i].IsVoltageSource())
				{
					Element[i].SetCurrent((XD1(o-1,0))*complex <double>(-1.0));
		         	o++;
			    }
	}
	

	for (int i=0;i<CircuitElement::TempCounter;i++)
	{
      if(Element[i].IsDepSource())
				{
					Element[i].SetCurrent((XD1(o-1,0))*complex <double>(-1.0));
		         	o++;
			    }
	}

	for (int i=0;i<CircuitElement::TempCounter;i++)
	{
		if(Element[i].IsResistance()||Element[i].IsCapacitor()||Element[i].IsInductor())
		{
		
			Element[i].SetCurrent(complex <float >(1,0)*(((Element[i].GetNode2())->GetVoltage())-((Element[i].GetNode1())->GetVoltage()))/Element[i].GetValue());
		
		}
		
	}
	}

	
		else 
	{

	XD= mb.inverse()*Q;
	}
	///////////////////////////////////////////////////////////////////////////////

for (int i=1;i<=Node::NodeCount;i++)
	{
	Nodes[i].SetVoltage(XD(i-1,0));
	}
	Nodes[0].SetVoltage(complex <float>(0,0));
    int o=Node::NodeCount;

	for (int i=0;i<CircuitElement::TempCounter;i++)
	{
		
			if(Element[i].IsVoltageSource())
				{Element[i].SetCurrent((XD(o,0))*complex <double>(-1.0));
			o++;}
	}

	

	for (int i=0;i<CircuitElement::TempCounter;i++)
	{
		if(Element[i].IsResistance()||Element[i].IsCapacitor()||Element[i].IsInductor())
		{
		
			Element[i].SetCurrent((((Element[i].GetNode1())->GetVoltage())-((Element[i].GetNode2())->GetVoltage()))/Element[i].GetValue());
		
		}
		
	}
	std::ofstream ofile("output.txt");
	for (int i=1;i<=Node::NodeCount;i++)
	{
		
	ofile << "V("<< i<< ")   "<<Nodes[i].GetVoltage() << std::endl;
	}
	
	
	for (int i=0;i<CircuitElement::TempCounter;i++)
	{

		ofile << "I("<< (Element[i].GetNode1())->GetName()<<","<<(Element[i].GetNode2())->GetName()<< ")   "<<Element[i].GetCurrent()<< std::endl;
	
	}
	
	}

	

	void CreateEmptyMat(complex<float> **Matrix, int N)
{
	for (int i = 0; i < N; i++)
	{
		Matrix[i] = new complex<float>[N];
	}
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			Matrix[i][j] = 0;
		}
	}
}