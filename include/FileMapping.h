#pragma once

#include <windows.h>
#include <sstream>
#include <string>
#include <cstdint>
#include <vector>

const std::string kSharedBufferName = "dropbox_buffer_name";
const std::string kEmptyCountSemaphoreName = "dropbox_empty_count";
const std::string kFullCountSemaphoreName = "dropbox_full_count";
const std::string kMutexName = "dropbox_mutex";
const std::size_t kSharedBufferSize = 1024 * 1024 * 4 * 4; // byte size
const std::size_t kWaitMilisecReadOrWriteBeforeFailing = 1000;

const std::size_t kTestNum = 16;

class FileMapping
{
public:
	static FileMapping &GetInstance();
	bool IsCreateSuccess();
	bool Reset();
	~FileMapping();

private:
	bool InitializeCreateAndOpen();
	bool FinalizeCloseAndDestory();

private:
	FileMapping();

private:
	bool m_isCreateSuccess;
	std::size_t m_fileSize;
	HANDLE m_sharedMap;
	void *m_memoryAddress;
	HANDLE m_mutex;
	HANDLE m_emptyCountSemaphore;
	HANDLE m_fullCountSemaphore;
};

class FileMappingDropbox
{
public:
	FileMappingDropbox();
	~FileMappingDropbox();
	bool WriteData(const std::vector<uint16_t> &data);
	bool ReadData(std::vector<uint16_t> &data);

private:
	bool InitializeOpen();
	bool FinalizeClose();

private:
	std::size_t m_fileSize;
	HANDLE m_sharedMap;
	void *m_memoryAddress;
	HANDLE m_mutex;
	HANDLE m_emptyCountSemaphore;
	HANDLE m_fullCountSemaphore;
};

