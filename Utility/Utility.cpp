#include "Utility.h"

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
