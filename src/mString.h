#pragma once
#ifndef NULL
	#define NULL 0
#endif 
class mString
{
public:
	mString(const char* str);
	mString() = default;
	/// @brief uses const so two construction work
	mString(const mString&);
	mString(mString&&) = default;
	~mString();

	mString& operator=(const mString&);
	mString& operator=(mString&&) = default;

	bool operator== (const mString&) const;
	bool operator==(const char* str) const { return *this == mString(str); }
	bool operator!=(const mString& other) const { return !(*this == other); }
	bool operator!=(const char* ptr) const { return !(*this == mString(ptr)); }
	bool operator<(const mString&) const;
	bool operator>(const mString&) const;

	char& operator[](unsigned int index);


	inline unsigned int getSize() const { return size; }
	const char* getStr() const;

private:
	/// @brief num of chars it contains
	unsigned int size = 0;
	char* ptr = nullptr;
};
