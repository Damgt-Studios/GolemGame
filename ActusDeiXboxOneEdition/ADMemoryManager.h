#pragma once
#include "pch.h"
#include <stdlib.h>
#include <crtdbg.h>
#include <cstring>
#include <memory>
#include <assert.h>
#include <string>
#include <exception>

namespace
{
	const int ADMEMORY_ARRAY_SIZE = 1000;
	const long long FIVE_GB = 500000000;
}

class MemoryManager
{
public:
	MemoryManager();
	~MemoryManager();
	void* Allocate(size_t size);							// Allocates a certain amount of the memoryBuffer for variable
	void Deallocate(void* object, size_t hIndex);			// Returns the memory from the deleted object back into the memoryBuffer
	size_t GetAvailableHandle();							// Returns current avaiable handle
	void* GetHandleObject(size_t index);					// Returns the object at the specified index of the handles array
private:
	char* memoryBuffer;										// Buffer that holds 5 GB of allocated memory
	long long allocatedSize;								// Variable that holds the current amount of memory being used
	char* memPointers[ADMEMORY_ARRAY_SIZE] = {};			// Array that holds the pointers to different locations in the memory buffer
	size_t memPointerIndex;									// Current index of the memPointers array
	size_t sizes[ADMEMORY_ARRAY_SIZE] = {};					// Array that stores the sizes of all the objects that were allocated
	size_t sizesIndex;										// Current index of the sizes array
	char* handles[ADMEMORY_ARRAY_SIZE] = {};				// Array the holds indirect paths to memory
	size_t handleIndex;										// Current index of the handles array
	size_t availableHandle;									// Current available handle for the Handle class

	size_t GetNextHandle();									// Helper function that returns the next available index of the handles array
};

extern MemoryManager memoryManager;

template<typename Type>
class Handle
{
public:
	Handle()
	{
		isArray = false;
		ptr = nullptr;
		handleIndex = -1;
	}

	Handle(size_t arraySize)
	{
		ptr = (Type*)memoryManager.Allocate(arraySize * sizeof(Type));
		handleIndex = memoryManager.GetAvailableHandle();
		isArray = true;
	}

	~Handle()
	{
		Delete();
	}

	Type* GetPointer()
	{
		ptr = (Type*)memoryManager.GetHandleObject(handleIndex);
		assert(ptr);
		return ptr;
	}

	Type& operator[](int index)
	{
		ptr = (Type*)memoryManager.GetHandleObject(handleIndex);
		assert(ptr);
		return ptr[index];
	}

	Type* operator->()
	{
		ptr = (Type*)memoryManager.GetHandleObject(handleIndex);
		assert(ptr);
		return ptr;
	}

	Type& operator*()
	{
		ptr = (Type*)memoryManager.GetHandleObject(handleIndex);
		assert(ptr);
		return *ptr;
	}

	void EqualPointer(Type* tPtr)
	{
		if (ptr)
			Delete();
		ptr = (Type*)memoryManager.Allocate(sizeof(Type));
		memcpy(ptr, tPtr, sizeof(Type));
		handleIndex = memoryManager.GetAvailableHandle();
		delete tPtr;
	}

	void EqualArray(Type* tPtr, size_t arraySize)
	{
		if (ptr)
			Delete();
		ptr = (Type*)memoryManager.Allocate(sizeof(Type) * arraySize);
		memcpy(ptr, tPtr, sizeof(Type) * arraySize);
		handleIndex = memoryManager.GetAvailableHandle();
		delete[] tPtr;
	}

#pragma region New()

	// Allocates a required chunk of memory from the memory buffer
	// If the array constructor is called this function isn't neccessary
	// CALL THIS FUNCTION BEFORE CALLING ANYTHING ELSE
	void New()
	{
		if (!isArray)
		{
			Type localVar;
			ptr = (Type*)memoryManager.Allocate(sizeof(localVar));
			memcpy(ptr, &localVar, sizeof(localVar));
			handleIndex = memoryManager.GetAvailableHandle();
		}
	}

	// Allocates a required chunk of memory from the memory buffer
	// If the array constructor is called this function isn't neccessary
	// CALL THIS FUNCTION BEFORE CALLING ANYTHING ELSE
	// 1 parameter version
	template <typename First>
	void New(First firstParam)
	{
		if (!isArray)
		{
			Type localVar(firstParam);
			ptr = (Type*)memoryManager.Allocate(sizeof(localVar));
			memcpy(ptr, &localVar, sizeof(localVar));
			handleIndex = memoryManager.GetAvailableHandle();
		}
	}

