/* Quick.cpp */
//----------------------------------------------------------------------------------------
//
//  Project: CCore 2.00
//
//  Tag: Target/Vanilla-H
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2015 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#include <CCore/inc/base/Quick.h>

#include <CCore/inc/math/UIntSlowMulAlgo.h>

namespace CCore {
namespace Quick {

/* functions */

#if 1

unsigned ScanLSBit(ScanUInt value) noexcept
 {
  unsigned ret=0;

  if( !(value&0xFFFF) ) ret+=16,value>>=16;
  if( !(value&0x00FF) ) ret+= 8,value>>= 8;

  static const unsigned Table[256]=
   {
    0,0,1,0, 2,0,1,0, 3,0,1,0, 2,0,1,0,
    4,0,1,0, 2,0,1,0, 3,0,1,0, 2,0,1,0,
    5,0,1,0, 2,0,1,0, 3,0,1,0, 2,0,1,0,
    4,0,1,0, 2,0,1,0, 3,0,1,0, 2,0,1,0,

    6,0,1,0, 2,0,1,0, 3,0,1,0, 2,0,1,0,
    4,0,1,0, 2,0,1,0, 3,0,1,0, 2,0,1,0,
    5,0,1,0, 2,0,1,0, 3,0,1,0, 2,0,1,0,
    4,0,1,0, 2,0,1,0, 3,0,1,0, 2,0,1,0,

    7,0,1,0, 2,0,1,0, 3,0,1,0, 2,0,1,0,
    4,0,1,0, 2,0,1,0, 3,0,1,0, 2,0,1,0,
    5,0,1,0, 2,0,1,0, 3,0,1,0, 2,0,1,0,
    4,0,1,0, 2,0,1,0, 3,0,1,0, 2,0,1,0,

    6,0,1,0, 2,0,1,0, 3,0,1,0, 2,0,1,0,
    4,0,1,0, 2,0,1,0, 3,0,1,0, 2,0,1,0,
    5,0,1,0, 2,0,1,0, 3,0,1,0, 2,0,1,0,
    4,0,1,0, 2,0,1,0, 3,0,1,0, 2,0,1,0
   };

  return ret+Table[value&0x00FF];
 }

#endif

#if 1

unsigned ScanMSBit(ScanUInt value) noexcept
 {
  unsigned ret=0;

  if( value&0xFFFF0000 ) ret+=16,value>>=16;
  if( value&0xFF00     ) ret+= 8,value>>= 8;

  static const unsigned Table[256]=
   {
    0,0,1,1, 2,2,2,2, 3,3,3,3, 3,3,3,3,
    4,4,4,4, 4,4,4,4, 4,4,4,4, 4,4,4,4,
    5,5,5,5, 5,5,5,5, 5,5,5,5, 5,5,5,5,
    5,5,5,5, 5,5,5,5, 5,5,5,5, 5,5,5,5,

    6,6,6,6, 6,6,6,6, 6,6,6,6, 6,6,6,6,
    6,6,6,6, 6,6,6,6, 6,6,6,6, 6,6,6,6,
    6,6,6,6, 6,6,6,6, 6,6,6,6, 6,6,6,6,
    6,6,6,6, 6,6,6,6, 6,6,6,6, 6,6,6,6,

    7,7,7,7, 7,7,7,7, 7,7,7,7, 7,7,7,7,
    7,7,7,7, 7,7,7,7, 7,7,7,7, 7,7,7,7,
    7,7,7,7, 7,7,7,7, 7,7,7,7, 7,7,7,7,
    7,7,7,7, 7,7,7,7, 7,7,7,7, 7,7,7,7,

    7,7,7,7, 7,7,7,7, 7,7,7,7, 7,7,7,7,
    7,7,7,7, 7,7,7,7, 7,7,7,7, 7,7,7,7,
    7,7,7,7, 7,7,7,7, 7,7,7,7, 7,7,7,7,
    7,7,7,7, 7,7,7,7, 7,7,7,7, 7,7,7,7
   };

  return ret+Table[value&0x00FF];
 }

#endif

#if 1

uint16 ByteSwap16(uint16 value) noexcept
 {
  return uint16( (value>>8)|(value<<8) );
 }

#endif

#if 1

uint32 ByteSwap32(uint32 value) noexcept
 {
  return uint32( (value<<24)|(value>>24)|((value>>8)&0xFF00)|((value<<8)&0xFF0000) );
 }

#endif

#if 1

uint64 ByteSwap64(uint64 value) noexcept
 {
  uint32 hi=uint32(value>>32);
  uint32 lo=uint32(value);

  return ByteSwap32(hi)|(uint64(ByteSwap32(lo))<<32);
 }

#endif

/* struct UIntMulFunc<uint32> */

UIntMulFunc<uint32>::Mul::Mul(UInt a,UInt b)
 {
  Math::UIntSlowMulFunc<uint32>::Mul res(a,b);

  hi=res.hi;
  lo=res.lo;
 }

auto UIntMulFunc<uint32>::Div(UInt hi,UInt lo,UInt den) -> UInt
 {
  return Math::UIntSlowMulFunc<uint32>::Div(hi,lo,den);
 }

auto UIntMulFunc<uint32>::Mod(UInt hi,UInt lo,UInt den) -> UInt
 {
  return Math::UIntSlowMulFunc<uint32>::Mod(hi,lo,den);
 }

auto UIntMulFunc<uint32>::MulDiv(UInt a,UInt b,UInt den) -> UInt
 {
  return Math::UIntSlowMulFunc<uint32>::MulDiv(a,b,den);
 }

UIntMulFunc<uint32>::DivMod::DivMod(UInt hi,UInt lo,UInt den)
 {
  Math::UIntSlowMulFunc<uint32>::DivMod res(hi,lo,den);

  div=res.div;
  mod=res.mod;
 }

auto UIntMulFunc<uint32>::ModMul(UInt a,UInt b,UInt mod) -> UInt
 {
  return Math::UIntSlowMulFunc<uint32>::ModMul(a,b,mod);
 }

auto UIntMulFunc<uint32>::ModMac(UInt s,UInt a,UInt b,UInt mod) -> UInt
 {
  return Math::UIntSlowMulFunc<uint32>::ModMac(s,a,b,mod);
 }

/* struct UIntMulFunc<uint64> */

UIntMulFunc<uint64>::Mul::Mul(UInt a,UInt b)
 {
  Math::UIntSlowMulFunc<uint64>::Mul res(a,b);

  hi=res.hi;
  lo=res.lo;
 }

auto UIntMulFunc<uint64>::Div(UInt hi,UInt lo,UInt den) -> UInt
 {
  return Math::UIntSlowMulFunc<uint64>::Div(hi,lo,den);
 }

auto UIntMulFunc<uint64>::Mod(UInt hi,UInt lo,UInt den) -> UInt
 {
  return Math::UIntSlowMulFunc<uint64>::Mod(hi,lo,den);
 }

auto UIntMulFunc<uint64>::MulDiv(UInt a,UInt b,UInt den) -> UInt
 {
  return Math::UIntSlowMulFunc<uint64>::MulDiv(a,b,den);
 }

UIntMulFunc<uint64>::DivMod::DivMod(UInt hi,UInt lo,UInt den)
 {
  Math::UIntSlowMulFunc<uint64>::DivMod res(hi,lo,den);

  div=res.div;
  mod=res.mod;
 }

auto UIntMulFunc<uint64>::ModMul(UInt a,UInt b,UInt mod) -> UInt
 {
  return Math::UIntSlowMulFunc<uint64>::ModMul(a,b,mod);
 }

auto UIntMulFunc<uint64>::ModMac(UInt s,UInt a,UInt b,UInt mod) -> UInt
 {
  return Math::UIntSlowMulFunc<uint64>::ModMac(s,a,b,mod);
 }

} // namespace Quick
} // namespace CCore


