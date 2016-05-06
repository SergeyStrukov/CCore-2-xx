/* InterfaceHost.h */
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

#ifndef CCore_inc_InterfaceHost_h
#define CCore_inc_InterfaceHost_h

#include <CCore/inc/TypeNumber.h>

namespace CCore {

/* functions */

void GuardNoInterface(StrLen from,StrLen name);

/* classes */

struct ProbeSet_InterfaceName;

template <class T,bool has_InterfaceName> struct GetInterfaceName;

struct InterfaceCaster;

struct InterfaceHost;

template <class T> class InterfaceCasterTo;

/* struct ProbeSet_InterfaceName */

struct ProbeSet_InterfaceName
 {
  template <class T,const StrLen *> struct Host;

  template <class T,class C=Host<T,&T::InterfaceName> > struct Condition;
 };

/* const Has_InterfaceName<T> */

template <class T>
const bool Has_InterfaceName = Meta::Detect<ProbeSet_InterfaceName,T> ;

/* struct GetInterfaceName<T,bool has_InterfaceName> */

template <class T,bool has_InterfaceName=Has_InterfaceName<T> > struct GetInterfaceName;

template <class T>
struct GetInterfaceName<T,true> : StrLen
 {
  GetInterfaceName() : StrLen(T::InterfaceName) {}
 };

template <class T>
struct GetInterfaceName<T,false> : StrLen
 {
  GetInterfaceName() : StrLen("<Unknown interface>") {}
 };

/* struct InterfaceCaster */

struct InterfaceCaster
 {
  virtual void cast(InterfaceHost *obj)=0;

  virtual TypeNumber getTypeNumber()=0;
 };

/* struct InterfaceHost */

struct InterfaceHost
 {
  virtual void requestInterface(InterfaceCaster &caster) { caster.cast(this); }

  template <class T>
  T * pickInterface()
   {
    InterfaceCasterTo<T> caster;

    requestInterface(caster);

    return caster;
   }

  template <class T>
  T * takeInterface(StrLen from)
   {
    T *ret=pickInterface<T>();

    if( !ret ) GuardNoInterface(from,GetInterfaceName<T>());

    return ret;
   }
 };

/* class InterfaceCasterTo<T> */

template <class T>
class InterfaceCasterTo : public InterfaceCaster
 {
   T *ret;

  private:

   virtual void cast(InterfaceHost *obj) { ret=dynamic_cast<T *>(obj); }

   virtual TypeNumber getTypeNumber() { return TypeNumberOf<T>(); }

  public:

   InterfaceCasterTo() : ret(0) {}

   operator T * () const { return ret; }
 };

} // namespace CCore

#endif


