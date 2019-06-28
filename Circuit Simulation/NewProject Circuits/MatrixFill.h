#pragma once
#include"Input.h"
#include <Eigen\Dense>
using Eigen::MatrixXd;
using Eigen::MatrixXcd;
class MatrixFill
{
	MatrixXcd Q;
	MatrixXcd A;
	MatrixXcd Output;
	int AMatrixSize;
	void CreateMatG(MatrixXcd&,Node**&,CircuitElement**&);
	void CreateMatI(MatrixXcd&,Node**&,CircuitElement**&);
	void CreateMatE(MatrixXcd&,complex<float>**&);
	void CreateMatQ(MatrixXcd&,MatrixXcd&);
	void CreateMatBandC(MatrixXcd&,MatrixXcd&,Node**&,CircuitElement**&,CircuitElement**&);
	void CreateMatA(MatrixXcd&,MatrixXcd&,MatrixXcd&,MatrixXcd&);
	void CreateMat(MatrixXcd&,int,int);
	void CheakDep(Node**&,CircuitElement**&);
	void VCVS(Node**&,CircuitElement**&,int);
	void VCCS(Node**&,CircuitElement**&,int);
	void CCVS(Node**&,CircuitElement**&,int);
	void CCCS(Node**&,CircuitElement**&,int);
	void CopyMatA(MatrixXcd&);
	void CopyMatQ(MatrixXcd&);
public:
	MatrixFill(Node**,CircuitElement**,CircuitElement**,complex<float>**);
	void SetOut(Node**,CircuitElement**);
	~MatrixFill(void);
};

