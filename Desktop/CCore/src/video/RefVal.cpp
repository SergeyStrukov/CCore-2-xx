/* RefVal.cpp */
//----------------------------------------------------------------------------------------
//
//  Project: CCore 2.00
//
//  Tag: Desktop
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2016 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#include <CCore/inc/video/RefVal.h>

#include <CCore/inc/Exception.h>

namespace CCore {
namespace Video {

/* GuardCtorRefValLock() */

void GuardCtorRefValLock()
 {
  Printf(Exception,"CCore::Video::CtorRefVal<...>::bind(...) : bind is locked");
 }

} // namespace Video
} // namespace CCore


