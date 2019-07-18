#include "MatrixFill.h"
MatrixFill::MatrixFill(vector<Node*>Nodes, vector<CircuitElement*>Element, vector<CircuitElement*>VS, vector<complex<float>>VSV)
{
	AMatrixSize = CircuitElement::VoltageCounter + Node::NodeCount;
	MatrixXcd A1(AMatrixSize,AMatrixSize) ;
	MatrixXcd G(Node::NodeCount,Node::NodeCount) ;
	MatrixXcd B(Node::NodeCount,CircuitElement::VoltageCounter) ;
	MatrixXcd C(CircuitElement::VoltageCounter,Node::NodeCount) ;
	MatrixXcd D(CircuitElement::VoltageCounter,CircuitElement::VoltageCounter) ;
	MatrixXcd I(Node::NodeCount,1) ;
	MatrixXcd E(CircuitElement::VoltageCounter,1) ;
	MatrixXcd Q1(CircuitElement::VoltageCounter + Node::NodeCount,1) ;
	CreateMat(G,Node::NodeCount,Node::NodeCount);
	CreateMat(B,Node::NodeCount,CircuitElement::VoltageCounter);
	CreateMat(C,CircuitElement::VoltageCounter,Node::NodeCount);
	CreateMat(D,CircuitElement::VoltageCounter,CircuitElement::VoltageCounter);
	CreateMat(I,Node::NodeCount,1);
	CreateMat(E,CircuitElement::VoltageCounter,1);
	CreateMatG(G,Nodes,Element);
	CreateMatI(I,Nodes,Element);
	CreateMatE(E,VSV);
	Q=Q1;
	CreateMatQ(I,E);
	CreateMatBandC(B,C,Nodes,Element,VS);
	A=A1;
	CreateMatA(G,B,C,D);
	CheakDep(Nodes,Element);
	
}
void MatrixFill::CheakDep(vector<Node*>&Nodes, vector<CircuitElement*>&Element)
{
	string ElementName;
	for (int i = 0; i <  CircuitElement::id; i++)
	{
		ElementName = Element[i]->GetElementName();
		if(Element[i]->IsDepSource())
		{
			 if(toupper(ElementName[0]) =='V' && toupper(ElementName[2]) == 'V')
			 {
				 VCVS(Nodes,Element,i);
			 }
			 else  if(toupper(ElementName[0]) == 'V' && toupper(ElementName[2]) == 'C')
			 {
				 VCCS(Nodes,Element,i);
			 }
			 else  if(toupper(ElementName[0]) == 'C' && toupper(ElementName[2]) == 'V')
			 {
				 CCVS(Nodes,Element,i);

			 }
			 else
			 {
				 CCCS(Nodes,Element,i);
			 }
		}
	}
}
void MatrixFill::VCVS(vector<Node*>&Nodes, vector<CircuitElement*>&Element, int i)
{
	MatrixXcd A2(AMatrixSize+1,AMatrixSize+1);
	CreateMat(A2,AMatrixSize+1,AMatrixSize+1);
	CopyMatA(A2);
	A=A2;
	MatrixXcd Q2(AMatrixSize+1,1);
	CreateMat(Q2,AMatrixSize+1,1);
	CopyMatQ(Q2);
	Q=Q2;
	CircuitElement** Temp;
	int N1,N2,N3,N4,index=0;
	
	N1=Element[i]->GetNode1()->GetName();		
	N2=Element[i]->GetNode2()->GetName();		
	N3=Element[i]->GetNode3()->GetName();		
	N4=Element[i]->GetNode4()->GetName();
	Temp=Element[i]->ElementBetweenNodes(N3,N4,index,Element);

	if(Temp[0]->IsResistance()||Temp[0]->IsCapacitor()||Temp[0]->IsInductor())
	{
		if(N1 == 0)
		{
			if(N4 == 0)
				A(AMatrixSize,N3-1)-=Element[i]->GetFactor()*complex<float>(1,0);
			else if(N3 == 0)
				A(AMatrixSize,N4-1)+=Element[i]->GetFactor()*complex<float>(1,0);
			else	
			{
				A(AMatrixSize,N3-1)-=Element[i]->GetFactor()*complex<float>(1,0);
				A(AMatrixSize,N4-1)+=Element[i]->GetFactor()*complex<float>(1,0);
			}
			A(N2-1,AMatrixSize) = complex<float>(1,0);
			A(AMatrixSize,N2-1)+=complex<float>(1,0);
		}
		else if(N2 == 0)
		{
			if(N4 == 0)
				A(AMatrixSize,N3-1)-=Element[i]->GetFactor()*complex<float>(1,0);
			else if(N3 == 0)
				A(AMatrixSize,N4-1)+=Element[i]->GetFactor()*complex<float>(1,0);
			else	
			{
				A(AMatrixSize,N3-1)-=Element[i]->GetFactor()*complex<float>(1,0);
				A(AMatrixSize,N4-1)+=Element[i]->GetFactor()*complex<float>(1,0);
			}
			A(N1-1,AMatrixSize) = complex<float>(-1,0);
			A(AMatrixSize,N1-1)-=complex<float>(1,0);
		}
		else
		{
			A(AMatrixSize,N4-1)+=Element[i]->GetFactor()*complex<float>(1,0);
			A(AMatrixSize,N3-1)-=Element[i]->GetFactor()*complex<float>(1,0);
			A(AMatrixSize,N1-1)+=complex<float>(1,0);
			A(AMatrixSize,N2-1)+=Element[i]->GetFactor()*complex<float>(1,0);
			A(N1-1,AMatrixSize) = complex<float>(-1,0);
			A(N2-1,AMatrixSize) = complex<float>(1,0);
			A(AMatrixSize,N2-1)+=complex<float>(1,0);
			A(AMatrixSize,N1-1)-=complex<float>(1,0);
		}
	}
	else
	{
		if(N1 == 0)
		{
			if(N4 == 0)
				A(AMatrixSize,N3-1)+=Element[i]->GetFactor()*complex<float>(1,0);
			else if(N3 == 0)
				A(AMatrixSize,N4-1)-=Element[i]->GetFactor()*complex<float>(1,0);
			else	
			{
				A(AMatrixSize,N3-1)+=Element[i]->GetFactor()*complex<float>(1,0);
				A(AMatrixSize,N4-1)-=Element[i]->GetFactor()*complex<float>(1,0);
			}
			A(N2-1,AMatrixSize) = complex<float>(1,0);
			A(AMatrixSize,N2-1)+=complex<float>(1,0);
		}
		else if(N2 == 0)
		{
			if(N4 == 0)
				A(AMatrixSize,N3-1)+=Element[i]->GetFactor()*complex<float>(1,0);
			else if(N3 == 0)
				A(AMatrixSize,N4-1)-=Element[i]->GetFactor()*complex<float>(1,0);
			else	
			{
				A(AMatrixSize,N3-1)+=Element[i]->GetFactor()*complex<float>(1,0);
				A(AMatrixSize,N4-1)-=Element[i]->GetFactor()*complex<float>(1,0);
			}
			A(N1-1,AMatrixSize) = complex<float>(-1,0);
			A(AMatrixSize,N1-1)-=complex<float>(1,0);
		}
		else
		{
			A(AMatrixSize,N4-1)-=Element[i]->GetFactor()*complex<float>(1,0);
			A(AMatrixSize,N3-1)+=Element[i]->GetFactor()*complex<float>(1,0);
			A(AMatrixSize,N1-1)+=complex<float>(1,0);
			A(AMatrixSize,N2-1)-=Element[i]->GetFactor()*complex<float>(1,0);
			A(N1-1,AMatrixSize) = complex<float>(-1,0);
			A(N2-1,AMatrixSize) = complex<float>(1,0);
			A(AMatrixSize,N2-1)+=complex<float>(1,0);
			A(AMatrixSize,N1-1)-=complex<float>(1,0);
		}				
}
	AMatrixSize++;
}
void MatrixFill::VCCS(vector<Node*> &Nodes,vector<CircuitElement*>& Element,int i)
{
	MatrixXcd A2(AMatrixSize+1,AMatrixSize+1);
	CreateMat(A2,AMatrixSize+1,AMatrixSize+1);
	CopyMatA(A2);
	MatrixXcd Q2(AMatrixSize+1,1);
	CreateMat(Q2,AMatrixSize+1,1);
	CopyMatQ(Q2);

	MatrixXcd A3(AMatrixSize+2,AMatrixSize+2);
	CreateMat(A3,AMatrixSize+2,AMatrixSize+2);
	CopyMatA(A3);
	MatrixXcd Q3(AMatrixSize+2,1);
	CreateMat(Q3,AMatrixSize+2,1);
	CopyMatQ(Q3);

	CircuitElement** Temp;
	int N1,N2,N3,N4,index=0;
	bool DoMatA2 =false,DoMatA3 =false;

	N1=Element[i]->GetNode1()->GetName();		
	N2=Element[i]->GetNode2()->GetName();		
	N3=Element[i]->GetNode3()->GetName();		
	N4=Element[i]->GetNode4()->GetName();
	Temp=Element[i]->ElementBetweenNodes(N3,N4,index,Element);

	if(Temp[0]->IsResistance()||Temp[0]->IsCapacitor()||Temp[0]->IsInductor())
	{
		A=A2;
		Q=Q2;
		if(N1 == 0)
		{
			if(N4 == 0)
				A(AMatrixSize,N3-1)-=Element[i]->GetFactor()/Temp[0]->GetValue();
			else if(N3 == 0)
				A(AMatrixSize,N4-1)+=Element[i]->GetFactor()/Temp[0]->GetValue();
			else	
			{
				A(AMatrixSize,N3-1)-=Element[i]->GetFactor()/Temp[0]->GetValue();
				A(AMatrixSize,N4-1)+=Element[i]->GetFactor()/Temp[0]->GetValue();
			}
			A(N2-1,AMatrixSize) = complex<float>(1,0);
			A(AMatrixSize,N2-1)+=complex<float>(1,0);
		}
		else if(N2 == 0)
		{
			if(N4 == 0)
				A(AMatrixSize,N3-1)-=Element[i]->GetFactor()/Temp[0]->GetValue();
			else if(N3 == 0)
				A(AMatrixSize,N4-1)+=Element[i]->GetFactor()/Temp[0]->GetValue();
			else	
			{
				A(AMatrixSize,N3-1)-=Element[i]->GetFactor()/Temp[0]->GetValue();
				A(AMatrixSize,N4-1)+=Element[i]->GetFactor()/Temp[0]->GetValue();
			}
			A(N1-1,AMatrixSize) = complex<float>(-1,0);
			A(AMatrixSize,N1-1)-=complex<float>(1,0);
		}
		else
		{
			A(AMatrixSize,N2-1)+=complex<float>(1,0);
			A(AMatrixSize,N1-1)-=complex<float>(1,0);
			A(AMatrixSize,N4-1)+=Element[i]->GetFactor()/Temp[0]->GetValue();
			A(AMatrixSize,N3-1)-=Element[i]->GetFactor()/Temp[0]->GetValue();
			A(N1-1,AMatrixSize)+= complex<float>(-1,0);
			A(N2-1,AMatrixSize)+= complex<float>(1,0);
		}
		AMatrixSize++;
	}
	else if(Temp[0]->IsCurrentSource())
	{
		A=A2;
		Q=Q2;
		if(N1 == 0)
		{
			A(AMatrixSize,N2-1) +=complex<float>(1,0); 
			A(N2-1,AMatrixSize) +=complex<float>(1,0); 
		}
		else if(N2 == 0)
		{
			A(AMatrixSize,N1-1) +=complex<float>(-1,0);
			A(N1-1,AMatrixSize) +=complex<float>(-1,0);
		}
		else
		{
			A(N2-1,AMatrixSize) +=complex<float>(1,0); 
			A(N1-1,AMatrixSize) +=complex<float>(-1,0);
			A(AMatrixSize,N1-1) +=complex<float>(-1,0);
			A(AMatrixSize,N2-1) +=complex<float>(1,0);
		}
		Q (AMatrixSize,0 )= Temp[0]->GetValue()*Element[i]->GetFactor(); 
		AMatrixSize++;
	}
	else
	{
		A=A3;
		Q=Q3;
		int index = 0;
		for (int i = 0; i < CircuitElement::id+CircuitElement::NumDep; i++)
		{
			if(Element[i]->IsVoltageSource())
			
				index++;
			if(Element[i]->GetNode1()->GetName() == N3 && Element[i]->GetNode2()->GetName() == N4)
				break;
		}
		A(AMatrixSize+1,AMatrixSize+1) = complex<float>(-1,0);
		if(N2 == 0)
		{
			A(N1-1,AMatrixSize) = complex<float>(-1,0);
			A(AMatrixSize,N1-1)-=complex<float>(1,0);	
		}
		else if(N1 == 0)
		{
			A(N2-1,AMatrixSize) = complex<float>(1,0);
			A(AMatrixSize,N2-1)+=complex<float>(1,0);
		}
		else
		{
			A(N2-1,AMatrixSize) = complex<float>(1,0);
			A(AMatrixSize,N2-1)+=complex<float>(1,0);
			A(N1-1,AMatrixSize) = complex<float>(-1,0);
			A(AMatrixSize,N1-1)-=complex<float>(1,0);	
		}
		A(AMatrixSize+1,Node::NodeCount+index-1) = complex<float>(1,0);
		A(AMatrixSize,AMatrixSize+1) =complex<float>(-1,0)*Element[i]->GetFactor();
		AMatrixSize+=2;
	}


}
void MatrixFill::CCVS(vector<Node*>&Nodes, vector<CircuitElement*>& Element,int i)
{
	CircuitElement** Temp;
	int N1,N2,N3,N4,index=0;

	N1=Element[i]->GetNode1()->GetName();		
	N2=Element[i]->GetNode2()->GetName();		
	N3=Element[i]->GetNode3()->GetName();		
	N4=Element[i]->GetNode4()->GetName();
	Temp=Element[i]->ElementBetweenNodes(N3,N4,index,Element);

	if(Temp[0]->IsResistance()||Temp[0]->IsCapacitor()||Temp[0]->IsInductor())
	{
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
	else 
	{
		if(N1 == 0)
		{
			if(N4 == 0)
				A(N2-1,N3-1)+=Element[i]->GetFactor();
			else if(N3 == 0)
				A(N2-1,N4-1)-=Element[i]->GetFactor();
			else	
			{
				A(N2-1,N3-1)+=Element[i]->GetFactor();
				A(N2-1,N4-1)-=Element[i]->GetFactor();
			}
		}
		else if(N2 == 0)
		{
			if(N4 == 0)
				A(N1-1,N3-1)+=Element[i]->GetFactor();
			else if(N3 == 0)
				A(N1-1,N4-1)-=Element[i]->GetFactor();
			else	
			{
				A(N2-1,N3-1)+=Element[i]->GetFactor();
				A(N2-1,N4-1)-=Element[i]->GetFactor();
			}
		}
		else
		{
			A(N2-1,N4-1)-=Element[i]->GetFactor();
			A(N2-1,N3-1)+=Element[i]->GetFactor();
			A(N1-1,N4-1)+=Element[i]->GetFactor();
			A(N1-1,N3-1)-=Element[i]->GetFactor();
		}
 }

}
void MatrixFill::CCCS(vector<Node*>&Nodes, vector<CircuitElement*>& Element,int i)
{
	MatrixXcd A2(AMatrixSize+1,AMatrixSize+1);
	CreateMat(A2,AMatrixSize+1,AMatrixSize+1);
	CopyMatA(A2);
	MatrixXcd Q2(AMatrixSize+1,1);
	CreateMat(Q2,AMatrixSize+1,1);
	CopyMatQ(Q2);
	CircuitElement** Temp;
	int N1,N2,N3,N4,index;

	 N1=Element[i]->GetNode1()->GetName();		
	 N2=Element[i]->GetNode2()->GetName();		
	 N3=Element[i]->GetNode3()->GetName();		
	 N4=Element[i]->GetNode4()->GetName();
	 Temp=Element[i]->ElementBetweenNodes(N3,N4,index,Element);
	 if(Temp[0]->IsResistance()||Temp[0]->IsCapacitor()||Temp[0]->IsInductor())
	 {
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
		 if(N2 == 0)
		 {
			 Q(N1-1,0)-=Temp[0]->GetValue()*Element[i]->GetFactor();
		 }
		 else if(N1 == 0)
		 {
			 Q(N2-1,0)+=Temp[0]->GetValue()*Element[i]->GetFactor();
		 }
		 else
		 {
			 Q(N2-1,0)+=Temp[0]->GetValue()*Element[i]->GetFactor();
			 Q(N1-1,0)-=Temp[0]->GetValue()*Element[i]->GetFactor();
		 }
		}
	 else
	 {
		 A=A2;
		 Q=Q2;
		 index = 0;
		 for (int i = 0; i < CircuitElement::id+CircuitElement::NumDep; i++)
		 {
			 if(Element[i]->IsVoltageSource())
				 index++;
			 if(Element[i]->GetNode1()->GetName() == N3 && Element[i]->GetNode2()->GetName() == N4)
				 break;
		 }
		 A(AMatrixSize,AMatrixSize) = complex<float>(-1,0);
		 if(N2 == 0)
		 {
			 A(N1-1,AMatrixSize) = complex<float>(1,0)*Element[i]->GetFactor();
		 }
		 else if(N1 == 0)
		 {
			 A(N2-1,AMatrixSize) = complex<float>(-1,0)*Element[i]->GetFactor();
		 }
		 else
		 {
			 A(N2-1,AMatrixSize) = complex<float>(-1,0)*Element[i]->GetFactor();
			 A(N1-1,AMatrixSize) = complex<float>(1,0)*Element[i]->GetFactor();
		 }
		 A(AMatrixSize,Node::NodeCount+index-1) = complex<float>(1,0);
	 }


}
void MatrixFill::CopyMatA(MatrixXcd& A2)
{
	for (int i = 0; i < AMatrixSize; i++)
	{
		for (int j = 0; j < AMatrixSize; j++)	
		{
			A2(i,j) = A(i,j);
		}
	}
}
void MatrixFill::CopyMatQ(MatrixXcd& Q2)
{
	for (int i = 0; i < AMatrixSize; i++)
	{
			Q2(i,0) = Q(i,0);
	}
}
void MatrixFill::CreateMatI(MatrixXcd& I, vector<Node*>& Nodes , vector<CircuitElement*>& Element)
{
	//size is to store the number of nodes connected to a certain node
	int size=0,index=0;
	complex<float> IElement=0;
	// to store an array of pointers to element between nodes we needed an array because there may be some elements between two nodes in parallel
	CircuitElement** Temp;
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
		I(i-1,0)=IElement;
		    IElement=0;

	}	
}
void MatrixFill::CreateMatE(MatrixXcd& E, vector<complex<float>>& VSV)
{
	
	for(int i=0;i<CircuitElement::VoltageCounter;i++)
	{
		E(i, 0) = VSV[i];
	}

}
void MatrixFill::CreateMatQ(MatrixXcd& I, MatrixXcd& E )
{
	 for (int C=0;C<Node::NodeCount;C++)
	{
		Q(C,0)=I(C,0);
	}

   int m=0;
   for (int l=Node::NodeCount;l<Node::NodeCount+CircuitElement::VoltageCounter ;l++)
	{ 
   	Q(l,0)=E(m,0);
      m++;	
	}
}
void MatrixFill::CreateMatA(MatrixXcd &G, MatrixXcd &B, MatrixXcd &C, MatrixXcd &D)
{
	
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
}
void MatrixFill::CreateMat(MatrixXcd & M , int row , int col)
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			M(i, j) = 0;
		}	
	}
}
void MatrixFill::CreateMatG(MatrixXcd& G, vector<Node*>&Nodes , vector<CircuitElement*>&Element)
{
	// some variable that will be used to store the number of elements in parallel between any two nodes by sending it by reference to the function (ElementsBetweenNodes) 
	int index=0;
	// will be used in various positions but its main purpose to fill the G Matrix
	complex<float> GElement=0;
	complex<float> IElement=0;
	//size is to store the number of nodes connected to a certain node
	int size=0,c=0;
	// to store an array of pointers to element between nodes we needed an array because there may be some elements between two nodes in parallel
	CircuitElement** Temp;
	//loop to iterate through every row in the G matrix i represents the node name so we started from 1 not zero as the MNA method excludes the ground
	for (int i = 1; i < Node::NodeCount+1; i++)
	{
		size = Nodes[i]->GetConnection().size();
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
}
void MatrixFill::CreateMatBandC(MatrixXcd &B, MatrixXcd &C, vector<Node*>& Nodes , vector<CircuitElement*>& Element, vector<CircuitElement*>& VS)
{
	int index=0;
	int size=0,c=0;
	CircuitElement** Temp;
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

		for (int i = 0; i < CircuitElement::VoltageCounter; i++)
	{
		for (int j = 0; j < Node::NodeCount; j++)
		{
			C(i,j) = B (j,i);
		}

	}

}
void MatrixFill::SetOut(vector<Node*> Nodes,vector<CircuitElement*> Element)
{
	MatrixXcd A1;
	MatrixXcd Q1;
	A1=A;
	Q1=Q;
	Output=A1.inverse()*Q1;
	int F = CircuitElement::VoltageCounter + Node::NodeCount;
	for (int i=0; i<CircuitElement::id; i++)
	{
		if(Element[i]->IsDepSource()&&(Element[i]->GetElementName()[0] == 'V' || Element[i]->GetElementName()[0] == 'v'))
		{
			Element[i]->SetCurrent(-Output(F,0));
			F++;
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
		{
			Element[i]->SetCurrent((Output(o,0))*complex <double>(-1.0));
			o++;
		}
	}

	for (int i=0;i<CircuitElement::TempCounter;i++)
	{
		if(Element[i]->IsResistance()||Element[i]->IsCapacitor()||Element[i]->IsInductor())
		{
			Element[i]->SetCurrent((((Element[i]->GetNode1())->GetVoltage())-((Element[i]->GetNode2())->GetVoltage()))/Element[i]->GetValue());
		}
	}

	CircuitElement** Temp;
	string ElementName;
	int N1,N2,N3,N4;	
	int flag =CircuitElement::NumDep;
	int i =0,index=0;
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
					Element[i]->SetCurrent(((Nodes[N3]->GetVoltage()-Nodes[N4]->GetVoltage())/Temp[0]->GetValue())*Element[i]->GetFactor());
					Element[i]->SetVoltage(Nodes[Element[i]->GetNode1()->GetName()]-Nodes[Element[i]->GetNode2()->GetName()]);
				}
				else if(Temp[0]->IsCurrentSource())
				{
					Element[i]->SetCurrent(Element[i]->GetFactor()*Temp[0]->GetValue());
					Element[i]->SetVoltage(Nodes[Element[i]->GetNode1()->GetName()]-Nodes[Element[i]->GetNode2()->GetName()]);
				}
				else if(Temp[0]->IsVoltageSource())
				{
				Element[i]->SetCurrent(Element[i]->GetFactor()*Temp[0]->GetCurrent());
				Element[i]->SetVoltage(Temp[0]->GetValue());
				}
				flag--;
			}
			if((ElementName[0]=='C'||ElementName[0]=='C')&&(ElementName[2]=='V'||ElementName[2]=='V'))
			{
				N1=Element[i]->GetNode1()->GetName();
				N2=Element[i]->GetNode2()->GetName();
				N3=Element[i]->GetNode3()->GetName();
				N4=Element[i]->GetNode4()->GetName();
				Temp=Element[i]->ElementBetweenNodes(N3,N4,index,Element);
				if(Temp[0]->IsResistance()||Temp[0]->IsCapacitor()||Temp[0]->IsInductor())
				{
					Element[i]->SetCurrent((Nodes[N3]->GetVoltage()-Nodes[N4]->GetVoltage())*Element[i]->GetFactor());
					Element[i]->SetVoltage((Nodes[N2]->GetVoltage()-Nodes[N1]->GetVoltage()));
				}
				else
				{
					Element[i]->SetCurrent((Nodes[N4]->GetVoltage()-Nodes[N3]->GetVoltage())*Element[i]->GetFactor());
					Element[i]->SetVoltage((Nodes[N2]->GetVoltage()-Nodes[N1]->GetVoltage()));
				}
				flag--;
			}

			}
			i++;
	}
	



}
MatrixFill::~MatrixFill(void)
{
}
