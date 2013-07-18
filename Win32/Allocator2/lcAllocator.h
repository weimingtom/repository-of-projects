#ifndef _LUCID_ALLOCATOR_H_
#define _LUCID_ALLOCATOR_H_

#include "lcException.h"
#include "lcSingleton.h"
#include "lcBackwardCompatibility.h"
#include "lcMutex.h"
//如果你想在预分配的内存上创建对象，用缺省的new操作符是行不通的。要解决这个问题，你可以用placement new构造。
#include <new>

//refer to:SGIMallocPool.hhp

namespace lc{

//TODO 此为非公开类

//refer to:http://holos.googlecode.com/svn/trunk/src/h_MemTracer.h
class MemoryTracer : public core::Singleton<MemoryTracer>,public Obsolete<MemoryTracer>{
private:
	struct TracerNode{
		TracerNode():Prev(NULL),Next(NULL),Addr(NULL),Size(0),File(NULL),Func(NULL),Line(-1){}
		TracerNode(void* p,u32 size):Prev(NULL),Next(NULL),Addr(p),Size(size),File(NULL),Func(NULL),Line(-1){}
		TracerNode(void* p,u32 size,const c8* file,const c8* func,s32 line):Prev(NULL),Next(NULL),Addr(p),Size(size),File(file),Func(func),Line(line){}

		TracerNode* Prev;
		TracerNode* Next;

		void*		Addr;
		u32			Size;
		const c8*	File;
		const c8*	Func;
		s32			Line;
	};

	static mutex s_mutex;

	TracerNode* m_pHead;
	TracerNode* m_pTail;

	u32			m_uAllocatedSize;
	u32			m_uAllocatedCount;
	u32			m_uMaxSize;

	MemoryTracer();
	static void lock();
	static void unlock();
	bool canAllocate(size_t sz);
	void addNode(TracerNode* node);
	void removeNode(TracerNode* node);
	void deallocateNode(TracerNode* node);

	friend class core::Singleton<MemoryTracer>;
	friend void* ::operator new(LC_ALLOC_PARAMS(size_t size));
	friend void ::operator delete(void* ptr);
public:

	void allocate(LC_ALLOC_PARAMS(void* p,size_t sz));
	void deallocate(void* ptr);
	u32 getAllocatedSize() const{return m_uAllocatedSize;}
	u32 getAllocatedCount() const{return m_uAllocatedCount;}
	void setMaxSize(size_t sz){m_uMaxSize=sz;}
	u32 getMaxSize() const{return m_uMaxSize;}
	void dump();
	virtual void destroy();
};

template<bool isMT,size_t size>
class FixedMemoryPool : public Uncopyable{
private:
	//ROUND_UP 将bytes上调至__ALIGN的倍数
	enum { ROUND = (size + s_uAlign - 1) & ~(s_uAlign - 1) };

	union FixedNode
	{
		union FixedNode* Node;
		c8 Data[1];
	};
	typedef typename VolatileTrait<FixedNode, isMT>::ValueType	FixedNodePtr;
	typedef typename LockTrait<isMT>::ValueType LockType;

	static const u32 s_uGrowth;
	static const u32 s_uAlign;

	LockType m_lock;

	// 内存池起始位置
	c8 *m_pStartFree;
	// 内存池结束位置
	c8 *m_pEndFree;
	// 所分配的空间大小
	u32 m_uCapacity;

	// free数组
	FixedNodePtr m_pFrees[1];

	// roundup 将bytes上调至s_uAlign的倍数
	static inline u32 roundup(u32 bytes)
	{
		return ((bytes) + s_uAlign - 1) & ~(s_uAlign - 1);
	}

	// 根据区块大小，决定使用第n号frees。n从0号起
	static inline size_t indexInFrees(size_t bytes)
	{
		return (((bytes) + s_uAlign - 1) / s_uAlign) - 1;
	}

	// !配置一块大空间,可容纳count个大小为unit的区块
	// 这些内存在物理地址上是连在一起的, 返回其指针
	c8* allocateChunk(size_t unit,size_t& count);

	// 返回一个大小为n的对象,并加入大小为n的其他区块到free队列中
	FixedNode* refill(size_t n);

	// 清空内存
	void clear();
public:
	FixedMemoryPool();
	~FixedMemoryPool();

	// n必须大于0
	void* allocate(size_t n);

	// p不能为空
	void deallocate(void *p, size_t n);
};

struct PrimitiveAllocateTrait{
	static void* allocate(size_t size);
	static void* allocate(size_t size,const c8* file,const c8* func,s32 line);
	static void deallocate(void *p, size_t size);
};

struct FixedSizeAllocateTrait{
	static void* allocate(size_t size);
	static void* allocate(size_t size,const c8* file,const c8* func,s32 line);
	static void deallocate(void *p, size_t size);
};

//TODO mem namespace
enum ENUM_ALLOCATE_STRATEGY{
	ENUM_ALLOCATE_STRATEGY_PRIMITIVE = 0,
	ENUM_ALLOCATE_STRATEGY_FIXED_SIZE,
	ENUM_ALLOCATE_STRATEGY_COUNT
};

template<ENUM_ALLOCATE_STRATEGY strategy>
struct AllocateTrait
{
	typedef PrimitiveAllocateTrait ValueType;
};

template<>
struct AllocateTrait<ENUM_ALLOCATE_STRATEGY_FIXED_SIZE>
{
	typedef FixedSizeAllocateTrait ValueType;
};

template<typename T,ENUM_ALLOCATE_STRATEGY strategy,bool isMT>
class Allocator{
	typedef typename AllocateTrait<strategy>::ValueType AllocateType;
public:
	T* allocate(size_t cnt)
	{
		return cnt==0?NULL:(T*)AllocateType::allocate(cnt* sizeof(T));
	}

	T* allocate(size_t cnt,const c8* file,const c8* func,s32 line)
	{
		return cnt==0?NULL:(T*)AllocateType::allocate(cnt* sizeof(T),file,func,line);
	}

	//! Deallocate memory for an array of objects
	void deallocate(T* ptr)
	{
		if(ptr==NULL)
			return;
		AllocateType::deallocate(ptr,sizeof(T));
	}

	//! Construct an element
	void construct(T* ptr, const T&e)
	{
		new ((void*)ptr) T(e);
	}

	//! Destruct an element
	void destruct(T* ptr)
	{
		ptr->~T();
	}
};

}

#endif