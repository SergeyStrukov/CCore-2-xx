/* testPrint.h */
//----------------------------------------------------------------------------------------
//
//  Project: CCore 2.00
//
//  Tag: XCore
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2016 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#ifndef CCore_test_testPrint_h
#define CCore_test_testPrint_h

#include <CCore/inc/Print.h>

#include <CCore/inc/AsyncBinaryFile.h>

namespace App {

/* using */

using namespace CCore;

/* classes */

class PrintFile;

class BinaryFile;

/* class PrintFile */

class PrintFile : public PrintAsyncFile
 {
  public:

   PrintFile();

   explicit PrintFile(StrLen file_name,FileOpenFlags oflags=Open_ToWrite);

   ~PrintFile();

   void open(StrLen file_name,FileOpenFlags oflags=Open_ToWrite);
 };

/* class BinaryFile */

class BinaryFile : public AsyncBinaryFile
 {
  public:

   BinaryFile();

   explicit BinaryFile(StrLen file_name,FileOpenFlags oflags=Open_ToWrite);

   ~BinaryFile();

   void open(StrLen file_name,FileOpenFlags oflags=Open_ToWrite);
 };

} // namespace App

#endif

