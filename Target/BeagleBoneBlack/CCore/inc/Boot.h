/* Boot.h */
//----------------------------------------------------------------------------------------
//
//  Project: CCore 2.00
//
//  Tag: Target/BeagleBoneBlack
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2016 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#ifndef CCore_inc_Boot_h
#define CCore_inc_Boot_h

#include <CCore/inc/net/PTPBoot.h>

namespace CCore {

/* classes */

class Boot;

/* class Boot */

class Boot : public Net::PTPBoot::BootInfo
 {
   class Wait : NoCopy
    {
      Event event;

     public:

      Wait();

      ~Wait();

      void complete();

      void wait();
    };

   Wait wait;

  private:

   using UnitType = unsigned ;

   static const uint32 MinAddress   = 0x8010'0000 ;
   static const uint32 LimAddress   = 0x9000'0000 ;
   static const uint32 TopAddress   = 0xA000'0000 ;
   static const uint32 AlignAddress =          4 ;

   const unsigned UnitLen = sizeof (UnitType) ;

   DynArray<UnitType> buf;

   void buildBoot(PtrLen<const UnitType> tail=Nothing);

   enum BootFlags
    {
     BootRun,
     BootFlash,
     BootFlashBoot
    };

  private:

   virtual void signal_complete();

   virtual void wait_complete();

  public:

   Boot();

   ~Boot();

   void show() const;

   void prepareBoot();

   void commitBoot() const;
 };

} // namespace CCore

#endif


