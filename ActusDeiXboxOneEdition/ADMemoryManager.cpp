#include "pch.h"
#include "ADMemoryManager.h"

MemoryManager memoryManager;

MemoryManager::MemoryManager()
{
	// Allocate 5 GB of RAM for application
	//memoryBuffer = (char*)calloc(FIVE_GB, sizeof(char));
	allocatedSize = 0;
	memset(handles, NULL, ADMEMORY_ARRAY_SIZE);
}

MemoryManager::~MemoryManager()
{
	free(memoryBuffer);
}

void* MemoryManager::Allocate(size_t size)
{
	if (allocatedSize + size <= FIVE_GB) // Check to make sure there is enough memory to allocate
	{
		allocatedSize += size;
		sizes[sizesIndex++] = size;
		if (memPointerIndex == 0)
		{
			memPointers[memPointerIndex++] = memoryBuffer;
			handles[handleIndex] = memoryBuffer;
			handleIndex = GetNextHandle();
			return memPointers[memPointerIndex - 1];
		}
		else
		{
			memPointers[memPointerIndex++] = memPointers[memPointerIndex - 1] + sizes[sizesIndex - 2];
			handles[handleIndex] = memPointers[memPointerIndex - 1];
			handleIndex = GetNextHandle();
			return memPointers[memPointerIndex - 1];
		}
	}
	assert(NULL);
	return nullptr;
}

void MemoryManager::Deallocate(void* object)
{
	int index = -1;
	int hIndex = -1;
	for (int i = 0; i < ADMEMORY_ARRAY_SIZE; ++i)
	{
		if (object == memPointers[i])
		{
			index = i;
			break;
		}
	}
	for (int i = 0; i < ADMEMORY_ARRAY_SIZE; ++i)
	{
		if (object == handles[i])
		{
			hIndex = i;
			break;
		}
	}
	if (index != -1 && hIndex != -1)
	{
		size_t size = sizes[index];
		handles[hIndex] = NULL;
		availableHandle = hIndex;

		if (index == memPointerIndex - 1)
		{
			memset(memPointers[index], 0, size);
			memPointers[index] = nullptr;
			sizes[index] = 0;
			allocatedSize -= size;
			--memPointerIndex;
			--sizesIndex;
			handleIndex = GetNextHandle();
		}
		else
		{
			memset(memPointers[index], 0, size);
			allocatedSize -= size;
			memcpy(memPointers[index], memPointers[index] + size, allocatedSize);
			for (int i = 0; i <= ADMEMORY_ARRAY_SIZE; ++i)
			{
				if (handles[i])
					handles[i] -= size;
			}
			int sizeDifference;
			for (int i = index; memPointers[i + 1]; ++i)
			{
				sizeDifference = (int)size - (int)sizes[i];
				memPointers[i] -= sizeDifference;
				sizes[i] = sizes[i + 1];
			}
			memset(memPointers[0] + allocatedSize, 0, allocatedSize);
			memPointers[--memPointerIndex] = nullptr;
			sizes[--sizesIndex] = 0;
			handleIndex = GetNextHandle();
		}
	}
}

size_t MemoryManager::GetAvailableHandle()
{
	size_t returned = availableHandle;
	availableHandle = GetNextHandle();
	return returned;
}

void* MemoryManager::GetHandleObject(size_t index)
{
	return handles[index];
}

size_t MemoryManager::GetNextHandle()
{
	for (int i = 0; i < ADMEMORY_ARRAY_SIZE; ++i)
	{
		if (handles[i] == NULL)
			return i;
	}
	assert(NULL);
	return 0;
}

//void* operator new(size_t size)
//{
//	return memoryManager.Allocate(size);
//}
//
//void* operator new[](size_t size)
//{
//	return memoryManager.Allocate(size);
//}