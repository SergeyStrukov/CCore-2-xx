/* ShapeLib.ButtonShape.cpp */
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

#include <CCore/inc/video/ShapeLib.h>

#include <CCore/inc/video/SmoothDrawArt.h>
#include <CCore/inc/video/FigureLib.h>

namespace CCore {
namespace Video {

/* class ButtonShape */

Point ButtonShape::getMinSize() const
 {
  TextSize ts=cfg.font.get()->text(Range(face));

  IntGuard( !ts.overflow );

  MCoord width=+cfg.width;

  MCoord ex=(Fraction(ts.dy)+2*width)/4;

  Coord dx=RoundUpLen(ex);
  Coord dy=RoundUpLen(width);

  return 2*Point(dx,dy)+Point(ts.full_dx,ts.dy)+(+cfg.space);
 }

void ButtonShape::draw(const DrawBuf &buf) const
 {
  MPane p(pane);

  if( !p ) return;

  Smooth::DrawArt art(buf);

  Font font=cfg.font.get();

  // figure

  MCoord width=+cfg.width;

  FontSize fs=font->getSize();

  MCoord ex=(Fraction(fs.dy)+2*width)/4;

  FigureButton fig(p,ex);

  VColor bottom=+cfg.bottom;

  // body

  if( down )
    {
     fig.curveSolid(art,bottom);
    }
  else
    {
     VColor top;

     if( mover && enable )
       top=+cfg.topUp;
     else
       top=+cfg.top;

     fig.curveSolid(art,TwoField(p.getTopLeft(),top,p.getBottomLeft(),bottom));
    }

  // text

  {
   Coord dx=RoundUpLen(ex);
   Coord dy=RoundUpLen(width);

   Point shift=Null;

   if( down ) shift=Point::Diag( (dy+1)/2 );

   VColor text=enable?+cfg.text:bottom;

   font->text(buf,pane.shrink(dx,dy)+shift,TextPlace(AlignX_Center,AlignY_Center),Range(face),text);
  }

  // border

  {
   VColor border;

   if( focus )
     {
      border=+cfg.focus;
     }
   else
     {
      if( enable )
        border=+cfg.border;
      else
        border=bottom;
     }

   fig.curveLoop(art,HalfPos,width,border);
  }
 }

} // namespace Video
} // namespace CCore


