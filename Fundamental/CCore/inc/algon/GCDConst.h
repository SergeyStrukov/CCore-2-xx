/* GCDConst.h */
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

#ifndef CCore_inc_algon_GCDConst_h
#define CCore_inc_algon_GCDConst_h

#include <CCore/inc/Gadget.h>

namespace CCore {
namespace Algon {

/* EuclidAlgo<UInt,UInt A,UInt B>() */

template <class UInt,UInt A,UInt B>
constexpr Meta::EnableIf< Meta::IsUInt<UInt> , UInt > EuclidAlgo()
 {
  return A? EuclidAlgo<UInt,B%(A?A:1),A>() : B ;
 }

/* const GCDConst<UInt,UInt A,UInt B> */

template <class UInt,UInt A,UInt B>
const UInt GCDConst = EuclidAlgo<UInt,A,B>() ;

/* const LCMConst<UInt,UInt A,UInt B> */

template <class UInt,UInt A,UInt B>
const UInt LCMConst = (A/GCDConst<UInt,A,B>)*B ; // no overflow check

} // namespace Algon
} // namespace CCore

#endif