	// Allocates a required chunk of memory from the memory buffer
	// If the array constructor is called this function isn't neccessary
	// CALL THIS FUNCTION BEFORE CALLING ANYTHING ELSE
	// 2 parameter version
	template <typename First, typename Second>
	void New(First firstParam, Second secondParam)
	{
		if (!isArray)
		{
			Type localVar(firstParam, secondParam);
			ptr = (Type*)memoryManager.Allocate(sizeof(localVar));
			memcpy(ptr, &localVar, sizeof(localVar));
			handleIndex = memoryManager.GetAvailableHandle();
		}
	}

	// Allocates a required chunk of memory from the memory buffer
	// If the array constructor is called this function isn't neccessary
	// CALL THIS FUNCTION BEFORE CALLING ANYTHING ELSE
	// 3 parameter version
	template <typename First, typename Second, typename Third>
	void New(First firstParam, Second secondParam, Third thirdParam)
	{
		if (!isArray)
		{
			Type localVar(firstParam, secondParam, thirdParam);
			ptr = (Type*)memoryManager.Allocate(sizeof(localVar));
			memcpy(ptr, &localVar, sizeof(localVar));
			handleIndex = memoryManager.GetAvailableHandle();
		}
	}

	// Allocates a required chunk of memory from the memory buffer
	// If the array constructor is called this function isn't neccessary
	// CALL THIS FUNCTION BEFORE CALLING ANYTHING ELSE
	// 4 parameter version
	template <typename First, typename Second, typename Third, typename Fourth>
	void New(First firstParam, Second secondParam, Third thirdParam, Fourth fourthParam)
	{
		if (!isArray)
		{
			Type localVar(firstParam, secondParam, thirdParam, fourthParam);
			ptr = (Type*)memoryManager.Allocate(sizeof(localVar));
			memcpy(ptr, &localVar, sizeof(localVar));
			handleIndex = memoryManager.GetAvailableHandle();
		}
	}

	// Allocates a required chunk of memory from the memory buffer
	// If the array constructor is called this function isn't neccessary
	// CALL THIS FUNCTION BEFORE CALLING ANYTHING ELSE
	// 5 parameter version
	template <typename First, typename Second, typename Third, typename Fourth, typename Fifth>
	void New(First firstParam, Second secondParam, Third thirdParam, Fourth fourthParam, Fifth fifthParam)
	{
		if (!isArray)
		{
			Type localVar(firstParam, secondParam, thirdParam, fourthParam, fifthParam);
			ptr = (Type*)memoryManager.Allocate(sizeof(localVar));
			memcpy(ptr, &localVar, sizeof(localVar));
			handleIndex = memoryManager.GetAvailableHandle();
		}
	}

	// Allocates a required chunk of memory from the memory buffer
	// If the array constructor is called this function isn't neccessary
	// CALL THIS FUNCTION BEFORE CALLING ANYTHING ELSE
	// 6 parameter version
	template <typename First, typename Second, typename Third, typename Fourth, typename Fifth, typename Sixth>
	void New(First firstParam, Second secondParam, Third thirdParam, Fourth fourthParam, Fifth fifthParam, Sixth sixthParam)
	{
		if (!isArray)
		{
			Type localVar(firstParam, secondParam, thirdParam, fourthParam, fifthParam, sixthParam);
			ptr = (Type*)memoryManager.Allocate(sizeof(localVar));
			memcpy(ptr, &localVar, sizeof(localVar));
			handleIndex = memoryManager.GetAvailableHandle();
		}
	}

	// Allocates a required chunk of memory from the memory buffer
	// If the array constructor is called this function isn't neccessary
	// CALL THIS FUNCTION BEFORE CALLING ANYTHING ELSE
	// 7 parameter version
	template <typename First, typename Second, typename Third, typename Fourth, typename Fifth, typename Sixth, typename Seventh>
	void New(First firstParam, Second secondParam, Third thirdParam, Fourth fourthParam, Fifth fifthParam, Sixth sixthParam, Seventh seventhParam)
	{
		if (!isArray)
		{
			Type localVar(firstParam, secondParam, thirdParam, fourthParam, fifthParam, sixthParam, seventhParam);
			ptr = (Type*)memoryManager.Allocate(sizeof(localVar));
			memcpy(ptr, &localVar, sizeof(localVar));
			handleIndex = memoryManager.GetAvailableHandle();
		}
	}

