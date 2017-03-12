// HuffmanTree.cpp : Defines the entry point for the console application.
// Author: David Naelitz
// Title: HuffmanTree for EECS 2510 Non-Linear
// Date: 2/20/2017
// Program will be named Huff. 
// -t file1 [file2] If file 2 is specified it is used to hold the 510-byte tree-building information
// otherwise produce file1.tree 
// -e Read specified file, Use the tree to encode a file embedding the tree information in the encoded file. 
// The tree building informartion is not saved to a seperate file. Both files can not refer to the same file. 
// -et file1 file2 [file3] encode the first file with the tree-building information block from the second file. 

#include "stdafx.h"
#include <string>
#include <iostream>
#include <fstream>
#include "Structure.h"
#include <time.h>

using namespace std;
std::ifstream reader;
std::ifstream reader2;
std::ofstream writer;
string inputFile;
string outputStream;
char outputChar;
string command;
bool blnTesting = true;
string file1, file2, file3;
int frequencies[256] = { 0 };
string strPaths[256] = { "" };
Structure nodeStructure;
time_t startTime, Endtime;
Node* currentNode;
void Help();
int* freqCounts(string fileName);
string ValidateArguments(int intArgumentCount, char* argumentArray[]);
void DecodeBit(unsigned char unsignedByte, unsigned char otherstuff);
bool Encode();
bool Decode();
string treeBuildingPairs;

int main(int argc, char* argv[])
{

	int* frequencies;


	//Validate arguments and set filenames here. Then no need for testing branch.
	//Also write change extension method.
	//use reader.get() to get a character at a time 
	//infile seekg ios::beg  , end,  rel?
	//compressed file should be 3,075,156
	// string array of paths
	//Write a method to step through all 256 characters and make their binary path and save it to string array of paths
	//Write a method that takes indexes and pairs them together.
	// Write a method that takes a char and gives their path back and put this in a loop 

	// Will validate the arguments and convert argument to lowercase and pass back the command to trigger
	// the appropriate action.
	command = ValidateArguments(argc, argv);
	if (command == "-t") // Builds the huffman tree and saves to a .tree file.
	{
		//todo: Make tree building info and make file two with file1.tree
		frequencies = freqCounts(file1);
		nodeStructure.countsToTree(frequencies);
		treeBuildingPairs = nodeStructure.GetTreeBuildingPairs();
		writer.open(file2, ios::binary);
		writer.write(treeBuildingPairs.c_str(), 510);
	}
	else if (command == "-e") // Builds the tree and encodes a file saving it to a .huf file. 
	{
		//todo: Read file and encode it putting the tree building information and the file in outputfile
		startTime = clock();
		frequencies = freqCounts(file1);
		nodeStructure.countsToTree(frequencies);
		treeBuildingPairs = nodeStructure.GetTreeBuildingPairs();
		nodeStructure.CreatePaths();
		Encode();
		Endtime = clock();
		cout << (Endtime - startTime);
		exit(0);

	}
	else if (command == "-d") // Decodes the file using the tree builing information at the beginning of the file.
	{
		reader.open(file1, ios::binary);
		writer.open(file2, ios::binary);
		Decode();
	}


	return 0;
}

// Function to output the acceptable commands
void Help()
{
	cout << "Huff -t file1 [file2]" << endl;
	cout << "Huff -e file1 [file2]" << endl;
	cout << "Huff -et file1 file2 [file3]" << endl;
	cout << "Huff -d file1 file2" << endl;
	cout << "Huff -h | -? | -help" << endl;
}

//This will validate the arguments and return the command string. 
string ValidateArguments(int intArgumentCount, char* argumentArray[])
{
	//The least amount of arguments you can have is two. 
	if (intArgumentCount < 2)
	{
		// During testing this is where I adjust commmand like arguments. 
		if (blnTesting)
		{
			file1 = "test.huf";
			file2 = "testdecode.txt";
			file3 = "Shakespear.huf";
			return "-d";
		}
		//todo:Output help and exit
		cout << "Invalid amount of arguments" << endl;
		Help();
		exit(0);
	}
	//The only time two arguments is okay is for help
	else if (intArgumentCount == 2)
	{
		if (std::strcmp(argumentArray[1], "-?") == 0 || std::strcmp(argumentArray[1], "-h") == 0)
		{
			Help();
			exit(0);
		}
	}
	else
	{
		//todo: Work on other cases and getting files named correctly
		return "-e";
	}

}

