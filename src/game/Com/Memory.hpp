#pragma once

#include "../stdafx.hpp"

class LargeLocal
{
public:
	LargeLocal(int sizeParam);
	~LargeLocal();

	std::uint8_t* LargeLocalGetBuf(int startPos, int size);

	void PopBuf();

private:
	int startPos;
	int size;
};

void LargeLocalInit();
void LargeLocalEnd(int startPos);
void LargeLocalEndRight(int startPos);
void LargeLocalReset();
void LargeLocalResetToMark(int markPos);
int LargeLocalBegin(int size);
int LargeLocalBeginRight(int size);
int LargeLocalGetMark();
void LargeLocalClear();

// Z (Name is from Quake III)
void* Z_MallocInternal(size_t size);
