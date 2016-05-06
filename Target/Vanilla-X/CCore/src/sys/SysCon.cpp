/* SysCon.cpp */
//----------------------------------------------------------------------------------------
//
//  Project: CCore 2.00
//
//  Tag: Target/Vanilla-X
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2016 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#include <CCore/inc/sys/SysCon.h>

#include <CCore/inc/dev/DevPlanInit.h>

namespace CCore {
namespace Sys {

/* GetPlanInitNode_...() */

namespace Private_SysCon {

/* class ImpCon */

class ImpCon : public ConBase
 {
  private:

   virtual void attachDefaultInput(ConInputFunction input_any)
    {
     Used(input_any);

     // TODO
    }

   virtual void detachDefaultInput()
    {
     // TODO
    }

   virtual void defaultOutput(NanoPacket<char> packet)
    {
     if( !packet ) return;

     // TODO

     packet.free();
    }

   virtual void enablePacket()
    {
     // TODO
    }

   virtual void disablePacket()
    {
     // TODO
    }

   virtual void showStat()
    {
     // TODO
    }

  public:

   ImpCon(TextLabel name,void *mem,ulen max_data_len,ulen count)
    : ConBase(name,mem,max_data_len,count)
    {
    }

   ~ImpCon()
    {
    }
 };

/* class DefaultCon */

class DefaultCon : NoCopy
 {
   static const ulen MaxDataLen = 512 ;
   static const ulen Count      = 100 ;
   static const ulen MemLen     = ConBase::MemLen(MaxDataLen,Count) ;

  private:

   InitStorage<MemLen> storage;

   ImpCon dev;

  public:

   DefaultCon() : dev("!SysCon",storage.getMem(),MaxDataLen,Count) {}

   ~DefaultCon() {}

   ConBase * getBase() { return &dev; }

   static const char * GetTag() { return "SysCon"; }
 };

PlanInitObject<DefaultCon,PlanInitReq<Dev::GetPlanInitNode_Dev>
                         ,PlanInitReq<GetPlanInitNode_TaskCore> > Object CCORE_INITPRI_1 ;

} // namespace Private_SysCon

using namespace Private_SysCon;

PlanInitNode * GetPlanInitNode_SysCon() { return &Object; }

/* class ConBase */

ConBase * ConBase::GetObject() { return Object->getBase(); }

} // namespace Sys
} // namespace CCore

#if 1

#include <__std_init.h>

using namespace CCore;

void __std_debug_init(void)
 {
  // TODO
 }

void __std_debug(const char *zstr)
 {
  Used(zstr);

  // TODO
 }

void __std_debug2(const char *ptr,__std_len_t len)
 {
  Used(ptr);
  Used(len);

  // TODO
 }

void __std_debug_console(const char *ptr,__std_len_t len)
 {
  Used(ptr);
  Used(len);

  // TODO
 }

void __std_debug_trap(unsigned LR,unsigned trap)
 {
  Used(LR);
  Used(trap);

  // TODO
 }

#endif


