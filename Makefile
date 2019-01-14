CXX := g++
CXXFLAGS := -std=c++14
PYMAKE_COMPILER := $(CXX)
PYMAKE_COMPILER_FLAGS := $(CXXFLAGS)
SRCEXT := cpp
SRCDIR := src
BUILDDIR := build
INSTALL_PATH := /usr/local/bin
TARGET := exe
TARGET_DIR := bin
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
LIB :=  -framework OpenGL -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network
else
LIB := -lGL -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network -pthread
endif
#SOURCES := $(wildcard $(SRCDIR)/*.$(SRCEXT))
SOURCES := $(shell find $(SRCDIR) -name "*.cpp")
OBJECTS := $(patsubst $(SRCDIR)/%.o,$(BUILDDIR)/%.o,$(SOURCES:.$(SRCEXT)=.o))

OBJDIRS:=$(dir $(OBJECTS))
DUMMY:=$(shell mkdir --parents $(OBJDIRS))


all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(PYMAKE_COMPILER) -o $(TARGET_DIR)/$(TARGET) $^ $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	$(PYMAKE_COMPILER) $< $(PYMAKE_COMPILER_FLAGS) -c -o $@ -MMD -MF $(@:.o=.d)


run: all
	(cd $(TARGET_DIR) && ./$(TARGET))


-include $(wildcard *.d)

