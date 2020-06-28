#include "FileMapping.h"

#include "plog/Log.h"
#include <iostream>

int main()
{
	plog::init(plog::info, "log\\0.txt", 2000000, 256);

	auto obj = FileMapping::GetInstance();
	if (obj.IsCreateSuccess())
	{
		while (true)
		{
			::Sleep(1000);
		}
	}

	return 0;
}