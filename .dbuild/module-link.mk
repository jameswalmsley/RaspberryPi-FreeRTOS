#
#	Standard module linker
#

#
# Here we filter out "archive" targets, because they are special!
#
AR_TARGETS=$(filter %.a, $(TARGETS))

$(AR_TARGETS):
ifeq ($(DBUILD_VERBOSE_CMD), 0)
	$(Q)$(PRETTY) --dbuild "AR" $(MODULE_NAME) $@
endif
	$(Q)$(AR) rvs $@ $(OBJECTS) 1> /dev/null 2> /dev/null

#
# RI Targets are the standard targets with the AR_TARGETS filtered out.
#


TEMP_TARGETS=$(filter-out $(AR_TARGETS), $(TARGETS))
KERN_TARGETS=$(filter-out *.img, $(TEMP_TARGETS))

RI_TARGETS=$(filter-out $(KERN_TARGETS), $(TEMP_TARGETS))


$(RI_TARGETS):
ifeq ($(RBUILD_VERBOSE_CMD), 0)
	$(Q)$(PRETTY) --rbuild "LD" $(MODULE_NAME) $@
endif
	$(Q)$(CXX) $(OBJECTS) $(ARCHIVES) $(LDFLAGS) $(LDLIBS) -o $@


%: %.c
ifeq ($(RBUILD_VERBOSE_CMD), 0)
	$(Q)$(PRETTY) --rbuild "CC|LD" $(MODULE_NAME) $@
endif
	$(Q)$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) $< -o $@

output/%: source/%.c
ifeq ($(RBUILD_VERBOSE_CMD), 0)
	$(Q)$(PRETTY) --rbuild "CC|LD" $(MODULE_NAME) $@
endif
	$(Q)$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) $< -o $@

output/%: source/%.o
ifeq ($(RBUILD_VERBOSE_CMD), 0)
	$(Q)$(PRETTY) --rbuild "LD" $(MODULE_NAME) $@
endif
	$(Q)$(CC) $(LDFLAGS) $(LDLIBS) $< -o $@

