/* ElementPool.h */
//----------------------------------------------------------------------------------------
//
//  Project: CCore 2.00
//
//  Tag: Fundamental Mini
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2015 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#ifndef CCore_inc_ElementPool_h
#define CCore_inc_ElementPool_h

#include <CCore/inc/List.h>
#include <CCore/inc/array/ArrayAlgo.h>

namespace CCore {

/* classes */

class MemPool;

class ElementPool;

/* class MemPool */

class MemPool : NoCopy
 {
   struct Node
    {
     SLink<Node> link;
    };

   static const ulen Delta = Align(sizeof (Node)) ;
   static const ulen MaxLen = AlignDown(MaxULen)-Delta ;

   using Algo = SLink<Node>::LinearAlgo<&Node::link> ;

  private:

   ulen block_len; // aligned , > Delta

   Algo::Top list;

   Place<void> block;
   Place<void> cur;
   ulen avail;

   ulen mem_cap;

  private:

   Place<void> allocBlock(ulen alloc_len);

   void newBlock();

  public:

   // constructors

   static const ulen DefaultBlockLen = 64_KByte ;

   MemPool() noexcept : MemPool(DefaultBlockLen) {}

   explicit MemPool(ulen block_len,ulen mem_cap=MaxULen);

   ~MemPool();

   // methods

   Place<void> alloc(ulen len);

   void shrink_extra();

   // swap/move objects

   void objSwap(MemPool &obj)
    {
     Swap(block_len,obj.block_len);
     Swap(list,obj.list);
     Swap(block,obj.block);
     Swap(cur,obj.cur);
     Swap(avail,obj.avail);
     Swap(mem_cap,obj.mem_cap);
    }

   explicit MemPool(ToMoveCtor<MemPool> obj)
    : block_len(obj->block_len),
      list(Replace_null(obj->list)),
      block(obj->block),
      cur(obj->cur),
      avail(obj->avail),
      mem_cap(obj->mem_cap)
    {
    }
 };

/* class ElementPool */

class ElementPool : NoCopy
 {
   MemPool pool;

  private:

   template <class T>
   Place<void> alloc(ulen len)
    {
     static_assert( Meta::HasTrivDtor<T> ,"CCore::ElementPool::createArray...<T>(...) : T must have a trivial destructor");

     return pool.alloc(LenOf(len,sizeof (T)));
    }

   struct Out
    {
     char *ptr;

     explicit Out(char *ptr_) : ptr(ptr_) {}

     void operator () () {}

     void operator () (StrLen str) { str.copyTo(ptr); ptr+=str.len; }

     template <class ... TT>
     void operator () (StrLen str,TT ... tt)
      {
       (*this)(str);
       (*this)(tt...);
      }
    };

   template <class ... TT>
   StrLen catStrLen(TT ... tt)
    {
     ulen len=LenAdd( tt.len ... );
     char *base=pool.alloc(len);

     Out out(base);

     out(tt...);

     return StrLen(base,len);
    }

  public:

   // constructors

   ElementPool() noexcept {}

   template <class ... TT>
   explicit ElementPool(TT ... tt) : pool(tt...) {}

   ~ElementPool() {}

   // methods

   template <class T,class ... SS>
   T * create(SS && ... ss)
    {
     static_assert( Meta::HasTrivDtor<T> ,"CCore::ElementPool::create<T>(...) : T must have a trivial destructor");

     return new(pool.alloc(sizeof (T))) T( std::forward<SS>(ss)... );
    }

   StrLen dup(StrLen str);

   template <class ... TT>
   StrLen cat(TT ... tt)
    {
     return catStrLen( StrLen(tt)... );
    }

   void shrink_extra() { pool.shrink_extra(); }

   // createArray

   template <class T>
   PtrLen<T> createArray(ulen len)
    {
     using Algo = ArrayAlgo<T> ;

     return Algo::Create_default(alloc<T>(len),len);
    }

   template <class T,class Creator>
   PtrLen<T> createArray(ulen len,Creator creator)
    {
     using Algo = ArrayAlgo<T> ;

     return Algo::Create(alloc<T>(len),len,creator);
    }

   template <class T>
   PtrLen<T> createArray_raw(ulen len)
    {
     using Algo = ArrayAlgo<T> ;

     return Algo::Create_raw(alloc<T>(len),len);
    }

   template <class T,class ... SS>
   PtrLen<T> createArray_fill(ulen len,SS && ... ss)
    {
     using Algo = ArrayAlgo<T> ;

     return Algo::Create_fill(alloc<T>(len),len, std::forward<SS>(ss)... );
    }

   template <class T>
   PtrLen<T> createArray_copy(ulen len,const T src[])
    {
     using Algo = ArrayAlgo<T> ;

     return Algo::Create_copy(alloc<T>(len),len,src);
    }

   template <class T>
   PtrLen<T> createArray_copy(PtrLen<const T> src)
    {
     return createArray_copy(src.len,src.ptr);
    }

   template <class T,class S>
   PtrLen<T> createArray_cast(ulen len,const S src[])
    {
     using Algo = ArrayAlgo<T> ;

     return Algo::Create_cast(alloc<T>(len),len,src);
    }

   template <class T,class S>
   PtrLen<T> createArray_cast(PtrLen<const S> src)
    {
     return createArray_cast(src.len,src.ptr);
    }

   template <class T>
   PtrLen<T> createArray_swap(ulen len,T objs[])
    {
     using Algo = ArrayAlgo<T> ;

     return Algo::Create_swap(alloc<T>(len),len,objs);
    }

   template <class T>
   PtrLen<T> createArray_swap(PtrLen<T> objs)
    {
     return createArray_swap(objs.len,objs.ptr);
    }

   // swap/move objects

   void objSwap(ElementPool &obj)
    {
     Swap(pool,obj.pool);
    }

   explicit ElementPool(ToMoveCtor<ElementPool> obj)
    : pool(ObjToMove(obj->pool))
    {
    }
 };

} // namespace CCore

#endif


