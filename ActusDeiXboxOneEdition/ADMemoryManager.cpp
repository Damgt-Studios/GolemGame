#include "pch.h"
#include "ADMemoryManager.h"

#ifndef AD_MEMORY_DEFAULT
MemoryManager memoryManager;

MemoryManager::MemoryManager()
{
	// Allocate 5 GB of RAM for application
	memoryBuffer = new char[FIVE_GB];
	memset(memoryBuffer, NULL, FIVE_GB);
	allocatedSize = 0;
	memPointerIndex = 0;
	sizesIndex = 0;
	handleIndex = 0;
	availableHandle = 0;
	memset(memPointers, NULL, ADMEMORY_ARRAY_SIZE * sizeof(char*));
	memset(sizes, NULL, ADMEMORY_ARRAY_SIZE * sizeof(size_t));
	memset(handles, NULL, ADMEMORY_ARRAY_SIZE * sizeof(char*));
	assert(memoryBuffer);
}

MemoryManager::~MemoryManager()
{
	delete[] memoryBuffer;
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
			availableHandle = handleIndex;
			handleIndex = GetNextHandle();
			return memPointers[memPointerIndex - 1];
		}
		else
		{
			memPointers[memPointerIndex++] = memPointers[memPointerIndex - 1] + sizes[sizesIndex - 2];
			handles[handleIndex] = memPointers[memPointerIndex - 1];
			availableHandle = handleIndex;
			handleIndex = GetNextHandle();
			return memPointers[memPointerIndex - 1];
		}
	}
	throw std::bad_alloc();
}

void MemoryManager::Deallocate(void* object, size_t hIndex)
{
	if (object)
	{
		int index = -1;
		for (int i = 0; i < ADMEMORY_ARRAY_SIZE; ++i)
		{
			if (object == memPointers[i])
			{
				index = i;
				break;
			}
		}
		if (index != -1 && hIndex != -1)
		{
			size_t size = sizes[index];
			handles[hIndex] = NULL;

			if (index == memPointerIndex - 1)
			{
				memset(memPointers[index], 0, size);
				memPointers[index] = nullptr;
				sizes[index] = 0;
				allocatedSize -= size;
				--memPointerIndex;
				--sizesIndex;
				handleIndex = GetNextHandle();
				availableHandle = handleIndex;
			}
			else
			{
				size_t sizeBeforeIndex = 0;
				for (int i = 0; i < index; ++i)
					sizeBeforeIndex += sizes[i];
				memset(memPointers[index], 0, size);
				allocatedSize -= size;
				memcpy(memPointers[index], memPointers[index] + size, allocatedSize - sizeBeforeIndex);
				for (int i = 0; i <= ADMEMORY_ARRAY_SIZE; ++i)
				{
					if (handles[i])
						if (handles[i] > memPointers[index])
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
				availableHandle = handleIndex;
			}
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
	throw std::bad_alloc();
}
#endif