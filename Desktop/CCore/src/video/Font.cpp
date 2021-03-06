/* Font.cpp */
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

#include <CCore/inc/video/Font.h>

#include <CCore/inc/video/SimpleConsole.h>

namespace CCore {
namespace Video {

namespace Private_Font {

class DefFont : public FontBase
 {
  private:

   class WorkBuf : public DrawBuf
    {
     public:

      explicit WorkBuf(const DrawBuf &buf) : DrawBuf(buf) {}

      template <class Pattern>
      static void Line(Raw *ptr,Pattern pat,Coord len,DesktopColor color) // len > 0
       {
        Coord gx=0;

        if( pat[gx] ) color.copyTo(ptr);

        while( ++gx<len )
          {
           ptr=NextX(ptr);

           if( pat[gx] ) color.copyTo(ptr);
          }
       }

      template <class Pattern>
      static void Line(Raw *ptr,Pattern pat,Coord gx,Coord len,DesktopColor color) // len > 0
       {
        Coord lim=gx+len;

        if( pat[gx] ) color.copyTo(ptr);

        while( ++gx<lim )
          {
           ptr=NextX(ptr);

           if( pat[gx] ) color.copyTo(ptr);
          }
       }

      void text(Coord x,Coord y,char ch,DesktopColor color) //  x < dx , y < dy , y > -DefaultFont::DY
       {
        if( x<=-DefaultFont::DX ) return;

        DefaultFont glyph(ch);

        Coord gy;
        Coord lim;

        if( y<0 )
          {
           gy=-y;
           y=0;

           lim=(dy<DefaultFont::DY)?Min<Coord>(DefaultFont::DY,dy+gy):DefaultFont::DY; // > gy
          }
        else
          {
           gy=0;

           lim=Min<Coord>(DefaultFont::DY,dy-y); // > gy
          }

        if( x>=0 )
          {
           Coord len=Min<Coord>(DefaultFont::DX,dx-x); // > 0

           Raw *ptr=place(Point(x,y));

           Line(ptr,glyph[gy],len,color);

           while( ++gy<lim )
             {
              ptr=nextY(ptr);

              Line(ptr,glyph[gy],len,color);
             }
          }
        else
          {
           Coord gx=-x;

           Coord len=Min<Coord>(DefaultFont::DX-gx,dx); // > 0

           Raw *ptr=place(Point(0,y));

           Line(ptr,glyph[gy],gx,len,color);

           while( ++gy<lim )
             {
              ptr=nextY(ptr);

              Line(ptr,glyph[gy],gx,len,color);
             }
          }
       }

      static Point Prepare(Coord px,Coord py,TextPlace place,AbstractSparseString &str)
       {
        Coord y;

        switch( place.align_y )
          {
           case AlignY_Top : y=0; break;

           case AlignY_Center : y=(py-DefaultFont::DY)/2; break;

           case AlignY_Bottom : y=py-DefaultFont::DY; break;

           default: y=IntSub(place.y,DefaultFont::BY);
          }

        Coord x;

        switch( place.align_x )
          {
           case AlignX_Left : x=0; break;

           case AlignX_Right :
            {
             ULenSat len=str.countLen();

             ulen cap=ulen(px/DefaultFont::DX)+1;

             if( len<=cap )
               {
                x=Coord( px-len.value*DefaultFont::DX );
               }
             else
               {
                str.cutSuffix(cap);

                x=Coord( px-cap*DefaultFont::DX );
               }
            }
           break;

           case AlignX_Center :
            {
             ULenSat len=str.countLen();

             ulen cap=ulen(px/DefaultFont::DX)+1;

             if( len<=cap )
               {
                x=Coord( px-len.value*DefaultFont::DX )/2;
               }
             else
               {
                ulen new_len=cap+str.cutCenter(cap);

                x=Coord( px-new_len*DefaultFont::DX )/2;
               }
            }
           break;

           default: x=place.x;
          }

        return Point(x,y);
       }

      void text(Point point,AbstractSparseString &str,DesktopColor color)
       {
        Coord x=mapX(point.x);
        Coord y=mapY(point.y);

        if( y>=dy || y<=-DefaultFont::DY ) return;

        str.applyCharWhile( [&] (char ch)
                                {
                                 if( x>=dx ) return false;

                                 text(x,y,ch,color);

                                 x+=DefaultFont::DX;

                                 return true;

                                } );
       }

      void text(Coord px,Coord py,TextPlace place,AbstractSparseString &str,DesktopColor color)
       {
        if( !*this ) return;

        Point point=Prepare(px,py,place,str);

        text(point,str,color);
       }
    };

   TextSize text(ulen len,bool overflow) const
    {
     TextSize ret;

     if( overflow )
       {
        ret.dx=MaxCoord;
        ret.overflow=true;
       }
     else
       {
        ret.dx=Coord( len*DefaultFont::DX );
        ret.overflow=false;
       }

     ret.full_dx=ret.dx;

     ret.dy=DefaultFont::DY;
     ret.by=DefaultFont::BY;
     ret.dx0=0;
     ret.dx1=0;
     ret.skew=0;

     return ret;
    }

