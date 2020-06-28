#include "FileMapping.h"
#include "plog/Log.h"

#include <fstream>
#include <vector>

int main()
{
	plog::init(plog::info, "log\\2.txt", 2000000, 256);
	FileMappingDropbox dropbox;

	std::vector<uint16_t> data;

	static int idx = 0;
	while (true)
	{
		if (dropbox.ReadData(data))
		{
			std::stringstream ss;	ss << idx;	std::string filename = "out\\";	filename += ss.str();
			std::ofstream ofs(filename + ".raw", std::ios::binary);
			if (ofs.is_open())
			{
				ofs.write(reinterpret_cast<const char *>(data.data()), data.size() * sizeof(data[0]));
				ofs.close();
			}
			++idx;
			if (idx == kTestNum * kTestNum)
			{
				break;
			}
		}
		else
		{
			::Sleep(100);
		}
	}
	return 0;
}