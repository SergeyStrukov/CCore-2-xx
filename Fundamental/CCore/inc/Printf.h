/* Printf.h */
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

#ifndef CCore_inc_Printf_h
#define CCore_inc_Printf_h

#include <CCore/inc/printf/PrintProxy.h>
#include <CCore/inc/printf/PrintBase.h>
#include <CCore/inc/DetectProxy.h>
#include <CCore/inc/Tuple.h>

namespace CCore {

/* GetTextDesc() */

inline const char * GetTextDesc(bool value) { return value?"true":"false"; }

inline int GetTextDesc(int value) { return value; }

inline unsigned GetTextDesc(unsigned value) { return value; }

inline long GetTextDesc(long value) { return value; }

inline unsigned long GetTextDesc(unsigned long value) { return value; }

inline long long GetTextDesc(long long value) { return value; }

inline unsigned long long GetTextDesc(unsigned long long value) { return value; }

/* classes */

template <int Sw,class T> struct PrintOptAdapters;

template <int Sw,class T> struct PrintAdapters;

template <class T> struct PrintOptAdapter;

template <class T> struct PrintAdapter;

struct PrintDevBase;

template <class P> class PrintfDev;

template <class P> class PutobjDev;

template <class OptType,class T> struct BindOptType;

/* struct PrintOptAdapters<int Sw,T> */

template <class T>
struct PrintOptAdapters<2,T>
 {
  using PrintOptType = typename PrintProxy<T>::OptType ;
 };

template <class T>
struct PrintOptAdapters<1,T>
 {
 };

template <class T>
struct PrintOptAdapters<0,T>
 {
  static const T & GetObj();

  using S = Meta::UnConst<Meta::UnRef<decltype( GetTextDesc(GetObj()) )> > ;

  using PrintOptType = typename PrintProxy<S>::OptType ;
 };

/* struct PrintAdapters<int Sw,T> */

template <class T>
struct PrintAdapters<2,T>
 {
  using ProxyType = typename PrintProxy<T>::ProxyType ;

  using OptType = typename PrintProxy<T>::OptType ;

  template <class P>
  static void Print(P &out,const char *ptr,const char *lim,const T &t)
   {
    OptType opt(ptr,lim);
    ProxyType proxy(t);

    proxy.print(out.printRef(),opt);
   }

  template <class P>
  static void Print(P &out,const OptType &opt,const T &t)
   {
    ProxyType proxy(t);

    proxy.print(out.printRef(),opt);
   }

  template <class P>
  static void Print(P &out,const T &t)
   {
    OptType opt;
    ProxyType proxy(t);

    proxy.print(out.printRef(),opt);
   }
 };

template <class T>
struct PrintAdapters<1,T>
 {
  using ProxyType = typename PrintProxy<T>::ProxyType ;

  template <class P>
  static void Print(P &out,const char *,const char *,const T &t)
   {
    ProxyType proxy(t);

    proxy.print(out.printRef());
   }

  template <class P,class OptType>
  static void Print(P &out,const OptType &,const T &t)
   {
    ProxyType proxy(t);

    proxy.print(out.printRef());
   }

  template <class P>
  static void Print(P &out,const T &t)
   {
    ProxyType proxy(t);

    proxy.print(out.printRef());
   }
 };

template <class T>
struct PrintAdapters<0,T>
 {
  template <class P,class S>
  static void PrintDesc(P &out,const char *ptr,const char *lim,S s)
   {
    PrintAdapter<S>::Print(out,ptr,lim,s);
   }

  template <class P,class OptType,class S>
  static void PrintDesc(P &out,const OptType &opt,S s)
   {
    PrintAdapter<S>::Print(out,opt,s);
   }

  template <class P,class S>
  static void PrintDesc(P &out,S s)
   {
    PrintAdapter<S>::Print(out,s);
   }


  template <class P>
  static void Print(P &out,const char *ptr,const char *lim,const T &t)
   {
    PrintDesc(out,ptr,lim,GetTextDesc(t));
   }

  template <class P,class OptType>
  static void Print(P &out,const OptType &opt,const T &t)
   {
    PrintDesc(out,opt,GetTextDesc(t));
   }

