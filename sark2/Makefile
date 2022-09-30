#===============================================================================
#
#  Description: Makefile the for qpe-library
#
#===============================================================================

#-------------  compile with profiling  ----------------------------------------
PROFILE         = NO

#-------------  name of the target  --------------------------------------------
TARGET          = sark2.a

# ------------  standard defines  ----------------------------------------------
PREDEFS         =

#-------------  directory of the target  ---------------------------------------
TARGET_DIR      = ./lib

#-------------  directory of source files --------------------------------------
SOURCE_DIR      = ./

# ------------  additional include directories  --------------------------------
INC_DIR         = ../include

include ../spinn2_common.mk

doc: doc_links

doc_links:
	@mkdir -p doc/html
	@ln -sf -T $(QUADPE_ROOT_PATH)/doc/html doc/html/globaldoc
