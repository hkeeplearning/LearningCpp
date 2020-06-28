#include "FileMapping.h"
#include "plog/Log.h"

#include <vector>
#include <fstream>

int main()
{
	plog::init(plog::info, "log\\1.txt", 2000000, 256);
	FileMappingDropbox dropbox;
	std::vector<uint16_t> data(kTestNum * kTestNum);
	for (int i = 0; i < kTestNum; ++i)
	{
		for (int j = 0; j < kTestNum; ++j)
		{
			data[i * kTestNum + j] = 65535;

			while (true)
			{
				bool isSuccess = dropbox.WriteData(data);
				if (isSuccess)
				{
					break;
				}
				else
				{
					::Sleep(100);
				}
			}
		}
	}

	return 0;
}