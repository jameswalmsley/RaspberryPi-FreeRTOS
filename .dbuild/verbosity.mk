V=0

# Set the default verbosity FLAGS.
VERBOSE_DEPS_PRETTY=0
VERBOSE_OBJECTS=0
VERBOSE_LINK=0

DBUILD_VERBOSE_DEPS=0
DBUILD_VERBOSE_CMD=0

# If a verbosity level has been specified, then we switch the verbosity flags.
ifeq ($(V), -1)
DBUILD_VERBOSE_CMD=1
endif

ifeq ($(V), 1)
DBUILD_VERBOSE_DEPS=1
endif

ifeq ($(V), 2)
DBUILD_VERBOSE_CMD=1
endif

ifeq ($(V), 3)
DBUILD_VERBOSE_DEPS=1
DBUILD_VERBOSE_CMD=1
endif

#
# Make Command Verbosity Control!
# Here we prefix all commands with $(Q) instead of @ to make them silent.
#
# Then we can control the command printing dynamically using the V (verbosity) variable.
#
Q=@
ifeq ($(V), 2)
Q=
endif
ifeq ($(V), 3)
Q=
endif
