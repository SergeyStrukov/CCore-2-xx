/* FigureLib.h */
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

#ifndef CCore_inc_video_FigureLib_h
#define CCore_inc_video_FigureLib_h

#include <CCore/inc/video/SmoothDrawArt.h>

namespace CCore {
namespace Video {

/* classes */

struct MPane;

template <class T,ulen Len> struct FigureBase;

struct DrawPoints;

template <ulen Len> struct FigurePoints;

struct DrawDots;

template <ulen Len> struct FigureDots;

struct FigureBox;

struct FigureTopBorder;

struct FigureBottomBorder;

struct FigureButton;

struct FigureAsterisk;

struct FigureLeftArrow;

struct FigureRightArrow;

struct FigureUpArrow;

struct FigureDownArrow;

struct FigureLeftMark;

struct FigureRightMark;

struct FigureUpMark;

struct FigureDownMark;

struct FigureRoundBox;

/* struct MPane */

struct MPane
 {
  MCoord x;
  MCoord y;
  MCoord dx;
  MCoord dy;

  MCoord ex;
  MCoord ey;

  // constructors

  MPane() : x(0),y(0),dx(0),dy(0),ex(0),ey(0) {}

  MPane(const Pane &pane)
   {
    x=MPoint::LShift(pane.x)-MPoint::Half;
    y=MPoint::LShift(pane.y)-MPoint::Half;
    dx=MPoint::LShift(pane.dx);
    dy=MPoint::LShift(pane.dy);

    ex=x+dx;
    ey=y+dy;
   }

  MPane(MCoord x_,MCoord ex_,MCoord y_,MCoord ey_)
   {
    x=x_;
    y=y_;
    ex=ex_;
    ey=ey_;

    dx=ex-x;
    dy=ey-y;
   }

  // methods

  bool operator + () const { return dx>0 && dy>0 ; }

  bool operator ! () const { return dx<=0 && dy<=0 ; }

  void square()
   {
    if( dx>dy )
      {
       dx=dy;
       ex=x+dx;
      }
    else
      {
       dy=dx;
       ey=y+dy;
      }
   }

  // points

  MPoint getBase() const { return MPoint(x,y); }

  MPoint getSize() const { return MPoint(dx,dy); }

  MPoint getTopLeft() const { return MPoint(x,y); }

  MPoint getTopRight() const { return MPoint(ex,y); }

  MPoint getBottomLeft() const { return MPoint(x,ey); }

  MPoint getBottomRight() const { return MPoint(ex,ey); }

  // shift

  MPane operator + (MPoint p) const
   {
    MPane ret(*this);

    ret.x+=p.x;
    ret.y+=p.y;

    ret.ex+=p.x;
    ret.ey+=p.y;

    return ret;
   }

  MPane operator += (MPoint p)
   {
    x+=p.x;
    y+=p.y;

    ex+=p.x;
    ey+=p.y;

    return *this;
   }

  // shrink

  MPane shrinkX(MCoord dleft,MCoord dright) const
   {
    return MPane(x+dleft,ex-dright,y,ey);
   }

  MPane shrinkX(MCoord dx) const
   {
    return MPane(x+dx,ex-dx,y,ey);
   }

  MPane shrinkY(MCoord dtop,MCoord dbottom) const
   {
    return MPane(x,ex,y+dtop,ey-dbottom);
   }

  MPane shrinkY(MCoord dy) const
   {
    return MPane(x,ex,y+dy,ey-dy);
   }

  MPane shrink(MCoord dx,MCoord dy) const
   {
    return MPane(x+dx,ex-dx,y+dy,ey-dy);
   }

  MPane shrink(MCoord dxy) const
   {
    return MPane(x+dxy,ex-dxy,y+dxy,ey-dxy);
   }
 };

/* struct FigureBase<T,ulen Len> */

template <class T,ulen Len>
struct FigureBase
 {
  T buf[Len];

  PtrLen<const T> get() const { return Range(buf); }

  T & operator [] (ulen ind) { return buf[ind]; }

  const T & operator [] (ulen ind) const { return buf[ind]; }
 };

/* struct DrawPoints */

struct DrawPoints
 {
  static void Path(Smooth::DrawArt &art,PtrLen<const MPoint> dots,MCoord width,VColor vc);

