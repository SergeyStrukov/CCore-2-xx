/* Integer.cpp */
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
//  Copyright (c) 2015 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#include <CCore/inc/math/Integer.h>

#include <CCore/inc/Exception.h>

namespace CCore {
namespace Math {

/* functions */

void GuardBitsOfOverflow()
 {
  Printf(Exception,"CCore::Math::Integer<Algo,ArrayType,ArrayAlgoType>::BitsOf::total() : overflow");
 }

void GuardIntegerInverseNotNormalized()
 {
  Printf(Exception,"CCore::Math::IntegerInverse<Algo,ArrayAlgoType> : not normalized");
 }

void GuardIntegerInverseOverflow()
 {
  Printf(Exception,"CCore::Math::IntegerInverse<Algo,ArrayAlgoType> : overflow");
 }

void GuardIntegerZeroDivide()
 {
  Printf(Exception,"CCore::Math::IntegerDivider<Algo,ArrayAlgoType> : zero divide");
 }

void GuardIntegerBadString()
 {
  Printf(Exception,"CCore::Math::IntegerFromString<Integer> : bad string");
 }

void GuardAddLenOverflow_ulen(ulen a,ulen b)
 {
  Printf(Exception,"CCore::Math::AddLen(#;,#;) : overflow",a,b);
 }

void GuardAddLenOverflow_ulen()
 {
  Printf(Exception,"CCore::Math::AddLen(a,...) : a is not in ulen value range");
 }

void GuardQSymEven()
 {
  Printf(Exception,"CCore::Math::GCDAlgo<Algo>::UnsignedQSym(a,b) : b is even");
 }

void GuardQSymNotCoprime()
 {
  Printf(Exception,"CCore::Math::GCDAlgo<Algo>::UnsignedQSym(a,b) : a and b are not coprime");
 }

/* struct IntegerPrintOpt */

IntegerPrintOpt::IntegerPrintOpt(const char *ptr,const char *lim)
 {
  StrParse dev(ptr,lim);

  Parse_empty(dev,show_sign);

  ParseUInt_empty(dev,width,0);

  if( !dev.finish() ) setDefault();
 }

} // namespace Math
} // namespace CCore

