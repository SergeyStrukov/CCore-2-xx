/* Point.cpp */
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

#include <CCore/inc/video/Point.h>

namespace CCore {
namespace Video {

/* struct Ratio */

Ratio Ratio::pow(unsigned deg) const
 {
  switch( deg )
    {
     case 0 : return Ratio(1,0);

     case 1 : return *this;

     default:
      {
       Ratio a=pow(deg>>1);

       if( deg&1u )
         {
          return a*a*(*this);
         }
       else
         {
          return a*a;
         }
      }
    }
 }

} // namespace Video
} // namespace CCore