// This goes through the file and finds the frequency counts and increments the value of a character each time it
// appears in the file. This is done by using its ascii value as the index. 
int* freqCounts(string fileName)
{
	reader.open(fileName, std::ios::binary);
	reader.seekg(0, ios::end);
	int intEndOfFile = reader.tellg(); // Used to keep track of how long the file is to use for loop
	reader.seekg(0, ios::beg);
	for (int intCounter = 0; intCounter < intEndOfFile; intCounter++)
	{
		frequencies[reader.get()]++;
	}

	return frequencies;
}

// This goes through the file and encodes character by character writing it to a second file. 
bool Encode()
{
	unsigned char charIAmGoingToWrite;
	char c;
	unsigned char P2[] = { 128, 64, 32, 16, 8, 4, 2, 1 };
	writer.open(file2, ios::binary);
	writer.write(treeBuildingPairs.c_str(), 510); //Takes the tree info and writes that information first.
	reader.seekg(0, ios::beg);					  //Make sure we are at the beginning of the file. 

	//Check to see that both the reader and writer are open.
	if (reader.is_open() && writer.is_open())
	{
		while (!reader.eof())					  //Go until the file is over
		{

			outputChar = reader.get();
			outputStream += nodeStructure.GetPath(reinterpret_cast<unsigned char&>(outputChar));
			while (outputStream.length() >= 8)
			{
				charIAmGoingToWrite = 0;
				for (int j = 0; j <= 7; j++)
				{
					if (outputStream[j] == '1') //Should not need == 1 but was not working otherwise
					{
						charIAmGoingToWrite |= P2[j];
					}
				}
				writer.put(charIAmGoingToWrite);
				outputStream = outputStream.substr(8, outputStream.length());
			}

		}
		outputStream += nodeStructure.largestPath; //Adds the largest path to pad the file. Still only adds one more byte.
		charIAmGoingToWrite = 0;
		for (int j = 0; j <= 7; j++)
		{
			if (outputStream[j]) charIAmGoingToWrite |= P2[j];
		}
		writer.put(charIAmGoingToWrite);
		return true;
	}


	return false;
}


// Goes through and every leaf node found in the built tree, write the char and return to root node. 
bool Decode()
{
	unsigned char charFromFile1, charFromFile2;
	char byte;
	if (reader.is_open()&& writer.is_open())
	{
		// Goes through and builds the tree by sending the two pairs that need to be linked together
		// until there is only the root node left in the array.
		for (int pairCounter = 0; pairCounter < 255; pairCounter++)
		{

			charFromFile1 = reader.get();
			cout << charFromFile1 << " - ";
			charFromFile2 = reader.get();
			cout << charFromFile2 << endl;
			nodeStructure.BuildFromPairs(charFromFile1, charFromFile2);
		}
		currentNode = nodeStructure.GetRoot();
		while (reader.get(byte))
		{
			auto unsignedByte = reinterpret_cast<unsigned char&>(byte);

			for (int intCounter = 7; intCounter >= 0; intCounter--)
			{
				if (currentNode->IsALeaf())
				{
					cout << unsigned char(currentNode->GetAsciiValue());
					currentNode = nodeStructure.GetRoot();
				}
				DecodeBit(unsignedByte, 1 << intCounter);
			}
		}


	}
	return true;
}

// Go left or right depending on if it is a one or zero.
void DecodeBit(unsigned char unsignedByte, unsigned char otherstuff)
{
	if ((unsignedByte & otherstuff) == otherstuff)
	{
		if (currentNode->GetRightNode() != nullptr) currentNode = currentNode->GetRightNode();
	}
	else
	{
		if (currentNode->GetLeftNode() != nullptr) currentNode = currentNode->GetLeftNode();
	}
}

