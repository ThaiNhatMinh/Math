#ifndef __STACK_ALLOCATOR__
#define __STACK_ALLOCATOR__
#include <stdlib.h>

// This a simple class manage stack alloc for some object in game
// And there is only one instace is created

class StackAllocator
{
public:
	StackAllocator(size_t totalByte);
	~StackAllocator();

	size_t GetCurrentOfset(){ return m_CurrentOffset;}
	void Reset(size_t offset = 0){ m_CurrentOffset = offset;}
	void* Alloc(size_t bytes);
	static StackAllocator* mAllocator;

private:
	StackAllocator();

	unsigned char* m_Alloction;
	size_t m_TotalByte;
	size_t m_CurrentOffset;
};


#endif