	// Allocates a required chunk of memory from the memory buffer
	// If the array constructor is called this function isn't neccessary
	// CALL THIS FUNCTION BEFORE CALLING ANYTHING ELSE
	// 8 parameter version
	template <typename First, typename Second, typename Third, typename Fourth, typename Fifth, typename Sixth, typename Seventh, typename Eighth>
	void New(First firstParam, Second secondParam, Third thirdParam, Fourth fourthParam, Fifth fifthParam, Sixth sixthParam, Seventh seventhParam, Eighth eighthParam)
	{
		if (!isArray)
		{
			Type localVar(firstParam, secondParam, thirdParam, fourthParam, fifthParam, sixthParam, seventhParam, eighthParam);
			ptr = (Type*)memoryManager.Allocate(sizeof(localVar));
			memcpy(ptr, &localVar, sizeof(localVar));
			handleIndex = memoryManager.GetAvailableHandle();
		}
	}

	// Array version of New() if array constructor isn't called
	// Allocates a required chunk of memory from the memory buffer
	// If the array constructor is called this function isn't neccessary
	// CALL THIS FUNCTION BEFORE CALLING ANYTHING ELSE
	void NewArray(size_t arraySize)
	{
		if (!isArray)
		{
			ptr = (Type*)memoryManager.Allocate(arraySize * sizeof(Type));
			handleIndex = memoryManager.GetAvailableHandle();
			isArray = true;
		}
	}

#pragma endregion

	// Deallocates the previously allocated memory
	// CALL THIS FUNCTION ONLY AFTER CALLING ANY VARIATION OF New()
	void Delete()
	{
		if (handleIndex >= 0 && handleIndex < ADMEMORY_ARRAY_SIZE)
		{
			ptr = (Type*)memoryManager.GetHandleObject(handleIndex);
			memoryManager.Deallocate(ptr, handleIndex);
			ptr = nullptr;
			isArray = false;
			handleIndex = -1;
		}
	}
private:
	int handleIndex;
	Type* ptr;
	bool isArray;
};

template<typename Type> class ADQueue;

template <typename Type>
class ADVector
{
	friend class ADQueue<Type>;
public:
	ADVector()
	{
		_size = 0;
		_capacity = 0;
	}

	~ADVector()
	{
		clear();
	}

	Type& operator[](int index)
	{
		return _array[index];
	}

	unsigned int size()
	{
		return _size;
	}

	unsigned int capacity()
	{
		return _capacity;
	}

	void clear()
	{
		_array.Delete();
		_size = 0;
		_capacity = 0;
	}

	void push_back(Type var)
	{
		if (_capacity == 0)
		{
			_capacity = 2;
			_array.NewArray(_capacity);
		}
		else if (_size == _capacity)
			push_back_resize(_size * 2);

		if (std::is_same<Type, std::string>::value)
		{
			memcpy(&_array[_size++], &var, sizeof(var));
		}
		else
		{
			_array[_size++] = var;
		}
	}

	void resize(unsigned int newCapacity)
	{
		if (newCapacity <= _capacity)
			return;

		if (_capacity == 0)
		{
			_array.NewArray(newCapacity);
		}
		else
		{
			Handle<Type> tempData(newCapacity);
			memcpy(tempData.GetPointer(), _array.GetPointer(), _size * sizeof(Type));
			_array.Delete();
			_array.NewArray(newCapacity);
			memcpy(_array.GetPointer(), tempData.GetPointer(), _size * sizeof(Type));
			tempData.Delete();
		}
		_capacity = newCapacity;
		_size = newCapacity;
	}

	Type* data()
	{
		return _array.GetPointer();
	}
private:
	Handle<Type> _array;
	unsigned int _size;
	unsigned int _capacity;

	void push_back_resize(unsigned int newCapacity)
	{
		_capacity = newCapacity;
		Handle<Type> tempData(_capacity);
		memcpy(tempData.GetPointer(), _array.GetPointer(), _size * sizeof(Type));
		_array.Delete();
		_array.NewArray(_capacity);
		memcpy(_array.GetPointer(), tempData.GetPointer(), _size * sizeof(Type));
		tempData.Delete();
	}
};

template <typename Type>
class ADQueue
{
public:
	ADQueue()
	{

	}

	~ADQueue()
	{
		clear();
	}

	bool empty()
	{
		if (_queue._size == 0)
			return true;
		else
			return false;
	}

	unsigned int size()
	{
		return _queue.size();
	}

	Type front()
	{
		return _queue[0];
	}

	void pop()
	{
		_queue[0] = 0;
		memcpy(&_queue[0], &_queue[1], (--_queue._size) * sizeof(Type));
	}

	void push(Type var)
	{
		_queue.push_back(var);
	}

	void clear()
	{
		_queue.clear();
	}
private:
	ADVector<Type> _queue;
};