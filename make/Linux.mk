
# common Linux settings

#USE_OPENMP          = 1
DSO_LDFLAGS        += -shared -luuid
DSO_SUFFIX          = so
WINDOW_SYSTEM      ?= GLX
WINDOW_SYSTEM_INCS += -I/usr/X11R6/include
AR                  = ld
ARFLAGS             = -r

# SUBARCH-specific settings

include $(TOP)/make/$(ARCH).$(SUBARCH).mk
