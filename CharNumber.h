#pragma once
#include <cmath>
#include <exception>
#include <string>

class CharNumber
{
private:
	//int maxValue = 255;
public:
	static const int maxNumber = 255;
	static char fromNumber(int wholeNumber)
	{
		if (wholeNumber <= maxNumber && wholeNumber >= 0)
		{
			return char(wholeNumber);
		}
		else
		{
			throw std::exception((std::string(__FUNCTION__) + (" Number can only be between 0 and " + std::to_string(maxNumber))).c_str());
		}
	}

	static int fromChar(char c)
	{
		int num = (int)c;
		if (num <= maxNumber && num >= 0)
		{
			return num;
		}
		else
		{
			throw std::exception((std::string(__FUNCTION__) + (" Number can only be between 0 and " + std::to_string(maxNumber))).c_str());
		}
	}
};