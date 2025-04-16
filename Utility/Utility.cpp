#include "Utility.h"

#include <fstream>
#include <stdexcept>
#include <string>

bool isASubsetOfB(const std::vector<const char*>& A, const std::vector<const char*>& B)
{
    for (auto elementA : A) {
	bool isElementAInB = false;
	for (auto elementB : B) {
	    if (strcmp(elementA, elementB) == 0) {
		isElementAInB = true;
		break;
	    }
	}
    
	if (!isElementAInB) {
	    return false;
	}
    }

    return true;
}

std::vector<char> getFileData(const char* filename)
{
    std::ifstream stream(filename);

    if (!stream.is_open()) {
	std::string errorMessage = "failed to open file: ";
	errorMessage += filename;
	throw std::runtime_error(errorMessage);
    }

    stream.seekg(0, stream.end);
    int length = stream.tellg();
    stream.seekg(0, stream.beg);

    std::vector<char> data(length);
    stream.read(&data.front(), length);

    stream.close();

    return data;
}
