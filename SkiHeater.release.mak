###########################################################
# Makefile generated by xIDE                               
#                                                          
# Project: SkiHeater
# Configuration: Release
# Generated: �� 10. ��� 01:02:11 2016
#                                                          
# WARNING: Do not edit this file. Any changes will be lost 
#          when the project is rebuilt.                    
#                                                          
###########################################################

OUTPUT=SkiHeater
OUTDIR=C:/Users/flaks/OneDrive/Documents/HC-04-06/SkiHeater
HARDWARE_INDEX=1
DEFS=-DNO_DEBUG 

DEBUGTRANSPORT=SPITRANS=USB SPIPORT=0
EXECUTION_MODE=vm
STACKSIZE=0
TRANSPORT=raw
FIRMWARE=unified
FIRMWAREIMAGE=
LIBRARY_VERSION=

LIBS=-lconnection -lspp -lregion -lservice -lbdaddr -lsdp_parse 
INPUTS=\
      Heater.led\
      bt_control_parse.parse\
      main.c\
      BtTask.c\
      PioTask.c\
      main.h\
      BtTask.h\
      PioTask.h\
      global.h
# Project-specific options
characters=0
messages=0
pcvalue=0
primitives=0
timestamps=0

-include SkiHeater.mak
include $(BLUELAB)/Makefile.vm