  public:

   DefFont() {}

   virtual ~DefFont() {}

   // AbstractFont

   virtual FontSize getSize() const
    {
     FontSize ret;

     ret.min_dx=DefaultFont::DX;
     ret.max_dx=DefaultFont::DX;
     ret.dy=DefaultFont::DY;
     ret.by=DefaultFont::BY;
     ret.dx0=0;
     ret.dx1=0;
     ret.skew=0;

     return ret;
    }

   static const ulen MaxLen = ulen(MaxCoord/DefaultFont::DX) ;

   virtual TextSize text(AbstractSparseString &str) const
    {
     ULenSat len=str.countLen();

     return text(len.value,len>MaxLen);
    }

   virtual ulen position(AbstractSparseString &,Point point) const
    {
     if( point.x<0 ) return 0;

     return 1+ulen(point.x/DefaultFont::DX);
    }

   virtual TextSize text(AbstractSparseString &,ulen pos) const
    {
     return text(pos,pos>MaxLen);
    }

   virtual ulen fit(AbstractSparseString &str,Coord full_dx) const
    {
     ULenSat len=str.countLen();

     ulen max_len=full_dx/DefaultFont::DX;

     if( len<=max_len ) return len.value;

     return max_len;
    }

   virtual void text(DrawBuf buf,Pane pane,TextPlace place,AbstractSparseString &str,VColor vc) const
    {
     WorkBuf out(buf.cutRebase(pane));

     out.text(pane.dx,pane.dy,place,str,vc);
    }
 };

DefFont Object CCORE_INITPRI_3 ;

} // namespace Private_Font

using namespace Private_Font;

/* struct AbstractSparseString */

ULenSat AbstractSparseString::countLen()
 {
  ULenSat ret;

  apply( [&ret] (PtrLen<const char> str) { ret+=str.len; } );

  return ret;
 }

/* class SingleString */

void SingleString::restart()
 {
  first=true;
 }

PtrLen<const char> SingleString::next()
 {
  if( first )
    {
     first=false;

     return str;
    }

  return Empty;
 }

void SingleString::cutSuffix(ulen len)
 {
  GuardLen(len,str.len);

  str=str.suffix(len);

  first=true;
 }

bool SingleString::cutCenter(ulen len)
 {
  GuardLen(len,str.len);

  ulen extra=str.len-len;
  ulen delta=extra/2;

  str=str.inner(delta,delta);

  first=true;

  return extra&1u;
 }

/* class DoubleString */

DoubleString::DoubleString(StrLen str1_,StrLen str2_)
 : str1(str1_),
   str2(str2_)
 {
  if( !str1 )
    {
     str1=str2;
     str2=Empty;
    }
 }

void DoubleString::restart()
 {
  ind=1;
 }

PtrLen<const char> DoubleString::next()
 {
  switch( ind )
    {
     case 1 : ind++; return str1;
     case 2 : ind++; return str2;

     default: return Empty;
    }
 }

void DoubleString::cutSuffix(ulen len)
 {
  if( len>str2.len )
    {
     len-=str2.len;

     GuardLen(len,str1.len);

     str1=str1.suffix(len);
    }
  else
    {
     str1=str2.suffix(len);
     str2=Empty;
    }

  ind=1;
 }

bool DoubleString::cutCenter(ulen len)
 {
  if( str1.len>str2.len )
    {
     ulen d=str1.len-str2.len;

     if( d>=len )
       {
        auto s=str1.suffix(d);

        ulen extra=d-len;
        ulen delta=extra/2;

        str1=s.inner(delta,delta);
        str2=Empty;

        ind=1;

        return extra&1u;
       }
     else
       {
        ulen s=len-d;

        bool ret = s&1u ;

        ulen len2=s/2+ret;
        ulen len1=d+len2;

        GuardLen(len2,str2.len);

        str1=str1.suffix(len1);
        str2=str2.prefix(len2);

        ind=1;

        return ret;
       }
    }
  else
    {
     ulen d=str2.len-str1.len;

     if( d>=len )
       {
        auto s=str2.prefix(d);

        ulen extra=d-len;
        ulen delta=extra/2;

        str1=s.inner(delta,delta);
        str2=Empty;

        ind=1;

        return extra&1u;
       }
     else
       {
        ulen s=len-d;

        bool ret = s&1u ;

        ulen len1=s/2+ret;
        ulen len2=d+len1;

        GuardLen(len1,str1.len);

        str1=str1.suffix(len1);
        str2=str2.prefix(len2);

        ind=1;

        return ret;
       }
    }
 }

/* class Font */

Font::Font()
 : ptr(&Object)
 {
  ptr->incRef();
 }

} // namespace Video
} // namespace CCore


