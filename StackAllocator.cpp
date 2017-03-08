#include "StackAllocator.h"
#include <assert.h>
StackAllocator* StackAllocator::mAllocator = nullptr;

StackAllocator::StackAllocator(size_t totalByte)
{
	m_Alloction = (unsigned char*)malloc(totalByte);
	m_TotalByte = totalByte;
	m_CurrentOffset = 0;

}


StackAllocator::~StackAllocator()
{
	free(m_Alloction);
}

void* StackAllocator::Alloc(size_t bytes)
{
	if(m_CurrentOffset + bytes > m_TotalByte)
	{
		assert(false);
		return nullptr;
	}
	void* alloc = m_Alloction + m_CurrentOffset;
	m_CurrentOffset +=bytes;
	return alloc;
}