  static void Loop(Smooth::DrawArt &art,PtrLen<const MPoint> dots,MCoord width,VColor vc);


  static void Path(Smooth::DrawArt &art,PtrLen<const MPoint> dots,HalfFlag half_flag,MCoord width,VColor vc);

  static void Loop(Smooth::DrawArt &art,PtrLen<const MPoint> dots,HalfFlag half_flag,MCoord width,VColor vc);


  static void Solid(Smooth::DrawArt &art,PtrLen<const MPoint> dots,SolidFlag solid_flag,VColor vc);

  static void Solid(Smooth::DrawArt &art,PtrLen<const MPoint> dots,SolidFlag solid_flag,const TwoField &field);

  static void Solid(Smooth::DrawArt &art,PtrLen<const MPoint> dots,SolidFlag solid_flag,const RadioField &field);


  static void Solid(Smooth::DrawArt &art,PtrLen<const MPoint> dots,VColor vc)
   {
    Solid(art,dots,SolidAll,vc);
   }

  static void Solid(Smooth::DrawArt &art,PtrLen<const MPoint> dots,const TwoField &field)
   {
    Solid(art,dots,SolidAll,field);
   }

  static void Solid(Smooth::DrawArt &art,PtrLen<const MPoint> dots,const RadioField &field)
   {
    Solid(art,dots,SolidAll,field);
   }


  static void CurvePath(Smooth::DrawArt &art,PtrLen<const MPoint> dots,MCoord width,VColor vc);

  static void CurveLoop(Smooth::DrawArt &art,PtrLen<const MPoint> dots,MCoord width,VColor vc);


  static void CurvePath(Smooth::DrawArt &art,PtrLen<const MPoint> dots,HalfFlag half_flag,MCoord width,VColor vc);

  static void CurveLoop(Smooth::DrawArt &art,PtrLen<const MPoint> dots,HalfFlag half_flag,MCoord width,VColor vc);


  static void CurveSolid(Smooth::DrawArt &art,PtrLen<const MPoint> dots,SolidFlag solid_flag,VColor vc);

  static void CurveSolid(Smooth::DrawArt &art,PtrLen<const MPoint> dots,SolidFlag solid_flag,const TwoField &field);

  static void CurveSolid(Smooth::DrawArt &art,PtrLen<const MPoint> dots,SolidFlag solid_flag,const RadioField &field);


  static void CurveSolid(Smooth::DrawArt &art,PtrLen<const MPoint> dots,VColor vc)
   {
    CurveSolid(art,dots,SolidAll,vc);
   }

  static void CurveSolid(Smooth::DrawArt &art,PtrLen<const MPoint> dots,const TwoField &field)
   {
    CurveSolid(art,dots,SolidAll,field);
   }

  static void CurveSolid(Smooth::DrawArt &art,PtrLen<const MPoint> dots,const RadioField &field)
   {
    CurveSolid(art,dots,SolidAll,field);
   }
 };

/* struct FigurePoints<ulen Len> */

template <ulen Len>
struct FigurePoints : FigureBase<MPoint,Len> , DrawPoints
 {
  using FigureBase<MPoint,Len>::buf;
  using FigureBase<MPoint,Len>::get;

  template <class FuncInit>
  void transform(FuncInit func_init) // MPoint func(MPoint)
   {
    FunctorTypeOf<FuncInit> func(func_init);

    for(MPoint &p : buf ) p=func(p);
   }

  template <class ... TT>
  void path(Smooth::DrawArt &art,TT && ... tt) const
   {
    Path(art,get(), std::forward<TT>(tt)... );
   }

  template <class ... TT>
  void loop(Smooth::DrawArt &art,TT && ... tt) const
   {
    Loop(art,get(), std::forward<TT>(tt)... );
   }

  template <class ... TT>
  void solid(Smooth::DrawArt &art,TT && ... tt) const
   {
    Solid(art,get(), std::forward<TT>(tt)... );
   }

  template <class ... TT>
  void curvePath(Smooth::DrawArt &art,TT && ... tt) const
   {
    CurvePath(art,get(), std::forward<TT>(tt)... );
   }

  template <class ... TT>
  void curveLoop(Smooth::DrawArt &art,TT && ... tt) const
   {
    CurveLoop(art,get(), std::forward<TT>(tt)... );
   }

