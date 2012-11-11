#
# Here we define the RBUILD default implicit C object build rule.
#
# This overrides Make's default implicit rule for C objects.
#

#
#	Include all object dependencies.
#
-include $(OBJECTS:.o=.d)

NEW_OBJECTS = $(addprefix $(BUILD_DIR),$(OBJECTS))

#NEW_OBJECTS = $(subst $(BASE),$(BUILD_DIR),$(OBJECTS))

ifeq ($(V), 3)
qd=
else
qd=@
endif


$(BUILD_DIR)%.o: $(BASE)%.c
ifeq ($(DBUILD_VERBOSE_CMD), 0)											# Pretty print on successful compile, but still display errors when they occur.
	$(Q)$(PRETTY) --dbuild "CC" $(MODULE_NAME) $(notdir $@)
endif
	@mkdir -p $(dir $@)
	$(Q)$(CC) -MD -MP $(CFLAGS) $< -o $@

#	@echo $($@:$(BASE)="jim")#$(BASE), "JIM", $(dir $@))

#	$(patsubst $(BASE), "", $(BUILD_ROOT)$@)
	$(POST_CC)

#ifeq ($(DBUILD_VERBOSE_DEPS), 1)
# 	$(Q)$(PRETTY) --dbuild "DEP" $(MODULE_NAME) $*.d
#endif
## http://scottmcpeak.com/autodepend/autodepend.html						# Autogenerate Dependencies for object.
# 	$(qd)$(CC) -MD -MP $(CFLAGS) $*.c



#																		# Fix the GCC dep file output format.
# 	$(qd)mv -f $*.d $*.d.tmp
# 	$(qd)sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
# 	$(qd)sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
# 	 sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
# 	$(qd)rm -f $*.d.tmp

$(OBJECTS): $(dir $(BUILD_ROOT)$($@:$(BASE)=""))

$(dir $(BUILD_ROOT)$($@:$(BASE)="")):
	mkdir -p $@


test:
	echo $(BASE)
	echo $(BUILD_DIR)
	echo $(OBJECTS)
	echo $(NEW_OBJECTS)

