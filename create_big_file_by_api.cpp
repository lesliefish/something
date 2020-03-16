/** @file   create_big_file_by_api.cpp
 *
 *  @brief  创建指定大小的文件(使用windows api)
 *  @note
 *  @author lesliefish
 *  @date   2020/03/16
 */
#include <iostream>
#include <string>
#include "tchar.h"
#include "windows.h"

using namespace std;

bool createEmptyFile(const int iSize, const std::string& strFileName)
{
	HANDLE hFile;
	HANDLE hMapFile;
	hFile = CreateFileA(_T(strFileName.c_str()),
		GENERIC_WRITE | GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	hMapFile = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, iSize, NULL);
	if (hMapFile == NULL)
	{
		CloseHandle(hFile);
		return false;
	}

	CloseHandle(hMapFile);
	CloseHandle(hFile);

	return true;
}

int main()
{
	string strFileWholePath = "c:\\lesliefish.tmp";
	int iSize = 1024 * 1024 * 1024;

	bool b = createEmptyFile(iSize, strFileWholePath);

	return 0;
}