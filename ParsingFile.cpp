#include "ParsingFile.hpp"

ParsingFile::ParsingFile(/* args */)
{
	std::cout << "hellow" << std::endl;
}

ParsingFile::~ParsingFile()
{
	std::cout << "Bye" << std::endl;

}

// void ParsingFile::	testError(int code)
// {

// }

void	ParsingFile:: getFile(std::string fileName)
{
	std::string myText;

	std::cout << "file name: " << fileName << std::endl;
	std::ifstream MyReadFile(fileName);// Read from the text file
	while (getline (MyReadFile, myText)) {
		std::cout << myText;// Output the text from the file
	}

	// Close the file
	MyReadFile.close();
}