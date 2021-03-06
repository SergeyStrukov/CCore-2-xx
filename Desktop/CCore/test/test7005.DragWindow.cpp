/* test7005.DragWindow.cpp */
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

#include <CCore/test/wintest.h>

#include <CCore/inc/video/CommonDrawArt.h>

namespace App {

namespace Private_7005 {

class ClientSubWindow : public SubWindow
 {
   DragShape::Config &cfg;
   Signal<> &update;

   enum Param
    {
     Frame,
     Title,

     BtnDx,
     BtnDy,

     Width
    };

   Param param = Frame ;

  public:

   ClientSubWindow(SubWindowHost &host,Signal<> &update_,DragShape::Config &cfg_)
    : SubWindow(host),
      cfg(cfg_),
      update(update_)
    {
    }

   virtual ~ClientSubWindow()
    {
    }

   // drawing

   virtual void layout()
    {
    }

   virtual void draw(DrawBuf buf,bool drag_active) const
    {
     Used(drag_active);

     CommonDrawArt art(buf);

     art.erase(Green);
    }

   // user input

   virtual void react(UserAction action)
    {
     action.dispatch(*this);
    }

   void react_Key(VKey vkey,KeyMod)
    {
     switch( vkey )
       {
        case VKey_F1 : param=Frame; break;
        case VKey_F2 : param=Title; break;
        case VKey_F3 : param=BtnDx; break;
        case VKey_F4 : param=BtnDy; break;
        case VKey_F5 : param=Width; break;
       }
    }

   void react_Wheel(Point,MouseKey,Coord delta)
    {
     switch( param )
      {
       case Frame : cfg.frame_dxy=Cap<Coord>(3,+cfg.frame_dxy+delta,100); break;

       case Title : cfg.title_dy=Cap<Coord>(10,+cfg.title_dy+delta,100); break;

       case BtnDx : cfg.btn_dx=Cap<Coord>(10,+cfg.btn_dx+delta,100); break;

       case BtnDy : cfg.btn_dy=Cap<Coord>(10,+cfg.btn_dy+delta,100); break;

       case Width : cfg.width=Cap<MCoord>(Fraction(1,2),+cfg.width+Fraction(delta,2),Fraction(100,2)); break;
      }

     update.assert();
    }
 };

} // namespace Private_7005

using namespace Private_7005;

/* TestMain() */

template <>
int TestMain<7005>(CmdDisplay cmd_display)
 {
  return ClientApplication<ClientSubWindow>::MainFrame(cmd_display,"Test7005");
 }

} // namespace App





