# Makefile
#----------------------------------------------------------------------------------------
#
#  Project: CCore 2.00
#
#  Tag: Target/BeagleBoneBlack
#
#  License: Boost Software License - Version 1.0 - August 17th, 2003 
#
#            see http://www.boost.org/LICENSE_1_0.txt or the local copy
#
#  Copyright (c) 2016 Sergey Strukov. All rights reserved.
#
#----------------------------------------------------------------------------------------

CCORE_ROOT = ../../..

CCORE_TARGET = BeagleBoneBlack

SRC_PATH_LIST = . \
                $(CCORE_ROOT)/Simple/CCore/test \
                $(CCORE_ROOT)/Fundamental/CCore/test \
                $(CCORE_ROOT)/Applied/CCore/test \
                $(CCORE_ROOT)/XCore/CCore/test \
                $(CCORE_ROOT)/Target/$(CCORE_TARGET)/CCore/test \


OBJ_PATH = ../test-obj

TARGET = main.exe

include $(CCORE_ROOT)/Target/Makefile.app

run: $(TARGET)
	~/bin/CCore-BeagleBoot.exe r 192.168.99.10 $(TARGET) && cd root && ~/bin/CCore-PTPServer.exe c
	
