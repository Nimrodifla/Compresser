#pragma once
#include "CharNumber.h"
#include <fstream>
#include <vector>
#include <map>
#include <iostream>
#include <string>
#include "compUncopPair.h"

class Compressor
{
private:

	std::map<Byte, compUncopPair*> availbleAlgos; // <Algo Id char, Algo function pointer>
	std::vector<Byte> algosInUse; // func IDs


	void printBuffer(Buffer& buff);
	compUncopPair* createCompUncompPair(algoFunc comp, algoFunc uncomp);
	void callAlgoFunction(Byte algoId, Buffer& buffParameter, bool isComp);
	float presentageDecreased(float beginSize, float endSize);
	Buffer getFileContent(std::string filepath);

	// COMPRESSING ALGORITHEMS

	static void compAlgo_FindSimpleRules(Buffer& buff);
	static void uncompAlgo_FindSimpleRules(Buffer& buff)
	{
		// to do
	}

	static void compAlgo_InARow(Buffer& buff);
	static void uncompAlgo_InARow(Buffer& buff)
	{
		// to do
	}


public:
	Compressor();
	Buffer Compress(std::string filepath);
};