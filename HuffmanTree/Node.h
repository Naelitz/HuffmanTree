#pragma once
class Node
{
public:
	void AddWeights(int weightA, int weightB);
	bool IsALeaf();
	void IncrementWeight();
	int GetWeight();
	void SetLeftChild(Node* leftChildNode);
	void SetRightChild(Node* rightChildNode);
	Node* GetLeftNode();
	Node* GetRightNode();
	void SetAsciiValue(int intValue);
	int GetAsciiValue();
	Node();
	Node(int startWeight);
	~Node();
private: 
	int weight = 0;
	int intAsciiValue;
	Node* leftChild;
	Node* rightChild;

};

