#include "FileMapping.h"

#include <stdexcept>
#include "plog/Log.h"

FileMapping & FileMapping::GetInstance()
{
	static FileMapping obj;
	return obj;
}

bool FileMapping::IsCreateSuccess()
{
	return m_isCreateSuccess;
}

FileMapping::FileMapping() : m_fileSize(0), m_sharedMap(NULL), m_memoryAddress(NULL),
m_mutex(NULL), m_fullCountSemaphore(NULL), m_emptyCountSemaphore(NULL), m_isCreateSuccess(false)
{
	InitializeCreateAndOpen();
}

FileMapping::~FileMapping()
{
	FinalizeCloseAndDestory();
}


bool FileMapping::Reset()
{
	if (NULL == m_memoryAddress)
	{
		return false;
	}
	if (WAIT_OBJECT_0 == ::WaitForSingleObject(m_fullCountSemaphore, kWaitMilisecReadOrWriteBeforeFailing))
	{
		;
	}
	::ReleaseSemaphore(m_emptyCountSemaphore, 1, 0);
	return false;
}

bool FileMapping::InitializeCreateAndOpen()
{
	bool isOk = true;
	do {
		m_fileSize = kSharedBufferSize;
		HANDLE hSharedMap = ::CreateFileMapping(INVALID_HANDLE_VALUE, // 创建一个与物理文件无关的句柄
			NULL, // 文件映射对象获得默认的安全描述
			PAGE_READWRITE | SEC_COMMIT,
			0,
			static_cast<DWORD>(m_fileSize),
			kSharedBufferName.c_str());
		if (NULL == hSharedMap || INVALID_HANDLE_VALUE == hSharedMap)
		{
			PLOG_ERROR << "Create file mapping failed.";
			isOk = false; break;
		}
		m_sharedMap = hSharedMap;
		m_memoryAddress = ::MapViewOfFile(m_sharedMap, FILE_MAP_ALL_ACCESS, 0, 0, m_fileSize);
		if (NULL == m_memoryAddress)
		{
			PLOG_ERROR << "Map view of file failed.";
			isOk = false; break;
		}
		memset(m_memoryAddress, 0, m_fileSize);
		PLOG_INFO << "Create shared memory map success.";

		m_emptyCountSemaphore = ::CreateSemaphore(NULL, 1, 1, kEmptyCountSemaphoreName.c_str());
		if (NULL == m_emptyCountSemaphore)
		{
			PLOG_ERROR << "Empty count semaphore create failed.";
			isOk = false; break;
		}
		PLOG_INFO << "Empty count semaphore create success.";
		m_fullCountSemaphore = ::CreateSemaphore(NULL, 0, 1, kFullCountSemaphoreName.c_str());
		if (NULL == m_fullCountSemaphore)
		{
			PLOG_ERROR << "Full count semaphore create failed.";
			isOk = false; break;
		}
		PLOG_INFO << "Full count semaphore create success.";
		m_mutex = ::CreateMutex(NULL, FALSE, kMutexName.c_str());
		if (NULL == m_mutex)
		{
			PLOG_ERROR << "Mutex create failed.";
			isOk = false; break;
		}
		PLOG_INFO << "Mutex create success.";
	} while (false);

	if (isOk)
	{
		m_isCreateSuccess = true;
		return true;
	}
	else
	{
		FinalizeCloseAndDestory();
		return false;
	}
}

bool FileMapping::FinalizeCloseAndDestory()
{
	if (NULL != m_memoryAddress)
	{
		if (FALSE == ::UnmapViewOfFile(m_memoryAddress))
		{
			return false;
		}
		m_memoryAddress = NULL;
	}
	if (NULL != m_sharedMap)
	{
		if (FALSE == ::CloseHandle(m_sharedMap))
		{
			return false;
		}
		m_sharedMap = NULL;
	}
	if (FALSE == ::DeleteFile(kSharedBufferName.c_str()))
	{
		return false;
	}

	if (NULL != m_emptyCountSemaphore)
	{
		if (FALSE == ::CloseHandle(m_emptyCountSemaphore))
		{
			return false;
		}
		m_emptyCountSemaphore = NULL;
	}
	if (NULL != m_fullCountSemaphore)
	{
		if (FALSE == ::CloseHandle(m_fullCountSemaphore))
		{
			return false;
		}
		m_fullCountSemaphore = NULL;
	}
	if (NULL != m_mutex)
	{
		if (FALSE == ::CloseHandle(m_mutex))
		{
			return false;
		}
		m_mutex = NULL;
	}
	return true;
}

FileMappingDropbox::FileMappingDropbox()
{
	InitializeOpen();
}

FileMappingDropbox::~FileMappingDropbox()
{
	FinalizeClose();
}

