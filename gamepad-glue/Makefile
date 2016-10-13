CC    := gcc
CXX   := g++
UNAME := $(shell uname -s)

OPTIMIZATION := -O3 -DNDEBUG
WARNINGS := -Wall -Wunreachable-code -Wfloat-equal -Wredundant-decls -Winit-self -Wpedantic
CFLAGS   := $(WARNINGS) $(OPTIMIZATION) -std=c99 -fPIC
CXXFLAGS := $(WARNINGS) $(OPTIMIZATION) -std=c++11
LDFLAGS  := -shared -fPIC
OBJDIR   := build

ifeq ($(UNAME),Darwin)
LIB_EXT :=.dylib
LDLIBS  += -framework IOKit -framework CoreFoundation
else ifeq ($(UNAME),Linux)
LIB_EXT :=.so
LDLIBS  += -pthread
endif

GAMEPAD_SRC_DIR := gamepad-backend
GAMEPAD_BUILD_DIR := $(GAMEPAD_SRC_DIR)/build
GAMEPAD_LIB := $(GAMEPAD_BUILD_DIR)/libGamepad.a

SOURCES := gamepad-glue.c
OBJECTS := $(addprefix $(OBJDIR)/objects/, $(SOURCES:.c=.o))
TARGET  := $(OBJDIR)/libGamepadGlue$(LIB_EXT)

TEST_SOURCES := test.c
TEST_OBJECTS := $(addprefix $(OBJDIR)/objects/, $(TEST_SOURCES:.c=.o))

.PHONY: all debug $(GAMEPAD_LIB) clean

all: $(TARGET)

-include $(OBJECTS:.o=.d)

debug: CFLAGS += -g -O1 -UNDEBUG
debug: CXXFLAGS += -g -O1 -UNDEBUG
debug: LDFLAGS += -g
debug: all

$(OBJECTS): | $(OBJDIR)/objects/
$(TEST_OBJECTS): | $(OBJDIR)/objects/

$(GAMEPAD_LIB):
	@$(MAKE) -C $(GAMEPAD_SRC_DIR)

test: $(TEST_OBJECTS) $(GAMEPAD_LIB)
	@printf "\e[1;32m LINK\e[m $@\n"
	@$(CC) $^ $(LDLIBS) -o $@

$(TARGET): $(OBJECTS) $(GAMEPAD_LIB)
	@printf "\e[1;32m LINK\e[m $@\n"
	@$(CC) $^ $(LDFLAGS) $(LDLIBS) -o $@

$(OBJDIR)/%.d:
	@true

$(OBJDIR)/objects/%.o: %.c
	@printf "\e[1;34m   CC\e[m $<\n"
	@$(CC) $(CFLAGS) -MMD -MF $(@:.o=.d) -c $< -o $@
	@sed -e 's/.*://' -e 's/\\$$//' -e 's/^ *//' < $(@:.o=.d) | fmt -1 | sed -e 's/$$/:/' >> $(@:.o=.d)

$(OBJDIR)/objects/%.o: %.cpp
	@printf "\e[1;34m  CXX\e[m $<\n"
	@$(CXX) $(CXXFLAGS) -MMD -MF $(@:.o=.d) -c $< -o $@
	@sed -e 's/.*://' -e 's/\\$$//' -e 's/^ *//' < $(@:.o=.d) | fmt -1 | sed -e 's/$$/:/' >> $(@:.o=.d)

$(OBJDIR):
	@printf "\e[1;33mMKDIR\e[m $@\n"
	@mkdir -p $@

$(OBJDIR)/%/: | $(OBJDIR)
	@printf "\e[1;33mMKDIR\e[m $@\n"
	@mkdir -p $@

clean:
	@printf "\e[1;31m   RM\e[m $(OBJDIR)\n"
	@$(MAKE) -C $(GAMEPAD_SRC_DIR) clean
	@rm -rf $(OBJDIR)
