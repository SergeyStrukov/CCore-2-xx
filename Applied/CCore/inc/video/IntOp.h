/* IntOp.h */
//----------------------------------------------------------------------------------------
//
//  Project: CCore 2.00
//
//  Tag: Applied
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2016 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#ifndef CCore_inc_video_IntOp_h
#define CCore_inc_video_IntOp_h

#include <CCore/inc/Gadget.h>

namespace CCore {
namespace Video {

/* guard functions */

void IntGuardAssert();

inline void IntGuard(bool cond) { if( !cond ) IntGuardAssert(); }

/* functions */

template <class SInt>
auto IntDist(SInt a,SInt b) // a <= b
 {
  return SIntFunc<SInt>::Dist(a,b);
 }

template <class SInt,class UInt>
auto IntMovePos(SInt a,UInt delta)
 {
  return SIntFunc<SInt>::MovePos(a,delta);
 }

template <class SInt,class UInt>
auto IntMoveNeg(SInt a,UInt delta)
 {
  return SIntFunc<SInt>::MoveNeg(a,delta);
 }

template <class SInt,class UInt>
auto IntMove(SInt a,SInt e,UInt delta)
 {
  return SIntFunc<SInt>::Move(a,e,delta);
 }

template <class SInt>
auto IntAbs(SInt a,SInt b)
 {
  return (a<=b)?IntDist(a,b):IntDist(b,a);
 }

template <class SInt>
auto IntAbs(SInt a)
 {
  return IntAbs<SInt>(a,0);
 }

/* functions */

 //
 // Can be substituted to provide checked operations.
 //

template <class SInt>
SInt IntAdd(SInt a,SInt b) { return a+b; } // may overflow

template <class SInt>
SInt IntSub(SInt a,SInt b) { return a-b; } // may overflow

template <class SInt>
SInt IntMul(SInt a,SInt b) { return a*b; } // may overflow

template <class SInt>
SInt IntDiv(SInt a,SInt b) { return a/b; } // may crash

 //
 // Can be substituted to provide the proper semantic.
 //

template <class SInt>
SInt IntLShift(SInt a,unsigned s) { return SIntFunc<SInt>::LShift(a,s); }

template <class SInt>
SInt IntRShift(SInt a,unsigned s) { return SIntFunc<SInt>::RShift(a,s); }

template <class SInt,class UInt>
SInt IntMask(SInt a,UInt mask) { return SIntFunc<SInt>::Mask(a,mask); }

/* sint16 functions */

template <class SInt>
Meta::EnableIf< Meta::IsSInt<SInt> , sint16 > To16(SInt x) { IntGuard( x>=-32768 && x<=32767 ); return (sint16)x; }

inline sint16 IntAdd(sint16 a,sint16 b) { return To16(sint32(a)+sint32(b)); }

inline sint16 IntSub(sint16 a,sint16 b) { return To16(sint32(a)-sint32(b)); }

inline sint16 IntMul(sint16 a,sint16 b) { return To16(sint32(a)*sint32(b)); }

inline sint16 IntDiv(sint16 a,sint16 b) { IntGuard( b!=0 ); return To16(sint32(a)/sint32(b)); }

/* classes */

template <class UInt,class> class DownBits;

/* class DownBits<UInt> */

template <class UInt,class=Meta::EnableIf< Meta::IsUInt<UInt> > >
class DownBits
 {
   static const unsigned MaxBits = Meta::UIntBits<UInt> ;

   UInt value;
   unsigned shift;

  public:

   DownBits()
    {
     value=0;
     shift=0;
    }

   template <class UInt1>
   explicit DownBits(UInt1 b) { init(b); }

   operator UInt() const { return value; }

   template <class UInt1>
   Meta::EnableIf< Meta::IsUInt<UInt1> > init(UInt1 b)
    {
     unsigned bits=UIntBitsOf(b);

     if( bits<=MaxBits )
       {
        shift=0;
        value=UInt(b);
       }
     else
       {
        shift=bits-MaxBits;
        value=UInt(b>>shift);
       }
    }

   template <class UInt1>
   Meta::EnableIf< Meta::IsUInt<UInt1> , UInt > operator () (UInt1 a) const
    {
     return UInt(a>>shift);
    }
 };

} // namespace Video
} // namespace CCore

#endif


