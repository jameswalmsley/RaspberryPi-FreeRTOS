#
#	Riegl Builder - Subdirectory handling.
#
#	@author	James Walmsley <jwalmsley@riegl.com>
#

.PHONY:$(SUBDIRS)

#
#	Each listed item in the SUBDIRS variable shall be executed in parralel.
#	We must therefore take care to provide a make job server.
#	Hence the +make command.
#

include $(addsuffix objects.mk, $(SUB_OBJDIRS))

$(SUBDIRS):
ifeq ($(DBUILD_VERBOSE_CMD), 0)
	$(Q)$(PRETTY) --dbuild "BUILD" $(MODULE_NAME) "Building $@"
endif
	$(Q)$(MAKE) -C $@ DBUILD_SPLASHED=1 $(SUBDIR_PARAMS)


#
#	Sub-dir Clean targets. (Creates $SUBDIR.clean).
#
$(SUBDIRS:%=%.clean):
ifeq ($(DBUILD_VERBOSE_CMD), 0)
	$(Q)$(PRETTY) --dbuild "CLDIR" $(MODULE_NAME) "$(@:%.clean=%)"
endif
	$(Q)$(MAKE) -C $(@:%.clean=%) DBUILD_SPLASHED=1 $(SUBDIR_PARAMS) clean


#
#	Hook sub-dir cleaning to the main clean method.
#
clean.subdirs: $(SUBDIRS:%=%.clean)
	@:
clean: clean.subdirs
