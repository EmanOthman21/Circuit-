Team Members
Mohamed Alaa 
Mohamed Ahmed Hisham 
Eman Othman   
Bassant Mohamed

The input should be written in "input.txt"
the output will appear after run in "output.txt"

- First write owmega as W  value at the first line

Ex: W 50

- Then write R,L,C as R1 node1 node2 value
Ex: R1 0 1 3
L1 0 1 0.2
C1 0 1 0.002
 
Note:
- In capacitor the value is the capacitance "by Farad".
- In inductor the value the inductance "by Henary".
- In resistance the value resistivity "by ohm". 

- Write the voltage and current sources as
 V1 or I1 node1 node2 value phase
Ex:
V1 0 1 10 30
I1 0 1 10 30

Note :
-In the voltage source the negative side is node1
-In the voltage source the postive side is node2

The dependent Sources is written at form of:
-VCCS: Voltage controlled current source
-VCVS: Voltage controlled voltage source
-CCCS: Current controlled current source
-CCVS: Current controlled voltage source

VCCS node1 node2 coefficient node3 node4
when node3 is the first node of the controlled voltage
node4 is the second node of the controlled  voltage

VCCS 1 2 2 4 5
VCVS 1 2 2 4 5
CCCS 1 2 2 4 5
CCVS 1 2 2 4 5


-The results of the nodes voltage and the current will be printed at form of :
V1 (x,y)
when V1=x+yi 
I(0,1) (x,y)
when I(0,1)=x+yi

-The currents will be printed in the same order we wrote in

Ex :
"Inputs"
R1 0 1 5
R2 2 3 4
"Outputs"
I1(0,1) (x1,y1)
I2(2,3) (x2,y2) 

-The order of R &C&L&V&I dosen't matter.

-You can write ml value e-3
Ex: 1e-3=0.001   "For ml"
Ex: 1e3=1000     "For kilo"



