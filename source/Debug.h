#pragma once
#define Check()	check(__FILE__,__LINE__)	//˵����֮ǰ�д���
#define ASSERT(x) if(!(x)) __debugbreak()	//��дһ�����Ժ��� �ѷֺŽ�������
#define Call(x) clearError();\
	x;\
	ASSERT(GLcall(#x, __FILE__, __LINE__))	//�÷�б��˳�Ӻ����� ע�ⷴб�ߺ�����ע��
void check(const char* file, int line);
void clearError();
void GLcall(const char* name, const char* file, int line);