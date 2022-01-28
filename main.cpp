#include <iostream>
#include <string>
#include <fstream>
#include "Compressor.h"

#define TEST_FILE_PATH "E://Roll//Presentation1.pptx"

int main(int argc, char* argv[])
{
	bool isTesting = true;
	Compressor* compressor = new Compressor();

	std::string filepath;
	if (!isTesting)
		filepath = std::string(argv[1]);
	else
		filepath = TEST_FILE_PATH; // for testing

	try
	{
		system("cls");
		std::cout << "Compressing File: " << filepath << "\n==================\n";

		Buffer compressed = compressor->Compress(filepath);
		std::cout << "Compressing Complete!\n";

		if (!isTesting || true)
		{
			// save new compressed file. extention: .nim ;)
			std::string newName = filepath;
			std::reverse(newName.begin(), newName.end());
			int dotIndex = newName.find('.');
			int slashIndex = newName.find('\\');
			std::reverse(newName.begin(), newName.end());

			if (dotIndex < slashIndex) // file have an extention
			{
				newName = newName.substr(0, newName.length() - (dotIndex + 1)); // removing existing extention
			}
			newName += "_comp" + std::string(COMP_FILE_EXTENTION);

			// save file
			std::ofstream savedFile;
			savedFile.open(newName);
			std::string newData = compressor->bufferToString(compressed);
			savedFile.write(newData.c_str(), newData.length());
			savedFile.close();
		}
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception Thrown: " << e.what() << "\n";
	}

	return 0;
}