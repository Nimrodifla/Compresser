#include <iostream>
#include "Compressor.h"

int main(int argc, char* argv[])
{
	Compressor* compressor = new Compressor();

	std::string filepath;
	//filepath = std::string(argv[1]);
	filepath = "E://Roll//test_bin_file.zzz";// for testing
	try
	{
		Buffer compressed = compressor->Compress(filepath);
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception Thrown: " << e.what() << "\n";
	}

	return 0;
}