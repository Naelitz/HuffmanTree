#include "stdafx.h"
#include "Structure.h"
#include "Node.h"
#include <string>
#include <fstream>

using namespace std;

Structure::Structure()
{
	
}

Node* Structure::countsToTree(int* counts)
{
	//todo:Given the frequency counts in counts, builds(and returns a pointer to the root of) the Huffman tree, and produces a 510-byte
	// string containing the tree-building sequences
	for(int intNodeCounter = 0; intNodeCounter < 256; intNodeCounter++)
	{
		weightNodes[intNodeCounter] = new Node(counts[intNodeCounter]);
		weightNodes[intNodeCounter]->SetAsciiValue(intNodeCounter);
	}
	do
	{
		LowestWeightA = nullptr;
		LowestWeightB = nullptr;
		LowestWeightAIndex = -1;
		LowestWeightBIndex = -1;
		for(int intCounterA = 0; intCounterA < 256; intCounterA++)
		{
			if(LowestWeightA == nullptr && weightNodes[intCounterA] != nullptr)
			{
				LowestWeightA = weightNodes[intCounterA];
				LowestWeightAIndex = intCounterA;
			}
			else if (weightNodes[intCounterA] == nullptr)
			{
				continue;
			}
			else if(weightNodes[intCounterA]->GetWeight() < LowestWeightA->GetWeight())
			{
				LowestWeightA = weightNodes[intCounterA];
				LowestWeightAIndex = intCounterA;
			}
			
		}
		for(int intCounterB = 0; intCounterB < 256; intCounterB++)
		{
			if (weightNodes[intCounterB] == nullptr)
			{
				continue;
			}
			else if(LowestWeightAIndex == intCounterB)
			{
				continue;
			}
			else if(LowestWeightB == nullptr && weightNodes[intCounterB] != nullptr)
			{
				LowestWeightB = weightNodes[intCounterB];
				LowestWeightBIndex = intCounterB;
			}
			else if(weightNodes[intCounterB]->GetWeight() < LowestWeightB->GetWeight())
			{
				LowestWeightB = weightNodes[intCounterB];
				LowestWeightBIndex = intCounterB;
			}
		}
		if (LowestWeightAIndex != -1 && LowestWeightBIndex != -1)
		{
			if (LowestWeightAIndex < LowestWeightBIndex)
			{
				//todo: make subtree and move to lowest weight a index
				treeBuildingPairs += reinterpret_cast<unsigned char&>(LowestWeightAIndex);
				treeBuildingPairs += reinterpret_cast<unsigned char&>(LowestWeightBIndex);
				auto newNode = new Node(LowestWeightA->GetWeight() + LowestWeightB->GetWeight());
				if (LowestWeightA->GetWeight() < LowestWeightB->GetWeight())
				{
					newNode->SetLeftChild(LowestWeightA);
					newNode->SetRightChild(LowestWeightB);
				}
				else
				{
					newNode->SetLeftChild(LowestWeightB);
					newNode->SetRightChild(LowestWeightA);
				}
				weightNodes[LowestWeightAIndex] = newNode;
				weightNodes[LowestWeightBIndex] = nullptr;
			}
			else
			{
				//todo: make subtree and move to lowest weight b index
				treeBuildingPairs += reinterpret_cast<unsigned char&>(LowestWeightBIndex);
				treeBuildingPairs += reinterpret_cast<unsigned char&>(LowestWeightAIndex);
				auto newNode = new Node(LowestWeightA->GetWeight() + LowestWeightB->GetWeight());
				if (LowestWeightA->GetWeight() < LowestWeightB->GetWeight())
				{
					newNode->SetLeftChild(LowestWeightA);
					newNode->SetRightChild(LowestWeightB);
				}
				else
				{
					newNode->SetLeftChild(LowestWeightB);
					newNode->SetRightChild(LowestWeightA);
				}
				weightNodes[LowestWeightBIndex] = newNode;
				weightNodes[LowestWeightAIndex] = nullptr;
			}
		}
		else
		{
			Root = LowestWeightA;
		}
		
	} while (LowestWeightA != nullptr && LowestWeightB != nullptr);
	
	return Root;
}

void Structure::IncrementWeight(int asciiValue)
{
	weightNodes[asciiValue]->IncrementWeight();
}

int Structure::GetWeight(int arrayIndex)
{
	return weightNodes[arrayIndex]->GetWeight();
	return 0;
}

string Structure::GetTreeBuildingPairs()
{
	return treeBuildingPairs;
}

void Structure::Traversal(Node* startNode, string strPath)
{
	if(startNode != nullptr)
	{
		Traversal(startNode->GetLeftNode(), strPath + "0");
		if(startNode->GetLeftNode() == nullptr && startNode->GetRightNode() == nullptr)
		{
			if(strPath.length() > largestPath.length())
			{
				largestPath = strPath;
			}
			strPaths[startNode->GetAsciiValue()] = strPath;
		}
		Traversal(startNode->GetRightNode(), strPath + "1");
	}
	
}

void Structure::CreatePaths()
{
	Traversal(Root, "");
}

std::string Structure::GetPath(unsigned char charWithAName)
{
	return strPaths[charWithAName];
}

void Structure::BuildFromPairs(int pair1, int pair2)
{
	if(Root == nullptr)
	{
		for(int intCounter = 0; intCounter < 255; intCounter++)
		{
			weightNodes[intCounter] = new Node();
			weightNodes[intCounter]->SetAsciiValue(intCounter);
		}
	}
	Node* newNode = new Node();
	if(pair1 < pair2)
	{
		newNode->SetLeftChild(weightNodes[pair1]);
		newNode->SetRightChild(weightNodes[pair2]);
		weightNodes[pair1] = newNode;
		weightNodes[pair2] = nullptr;
	}
	else
	{
		newNode->SetLeftChild(weightNodes[pair2]);
		newNode->SetRightChild(weightNodes[pair1]);
		weightNodes[pair2] = newNode;
		weightNodes[pair1] = nullptr;
	}

	
	
	Root = newNode;
}

Node* Structure::GetRoot()
{
	return Root;
}



Structure::~Structure()
{
}
