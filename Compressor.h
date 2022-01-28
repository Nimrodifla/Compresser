#pragma once
#include "CharNumber.h"
#include <fstream>
#include <vector>
#include <map>
#include <iostream>
#include <string>
#include "compUncopPair.h"

#define DELIMITER '$'
#define COMP_FILE_EXTENTION ".nim"

class Compressor
{
private:

	std::map<Byte, compUncopPair*> availbleAlgos; // <Algo Id char, Algo function pointer>
	std::vector<Byte> algosInUse; // func IDs

	static void printBuffer(Buffer& buff);
	compUncopPair* createCompUncompPair(algoFunc comp, algoFunc uncomp);
	void addCompUncompPair(Byte id, algoFunc comp, algoFunc uncomp);
	void callAlgoFunction(Byte algoId, Buffer& buffParameter, bool isComp);
	float presentageDecreased(float beginSize, float endSize);
	Buffer getFileContent(std::string filepath);

	// COMPRESSING ALGORITHEMS

	static void compAlgo_FindSimpleRules(Buffer& buff);
	static void uncompAlgo_FindSimpleRules(Buffer& buff);

	static void compAlgo_InARow(Buffer& buff);
	static void uncompAlgo_InARow(Buffer& buff);

	static void compAlgo_repeatingPattrens(Buffer& buff);
	static void uncompAlgo_repeatingPattrens(Buffer& buff);


public:
	Compressor();

	Buffer Compress(std::string filepath);

	static std::string bufferToString(Buffer& buff);
	static Buffer stringToBuffer(std::string str);
};