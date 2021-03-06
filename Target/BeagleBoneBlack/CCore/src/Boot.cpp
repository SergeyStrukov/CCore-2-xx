/* Boot.cpp */
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

#include <CCore/inc/Boot.h>

#include <CCore/inc/Print.h>
#include <CCore/inc/Exception.h>

#include <string.h>

extern "C" {

extern void __std_boot(const unsigned *ptr,const unsigned *lim);

extern const unsigned __std_boot_beg;
extern const unsigned __std_boot_end;

} // extern "C"

namespace CCore {

/* class Boot::Wait */

Boot::Wait::Wait()
 : event("Boot::Wait")
 {
 }

Boot::Wait::~Wait()
 {
 }

void Boot::Wait::complete()
 {
  event.trigger();
 }

void Boot::Wait::wait()
 {
  ulen cnt=0;

  for(;;)
    if( event.wait(3_sec) )
      {
       Printf(Con,"\ndone\n\n");

       return;
      }
    else
      {
       if( !cnt ) cnt=10;

       Printf(Con,"waiting #;#;\r",RepeatChar(cnt--,'.'),RepeatChar(10,' '));
      }
 }

/* class Boot */

void Boot::buildBoot(PtrLen<const UnitType> tail)
 {
  ulen total=tail.len+2;

  if( entry_point<MinAddress || entry_point>=LimAddress )
    {
     Printf(Exception,"CCore::Boot::buildBoot() : bad entry point #8.16i;",entry_point);
    }

  if( entry_point%AlignAddress )
    {
     Printf(Exception,"CCore::Boot::buildBoot() : unaligned entry point #8.16i;",entry_point);
    }

  for(auto r=Range(table); +r ;++r)
    {
     auto address=r->getAddress();
     auto len=r->getLen();

     if( address<MinAddress || len>LimAddress || address>LimAddress-len )
       {
        Printf(Exception,"CCore::Boot::buildBoot() : bad section placement #8.16i; #8.16i;",address,len);
       }

     if( address%AlignAddress )
       {
        Printf(Exception,"CCore::Boot::buildBoot() : unaligned section #8.16i;",address);
       }

     UnitType len_u=UnitType( (len+UnitLen-1)/UnitLen );

     if( !len_u ) continue;

     total+=(2+len_u);

     if( total>(TopAddress-LimAddress)/UnitLen )
       {
        Printf(Exception,"CCore::Boot::buildBoot() : too large boot #;",total);
       }
    }

  buf.erase();

  UnitType *out=buf.extend_default(total).ptr;

  for(auto r=Range(table); +r ;++r)
    {
     auto address=r->getAddress();
     auto len=r->getLen();

     UnitType len_u=UnitType( (len+UnitLen-1)/UnitLen );

     if( !len_u ) continue;

     *(out++)=(UnitType)address;
     *(out++)=len_u;

     const uint8 *data=r->getData().ptr;

     memcpy(out,data,(ulen)len);

     out+=len_u;
    }

  *(out++)=(UnitType)entry_point;
  *(out++)=0;

  tail.copyTo(out);
 }

void Boot::signal_complete()
 {
  wait.complete();
 }

void Boot::wait_complete()
 {
  wait.wait();
 }

Boot::Boot()
 {
 }

Boot::~Boot()
 {
 }

void Boot::show() const
 {
  for(auto r=Range(table); +r ;++r)
    Printf(Con,"#8.16i; #8.16i;\n",r->getAddress(),r->getLen());

  Printf(Con,"\nentry_point = #8.16i;\n",entry_point);
 }

void Boot::prepareBoot()
 {
  switch( flags )
    {
     case BootRun :
      {
       buildBoot(Range(&__std_boot_beg,&__std_boot_end));
      }
     break;

     case BootFlash :
      {
       // TODO
      }
     break;

     case BootFlashBoot :
      {
       // TODO
      }
     break;

     default:
      {
       Printf(Exception,"CCore::Boot::boot() : bad boot flag");
      }
    }
 }

void Boot::commitBoot() const
 {
  switch( flags )
    {
     case BootRun :
      {
       __std_boot(buf.getPtr(),buf.getPtr()+buf.getLen());
      }
     break;

     case BootFlash :
      {
       // TODO
      }
     break;

     case BootFlashBoot :
      {
       // TODO
      }
     break;
    }
 }

} // namespace CCore


