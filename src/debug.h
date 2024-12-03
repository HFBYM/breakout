#pragma once
#define Check()	check(__FILE__,__LINE__)	//check if any error before this line
#define ASSERT(x) if(!(x)) __debugbreak()

void check(const char* file, int line);
void clearError();
