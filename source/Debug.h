#pragma once
#define Check()	check(__FILE__,__LINE__)	//说明这之前有错误
#define ASSERT(x) if(!(x)) __debugbreak()	//先写一个断言函数 把分号交给外面
#define Call(x) clearError();\
	x;\
	ASSERT(GLcall(#x, __FILE__, __LINE__))	//用反斜线顺延宏命令 注意反斜线后不能有注释
void check(const char* file, int line);
void clearError();
void GLcall(const char* name, const char* file, int line);