bool FileMappingDropbox::WriteData(const std::vector<uint16_t> &data)
{
	bool isWriteFailed = false;
	if (WAIT_OBJECT_0 == ::WaitForSingleObject(m_emptyCountSemaphore, kWaitMilisecReadOrWriteBeforeFailing))
	{
		if (WAIT_OBJECT_0 == ::WaitForSingleObject(m_mutex, kWaitMilisecReadOrWriteBeforeFailing))
		{
			try
			{
				void *address = m_memoryAddress;
				std::size_t dataSize = data.size();
				memcpy(address, &dataSize, sizeof(dataSize));
				address = reinterpret_cast<char *>(address) + sizeof(dataSize);
				memcpy(address, data.data(), dataSize * sizeof(data.front()));
				PLOG_INFO << "Write shared memory success.";
			}
			catch (...)
			{
				PLOG_ERROR << "Write shared memory has some unknown exception.";
				isWriteFailed = true;
			}
			::ReleaseMutex(m_mutex);
		}
		else
		{
			LOG_WARNING << "Write data failed. Could not acquire lock. Please try again later.";
			isWriteFailed = true;
		}
	}
	else
	{
		LOG_WARNING << "Write data failed, Previous content has not yet read.";
		return false;
	}
	if (isWriteFailed)
	{
		::ReleaseSemaphore(m_emptyCountSemaphore, 1, 0);
		return false;
	}
	else
	{
		::ReleaseSemaphore(m_fullCountSemaphore, 1, 0);
		return true;
	}
}

bool FileMappingDropbox::ReadData(std::vector<uint16_t> &data)
{
	bool isReadFailed = false;
	if (WAIT_OBJECT_0 == ::WaitForSingleObject(m_fullCountSemaphore, kWaitMilisecReadOrWriteBeforeFailing))
	{
		if (WAIT_OBJECT_0 == ::WaitForSingleObject(m_mutex, kWaitMilisecReadOrWriteBeforeFailing))
		{
			try
			{
				void *address = m_memoryAddress;
				std::size_t dataSize = 0;
				memcpy(&dataSize, address, sizeof(dataSize));
				address = reinterpret_cast<char *>(address) + sizeof(dataSize);
				data.resize(dataSize);
				memcpy(data.data(), address, dataSize * sizeof(data.front()));
				PLOG_INFO << "Read shared memory success.";
			}
			catch (...)
			{
				PLOG_ERROR << "Read shared memory has some unknown exception.";
				isReadFailed = true;
			}
			::ReleaseMutex(m_mutex);
		}
		else
		{
			LOG_WARNING << "Read data failed. Could not acquire lock. Please try again later.";
			isReadFailed = true;
		}
	}
	else
	{
		LOG_WARNING << "Read data failed, no content has been written.";
		return false;
	}
	if (isReadFailed)
	{
		::ReleaseSemaphore(m_fullCountSemaphore, 1, 0);
		return false;
	}
	else
	{
		::ReleaseSemaphore(m_emptyCountSemaphore, 1, 0);
		return true;
	}
}

bool FileMappingDropbox::InitializeOpen()
{
	bool isOk = true;
	do {
		m_fileSize = kSharedBufferSize;
		HANDLE hSharedMap = ::OpenFileMapping(FILE_MAP_ALL_ACCESS,
			FALSE,
			kSharedBufferName.c_str());
		if (NULL == hSharedMap || INVALID_HANDLE_VALUE == hSharedMap)
		{
			isOk = false; break;
		}
		m_sharedMap = hSharedMap;
		m_memoryAddress = ::MapViewOfFile(m_sharedMap, FILE_MAP_ALL_ACCESS, 0, 0, m_fileSize);
		if (NULL == m_memoryAddress)
		{
			isOk = false; break;
		}

		m_emptyCountSemaphore = ::OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, kEmptyCountSemaphoreName.c_str());
		if (NULL == m_emptyCountSemaphore)
		{
			isOk = false; break;
		}
		m_fullCountSemaphore = ::OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, kFullCountSemaphoreName.c_str());
		if (NULL == m_fullCountSemaphore)
		{
			isOk = false; break;
		}
		m_mutex = ::OpenMutex(MUTEX_ALL_ACCESS, FALSE, kMutexName.c_str());
		if (NULL == m_mutex)
		{
			isOk = false; break;
		}
	} while (false);

	if (isOk)
	{
		return true;
	}
	else
	{
		FinalizeClose();
		return false;
	}
}

bool FileMappingDropbox::FinalizeClose()
{
	if (NULL != m_memoryAddress)
	{
		if (FALSE == ::UnmapViewOfFile(m_memoryAddress))
		{
			return false;
		}
		m_memoryAddress = NULL;
	}
	if (NULL != m_sharedMap)
	{
		if (FALSE == ::CloseHandle(m_sharedMap))
		{
			return false;
		}
		m_sharedMap = NULL;
	}
	if (FALSE == ::DeleteFile(kSharedBufferName.c_str()))
	{
		return false;
	}

	if (NULL != m_emptyCountSemaphore)
	{
		if (FALSE == ::CloseHandle(m_emptyCountSemaphore))
		{
			return false;
		}
		m_emptyCountSemaphore = NULL;
	}
	if (NULL != m_fullCountSemaphore)
	{
		if (FALSE == ::CloseHandle(m_fullCountSemaphore))
		{
			return false;
		}
		m_fullCountSemaphore = NULL;
	}
	if (NULL != m_mutex)
	{
		if (FALSE == ::CloseHandle(m_mutex))
		{
			return false;
		}
		m_mutex = NULL;
	}
	return true;
}
