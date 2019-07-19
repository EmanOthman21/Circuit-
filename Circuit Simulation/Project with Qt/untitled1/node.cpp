#include "Node.h"

Node::Node(void)
{
    frequency = 0;
}
Node::Node(int n)
{
    SetName(n);
}
Node* Node::ArrayOfJoints(vector<Node*>Num)
{
    for (int i = 0; i <= Node::NodeCount; i++)
    {
        if (IsJoint(Num[i]))
            Node::JointNum++;
    }
    Node *jointnum = new Node[Node::JointNum];
    int jointcount = 0;
    for (int i = 0; i <= Node::NodeCount; i++)
    {
        if (IsJoint(Num[i]))
        {
            jointnum[jointcount] = *Num[i];
            jointcount++;
        }
    }
    return jointnum;
}


std::set<int>& Node::GetConnection()
{
    return connection;
}


bool Node::IsJoint(Node *j)
{
    if(j->frequency > 2)
        return true;
    return false;
}

void Node::Connect(int ConnectedNode)
{
    connection.insert(ConnectedNode);
}

void Node::SetVoltage(complex <float> v)
{

    Voltage=v;
}
void Node::SetName(int n)
{
    name = n;
}

int Node::GetName()
{
    return name;
}

complex <float> Node::GetVoltage()
{

    return Voltage;

}

void Node::IncrementFrequency()
{
    frequency++;
}

Node::~Node(void)
{
}
