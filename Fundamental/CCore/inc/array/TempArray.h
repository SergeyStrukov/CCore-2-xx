/* TempArray.h */
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

#ifndef CCore_inc_array_TempArray_h
#define CCore_inc_array_TempArray_h

#include <CCore/inc/MemBase.h>

#include <CCore/inc/algon/ApplyToRange.h>

namespace CCore {

/* classes */

template <class T,ulen StackLen,class=Meta::EnableIf< Meta::IsPOD<T> > > class TempArray;

/* class TempArray<T,ulen StackLen> */

template <class T,ulen StackLen,class>
class TempArray : NoCopy
 {
   T *ptr;
   ulen len;

   T buf[StackLen];

  private:

   void init(ulen len_) // len_ <= StackLen
    {
     ptr=buf;
     len=len_;
    }

   void alloc(ulen len_) // len_ > StackLen
    {
     ptr=static_cast<T *>(MemAlloc(LenOf(len_,sizeof (T))));

     len=len_;
    }

   void free()
    {
     if( ptr!=buf ) MemFree(ptr);
    }

   void recreate(ulen len_)
    {
     if( len_<=StackLen )
       {
        free();

        init(len_);
       }
     else
       {
        T *old_ptr=ptr;

        alloc(len_);

        if( old_ptr!=buf ) MemFree(old_ptr);
       }
    }

  public:

   // constructors

   TempArray() noexcept { init(0); }

   explicit TempArray(ulen len_)
    {
     if( len_<=StackLen )
       {
        init(len_);
       }
     else
       {
        alloc(len_);
       }
    }

   ~TempArray() { free(); }

   // methods

   void provide(ulen len_)
    {
     if( len_<=len ) return;

     recreate(len_);
    }

   void reset(ulen len_)
    {
     if( len_<=len )
       {
        len=len_;

        return;
       }

     recreate(len_);
    }

   bool extend(ulen len_)
    {
     if( len_<=len ) return false;

     if( len_<=StackLen )
       {
        len=len_;
       }
     else
       {
        T *old_ptr=ptr;
        ulen old_len=len;

        alloc(len_);

        Range(ptr,old_len).copy(old_ptr);

        if( old_ptr!=buf ) MemFree(old_ptr);
       }

     return true;
    }

   // range access

   T * getPtr() { return ptr; }

   const T * getPtr() const { return ptr; }

   const T * getPtr_const() const { return ptr; }

   ulen getLen() const { return len; }

   // index access

   T & operator [] (ulen index)
    {
     return ptr[index];
    }

   const T & operator [] (ulen index) const
    {
     return ptr[index];
    }

   T & at(ulen index)
    {
     GuardIndex(index,len);

     return ptr[index];
    }

   const T & at(ulen index) const
    {
     GuardIndex(index,len);

     return ptr[index];
    }

   // apply

   template <class FuncInit>
   void apply(FuncInit func_init) { Algon::ApplyToRange(Range(*this),func_init); }

   template <class FuncInit>
   void apply(FuncInit func_init) const { Algon::ApplyToRange(Range(*this),func_init); }

   template <class FuncInit>
   void apply_const(FuncInit func_init) const { Algon::ApplyToRange(Range(*this),func_init); }

   template <class FuncInit>
   void applyReverse(FuncInit func_init) { Algon::ApplyToRange(RangeReverse(*this),func_init); }

   template <class FuncInit>
   void applyReverse(FuncInit func_init) const { Algon::ApplyToRange(RangeReverse(*this),func_init); }

   template <class FuncInit>
   void applyReverse_const(FuncInit func_init) const { Algon::ApplyToRange(RangeReverse(*this),func_init); }
 };

} // namespace CCore

#endif


