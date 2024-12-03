#include"mString.h"
#include"check.h"

mString::mString(const char* str)
{
	if(str)
	{
		char c = NULL;
		for (size = 0; (c = str[size]) != NULL; size++);
		ptr = new char[size + 1];
		for (unsigned int i = 0; i <= size; i++)
			ptr[i] = str[i];
	}
}

mString::mString(const mString& other)
{
	if(other.ptr)
	{
		size = other.size;
		ptr = new char[size + 1];
		for (unsigned int i = 0; i <= size; i++)
			ptr[i] = other.ptr[i];
	}
}

mString& mString::operator=(const mString& other)
{
	if (*this == other)
		return *this;
	if (ptr)
		delete[] ptr;
	size = other.size;
	if(other.ptr)
	{
		ptr = new char[size + 1];
		for (unsigned int i = 0; i <= size; i++)
			ptr[i] = other.ptr[i];
	}
	return *this;
}

bool mString::operator==(const mString& other) const
{
	if(size != other.size)
		return false;
	if (!ptr)
		return true;
	for (unsigned int i = 0; i < size; i++)
		if (ptr[i] != other.ptr[i])
			return false;
	return true;
}

bool mString::operator<(const mString& other) const
{
	if(!ptr && !other.ptr)
		return false;
	if (!other.ptr)
		return false;
	if (!ptr)
		return true;

	unsigned int min_size = size < other.size ? size : other.size;
	for (unsigned int i = 0; i < min_size; i++)
	{
		if (ptr[i] > other.ptr[i])
			return false;
		else if (ptr[i] < other.ptr[i])
			return true;
	}
	return other.size > size;
}

bool mString::operator>(const mString& other) const
{
	if (!ptr && !other.ptr)
		return false;
	if (!other.ptr)
		return true;
	if (!ptr)
		return false;

	unsigned int min_size = size < other.size ? size : other.size;
	for (unsigned int i = 0; i < min_size; i++)
	{
		if (ptr[i] > other.ptr[i])
			return true;
		else if (ptr[i] < other.ptr[i])
			return false;
	}
	return other.size < size;
}

char& mString::operator[](unsigned int index)
{
	if (!ptr)
		ERROR_LOG("ERROR::MSTRING: access empty mstring");
	if (index >= size)
		ERROR_LOG("ERROR::MSTRING: index out of range in " << ptr);
	return ptr[index];
}

const char* mString::getStr() const
{
	return static_cast<const char*> (ptr);
}

mString::~mString()
{
	if (ptr)
		delete[] ptr;
	ptr = nullptr;
}
