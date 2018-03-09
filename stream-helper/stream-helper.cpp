#include "stdafx.h"

#define MAP_OBJECT_NAME "$pcars2$"


int main()
{

	HANDLE fileHandle = OpenFileMapping(PAGE_READONLY, FALSE, MAP_OBJECT_NAME );
	if (fileHandle == NULL)
	{
		printf("Could not open file mapping object (%d).\n", GetLastError());
		return 1;
	}

	const SharedMemory* sharedData = (SharedMemory*)MapViewOfFile(fileHandle, PAGE_READONLY, 0, 0, sizeof(SharedMemory));
	SharedMemory* localCopy = new SharedMemory;
	if (sharedData == NULL) 
	{
		printf("Could not map view of file (%d).\n", GetLastError());
		CloseHandle(fileHandle);
		return 1;
	}

	if (sharedData->mVersion != SHARED_MEMORY_VERSION)
	{
		printf("Data version mismatch\n");
		return 1;
	}


	//
	// Display code
	//
	printf("Project Cars 2 - Stream Helper Tool\n");
	printf("Version: 0.1\n");
	printf("Data Version: (%d).\n", sharedData->mVersion);
	printf("ESC TO EXIT \n\n");


	while (true)
	{
		if (_kbhit() && _getch() == 27)
		{
			break;
		}
	}
	UnmapViewOfFile(sharedData);
	CloseHandle(fileHandle);
	delete localCopy;
    return 0;
}

