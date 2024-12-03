#pragma once
#include<iostream>
#define ERROR_LOG(x) {\
	std::cout<< x <<std::endl;\
	__debugbreak();}
#define ASSERT_LOG(beTrue, message) {if(!(beTrue)) ERROR_LOG(message)}