  template <class ... TT>
  void curveSolid(Smooth::DrawArt &art,TT && ... tt) const
   {
    CurveSolid(art,get(), std::forward<TT>(tt)... );
   }
 };

/* struct DrawDots */

struct DrawDots
 {
  static void CurvePath(Smooth::DrawArt &art,PtrLen<const Smooth::Dot> dots,MCoord width,VColor vc);

  static void CurveLoop(Smooth::DrawArt &art,PtrLen<const Smooth::Dot> dots,MCoord width,VColor vc);


  static void CurvePath(Smooth::DrawArt &art,PtrLen<const Smooth::Dot> dots,HalfFlag half_flag,MCoord width,VColor vc);

  static void CurveLoop(Smooth::DrawArt &art,PtrLen<const Smooth::Dot> dots,HalfFlag half_flag,MCoord width,VColor vc);


  static void CurveSolid(Smooth::DrawArt &art,PtrLen<const Smooth::Dot> dots,SolidFlag solid_flag,VColor vc);

  static void CurveSolid(Smooth::DrawArt &art,PtrLen<const Smooth::Dot> dots,SolidFlag solid_flag,const TwoField &field);

  static void CurveSolid(Smooth::DrawArt &art,PtrLen<const Smooth::Dot> dots,SolidFlag solid_flag,const RadioField &field);


  static void CurveSolid(Smooth::DrawArt &art,PtrLen<const Smooth::Dot> dots,VColor vc)
   {
    CurveSolid(art,dots,SolidAll,vc);
   }

  static void CurveSolid(Smooth::DrawArt &art,PtrLen<const Smooth::Dot> dots,const TwoField &field)
   {
    CurveSolid(art,dots,SolidAll,field);
   }

  static void CurveSolid(Smooth::DrawArt &art,PtrLen<const Smooth::Dot> dots,const RadioField &field)
   {
    CurveSolid(art,dots,SolidAll,field);
   }
 };

/* struct FigureDots<ulen Len> */

template <ulen Len>
struct FigureDots : FigureBase<Smooth::Dot,Len> , DrawDots
 {
  using FigureBase<Smooth::Dot,Len>::buf;
  using FigureBase<Smooth::Dot,Len>::get;

  template <class FuncInit>
  void transform(FuncInit func_init) // MPoint func(MPoint)
   {
    FunctorTypeOf<FuncInit> func(func_init);

    for(Smooth::Dot &p : buf ) p.point=func(p.point);
   }

  template <class ... TT>
  void curvePath(Smooth::DrawArt &art,TT && ... tt) const
   {
    CurvePath(art,get(), std::forward<TT>(tt)... );
   }

  template <class ... TT>
  void curveLoop(Smooth::DrawArt &art,TT && ... tt) const
   {
    CurveLoop(art,get(), std::forward<TT>(tt)... );
   }

  template <class ... TT>
  void curveSolid(Smooth::DrawArt &art,TT && ... tt) const
   {
    CurveSolid(art,get(), std::forward<TT>(tt)... );
   }
 };

/* struct FigureBox */

struct FigureBox : FigurePoints<4>
 {
  FigureBox(MCoord x0,MCoord x1,MCoord y0,MCoord y1);

  explicit FigureBox(const MPane &p) : FigureBox(p.x,p.ex,p.y,p.ey) {}
 };

/* struct FigureTopBorder */

struct FigureTopBorder : FigurePoints<6>
 {
  FigureTopBorder(MCoord x0,MCoord x1,MCoord y0,MCoord y1,MCoord w);

  FigureTopBorder(const MPane &p,MCoord w) : FigureTopBorder(p.x,p.ex,p.y,p.ey,w) {}

  FigurePoints<6> getLeftCut(MCoord t) const;

  FigurePoints<4> getRightCut(MCoord t) const;
 };

/* struct FigureBottomBorder */

struct FigureBottomBorder : FigurePoints<6>
 {
  FigureBottomBorder(MCoord x0,MCoord x1,MCoord y0,MCoord y1,MCoord w);

  FigureBottomBorder(const MPane &p,MCoord w) : FigureBottomBorder(p.x,p.ex,p.y,p.ey,w) {}
 };

/* struct FigureButton */

struct FigureButton : FigureDots<10>
 {
  FigureButton(MCoord x0,MCoord x1,MCoord y0,MCoord y1,MCoord ex);

