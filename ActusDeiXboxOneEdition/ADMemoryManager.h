#pragma once
#include "pch.h"
#include <stdlib.h>
#include <crtdbg.h>
#include <cstring>
#include <memory>
#include <assert.h>
#include <string>

namespace
{
	const int ADMEMORY_ARRAY_SIZE = 1000;
	const long long FIVE_GB = 4000000000;
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
	size_t memPointerIndex = 0;								// Current index of the memPointers array
	size_t sizes[ADMEMORY_ARRAY_SIZE] = {};					// Array that stores the sizes of all the objects that were allocated
	size_t sizesIndex = 0;									// Current index of the sizes array
	char* handles[ADMEMORY_ARRAY_SIZE] = {};				// Array the holds indirect paths to memory
	size_t handleIndex = 0;									// Current index of the handles array
	size_t availableHandle = 0;								// Current available handle for the Handle class

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
		if (!ptr)
			assert(NULL);
		return ptr;
	}

	Type& operator[](int index)
	{
		ptr = (Type*)memoryManager.GetHandleObject(handleIndex);
		if (!ptr)
			assert(NULL);
		return ptr[index];
	}

	Type* operator->()
	{
		ptr = (Type*)memoryManager.GetHandleObject(handleIndex);
		if (!ptr)
			assert(NULL);
		return ptr;
	}

	Type& operator*()
	{
		ptr = (Type*)memoryManager.GetHandleObject(handleIndex);
		if (!ptr)
			assert(NULL);
		return *ptr;
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
		if (handleIndex >= 0)
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
		bool isString = false;
		if (std::is_same<Type, std::string>::value)
			isString = true;
		if (_capacity == 0)
		{
			_capacity = 2;
			_array.NewArray(_capacity);
		}
		else if (_size == _capacity)
			resize(_size * 2, false);

		if (isString)
			memcpy(&_array[_size++], &var, sizeof(var));
		else
			_array[_size++] = var;
	}

	void resize(unsigned int newCapacity, bool isCalled = true)
	{
		bool isFirst = false;
		if (_capacity == 0)
			isFirst = true;
		if (newCapacity > _capacity)
			_capacity = newCapacity;
		else
			return;

		if (isFirst)
		{
			_array.NewArray(newCapacity);
		}
		else
		{
			Handle<Type> tempData(_capacity);
			memcpy(tempData.GetPointer(), _array.GetPointer(), _size * sizeof(Type));
			_array.Delete();
			_array.NewArray(_capacity);
			memcpy(_array.GetPointer(), tempData.GetPointer(), _size * sizeof(Type));
			tempData.Delete();
		}
		if (isCalled)
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