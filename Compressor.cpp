#include "Compressor.h"

Compressor::Compressor()
{
	this->algosInUse.clear();
	this->availbleAlgos.clear();
}

compUncopPair* Compressor::createCompUncompPair(algoFunc comp, algoFunc uncomp)
{
	compUncopPair* pair = new compUncopPair(comp, uncomp);
	return pair;
}

void Compressor::printBuffer(Buffer& buff)
{
	int i = 0;
	std::string res = "";
	for (i = 0; i < buff.size(); i++)
	{
		res += buff[i];
	}

	std::cout << res << "\n";
}

void Compressor::callAlgoFunction(Byte algoId, Buffer& buffParameter, bool isComp)
{
	Buffer duplicateBuff = buffParameter;
	int beginSize = duplicateBuff.size();
	algoFunc func = nullptr;

	if (isComp)
		func = availbleAlgos.at(algoId)->getComp();
	else
		func = availbleAlgos.at(algoId)->getUncomp();
	(func)(duplicateBuff); // calls the function

	int endSize = duplicateBuff.size();

	// check if comp algo reduced the size
	// if reduced -> save it and use it
	// if didnt -> dont use it
	if (endSize <= beginSize)
	{
		algosInUse.push_back(algoId);
		buffParameter = duplicateBuff;
	}
	else
	{
		std::cout << "Algo id: " << algoId << " didn't help, didnt used it.\n";
	}
}



float Compressor::presentageDecreased(float beginSize, float endSize)
{
	return (endSize / beginSize - 1) * -100;
}

Buffer Compressor::getFileContent(std::string filepath)
{
	std::ifstream input(filepath, std::ios::binary);

	// copies all data into buffer
	Buffer buffer(std::istreambuf_iterator<char>(input), {});

	input.close();

	return buffer;
}

// COMPRESSING ALGORITHEMS

void Compressor::compAlgo_FindSimpleRules(Buffer& buff)
{
	int maxPattersSize = 100;
	Buffer result;
	//Buffer slice = Buffer(buff.begin(), buff.begin() + sliceSize);

}

void Compressor::compAlgo_InARow(Buffer& buff)
{

	Buffer result;
	int inARowCounter = 0;

	for (int i = 0; i < buff.size(); i++)
	{
		Byte b = buff[i];
		bool isTheLastByte = (i >= buff.size() - 1);
		if (!isTheLastByte)
		{
			Byte nextByte = buff[i + 1];
			if (b == nextByte && inARowCounter < CharNumber::maxNumber)
			{
				inARowCounter++;
			}
			else
			{
				// add the count of the char
				result.push_back(CharNumber::fromNumber(inARowCounter + 1));
				// add the Byte (char) that is written that many times
				result.push_back(b);
				// reset counter
				inARowCounter = 0;
			}
		}
		else
		{
			if (inARowCounter == 0)
			{
				// add the count of the char
				result.push_back(CharNumber::fromNumber(1));
				// add the Byte (char) that is written that many times
				result.push_back(b);
			}
			else
			{
				// add the count of the char
				result.push_back(CharNumber::fromNumber(inARowCounter + 1));
				// add the Byte (char) that is written that many times
				result.push_back(b);
				// reset counter
				inARowCounter = 0;
			}
		}
	}

	buff = result;
}


Buffer Compressor::Compress(std::string filepath)
{
	// initializing
	Buffer data = getFileContent(filepath);
	int beginSize = data.size();
	// add all availabe comp funcs
	availbleAlgos.clear();
	//availbleAlgos.insert(std::pair<Byte, compUncopPair>('a', compUncopPair(&compAlgo_FindSimpleRules, &uncompAlgo_FindSimpleRules)));
	availbleAlgos.insert(std::pair<Byte, compUncopPair*>(/*'b'*/ 'a', createCompUncompPair(&compAlgo_InARow, &uncompAlgo_InARow)));

	int availableAlgosCount = availbleAlgos.size();


	// compress - calls all algos
	for (int i = 0; i < availableAlgosCount; i++)
	{
		callAlgoFunction('a' + i, data, true); // true - mwans we are compressing and now UNcompressing
	}


	// finilazing
	printBuffer(data);
	int endSize = data.size();
	std::cout << presentageDecreased(beginSize, endSize) << "% size decrease!\n";
	return data;
}