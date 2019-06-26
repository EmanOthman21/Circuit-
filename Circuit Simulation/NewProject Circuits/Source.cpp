#include"Input.h"
#include <Eigen\Dense>
#include <complex>
#include <cmath>
using Eigen::MatrixXd;
using Eigen::MatrixXcd;


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
void CreateEmptyMat(complex<float>**Matrix, int N);


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
	int vsindex =1;
	int csindex =1;
		
	// constructing maxelement of elements made id = maxelement that's why I reinitialize it
	CircuitElement::TempCounter = 0;
	CircuitElement::NumDep = 0;
	
	Input In("Input.txt",Nodes,Element,CircuitElement::TempCounter,VS,CS,VSV);

	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																													  //
	//					           declaring some variables that will be needed later									  //
	//																													  //
	//																													  //
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//the last node of the nodes entered is the number stored in nodecount but we count from zero 
	Node::NodeCount; 
	//array to store the names of the joints.. I had to call it using some node if anyone has an idea to call it directly feel free to edit 
	Joints = Nodes[0]->ArrayOfJoints(Nodes);
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
	// creating the matrix dynamically and initializing every element with zero
	MatrixXcd G(Node::NodeCount,Node::NodeCount);
	for (int i = 0; i < Node::NodeCount; i++)
	{
		for (int j = 0; j < Node::NodeCount; j++)
		{
		G(i,j)=0;

		}
	}
	//size is to store the number of nodes connected to a certain node
	int size=0,c=0;
	// to store an array of pointers to element between nodes we needed an array because there may be some elements between two nodes in parallel
	CircuitElement**Temp;

	//loop to iterate through every row in the G matrix i represents the node name so we started from 1 not zero as the MNA method excludes the ground
	for (int i = 1; i < Node::NodeCount+1; i++)
	{
		size=Nodes[i]->GetConnection().size();
		//loops over every node connected to the node in the current row
		for (std::set<int>::iterator it = Nodes[i]->GetConnection().begin(); it != Nodes[i]->GetConnection().end(); ++it)
		{
			Temp=Element[i]->ElementBetweenNodes(i,*it,index,Element);
			//to get the total G (conductance) connected to each node
			for (int k = 0; k < index; k++)
			{
				if(Temp[k]->IsResistance()||Temp[k]->IsCapacitor()||Temp[k]->IsInductor())
				GElement+=complex<float>(1,0)/Temp[k]->GetValue();
				
			}
		}
		// to put the diagonal elements only
		G(i-1,i-1)=GElement;
		GElement=0;

		//to fill the rest of the elements in matrix G 
		for (std::set<int>::iterator it = Nodes[i]->GetConnection().begin(); it != Nodes[i]->GetConnection().end(); ++it)
		{
			// to execlude node zero
			if(*it==0)
				continue;
			Temp=Element[i]->ElementBetweenNodes(i,*it,index,Element);    //
			// iterate over each element in parallel between the two nodes entered in the previous line
			for (int k = 0; k < index; k++)
			{
				if(Temp[k]->IsResistance()||Temp[k]->IsCapacitor()||Temp[k]->IsInductor())
				GElement+=complex <float>(1,0)/Temp[k]->GetValue();
			}

			G(i-1,*it-1) = -GElement;
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

	MatrixXcd B(Node::NodeCount,CircuitElement::VoltageCounter);
	for (int i = 0; i < Node::NodeCount; i++)
	{
		for (int j = 0; j < CircuitElement::VoltageCounter; j++)
		{
			B(i,j) = 0;
		}
	}
	// to iterate through everynode execuling the ground
	for (int i = 1; i < Node::NodeCount+1; i++)
	{
		size=Nodes[i]->GetConnection().size();
		// to iterate over every node connected to node number i
		for (std::set<int>::iterator it = Nodes[i]->GetConnection().begin(); it != Nodes[i]->GetConnection().end(); ++it)
		{
			Temp=Element[i]->ElementBetweenNodes(i,*it,index,Element);
			// to iterate through every element in parallel between two certain nodes
			for(int x=0;x<index;x++)
			{
				if(Temp[x]->IsVoltageSource())
				{
					// starting to fill every column in the current row (ie the row of the node number i)
					for (int k = 0; k < CircuitElement::VoltageCounter; k++)
					{
						if(VS[k+1]->GetNode1()==Temp[x]->GetNode1()&&VS[k+1]->GetNode2()==Temp[x]->GetNode2())
							if(VS[k+1]->GetNode1()->GetName()==Nodes[i]->GetName())
								B(i-1,k)=-1;
							else
								B(i-1,k)=1;
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
	MatrixXcd C (CircuitElement::VoltageCounter, Node::NodeCount) ;
	// temporarly the matrix C is the transpose of the matrix B untill we see how to add the dependent sources
	for (int i = 0; i < CircuitElement::VoltageCounter; i++)
	{
		for (int j = 0; j < Node::NodeCount; j++)
		{
			C(i,j) = B (j,i);
		}

	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																													  //
	//					               create and fill in the D matirx					        						  //
	//																													  //
	//																													  //
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// this matrix is part of the big matrix it's temporarly zero matrix but it needs to be modified when we add dependent sources
	MatrixXcd D(CircuitElement::VoltageCounter,CircuitElement::VoltageCounter);
	for (int i = 0; i < CircuitElement::VoltageCounter; i++)
	{
		for (int j = 0; j < CircuitElement::VoltageCounter; j++)
		{
			D(i,j) = 0;
		}
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																													  //
	//					               create and fill in the main matirx A					        					  //
	//																													  //
	//																													  //
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int AMatrixSize = CircuitElement::VoltageCounter + Node::NodeCount;
	MatrixXcd A(AMatrixSize,AMatrixSize);
	
	// this for loop to fill G and B into the matrix (first nodecount rows)
	for (int i = 0; i < Node::NodeCount; i++)
	{
		int j = 0;
		for (j; j < Node::NodeCount; j++)
		{
			A(i,j) = G(i,j);
		}
		for (j ; j < AMatrixSize; j++)
		{
			A(i,j) = B(i,j - Node::NodeCount);
		}
	}
	
	// this for loop to fill C and D matrices into the matrix (remaining rows) 
	for (int i = Node::NodeCount; i < AMatrixSize; i++)
	{
		int j = 0;
		for (j; j < Node::NodeCount; j++)
		{
			A(i,j) = C(i - Node::NodeCount,j);
		}
		for (j ; j < AMatrixSize; j++)
		{
			A(i,j) = D(i - Node::NodeCount,j - Node::NodeCount);
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
		size=Nodes[i]->GetConnection().size();
		//loops over every node connected to the node in the current row
		for (std::set<int>::iterator it = Nodes[i]->GetConnection().begin(); it != Nodes[i]->GetConnection().end(); ++it)
		{
			Temp=Element[i]->ElementBetweenNodes(i,*it,index,Element);
			
			for (int k = 0; k < index; k++)
			{
				if(Temp[k]->IsCurrentSource())
				{
					if (Nodes[i]->GetName()==(Temp[k]->GetNode2())->GetName())
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

	for(int i=0;i<EMatrixSize;i++)
	{
	E[i]=*VSV[i+1];
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																													  //
	//					               create and fill in the Q matirx					        						  //
	//																													  //
	//																													  //
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
     int ZMatrixSize=vsindex+Node::NodeCount;
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

	int XMatrixSize=vsindex+Node::NodeCount-1;
	MatrixXcd Output(XMatrixSize,1);
	MatrixXcd A2 (AMatrixSize+1, AMatrixSize+1);
	MatrixXcd A3 (AMatrixSize+2, AMatrixSize+2);
	MatrixXcd Q3 (AMatrixSize+2, 1);
	bool DOA1 = false;
	bool DOA2 = false;
	bool DOA3 = false;
	for (int i = 0; i < XMatrixSize; i++)
	{
		Output(i,0)=0;
	}
	string ElementName;
	int N1,N2,N3,N4;
	for (int i = 0; i < CircuitElement::NumDep+ CircuitElement::id; i++)
	{
		if(Element[i]->IsDepSource())
		{
			ElementName = Element[i]->GetElementName();
			if((ElementName[0]=='v'||ElementName[0]=='V')&&(ElementName[2]=='C'||ElementName[2]=='c'))
			{
		 for (int i = 0; i < AMatrixSize; i++)
		 {
			 for (int j = 0; j < AMatrixSize; j++)
			 {
				 A2(i,j) = A(i,j);
			 }

		 }
		 for (int i = 0; i < AMatrixSize+1; i++)
		 {
			 A2(AMatrixSize,i) = 0;
			 A2(i,AMatrixSize) = 0;
		 }

				N1=Element[i]->GetNode1()->GetName();		
				N2=Element[i]->GetNode2()->GetName();		
				N3=Element[i]->GetNode3()->GetName();		
				N4=Element[i]->GetNode4()->GetName();
				Temp=Element[i]->ElementBetweenNodes(N3,N4,index,Element);
				if(Temp[0]->IsResistance()||Temp[0]->IsCapacitor()||Temp[0]->IsInductor())
				{
					DOA2=true;
							if(N1 == 0)
					{
						if(N4 == 0)
							A2(AMatrixSize,N3-1)+=Element[i]->GetFactor()/Temp[0]->GetValue();
						else if(N3 == 0)
						A2(AMatrixSize,N4-1)-=Element[i]->GetFactor()/Temp[0]->GetValue();
						else	
						{
							A2(AMatrixSize,N3-1)+=Element[i]->GetFactor()/Temp[0]->GetValue();
							A2(AMatrixSize,N4-1)-=Element[i]->GetFactor()/Temp[0]->GetValue();
						}
						A2(N2-1,AMatrixSize) = complex<float>(1,0);
						A2(AMatrixSize,N2-1)+=complex<float>(1,0);
						
					}
					else if(N2 == 0)
					{
						if(N4 == 0)
						A2(AMatrixSize,N3-1)+=Element[i]->GetFactor()/Temp[0]->GetValue();
						else if(N3 == 0)
						A2(AMatrixSize,N4-1)-=Element[i]->GetFactor()/Temp[0]->GetValue();
						else	
						{
							A2(AMatrixSize,N3-1)+=Element[i]->GetFactor()/Temp[0]->GetValue();
							A2(AMatrixSize,N4-1)-=Element[i]->GetFactor()/Temp[0]->GetValue();
						}
						A2(N1-1,AMatrixSize) = complex<float>(-1,0);
						A2(AMatrixSize,N1-1)-=complex<float>(1,0);
					}
			
					else
					{

						A2(AMatrixSize,N4-1)-=Element[i]->GetFactor()/Temp[0]->GetValue();
						A2(AMatrixSize,N3-1)+=Element[i]->GetFactor()/Temp[0]->GetValue();
						A2(AMatrixSize,N1-1)+=complex<float>(1,0);
						A2(AMatrixSize,N2-1)-=Element[i]->GetFactor()/Temp[0]->GetValue();
						A2(N1-1,AMatrixSize) = complex<float>(-1,0);
						A2(N2-1,AMatrixSize) = complex<float>(1,0);
						A2(AMatrixSize,N2-1)+=complex<float>(1,0);
						A2(AMatrixSize,N1-1)-=complex<float>(1,0);
						
					}
			}
				else if(Temp[0]->IsCurrentSource())
				{
					if(N2 == 0)
					{
					Q(N1-1,0)-=Temp[0]->GetValue();
					}
					else if(N1 == 0)
					{
					Q(N2-1,0)+=Temp[0]->GetValue();
					}
					else
					{
					Q(N2-1,0)+=Temp[0]->GetValue();
					Q(N1-1,0)-=Temp[0]->GetValue();
					}
				DOA1 = true;

				}
				else
				{
					DOA3= true;
					 int index = 0;
					for (int i = 0; i < CircuitElement::id+CircuitElement::NumDep; i++)
					{
						if(Element[i]->IsVoltageSource())
							index++;
						if(Element[i]->GetNode1()->GetName() == N3 && Element[i]->GetNode2()->GetName() == N4)
							break;
					
					}

					for (int i = 0; i < AMatrixSize; i++)
					{
						for (int j = 0; j < AMatrixSize; j++)
						{
							A3(i,j) = A(i,j);
						}
					 
					}
		 
					for (int i = 0; i < AMatrixSize+2; i++)
					{
						Q3(i,0) = 0;
						A3(AMatrixSize,i) = 0;
						A3(AMatrixSize+1,i) = 0;
						A3(i,AMatrixSize+1) = 0;
						A3(i,AMatrixSize) = 0;
					}
					for (int i = 0; i < AMatrixSize; i++)
					{
						Q3(i,0) = Q(i,0);
					}

					A3(AMatrixSize+1,AMatrixSize+1) = complex<float>(-1,0);
					if(N2 == 0)
					{
					A3(N1-1,AMatrixSize) = complex<float>(-1,0);
					A3(AMatrixSize,N1-1)-=complex<float>(1,0);	
					}
					else if(N1 == 0)
					{
					A3(N2-1,AMatrixSize) = complex<float>(1,0);
					A3(AMatrixSize,N2-1)+=complex<float>(1,0);
					}
					else
					{
					A3(N2-1,AMatrixSize) = complex<float>(1,0);
					A3(AMatrixSize,N2-1)+=complex<float>(1,0);
					A3(N1-1,AMatrixSize) = complex<float>(-1,0);
					A3(AMatrixSize,N1-1)-=complex<float>(1,0);	
					}
					A3(AMatrixSize+1,Node::NodeCount+index-1) = complex<float>(1,0);
					A3(AMatrixSize,AMatrixSize+1) =complex<float>(-1,0)*Element[i]->GetFactor();

				}
			}
			

	 if((ElementName[0]=='v'||ElementName[0]=='V')&&(ElementName[2]=='V'||ElementName[2]=='v'))
	 {
		 DOA2 = true;
		 for (int i = 0; i < AMatrixSize; i++)
		 {
			 for (int j = 0; j < AMatrixSize; j++)
			 {
				 A2(i,j) = A(i,j);
			 }

		 }
		 for (int i = 0; i < AMatrixSize+1; i++)
		 {
			 A2(AMatrixSize,i) = 0;
			 A2(i,AMatrixSize) = 0;
		 }
				 
		 N1=Element[i]->GetNode1()->GetName();		
		 N2=Element[i]->GetNode2()->GetName();		
		 N3=Element[i]->GetNode3()->GetName();		
		 N4=Element[i]->GetNode4()->GetName();
					if(N1 == 0)
					{
						if(N4 == 0)
							A2(AMatrixSize,N3-1)+=Element[i]->GetFactor()*complex<float>(1,0);
						else if(N3 == 0)
						A2(AMatrixSize,N4-1)-=Element[i]->GetFactor()*complex<float>(1,0);
						else	
						{
							A2(AMatrixSize,N3-1)+=Element[i]->GetFactor()*complex<float>(1,0);
							A2(AMatrixSize,N4-1)-=Element[i]->GetFactor()*complex<float>(1,0);
						}
						A2(N2-1,AMatrixSize) = complex<float>(1,0);
						A2(AMatrixSize,N2-1)+=complex<float>(1,0);
						
					}
					else if(N2 == 0)
					{

						if(N4 == 0)
						A2(AMatrixSize,N3-1)+=Element[i]->GetFactor()*complex<float>(1,0);
						else if(N3 == 0)
						A2(AMatrixSize,N4-1)-=Element[i]->GetFactor()*complex<float>(1,0);
						else	
						{
							A2(AMatrixSize,N3-1)+=Element[i]->GetFactor()*complex<float>(1,0);
							A2(AMatrixSize,N4-1)-=Element[i]->GetFactor()*complex<float>(1,0);
						}
						A2(N1-1,AMatrixSize) = complex<float>(-1,0);
						A2(AMatrixSize,N1-1)-=complex<float>(1,0);
					}
			
					else
					{

						A2(AMatrixSize,N4-1)-=Element[i]->GetFactor()*complex<float>(1,0);
						A2(AMatrixSize,N3-1)+=Element[i]->GetFactor()*complex<float>(1,0);
						A2(AMatrixSize,N1-1)+=complex<float>(1,0);
						A2(AMatrixSize,N2-1)-=Element[i]->GetFactor()*complex<float>(1,0);
						A2(N1-1,AMatrixSize) = complex<float>(-1,0);
						A2(N2-1,AMatrixSize) = complex<float>(1,0);
						A2(AMatrixSize,N2-1)+=complex<float>(1,0);
						A2(AMatrixSize,N1-1)-=complex<float>(1,0);
						
					}
				
		

	 }

	 if((ElementName[0]=='C'||ElementName[0]=='C')&&(ElementName[2]=='C'||ElementName[2]=='c'))
	 {
		 
		 N1=Element[i]->GetNode1()->GetName();		
		 N2=Element[i]->GetNode2()->GetName();		
		 N3=Element[i]->GetNode3()->GetName();		
		 N4=Element[i]->GetNode4()->GetName();
				Temp=Element[i]->ElementBetweenNodes(N3,N4,index,Element);
				if(Temp[0]->IsResistance()||Temp[0]->IsCapacitor()||Temp[0]->IsInductor())
				{
					DOA1= true;
					if(N1 == 0)
					{
						if(N4 == 0)
						A(N2-1,N3-1)-=Element[i]->GetFactor()*complex<float>(1,0)/Temp[0]->GetValue();
						else if(N3 == 0)
						A(N2-1,N4-1)+=Element[i]->GetFactor()*complex<float>(1,0)/Temp[0]->GetValue();
						else	
						{
							A(N2-1,N3-1)-=Element[i]->GetFactor()*complex<float>(1,0)/Temp[0]->GetValue();
							A(N2-1,N4-1)+=Element[i]->GetFactor()*complex<float>(1,0)/Temp[0]->GetValue();
						}
						
					}

					else if(N2 == 0)
					{

						if(N4 == 0)
						A(N1-1,N3-1)-=Element[i]->GetFactor()*complex<float>(1,0)/Temp[0]->GetValue();
						else if(N3 == 0)
						A(N1-1,N4-1)+=Element[i]->GetFactor()*complex<float>(1,0)/Temp[0]->GetValue();
						else	
						{
							A(N2-1,N3-1)-=Element[i]->GetFactor()*complex<float>(1,0)/Temp[0]->GetValue();
							A(N2-1,N4-1)+=Element[i]->GetFactor()*complex<float>(1,0)/Temp[0]->GetValue();
						}
					}
			
					else
					{

						A(N2-1,N4-1)+=Element[i]->GetFactor()*complex<float>(1,0)/Temp[0]->GetValue();
						A(N2-1,N3-1)-=Element[i]->GetFactor()*complex<float>(1,0)/Temp[0]->GetValue();
						A(N1-1,N4-1)-=Element[i]->GetFactor()*complex<float>(1,0)/Temp[0]->GetValue();
						A(N1-1,N3-1)+=Element[i]->GetFactor()*complex<float>(1,0)/Temp[0]->GetValue();
					}
				}
				else if(Temp[0]->IsCurrentSource())
				{
					DOA1 =true;
					if(N2 == 0)
					{
					Q(N1-1,0)-=Temp[0]->GetValue();
					}
					else if(N1 == 0)
					{
					Q(N2-1,0)+=Temp[0]->GetValue();
					}
					else
					{
					Q(N2-1,0)+=Temp[0]->GetValue();
					Q(N1-1,0)-=Temp[0]->GetValue();
					}
				}
				else
				{
					 int index = 0;
					for (int i = 0; i < CircuitElement::id+CircuitElement::NumDep; i++)
					{
						if(Element[i]->IsVoltageSource())
							index++;
						if(Element[i]->GetNode1()->GetName() == N3 && Element[i]->GetNode2()->GetName() == N4)
							break;
						
					}

					for (int i = 0; i < AMatrixSize; i++)
					{
						for (int j = 0; j < AMatrixSize; j++)
						{
							A2(i,j) = A(i,j);
						}
					 
					}
		 
					for (int i = 0; i < AMatrixSize+1; i++)
					{
						A2(AMatrixSize,i) = 0;
						A2(i,AMatrixSize) = 0;
					}

					A2(AMatrixSize,AMatrixSize) = complex<float>(-1,0);
					if(N2 == 0)
					{
					A2(N1-1,AMatrixSize) = complex<float>(1,0)*Element[i]->GetFactor();
					}
					else if(N1 == 0)
					{
					A2(N2-1,AMatrixSize) = complex<float>(-1,0)*Element[i]->GetFactor();
					}
					else
					{
					A2(N2-1,AMatrixSize) = complex<float>(-1,0)*Element[i]->GetFactor();
					A2(N1-1,AMatrixSize) = complex<float>(1,0)*Element[i]->GetFactor();
					}
					A2(AMatrixSize,Node::NodeCount+index-1) = complex<float>(1,0);
					DOA2= true;
				}


			
	 }

	if((ElementName[0]=='C'||ElementName[0]=='C')&&(ElementName[2]=='V'||ElementName[2]=='V'))
	{
		DOA1 =true;
		N1=Element[i]->GetNode1()->GetName();		
		 N2=Element[i]->GetNode2()->GetName();		
		 N3=Element[i]->GetNode3()->GetName();		
		 N4=Element[i]->GetNode4()->GetName();
					if(N1 == 0)
					{
						if(N4 == 0)
						A(N2-1,N3-1)-=Element[i]->GetFactor();
						else if(N3 == 0)
						A(N2-1,N4-1)+=Element[i]->GetFactor();
						else	
						{
							A(N2-1,N3-1)-=Element[i]->GetFactor();
							A(N2-1,N4-1)+=Element[i]->GetFactor();
						}
						
					}

					else if(N2 == 0)
					{

						if(N4 == 0)
						A(N1-1,N3-1)-=Element[i]->GetFactor();
						else if(N3 == 0)
						A(N1-1,N4-1)+=Element[i]->GetFactor();
						else	
						{
							A(N2-1,N3-1)-=Element[i]->GetFactor();
							A(N2-1,N4-1)+=Element[i]->GetFactor();
						}
					
					}
			
					else
					{

						A(N2-1,N4-1)+=Element[i]->GetFactor();
						A(N2-1,N3-1)-=Element[i]->GetFactor();
						A(N1-1,N4-1)-=Element[i]->GetFactor();
						A(N1-1,N3-1)+=Element[i]->GetFactor();
					}
			


	}	

		}
	}

	if(DOA2)
	{
	MatrixXcd Q2 (EMatrixSize + iMatrixSize+1, 1); 
	for (int i = 0; i <EMatrixSize + iMatrixSize  ; i++)
	{
		Q2(i,0)=Q(i,0);
	} 
	Q2(EMatrixSize + iMatrixSize,0) = complex<float> (0,0);
	Output=A2.inverse()*Q2;
	int F = CircuitElement::VoltageCounter + Node::NodeCount;
	for (int i=0;i<=CircuitElement::id+CircuitElement::NumDep;i++)
	{
		if(Element[i]->IsDepSource()&&(Element[i]->GetElementName()[0] == 'V' || Element[i]->GetElementName()[0] == 'v'))
		{
			Element[i]->SetCurrent(-Output(F,0));
			F++;
		}

	}
	}
	else if(DOA1)
		Output=A.inverse()*Q;
	else if(DOA3)
	{
		Output = A3.inverse() * Q3;
		int F = CircuitElement::VoltageCounter + Node::NodeCount;
	for (int i=0;i<=CircuitElement::id+CircuitElement::NumDep;i++)
	{
		if(Element[i]->IsDepSource()&&(Element[i]->GetElementName()[0] == 'V' || Element[i]->GetElementName()[0] == 'v'))
		{
			Element[i]->SetCurrent(-Output(F,0));
			F++;
		}
	
	}
	}

for (int i=1;i<=Node::NodeCount;i++)
	{
	Nodes[i]->SetVoltage(Output(i-1,0));
	}
	Nodes[0]->SetVoltage(complex <float>(0,0));
	
	int o=Node::NodeCount;

	for (int i=0;i<CircuitElement::TempCounter;i++)
	{
		
			if(Element[i]->IsVoltageSource())
				{Element[i]->SetCurrent((Output(o,0))*complex <double>(-1.0));
			o++;}
	}

	

	for (int i=0;i<CircuitElement::TempCounter;i++)
	{
		if(Element[i]->IsResistance()||Element[i]->IsCapacitor()||Element[i]->IsInductor())
		{
		
			Element[i]->SetCurrent((((Element[i]->GetNode1())->GetVoltage())-((Element[i]->GetNode2())->GetVoltage()))/Element[i]->GetValue());
		
		}
		
	}


	int flag =CircuitElement::NumDep;
	int i =0;
	while(flag)
	{
			if(Element[i]->IsDepSource())
		{
			ElementName = Element[i]->GetElementName();
			if((ElementName[0]=='v'||ElementName[0]=='V')&&(ElementName[2]=='C'||ElementName[2]=='c'))
			 {
				 Element[i]->SetVoltage(Nodes[Element[i]->GetNode1()->GetName()]-Nodes[Element[i]->GetNode2()->GetName()]);
				 flag--;

			 }
			
			 if((ElementName[0]=='v'||ElementName[0]=='V')&&(ElementName[2]=='V'||ElementName[2]=='v'))
			 {
				Element[i]->SetVoltage(Nodes[Element[i]->GetNode1()->GetName()]-Nodes[Element[i]->GetNode2()->GetName()]);
				 flag--;

			 }

			 if((ElementName[0]=='C'||ElementName[0]=='C')&&(ElementName[2]=='C'||ElementName[2]=='c'))
			 {
				 N3=Element[i]->GetNode3()->GetName();
				 N4=Element[i]->GetNode4()->GetName();
				Temp=Element[i]->ElementBetweenNodes(N3,N4,index,Element);
				if(Temp[0]->IsResistance()||Temp[0]->IsCapacitor()||Temp[0]->IsInductor())
				{
				Element[i]->SetCurrent((Nodes[N3]->GetVoltage()-Nodes[N4]->GetVoltage())/Temp[0]->GetValue());
				Element[i]->SetVoltage(Nodes[Element[i]->GetNode1()->GetName()]-Nodes[Element[i]->GetNode2()->GetName()]);
				}
				else if(Temp[0]->IsCurrentSource())
				{
					Element[i]->SetCurrent(Element[i]->GetFactor()*Temp[0]->GetValue());
					Element[i]->SetVoltage(Nodes[Element[i]->GetNode1()->GetName()]-Nodes[Element[i]->GetNode2()->GetName()]);
				}
				else if(Temp[0]->IsVoltageSource())
				{

				Element[i]->SetCurrent(Temp[0]->GetCurrent());
				Element[i]->SetVoltage(Temp[0]->GetValue());
				
				}



				flag--;
			 }

			if((ElementName[0]=='C'||ElementName[0]=='C')&&(ElementName[2]=='V'||ElementName[2]=='V'))
			{

				 N3=Element[i]->GetNode3()->GetName();
				 N4=Element[i]->GetNode4()->GetName();
				Temp=Element[i]->ElementBetweenNodes(N3,N4,index,Element);
				if(Temp[0]->IsResistance()||Temp[0]->IsCapacitor()||Temp[0]->IsInductor())
				{
				Element[i]->SetCurrent(Nodes[N3]->GetVoltage()-Nodes[N4]->GetVoltage());
				Element[i]->SetVoltage(Nodes[Element[i]->GetNode1()->GetName()]-Nodes[Element[i]->GetNode2()->GetName()]);
				}
				else if(Temp[0]->IsCurrentSource())
				{
					complex <float> t = (Nodes[Element[i]->GetNode3()->GetName()]-Nodes[Element[i]->GetNode4()->GetName()]);
					Element[i]->SetCurrent(t*Element[i]->GetFactor());
					Element[i]->SetVoltage(Nodes[Element[i]->GetNode1()->GetName()]-Nodes[Element[i]->GetNode2()->GetName()]);
				}
				else if(Temp[0]->IsVoltageSource())
				{

				Element[i]->SetCurrent(Temp[0]->GetValue()*Element[i]->GetFactor());
				Element[i]->SetVoltage(Nodes[Element[i]->GetNode1()->GetName()]-Nodes[Element[i]->GetNode2()->GetName()]);
				
				}

				flag--;

			}

			}
			i++;
	}
    
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

	