  template <class P>
  static void Print(P &out,const T &t)
   {
    PrintDesc(out,GetTextDesc(t));
   }
 };

/* struct PrintOptAdapter<T> */

template <class T>
struct PrintOptAdapter : PrintOptAdapters<ProxySwitch<PrintProxy<T> >,T> {};

/* struct PrintAdapter<T> */

template <class T>
struct PrintAdapter : PrintAdapters<ProxySwitch<PrintProxy<T> >,T> {};

/* struct PrintDevBase */

struct PrintDevBase : NoCopy
 {
  static const char OpenFormat  = '#' ;
  static const char CloseFormat = ';' ;

  static bool NotFound(char cur,char ch) { return cur && cur!=ch ;  }

  static const char * Find(const char *format,char ch);

  struct OptStr
   {
    const char *ptr;
    const char *lim;

    OptStr(NothingType) : ptr(0),lim(0) {}

    OptStr(const char *ptr_,const char *lim_) : ptr(ptr_),lim(lim_) {}

    template <class P,class T>
    void print(P &out,const T &t) const
     {
      if( !ptr )
        {
         // print nothing
        }
      else
        {
         PrintAdapter<T>::Print(out,ptr,lim,t);
        }
     }
   };
 };

/* class PrintfDev<P> */

template <class P>
class PrintfDev : PrintDevBase
 {
   P out;
   const char *format;

  private:

   OptStr find();

   // step_tuple

   void step_tuple(const Tuple<> &) {}

   template <class T>
   void step_tuple(const Tuple<T> &tuple) { step(tuple.first); }

   template <class T,class S,class ... TT>
   void step_tuple(const Tuple<T,S,TT...> &tuple)
    {
     step(tuple.first);
     step_tuple(tuple.rest);
    }

   // step

   template <class T>
   void step(const T &t)
    {
     find().print(out,t);
    }

   template <class ... TT>
   void step(const Tuple<TT...> &tuple)
    {
     step_tuple(tuple);
    }

   // print

   void print() {}

   template <class T1,class ... TT>
   void print(const T1 &t1,const TT & ... tt)
    {
     step(t1);

     print(tt...);
    }

   template <class T1,class T2,class ... TT>
   void print(const T1 &t1,const T2 &t2,const TT & ... tt)
    {
     step(t1);
     step(t2);

     print(tt...);
    }

   template <class T1,class T2,class T3,class ... TT>
   void print(const T1 &t1,const T2 &t2,const T3 &t3,const TT & ... tt)
    {
     step(t1);
     step(t2);
     step(t3);

     print(tt...);
    }

  public:

   template <class S>
   PrintfDev(S &&out_,const char *format_) : out(out_),format(format_) {}

   ~PrintfDev() noexcept(false) {}

   template <class ... TT>
   void operator () (const TT & ... tt)
    {
     print(tt...);

     while( *format ) find();
    }
 };

template <class P>
auto PrintfDev<P>::find() -> OptStr // ^aaaaaa##aaaaaaaaaaaa#opt;
 {
  for(;;)
    {
     const char *ptr=Find(format,OpenFormat);

     out.put(format,Dist(format,ptr));

     if( *ptr )                // ^##aaaaaaaaaaaa#opt;
       {
        ptr++;

        if( *ptr==OpenFormat ) // ^#aaaaaaaaaaaa#opt;
          {
           out.put(OpenFormat);

           format=ptr+1;

           // continue;
          }
        else                   // ^opt;
          {
           const char *lim=Find(ptr,CloseFormat);

           if( *lim )
             {
              format=lim+1;

              return OptStr(ptr,lim);
             }
           else // no ;
             {
              format=ptr;

              return OptStr(ptr,ptr);
             }
          }
       }
     else // ^end-of-str
       {
        format=ptr;

        return Nothing;
       }
    }
 }

/* class PrintfDev<void> */

template <>
class PrintfDev<void>
 {
  public:

   template <class S>
   PrintfDev(S &&,const char *) {}

   template <class ... TT>
   void operator () (const TT & ...) {}
 };

/* class PutobjDev<P> */

template <class P>
class PutobjDev : NoCopy
 {
   P out;

  private:

   // step_tuple

   void step_tuple(const Tuple<> &) {}

   template <class T>
   void step_tuple(const Tuple<T> &tuple) { step(tuple.first); }

   template <class T,class S,class ... TT>
   void step_tuple(const Tuple<T,S,TT...> &tuple)
    {
     step(tuple.first);
     step_tuple(tuple.rest);
    }

