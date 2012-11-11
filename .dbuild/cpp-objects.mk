#
# Here we define the RBUILD default implicit C object build rule.
#
# This overrides Make's default implicit rule for C objects.
#

-include $(OBJECTS:.o=.d)												# Include all dependency information for all objects. (Prefixing - means if they exist).

ifeq ($(V), 3)
qd=
else
qd=@
endif

%.o : %.cpp
ifeq ($(RBUILD_VERBOSE_CMD), 0) 											# Pretty print on successful compile, but still display errors when they occur.
	$(Q)$(PRETTY) --rbuild "CXX" $(MODULE_NAME) $*.o
endif																	# Full verbose output for debugging (make V=1).
	$(Q)$(CXX) -MD -MP $(CXXFLAGS) $(CFLAGS) $*.cpp -o $*.o
	$(POST_CC)

ifeq ($(RBUILD_VERBOSE_DEPS), 1)
#	$(Q)$(PRETTY) --rbuild "DEP" $(MODULE_NAME) $*.d
endif
# http://scottmcpeak.com/autodepend/autodepend.html						# Autogenerate Dependencies for object.
#	$(qd)$(CXX) -MM $(CXXFLAGS) $(CFLAGS) $*.cpp -MF $*.d
#																		# Fix the GCC dep file output format.
#	$(qd)mv -f $*.d $*.d.tmp
#	$(qd)sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
#	$(qd)sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
#	 sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
#	$(qd)rm -f $*.d.tmp
