#pragma once
#include "Node.h"
#include <string>
class Structure
{
public:
	void IncrementWeight(int asciiValue);
	void BuildFromPairs(int pair1, int pair2);
	Node* countsToTree(int* counts);
	Node* GetRoot();
	int GetWeight(int arrayIndex);
	std::string GetPath(unsigned char charWithAName);
	std::string GetTreeBuildingPairs();
	std::string largestPath = "";
	void CreatePaths();
	void MakeTreeFromPairs();
	Structure();
	~Structure();
private:
	Node** pointerNodes[256];
	Node* weightNodes[256];
	std::string strPaths[256] = {""};
	void Traversal(Node* startNode, std::string strPath);
	Node* LowestWeightA;
	int LowestWeightAIndex;
	Node* LowestWeightB;
	int LowestWeightBIndex;
	Node* Root;
	std::string treeBuildingPairs;
	
	
};

