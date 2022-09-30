# ===============================================================================
#
#  Description: Common makefile for arm applications and libraries
#
#===============================================================================

#-------------  use linktime optimization  -------------------------------------
USE_LTO        ?= NO

#-------------  compile with profiling  ----------------------------------------
PROFILE        ?= NO

#-------------  list of all source files  --------------------------------------
SOURCES        ?= *.c *.cc

#-------------  directory of source files --------------------------------------
SOURCE_DIR     ?= .

#-------------  directory of the dependency  -----------------------------------
DEPS_DIR       ?= ./.deps

#-------------  directory of the target  ---------------------------------------
TARGET_DIR     ?= ./binaries

#-------------  directory of the asm files -------------------------------------
ASM_DIR        ?= ./.asm

#-------------  directory for includes and libraries  --------------------------
INC_DIR        ?= .
ALL_INC_DIR     = $(INC_DIR) $(addsuffix /include, $(LIBRARIES))
ALL_LIB_DIR     = $(LIB_DIR) $(addsuffix /lib, $(LIBRARIES))

#-------------  path to doxyfile  ----------------------------------------------
DOXYFILE       ?= ./doxy/Doxyfile
DOXY_DOCPATH   ?= ./doc

#-------------  quadpe_jtag_app arguments  -----------------------------------
EXEC_SCRIPT    ?= run.tcl
LOGLEVEL       ?= 1
LOGFILE        ?= run.log

# ------------  build-tools  ---------------------------------------------------
GP             ?= arm-none-eabi-
CC             := $(GP)gcc
CXX            := $(GP)g++
LD             := $(GP)ld
OD             := $(GP)objdump -dxt -S
OC             := $(GP)objcopy
AR             := $(GP)ar
SIZE           := $(GP)size
LN             := ln -sf
RM             := rm -f
RMDIR          := rmdir
MKDIR          := mkdir -p

# ------------  compiler flags  ------------------------------------------------
WARNFLAGS      ?= -Wall
# DEBUG!
OPTFLAGS       ?= -O2 -fdata-sections -ffunction-sections \
                  -fsingle-precision-constant -fdiagnostics-color 
ARCHFLAGS      ?= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mfp16-format=ieee  -mthumb
CFLAGS         ?= -std=gnu99 $(MEAS_OP)
CXXFLAGS       ?= -fno-exceptions -fno-rtti
CPPFLAGS       ?= $(addprefix -D, $(DEFS) $(PREDEFS)) $(addprefix -I, $(ALL_INC_DIR)) $(EXTRA_CPPFLAGS)
LDFLAGS        := -Wl,-O1 -Wl,--gc-sections -T$(LINKERSCRIPT) -nostartfiles -nostdlib  
ASMFLAG        ?= -adhln

ifeq ($(USE_LTO), YES)
OPTFLAGS       := -flto  $(OPTFLAGS)
LDFLAGS        := -flto -fuse-linker-plugin $(LDFLAGS)
endif

ifeq ($(PROFILE), YES)
PIFLAGS=-pg
endif

ALL_CFLAGS    := $(WARNFLAGS) $(PIFLAGS) $(OPTFLAGS) $(ARCHFLAGS) $(CFLAGS)   $(CPPFLAGS)
ALL_CXXFLAGS  := $(WARNFLAGS) $(PIFLAGS) $(OPTFLAGS) $(ARCHFLAGS) $(CXXFLAGS) $(CPPFLAGS)
ALL_LDFLAGS   := $(PIFLAGS) $(LDFLAGS)

# ------------  generate the names of the source/object files  -----------------
ALL_SOURCES    = $(wildcard $(addprefix  $(SOURCE_DIR)/,$(SOURCES)))
BASENAMES      = $(basename $(ALL_SOURCES:$(SOURCE_DIR)/%=%))
OBJECTS        = $(addprefix $(TARGET_DIR)/, $(addsuffix .o, $(BASENAMES)))
DEPS           = $(addprefix $(DEPS_DIR)/, $(addsuffix .d, $(BASENAMES)))

# ------------ basic compile rule ----------------------------------------------

.DEFAULT_GOAL := all

all: $(TARGET_DIR)/$(TARGET)

$(DEPS_DIR):
	@$(MKDIR) $(DEPS_DIR)

$(TARGET_DIR):
	@$(MKDIR) $(TARGET_DIR)

$(ASM_DIR):
	@$(MKDIR) $(ASM_DIR)

$(DEPS_DIR)/%.d: $(SOURCE_DIR)/%.c $(MAKEFILE_LIST) | $(DEPS_DIR)
	@$(CC) -MM -E $(CPPFLAGS) $< | perl -p -e 's#[^:]*:#$(TARGET_DIR)/$$&#' > $@
	@$(LN) $*.d $(DEPS_DIR)/$*.dep

$(DEPS_DIR)/%.d: $(SOURCE_DIR)/%.cc $(MAKEFILE_LIST) | $(DEPS_DIR)
	@$(CXX) -MM -E $(CPPFLAGS) $< | perl -p -e 's#[^:]*:#$(TARGET_DIR)/$$&#' > $@
	@$(LN) $*.d $(DEPS_DIR)/$*.dep

-include ${DEPS:.d=.dep}

$(TARGET_DIR)/%.o: $(SOURCE_DIR)/%.c $(DEPS_DIR)/%.d | $(TARGET_DIR) $(ASM_DIR)
	$(CC) $(ALL_CFLAGS) -c $< -o $@ -Wa,$(ASMFLAG)=$(ASM_DIR)/$*.s -g

$(TARGET_DIR)/%.o: $(SOURCE_DIR)/%.cc $(DEPS_DIR)/%.d | $(TARGET_DIR) $(ASM_DIR)
	$(CXX) $(ALL_CXXFLAGS) -c $< -o $@ -Wa,$(ASMFLAG)=$(ASM_DIR)/$*.s -g

#$(TARGET_DIR)/%.o: $(SOURCE_DIR)/%.c $(DEPS_DIR)/%.d | $(TARGET_DIR) $(ASM_DIR)
#	$(CC) $(ALL_CFLAGS) -c $< -o $@ -Wa,$(ASMFLAG)=$(ASM_DIR)/$*.s 

#$(TARGET_DIR)/%.o: $(SOURCE_DIR)/%.cc $(DEPS_DIR)/%.d | $(TARGET_DIR) $(ASM_DIR)
#	$(CXX) $(ALL_CXXFLAGS) -c $< -o $@ -Wa,$(ASMFLAG)=$(ASM_DIR)/$*.s 



$(TARGET_DIR)/%.o: $(SOURCE_DIR)/%.s | $(TARGET_DIR)
	$(CC) $(ALL_CFLAGS) -c $< -o $@


$(TARGET_DIR)/%.a: $(OBJECTS)
	$(AR) rcs $@ $(OBJECTS)

$(TARGET_DIR)/%.elf: $(OBJECTS)
	$(CC) $(OBJECTS) $(WARNFLAGS) $(ARCHFLAGS) $(ALL_LDFLAGS) -o $@ $(addprefix -L, $(ALL_LIB_DIR)) $(LIBS)
	$(SIZE) $@

$(TARGET_DIR)/%.mem: $(TARGET_DIR)/%.elf
	@$(OC) -O verilog $< $@

EXEC_ENV               ?= MEMFILE=$(TARGET_DIR)/$(TARGET)
EXEC_APP               ?= quadpe_jtag_app
EXEC_APP_ARGS_DEFAULT   = $(addprefix -L , $(LOGLEVEL))
ifdef LOGFILE
EXEC_APP_ARGS_DEFAULT  += $(addprefix --logfile , $(LOGFILE))
endif
EXEC_APP_ARGS_DEFAULT  += $(addprefix --script , $(EXEC_SCRIPT))

EXEC_APP_ARGS          ?= $(EXEC_APP_ARGS_DEFAULT)


doc:
	doxygen $(DOXYFILE) |grep -i warning; true

viewdoc:
	@$${BROWSER:-firefox} $(DOXY_DOCPATH)/html/index.html >/dev/null 2>&1 &

clean:
	-@$(RM) $(addprefix $(TARGET_DIR)/, *.o *.d *.a *.s *.elf *.mem $(TARGET)) $(ASM_DIR)/*.s $(addprefix $(DEPS_DIR)/, *.d *.dep)
	-@$(RMDIR) $(DEPS_DIR) $(TARGET_DIR) $(ASM_DIR) 2>/dev/null; true
	-@$(RM) -rf $(addprefix ./$(DOXY_DOCPATH)/, html latex rtf man); true
	-@$(RMDIR) $(DOXY_DOCPATH) 2>/dev/null; true


ifndef ICPRO_DIR
run: $(TARGET_DIR)/$(TARGET)
	$(EXEC_ENV) $(EXEC_APP) $(EXEC_APP_ARGS)
else

EXPORT_TARGET := $(shell /usr/bin/env perl -e '$$_ = "$(CURDIR)"; s|.*/([^/]+)\.([^/]+)$$|$$ENV{ICPRO_DIR}/units/$$1/simulation/ncsim/$$2/progmem/arm.mem|; print')

export: SHELL:=/bin/sh
export: $(TARGET_DIR)/$(TARGET)
	@if [ -e `dirname $(EXPORT_TARGET)` ]; then \
		echo cp $(TARGET_DIR)/$(TARGET) $(EXPORT_TARGET); \
		     cp $(TARGET_DIR)/$(TARGET) $(EXPORT_TARGET); \
	else \
		echo "Target directory "`dirname $(EXPORT_TARGET)`" does not exist."; \
		false; \
	fi

run: SHELL:=/bin/sh
rungui: SHELL:=/bin/sh
runGUI: SHELL:=/bin/sh
run rungui runGUI: export
	@if [ -e `dirname $(EXPORT_TARGET)` ]; then                               \
		cd `dirname $(EXPORT_TARGET)`/..;                                     \
	    icncsim create-temp-links > /dev/null 2> /dev/null;                   \
		source $(MODULESHOME)/init/sh;                                        \
		module load $(ICPRO_IUS_MODULE);                                      \
		$(MAKE) -s -f Makefile.ncsim $@;                                      \
	else                                                                      \
		echo "Target directory "`dirname $(EXPORT_TARGET)`" does not exist."; \
		false;                                                                \
	fi

execute: SHELL:=/bin/sh
execute: export
	@if [ -e `dirname $(EXPORT_TARGET)` ]; then                               \
		cd `dirname $(EXPORT_TARGET)`/..;                                     \
	    icncsim create-temp-links > /dev/null 2> /dev/null;                   \
		source $(MODULESHOME)/init/sh;                                        \
		module load $(ICPRO_IUS_MODULE);                                      \
		$(MAKE) -s -f Makefile.ncsim run | sed -n -e 's/quadpe PRINTF: //p';  \
	else                                                                      \
		echo "Target directory "`dirname $(EXPORT_TARGET)`" does not exist."; \
		false;                                                                \
	fi

.PHONY: export rungui runGUI execute

endif

.SECONDARY: $(DEPS) $(OBJECTS) $(addprefix $(basename $(TARGET_DIR)/$(TARGET)), .elf .mem)

.PHONY: all run doc view clean 

