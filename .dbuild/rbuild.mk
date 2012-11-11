#
#	Riegl Builder - Designed and Created by James Walmsley
#
#	Riegl Builder attempts to provide a clean framework for creating organised
#	and "pretty" builds with minimal effort.
#
#	Riegl Builder is based upon the vebuild as found in the FullFAT project
#	in the .vebuild directory.
#
#	@see 		github.com/FullFAT/FullFAT/
#	@author 	James Walmsley <jwalmsley@riegl.com>
#
#	@version	1.0.0 (Armstrong) -
#

RBUILD_VERSION_MAJOR=1
RBUILD_VERSION_MINOR=0
RBUILD_VERSION_REVISION=0

RBUILD_VERSION_NAME=Armstrong
RBUILD_VERSION_DATA=May 2012

#
#	Let's ensure we have a well purified MAKE environment, and make its output less
#	redundant.
#
MAKEFLAGS += -rR --no-print-directory

#
#	Optional Include directive, rbuild attempts to build using lists of objects,
#	targets and subdirs as found in objects.mk and subdirs.mk
#
-include objects.mk
-include targets.mk
-include subdirs.mk

#
#	A top-level configuration file can be found in the project root file.
#
-include $(BASE).rbuild.config.mk

#
#	This config file can be overridden or extended in any sub-directory
#
-include .rbuild.config.mk

#
#	Provide a default target named all,
#	This is dependent on $(TARGETS) and $(SUBDIRS)
#
#	All is finally dependent on silent, to keep make silent when it has
#	nothing more to do.
#
rbuild_entry: rbuild_splash | all
all: $(TARGETS) $(SUBDIRS) $(MODULE_TARGET) | silent

#
#	Defaults for compile/build toolchain environments.
#
CC		= gcc
CXX		= g++
CFLAGS	+= -c

#
#	Over-time we can provide some simple macros to force certain kinds
#	of build, without having to modify any make files.
#
#	Assuming that no sub-project overrides our CFLAGS.
#
ifeq ($(CONFIG_i386), 1)
CFLAGS  += -m32
LDFLAGS += -m32
endif

ifeq ($(RBUILD_TODO), 1)
RBUILD_TODOSTRINGS="(TODO|FIXME|TOFIX|FINISHME|BROKEN)"
POST_CC = $(Q)-grep -EHn $(RBUILD_TODOSTRINGS) $< | $(PTODO) $(BASE)rbuild.todo $(MODULE_NAME)
endif

#
#	Include the different rbuild modules
#
include $(BASE).rbuild/verbosity.mk
include $(BASE).rbuild/pretty.mk
include $(BASE).rbuild/subdirs.mk
include $(BASE).rbuild/clean.mk
include $(BASE).rbuild/module-link.mk
include $(BASE).rbuild/c-objects.mk
include $(BASE).rbuild/cpp-objects.mk

#
#	Rbuild Splash
#
.PHONY: rbuild_splash
rbuild_splash:
ifeq ($(RBUILD_SPLASHED), 1)
else
	@echo " Riegl Builder - Unified Build Environment"
	@echo " Version ($(RBUILD_VERSION_MAJOR).$(RBUILD_VERSION_MINOR).$(RBUILD_VERSION_REVISION) - $(RBUILD_VERSION_NAME))"
ifeq ($(RBUILD_TODO), 1)
	@rm -rf $(BASE)rbuild.todo
	@touch $(BASE)rbuild.todo
	@echo "Filename                  : Line  : Module          : TODO message" >> $(BASE)rbuild.todo
	@echo "===============================================================================" >> $(BASE)rbuild.todo
endif
endif

#
#	Finally provide an implementation of the silent target.
#
silent:
	@:
