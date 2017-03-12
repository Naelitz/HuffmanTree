#include "stdafx.h"
#include "Structure.h"
#include "Node.h"

using namespace std;

Node::Node(int startWeight)
{
	weight = startWeight;
}

Node::Node()
{
	
}

bool Node::IsALeaf()
{
	if(leftChild == nullptr && rightChild == nullptr)
	{
		return true;
	}
	else return false;
}


void Node::IncrementWeight()
{
	weight++;
}

int Node::GetWeight()
{
	return weight;
}

void Node::SetLeftChild(Node* leftChildNode)
{
	leftChild = leftChildNode;
}

void Node::SetRightChild(Node* rightChildNode)
{
	rightChild = rightChildNode;
}

Node* Node::GetLeftNode()
{
	return leftChild;
}

Node* Node::GetRightNode()
{
	return rightChild;
}


int Node::GetAsciiValue()
{
	return intAsciiValue;
}

void Node::SetAsciiValue(int intValue)
{
	intAsciiValue = intValue;
}


Node::~Node()
{
}
