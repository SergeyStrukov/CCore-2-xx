/* CommonIntAlgo.h */
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

#ifndef CCore_inc_algon_CommonIntAlgo_h
#define CCore_inc_algon_CommonIntAlgo_h

#include <CCore/inc/Swap.h>

namespace CCore {
namespace Algon {

/* classes */

template <class UInt,class=Meta::EnableIf< Meta::IsUInt<UInt> > > struct BitIntAlgo;

template <class UInt,class Algo=BitIntAlgo<UInt>,class=Meta::EnableIf< Meta::IsUInt<UInt> > > struct CommonIntAlgo;

/* struct BitIntAlgo<UInt> */

template <class UInt,class>
struct BitIntAlgo
 {
  static bool IsEven(UInt a)
   {
    return !(a&UInt(1));
   }

  static unsigned BitScan(UInt a) // a!=0
   {
    return UIntFunc<UInt>::CountZeroLSB(a);
   }

  static UInt OddPart(UInt a) // a!=0
   {
    return a>>BitScan(a);
   }
 };

/* struct CommonIntAlgo<UInt,Algo> */

template <class UInt,class Algo,class>
struct CommonIntAlgo : Algo
 {
  using Algo::BitScan;
  using Algo::OddPart;

  static void Order(UInt &a,UInt &b)
   {
    if( a>b ) CopySwap(a,b);
   }

  static UInt GCD(UInt a,UInt b)
   {
    if( !a ) return b;
    if( !b ) return a;

    unsigned s=BitScan(a|b);

    a=OddPart(a>>s);
    b=OddPart(b>>s);

    for(;;)
      {
       Order(a,b);

       if( !(b-=a) ) return a<<s;

       b=OddPart(b>>1);
      }
   }

  static UInt LCM(UInt a,UInt b) // no overflow check
   {
    UInt d=GCD(a,b);

    if( !d ) return 0;

    return (a/d)*b;
   }
 };

/* functions */

template <class UInt>
Meta::EnableIf< Meta::IsUInt<UInt> , UInt > GCD(UInt a,UInt b) { return CommonIntAlgo<UInt>::GCD(a,b); }

template <class UInt>
Meta::EnableIf< Meta::IsUInt<UInt> , UInt > LCM(UInt a,UInt b) { return CommonIntAlgo<UInt>::LCM(a,b); }

} // namespace Algon
} // namespace CCore

#endif


