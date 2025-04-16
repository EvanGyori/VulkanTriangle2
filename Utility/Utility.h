#pragma once

#include <vector>
#include <algorithm>

bool isASubsetOfB(const std::vector<const char*>& A, const std::vector<const char*>& B);

template<typename T>
T clamp(T x, T min, T max)
{
    return std::max(std::min(x, max), min);
}

std::vector<char> getFileData(const char* filename);