  FigureButton(const MPane &p,MCoord ex) : FigureButton(p.x,p.ex,p.y,p.ey,ex) {}

  FigureDots<6> getTop() const;

  FigureDots<6> getBottom() const;
 };

/* struct FigureAsterisk */

struct FigureAsterisk : FigurePoints<30>
 {
  FigureAsterisk(MPoint center,MCoord radius);
 };

/* struct FigureLeftArrow */

struct FigureLeftArrow : FigureDots<4>
 {
  FigureLeftArrow(MCoord x0,MCoord x1,MCoord y0,MCoord y1);

  explicit FigureLeftArrow(const MPane &p) : FigureLeftArrow(p.x,p.ex,p.y,p.ey) {}
 };

/* struct FigureRightArrow */

struct FigureRightArrow : FigureDots<4>
 {
  FigureRightArrow(MCoord x0,MCoord x1,MCoord y0,MCoord y1);

  explicit FigureRightArrow(const MPane &p) : FigureRightArrow(p.x,p.ex,p.y,p.ey) {}
 };

/* struct FigureUpArrow */

struct FigureUpArrow : FigureDots<4>
 {
  FigureUpArrow(MCoord x0,MCoord x1,MCoord y0,MCoord y1);

  explicit FigureUpArrow(const MPane &p) : FigureUpArrow(p.x,p.ex,p.y,p.ey) {}
 };

/* struct FigureDownArrow */

struct FigureDownArrow : FigureDots<4>
 {
  FigureDownArrow(MCoord x0,MCoord x1,MCoord y0,MCoord y1);

  explicit FigureDownArrow(const MPane &p) : FigureDownArrow(p.x,p.ex,p.y,p.ey) {}
 };

/* struct FigureLeftMark */

struct FigureLeftMark : FigurePoints<3>
 {
  FigureLeftMark(MCoord x,MCoord y,MCoord len);

  FigureLeftMark(MPoint point,MCoord len) : FigureLeftMark(point.x,point.y,len) {}

  FigureLeftMark(const MPane &p,MCoord len) : FigureLeftMark(p.x,p.y+p.dy/2,len) {}
 };

/* struct FigureRightMark */

struct FigureRightMark : FigurePoints<3>
 {
  FigureRightMark(MCoord x,MCoord y,MCoord len);

  FigureRightMark(MPoint point,MCoord len) : FigureRightMark(point.x,point.y,len) {}

  FigureRightMark(const MPane &p,MCoord len) : FigureRightMark(p.ex,p.y+p.dy/2,len) {}
 };

/* struct FigureUpMark */

struct FigureUpMark : FigurePoints<3>
 {
  FigureUpMark(MCoord x,MCoord y,MCoord len);

  FigureUpMark(MPoint point,MCoord len) : FigureUpMark(point.x,point.y,len) {}

  FigureUpMark(const MPane &p,MCoord len) : FigureUpMark(p.x+p.dx/2,p.y,len) {}
 };

/* struct FigureDownMark */

struct FigureDownMark : FigurePoints<3>
 {
  FigureDownMark(MCoord x,MCoord y,MCoord len);

  FigureDownMark(MPoint point,MCoord len) : FigureDownMark(point.x,point.y,len) {}

  FigureDownMark(const MPane &p,MCoord len) : FigureDownMark(p.x+p.dx/2,p.ey,len) {}
 };

/* struct FigureRoundBox */

struct FigureRoundBox : FigureDots<28>
 {
  FigureRoundBox(MCoord x0,MCoord x1,MCoord y0,MCoord y1,MCoord r);

  FigureRoundBox(const MPane &p,MCoord r) : FigureRoundBox(p.x,p.ex,p.y,p.ey,r) {}

  MPoint getLeftTop() const { return buf[0].point; }

  MPoint getRightTop() const { return buf[15].point; }

  MPoint getLeftBottom() const { return buf[1].point; }

  MPoint getRightBottom() const { return buf[14].point; }

  MPoint getTopLeft() const { return buf[22].point; }

  MPoint getTopRight() const { return buf[21].point; }

  MPoint getBottomLeft() const { return buf[7].point; }

  MPoint getBottomRight() const { return buf[8].point; }
 };

} // namespace Video
} // namespace CCore

#endif