   // step

   template <class T>
   void step(const T &t)
    {
     PrintAdapter<T>::Print(out,t);
    }

   template <class ... TT>
   void step(const Tuple<TT...> &tuple)
    {
     step_tuple(tuple);
    }

   // print

   void print() {}

   template <class T1,class ... TT>
   void print(const T1 &t1,const TT & ... tt)
    {
     step(t1);

     print(tt...);
    }

   template <class T1,class T2,class ... TT>
   void print(const T1 &t1,const T2 &t2,const TT & ... tt)
    {
     step(t1);
     step(t2);

     print(tt...);
    }

   template <class T1,class T2,class T3,class ... TT>
   void print(const T1 &t1,const T2 &t2,const T3 &t3,const TT & ... tt)
    {
     step(t1);
     step(t2);
     step(t3);

     print(tt...);
    }

  public:

   template <class S>
   explicit PutobjDev(S &&out_) : out(out_) {}

   ~PutobjDev() noexcept(false) {}

   template <class ... TT>
   void operator () (const TT & ... tt)
    {
     print(tt...);
    }

   // put

   void put() {}

   template <class ... TT>
   void put(char ch1,TT ... cch)
    {
     out.put(ch1);

     put(cch...);
    }

   template <class ... TT>
   void put(char ch1,char ch2,TT ... cch)
    {
     out.put(ch1);
     out.put(ch2);

     put(cch...);
    }

   template <class ... TT>
   void put(char ch1,char ch2,char ch3,TT ... cch)
    {
     out.put(ch1);
     out.put(ch2);
     out.put(ch3);

     put(cch...);
    }
 };

/* class PutobjDev<void> */

template <>
class PutobjDev<void>
 {
  public:

   template <class S>
   explicit PutobjDev(S &&) {}

   template <class ... TT>
   void operator () (const TT & ...) {}

   void put() {}

   template <class ... TT>
   void put(char,TT ... cch) { put(cch...); }
 };

/* struct BindOptType<OptType,T> */

template <class OptType,class T>
struct BindOptType
 {
  const OptType &opt;
  const T &t;

  BindOptType(const OptType &opt_,const T &t_) : opt(opt_),t(t_) {}

  template <class P>
  void print(P &out) const
   {
    PrintAdapter<T>::Print(out,opt,t);
   }
 };

/* BindOpt() */

template <class OptType,class T>
BindOptType<OptType,T> BindOpt(const OptType &opt,const T &t) { return BindOptType<OptType,T>(opt,t); }

/* Printf() */

template <class P,class ... TT>
void Printf(P &&out,const char *format,const TT & ... tt) CCORE_NOINLINE ;

template <class P,class ... TT>
void Printf(P &&out,const char *format,const TT & ... tt)
 {
  PrintfDev<PrintOutType<Meta::UnRef<P> > > dev(out,format);

  dev(tt...);
 }

/* Putobj() */

template <class P,class ... TT>
void Putobj(P &&out,const TT & ... tt) CCORE_NOINLINE ;

template <class P,class ... TT>
void Putobj(P &&out,const TT & ... tt)
 {
  PutobjDev<PrintOutType<Meta::UnRef<P> > > dev(out);

  dev(tt...);
 }

/* Putch() */

template <class P,class ... TT>
void Putch(P &&out,TT ... tt) CCORE_NOINLINE ;

template <class P,class ... TT>
void Putch(P &&out,TT ... tt)
 {
  PutobjDev<PrintOutType<Meta::UnRef<P> > > dev(out);

  dev.put(tt...);
 }

/* struct PrintProxy<MSec> */

template <>
struct PrintProxy<MSec>
 {
  struct ProxyType
   {
    unsigned time_msec;

    explicit ProxyType(MSec time) : time_msec(+time) {}

    template <class P>
    void print(P &out) const
     {
      if( time_msec<1000 )
        {
         Printf(out,"#; msec",time_msec);
        }
      else if( time_msec<60*1000 )
        {
         unsigned t=time_msec/1000;

         Printf(out,"#;.#; sec",t,time_msec%1000);
        }
      else
        {
         unsigned t=time_msec/1000;

         Printf(out,"#; min #;.#; sec",t/60,t%60,time_msec%1000);
        }
     }
   };
 };

} // namespace CCore

#endif



