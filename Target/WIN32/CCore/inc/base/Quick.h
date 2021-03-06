/* Quick.h */
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

#ifndef CCore_inc_base_Quick_h
#define CCore_inc_base_Quick_h

#include <CCore/inc/base/PlatformBase.h>

namespace CCore {
namespace Quick {

/* consts */

const bool ScanLSBitIsFast = true ;
const bool ScanMSBitIsFast = true ;

/* types */

using ScanUInt = uint32 ;

/* functions */

unsigned ScanLSBit(ScanUInt value) noexcept;

unsigned ScanMSBit(ScanUInt value) noexcept;

uint16 ByteSwap16(uint16 value) noexcept;

uint32 ByteSwap32(uint32 value) noexcept;

uint64 ByteSwap64(uint64 value) noexcept;

/* classes */

template <unsigned UIntBits> struct UIntMulSelect;

template <class UInt> struct UIntMulFunc;

/* struct UIntMulSelect<unsigned UIntBits> */

template <>
struct UIntMulSelect<8>
 {
  using ExtType = uint16 ;

  enum { IsDoubleType = true };
 };

template <>
struct UIntMulSelect<16>
 {
  using ExtType = uint32 ;

  enum { IsDoubleType = true };
 };

template <>
struct UIntMulSelect<32>
 {
  using ExtType = uint32 ;

  enum { IsDoubleType = false };
 };

template <>
struct UIntMulSelect<64>
 {
  using ExtType = uint64 ;

  enum { IsDoubleType = false };
 };

/* struct UIntMulFunc<uint32> */

template <>
struct UIntMulFunc<uint32>
 {
  using UInt = uint32 ;

  struct Mul
   {
    UInt hi;
    UInt lo;

    Mul(UInt a,UInt b);
   };

  static UInt Div(UInt hi,UInt lo,UInt den); // hi<den

  static UInt Mod(UInt hi,UInt lo,UInt den); // hi<den

  static UInt MulDiv(UInt a,UInt b,UInt den); // hi(a*b)<den

  struct DivMod
   {
    UInt div;
    UInt mod;

    DivMod(UInt hi,UInt lo,UInt den); // hi<den
   };

  static UInt ModMul(UInt a,UInt b,UInt mod); // a,b < mod

  static UInt ModMac(UInt s,UInt a,UInt b,UInt mod); // s,a,b < mod
 };

/* struct UIntMulFunc<uint64> */

template <>
struct UIntMulFunc<uint64>
 {
  using UInt = uint64 ;

  struct Mul
   {
    UInt hi;
    UInt lo;

    Mul(UInt a,UInt b);
   };

  static UInt Div(UInt hi,UInt lo,UInt den); // hi<den

  static UInt Mod(UInt hi,UInt lo,UInt den); // hi<den

  static UInt MulDiv(UInt a,UInt b,UInt den); // hi(a*b)<den

  struct DivMod
   {
    UInt div;
    UInt mod;

    DivMod(UInt hi,UInt lo,UInt den); // hi<den
   };

  static UInt ModMul(UInt a,UInt b,UInt mod); // a,b < mod

  static UInt ModMac(UInt s,UInt a,UInt b,UInt mod); // s,a,b < mod
 };

} // namespace Quick
} // namespace CCore

#endif


