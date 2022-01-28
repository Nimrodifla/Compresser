#pragma once

typedef char Byte;
typedef std::vector<Byte> Buffer;
typedef void (*algoFunc)(Buffer&); // "algo function pointer" is now set to "algoFunc"

class compUncopPair
{
private:
	algoFunc compressingFunction;
	algoFunc UncompressingFunction;

public:
	compUncopPair(algoFunc comp, algoFunc uncomp)
	{
		this->compressingFunction = comp;
		this->UncompressingFunction = uncomp;
	}

	algoFunc getComp()
	{
		return this->compressingFunction;
	}

	algoFunc getUncomp()
	{
		return this->UncompressingFunction;
	}
};