#===============================================================================
#
#  Description: Makefile the for quadpe-applications
#
#===============================================================================

#-------------  name of the application-----------------------------------------
TARGET         ?= app.mem

MAKEFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
CURRENT_DIR := $(dir $(MAKEFILE_PATH))

#-------------  path to libraries  ---------------------------------------------
LIB_SARK         ?= $(CURRENT_DIR)/
#LIB_QPE         = $(CURRENT_DIR)old
#LIB_M           = $(CURRENT_DIR)float-libm
LIB_E           = $(CURRENT_DIR)event_based_api
#LIB_ML           = $(CURRENT_DIR)ml-lib
LIBRARIES      += $(LIB_QPE) $(LIB_E)

# ------------  additional libraries  ------------------------------------------
#LIBS           += -lqpe -lm -le -lml

# ------------  linkerscript  --------------------------------------------------
LINKERSCRIPT    = $(LIB_SARK)/lnk/sark2.ld
#LINKERSCRIPT    = $(LIB_SARK)/lnk/sark2_64k.ld

# ------------  standard defines  ----------------------------------------------
PREDEFS        ?= USE_RECORD

ifdef ICPRO_DIR
PREDEFS        += PRINTF_SIM
endif

include $(CURRENT_DIR)spinn2_common.mk

# forcing relink....
ELF_TARGET = $(TARGET_DIR)/$(TARGET:.mem=.elf)
$(ELF_TARGET):

dump:
	$(OD) $(TARGET_DIR)/$(basename $(TARGET)).elf

mrproper: clean
	-rm -f ./*.rec ./run.log

.PHONY: dump mrproper

# vim: filetype=make
