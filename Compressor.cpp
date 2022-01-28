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

void Compressor::addCompUncompPair(Byte id, algoFunc comp, algoFunc uncomp)
{
	this->availbleAlgos.insert(std::pair<Byte, compUncopPair*>(id, createCompUncompPair(comp, uncomp)));
}

std::string Compressor::bufferToString(Buffer& buff)
{
	int i = 0;
	std::string res = "";
	for (i = 0; i < buff.size(); i++)
	{
		res += buff[i];
	}

	return res;
}

void Compressor::printBuffer(Buffer& buff)
{
	std::cout << bufferToString(buff) << "\n";
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
	if (endSize < beginSize)
	{
		algosInUse.push_back(algoId);
		buffParameter = duplicateBuff;
	}
	else
	{
		std::cout << "Algo id: '" << algoId << "' didn't help, didnt used it.\n";
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

// COMPRESSING & UNCOMPRESSING ALGORITHEMS

void Compressor::compAlgo_FindSimpleRules(Buffer& buff)
{
	
	int i = 0, j = 0, off = 0;
	int maxJumps = buff.size() - 2;
	if (maxJumps > CharNumber::maxNumber)
	{
		maxJumps = CharNumber::maxNumber;
	}

	Buffer header;

	for (i = 1; i < maxJumps; i++)
	{
		Byte b;
		bool equal = true;
		bool assigned = false;
		for (j = 0; j < buff.size() && equal; j += i)
		{
			if (!assigned)
			{
				b = buff[j];
				assigned = true;
			}
			else
			{
				if (b != buff[j])
				{
					equal = false;
				}
			}
		}

		if (equal && assigned)
		{
			// check if this pattern overloading other one
			// if so, dont add it
			bool found = false;
			for (j = 0; j < header.size() && !found; j += 2)
			{
				if (header[j] == b)
				{
					found = true;
				}
			}

			if (!found)
			{
				int offset = 0;
				int jumps = i;

				header.push_back(CharNumber::fromNumber(offset)); // offset - right now always 0
				header.push_back(CharNumber::fromNumber(jumps)); // jumps
				header.push_back(b); // the byte that repeats

				// remove pattern from buff
				Buffer newBuff;
				for (j = 0; j < buff.size(); j++)
				{
					if ((j + offset) % jumps != 0)
					{
						newBuff.push_back(buff[j]);
					}
				}

				buff = newBuff;
			}
		}
	}

	// append header at the start
	buff.insert(buff.begin(), DELIMITER);
	
	for (i = header.size() - 1; i >= 0; i--)
	{
		buff.insert(buff.begin(), header[i]);
	}
}

void Compressor::uncompAlgo_FindSimpleRules(Buffer& buff)
{

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
			if (b == nextByte && inARowCounter < CharNumber::maxNumber-1)
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

void Compressor::uncompAlgo_InARow(Buffer& buff)
{

}

void Compressor::compAlgo_repeatingPattrens(Buffer& buff)
{
	int i = 0, j = 0, k = 0;
	int maxPatternSize = 10;
	if (maxPatternSize > CharNumber::maxNumber)
	{
		maxPatternSize = CharNumber::maxNumber;
	}
	if (maxPatternSize > buff.size())
	{
		maxPatternSize = buff.size();
	}

	Byte id = 'a';
	std::map<std::string, std::vector<int>*> patterns; // <pattern (first latter is it's id), indexes of apearance>
	std::string bufferAsString = bufferToString(buff);

	for (i = 2; i < maxPatternSize; i++) // starting with searching 2 Bytes patterns and getting bigger
	{
		//std::cout << " - " << i << " / " << maxPatternSize << "...\n";
		int patternSize = i;
		for (j = 0; j < bufferAsString.size() - patternSize + 1 && j < CharNumber::maxNumber; j++) // picks a patters
		{
			std::cout << " - " << i << " / " << maxPatternSize << ": ";
			std::cout << " - - " << j << " / " << (buff.size() - patternSize + 1) << "...\n";
			int patternStartIndex = j;
			std::string pattern = bufferAsString.substr(patternStartIndex, patternSize);
			for (k = 0; k < bufferAsString.size() - patternSize + 1 && k < CharNumber::maxNumber; k++)
			{
				if (k != patternStartIndex)
				{
					bool added = true;
					//std::cout << k << " " << patternSize << ":: ";
					if (pattern == bufferAsString.substr(k, patternSize))
					{
						if (patterns.find(pattern) != patterns.end()) // patterns already found in buff
						{
							if (patterns.at(pattern)->size() < CharNumber::maxNumber)
							{
								patterns.at(pattern)->push_back(k);
							}
							else
							{
								added = false;
							}
						}
						else // first apearance of pattern
						{
							std::vector<int>* indexes = new std::vector<int>({ patternStartIndex, k });
							patterns.insert(std::pair<std::string, std::vector<int>*>(id + pattern, indexes));
							id++; // inc id
						}

						if (added)
						{
							// remove from buffer and replace with a byte (placeholder, doesnt matter what byte)
							//std::cout << k << " " << patternSize << ": ";
							bufferAsString = bufferAsString.substr(0, k) + '%' + bufferAsString.substr(k + patternSize);
						}
					}
				}
			}
		}
	}

	std::string header = "";
	for (auto iter = patterns.begin(); iter != patterns.end(); iter++)
	{
		header += CharNumber::fromNumber(iter->first.length()) + iter->first + CharNumber::fromNumber(iter->second->size());
		for (int i = 0; i < iter->second->size(); i++)
		{
			header += CharNumber::fromNumber((*iter->second)[i]);
		}
	}

	bufferAsString = header + bufferAsString;

	Buffer newBuff(bufferAsString.begin(), bufferAsString.end());
	buff = newBuff;
}

void Compressor::uncompAlgo_repeatingPattrens(Buffer& buff)
{

}


Buffer Compressor::Compress(std::string filepath)
{
	// initializing
	Buffer data = getFileContent(filepath);
	int beginSize = data.size();
	// add all availabe comp funcs
	addCompUncompPair('a', &compAlgo_FindSimpleRules, &uncompAlgo_FindSimpleRules);
	addCompUncompPair('b', &compAlgo_InARow, &uncompAlgo_InARow);
	addCompUncompPair('c', &compAlgo_repeatingPattrens, &uncompAlgo_repeatingPattrens);
	int availableAlgosCount = availbleAlgos.size();


	// compress - calls all algos
	for (int i = 0; i < availableAlgosCount; i++)
	{
		std::cout << "compressing: " << i + 1 << " / " << availableAlgosCount << "\n";
		callAlgoFunction('a' + i, data, true); // true - mwans we are compressing and now UNcompressing
	}


	// finilazing
	
	// add the algos we used - in reverese
	data.insert(data.begin(), DELIMITER);
	for (int i = 0; i < algosInUse.size(); i++)
	{
		data.insert(data.begin(), algosInUse[i]);
	}

	//printBuffer(data);
	int endSize = data.size();
	std::cout << presentageDecreased(beginSize, endSize) << "% size decrease!\n";

	// free memory
	for (auto it = availbleAlgos.begin(); it != availbleAlgos.end(); it++)
	{
		delete it->second;
	}
	
	return data;
}