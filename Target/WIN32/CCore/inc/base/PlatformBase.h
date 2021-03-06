/* PlatformBase.h */
//----------------------------------------------------------------------------------------
//
//  Project: CCore 2.00
//
//  Tag: Target/WIN32
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2015 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#ifndef CCore_inc_base_PlatformBase_h
#define CCore_inc_base_PlatformBase_h

/* target label */

#define CCORE_TARGET WIN32

/* init priority */

#define CCORE_INITPRI_0 __attribute__((init_priority(101)))
#define CCORE_INITPRI_1 __attribute__((init_priority(102)))
#define CCORE_INITPRI_2 __attribute__((init_priority(103)))
#define CCORE_INITPRI_3 __attribute__((init_priority(104)))

/* inlining disabler */

#define CCORE_NOINLINE __attribute__((noinline))

/* includes */

#include <cstddef>
#include <type_traits>
#include <initializer_list>
#include <utility>

/* restrict */

#define restrict __restrict

/* namespace CCore */

namespace CCore {

/* consts */

const bool IsLEPlatform  = true ;

const bool Is2sComplementArithmetic = true ;

const unsigned MaxBitLen = 64   ;

const unsigned MaxAlign  = 16   ;

/* types */

using ulen = unsigned int ;
using uptr = unsigned int ;

using uint8  =      unsigned char ;
using uint16 =     unsigned short ;
using uint32 =       unsigned int ;
using uint64 = unsigned long long ;

using sint8  =        signed char ;
using sint16 =       signed short ;
using sint32 =         signed int ;
using sint64 =   signed long long ;

/* consts */

const ulen DefaultPacketCount = 50'000 ;

const unsigned DefaultEventElementCount = 10'000 ;

/* namespace Bits */

namespace Bits {

const unsigned unsigned_char      =  8 ;
const unsigned unsigned_short     = 16 ;
const unsigned unsigned_int       = 32 ;
const unsigned unsigned_long      = 32 ;
const unsigned unsigned_long_long = 64 ;

} // namespace Bits

/* namespace ExtraInt */

namespace ExtraInt {

/* classes */

template <class T> struct Prop;

/* struct Prop<T> */

template <class T>
struct Prop
 {
  enum
   {
    IsSigned   = false,
    IsUnsigned = false
   };
 };

 //
 // template <>
 // struct Prop<???>
 //  {
 //   enum
 //    {
 //     IsSigned   = true,
 //     IsUnsigned = false,
 //    };
 //
 //   using SType = ??? ;
 //   using UType = ??? ;
 //
 //   using PromoteSType = ??? ;
 //   using PromoteUType = ??? ;
 //  };
 //
 // template <>
 // struct Prop<???>
 //  {
 //   enum
 //    {
 //     IsSigned   = false,
 //     IsUnsigned = true,
 //
 //     Bits = ???
 //    };
 //
 //   using PromoteUType = ??? ;
 //  };
 //

} // namespace ExtraInt

/* namespace Meta */

namespace Meta {

/* classes */

template <class UInt> struct DoubleUIntCtor;

/* struct DoubleUIntCtor<UInt> */

template <class UInt>
struct DoubleUIntCtor
 {
  using Ret = void ;
 };

template <>
struct DoubleUIntCtor<uint8>
 {
  using Ret = uint16 ;
 };

template <>
struct DoubleUIntCtor<uint16>
 {
  using Ret = uint32 ;
 };

template <>
struct DoubleUIntCtor<uint32>
 {
  using Ret = uint64 ;
 };

/* type DoubleUInt<UInt> */

template <class UInt>
using DoubleUInt = typename DoubleUIntCtor<UInt>::Ret ;

} // namespace Meta

/* Used() */

template <class T> void Used(T &) {}

/* OptimizeBarrier() */

void OptimizeBarrier(void *ptr,ulen len);

} // namespace CCore

#endif

