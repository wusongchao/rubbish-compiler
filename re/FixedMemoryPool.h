#pragma once
#include <type_traits>
#include <memory>
#include <new>

using std::unique_ptr;
using std::make_unique;

// some bullshit

// we choose to let the user define the block size he/she need
// since the user could choose different strategy when dealing with
// different context or different size object
// so in some cases, it can't be determined at compile time

// Let the MemoryPool class to determine where to allocate 
// and the block size.

// What we need?
// a pointer to identify the "head" block
// a flag to determine the "free" pointer position(also help to destruction)
// a flag to help us judge whether the block is full or not(need?)
// -> since we pass the blockSize as a argument to MemoryPool
// (though in many case, the blockSize comes from a default parameter)
// so we only need to compare the "free" position with the blockSize
// to judge whether the block is full or not

// The class MemoryBlock should be completely managed by class MemoryPool
// Similar to package private accessibility
// Just a private nested class that can ensure that only the FixedMemoryPool can access the MemoryPool
// Don't need to complicate the problem and using method like: http://seanmiddleditch.com/enabling-make_unique-with-private-constructors/
// like below

//template
//<typename T>
//class MemoryBlock
//{
//    friend class FixedMemoryPool<T>;
//    struct _ConstructorTag { explicit _ConstructorTag() = default };
//public:
//    MemoryBlock(size_t blockSize, _ConstructorTag)
//    {
//        // ...
//    }
//};
//template
//<typename T>
//class FixedMemoryPool
//{
//    FixedMemoryPool(size_t blockSize = 64)
//        :blockSize(blockSize), head(make_unique<MemoryBlock<T>>(blockSize, MemoryBlock<T>::_ConstructorTag{}))
//    {
//    }
//};

template
<typename T>
class FixedMemoryPool 
{
    template<typename T>
    struct MemoryBlock
    {
        friend class FixedMemoryPool<T>;
        using blockPtr = unique_ptr<MemoryBlock<T>>;
        //struct _ConstructorTag { explicit _ConstructorTag() = default; };
        MemoryBlock(size_t blockSize)
            :data(reinterpret_cast<T*>(operator new(sizeof(T) * blockSize))), allocatedCounter(0), next(nullptr)
        {
        }

        ~MemoryBlock()
        {
            for (size_t i = 0; i != allocatedCounter; i++) {
                (data + i) -> ~T();
            }
            operator delete(data);
        }
        T* data;
        // raw pointer for these reasons:
        // 1. The interface of MemoryPool return raw pointer
        // 2. The semantic of MemoryPool's newElement is shared_ptr
        // 3. Even if use the smart pointer, We have to define own delete startegy

        size_t allocatedCounter;

        blockPtr next;

        template
        <typename... Args>
        T* construct(Args&&... args)
        {
            return new (data + (allocatedCounter++)) T(std::forward<Args>(args)...);
        }

        MemoryBlock(const MemoryBlock&) = delete;
        MemoryBlock& operator=(const MemoryBlock&) = delete;
    };

public:
    using valueType = T;

    FixedMemoryPool(size_t blockSize = 64)
        :blockSize(blockSize), head(make_unique<MemoryBlock<T>>(blockSize))
    {
    }

    FixedMemoryPool(const FixedMemoryPool&) = delete;
    FixedMemoryPool& operator=(const FixedMemoryPool&) = delete;

    FixedMemoryPool(FixedMemoryPool&& pool) = delete;
    FixedMemoryPool& operator=(FixedMemoryPool&&) = delete;

    template
    <typename... Args>
    T* newElement(Args&&... args)
    {
        if (head->allocatedCounter >= blockSize) {
            expand();
        }
        return head->construct(std::forward<Args>(args)...);
    }

    ~FixedMemoryPool() = default;
private:
    void expand()
    {
        auto newBlock = make_unique<MemoryBlock<T>>(blockSize);
        newBlock->next = std::move(head);
        head = std::move(newBlock);
    }

    size_t blockSize;

    unique_ptr<MemoryBlock<T>> head;
};
