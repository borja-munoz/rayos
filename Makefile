#
#  Makefile
#  Referencia: https://hiltmon.com/blog/2015/09/28/the-simple-c-plus-plus-makefile-executable-edition/
#

# HIL: No spaces or comments after otherwise it captures them!
# Determine the platform
UNAME_S := $(shell uname -s)

# CC
ifeq ($(UNAME_S),Darwin)
  CC := clang++ -arch x86_64
else
  CC := g++
endif

# Folders
SRCDIR := src
BUILDDIR := build
TARGETDIR := bin

# Targets
EXECUTABLE := rayos
TARGET := $(TARGETDIR)/$(EXECUTABLE)

# Final Paths
INSTALLBINDIR := bin

# Code Lists
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name '*.$(SRCEXT)')
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

# Folder Lists
# Note: Intentionally excludes the root of the include folder so the lists are clean
INCDIRS := $(shell find include/**/* -name '*.h' -exec dirname {} \; | sort | uniq)
INCLIST := $(patsubst include/%,-I include/%,$(INCDIRS))
BUILDLIST := $(patsubst include/%,$(BUILDDIR)/%,$(INCDIRS))
# MYINCDIRS := .
# INCDIRS := $(shell find $(MYINCDIRS) -name '*.h' -exec dirname {} \; | sort | uniq)
# INCLIST := $(patsubst $(MYINCDIRS)/%,-I include/%,$(INCDIRS))
# BUILDLIST := $(patsubst $(INCDIRS)/%,$(BUILDDIR)/%,$(INCDIRS))

# Shared Compiler Flags
CFLAGS := -c -g -msse2
INC := -I include $(INCLIST) -I /usr/local/include
# LIB := -L 
LIB := -L /usr/local/lib -lpbrtParser

# Platform Specific Compiler Flags
ifeq ($(UNAME_S),Linux)
    CFLAGS += -std=gnu++11 -O2 # -fPIC
else
  CFLAGS += -std=c++11 -stdlib=libc++ -O3 -fdeclspec
endif

$(TARGET): $(OBJECTS)
	@mkdir -p $(TARGETDIR)
	@echo "Linking..."
	@echo "  Linking $(TARGET)"; $(CC) $^ -o $(TARGET) $(LIB) 

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDLIST)
	@echo "Compiling $<..."; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo "Cleaning $(TARGET)..."; $(RM) -r $(BUILDDIR) $(TARGET)

install:
	@echo "Installing $(EXECUTABLE)..."; cp $(TARGET) $(INSTALLBINDIR)
  
distclean:
	@echo "Removing $(EXECUTABLE)"; rm $(INSTALLBINDIR)/$(EXECUTABLE)

.PHONY: clean
