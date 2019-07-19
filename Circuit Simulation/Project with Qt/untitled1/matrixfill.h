#pragma once
#include"Input.h"
#include <eigen/Eigen/Dense>
using Eigen::MatrixXd;
using Eigen::MatrixXcd;
class MatrixFill
{
    MatrixXcd Q;
    MatrixXcd A;
    MatrixXcd Output;
    int AMatrixSize;
    void CreateMatG(MatrixXcd&, vector<Node*>&, vector<CircuitElement*>&);
    void CreateMatI(MatrixXcd&, vector<Node*>&, vector<CircuitElement*>&);
    void CreateMatE(MatrixXcd&,vector<complex<float>>&);
    void CreateMatQ(MatrixXcd&,MatrixXcd&);
    void CreateMatBandC(MatrixXcd&,MatrixXcd&, vector<Node*>&, vector<CircuitElement*>&, vector<CircuitElement*>&);
    void CreateMatA(MatrixXcd&,MatrixXcd&,MatrixXcd&,MatrixXcd&);
    void CreateMat(MatrixXcd&,int,int);
    void CheakDep(vector<Node*>&,vector<CircuitElement*>&);
    void VCVS(vector<Node*>&, vector<CircuitElement*>&, int);
    void VCCS(vector<Node*>&, vector<CircuitElement*>&, int);
    void CCVS(vector<Node*>&, vector<CircuitElement*>&, int);
    void CCCS(vector<Node*>&, vector<CircuitElement*>&, int);
    void CopyMatA(MatrixXcd&);
    void CopyMatQ(MatrixXcd&);
public:
    MatrixFill(vector<Node*>,vector<CircuitElement*>,vector<CircuitElement*>,vector<complex<float>>);
    void SetOut(vector<Node*>,vector<CircuitElement*>);
    ~MatrixFill(void);
};

