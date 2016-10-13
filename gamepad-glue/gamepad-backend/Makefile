CC    := gcc
CXX   := g++
AR    := ar
UNAME := $(shell uname -s)

OPTIMIZATION := -O3 -DNDEBUG
WARNINGS := -Wall -Wunreachable-code -Wfloat-equal -Wredundant-decls -Winit-self -Wpedantic
CFLAGS   := $(WARNINGS) $(OPTIMIZATION) -std=c99 -fPIC
CXXFLAGS := $(WARNINGS) $(OPTIMIZATION) -std=c++11
LDFLAGS  :=
OBJDIR   := build
ARFLAGS  := rcs

CPPSOURCES  :=
CSOURCES    := Gamepad.c Gamepad_private.c
OBJECTS     := $(addprefix $(OBJDIR)/objects/, $(CPPSOURCES:.cpp=.o) $(CSOURCES:.c=.o))
Gamepad     := $(OBJDIR)/libGamepad.a

.PHONY: all debug clean
all: $(Gamepad)

-include $(OBJECTS:.o=.d)

debug: CFLAGS += -g -O1 -UNDEBUG
debug: CXXFLAGS += -g -O1 -UNDEBUG
debug: LDFLAGS += -g
debug: all

$(OBJECTS): | $(OBJDIR)/objects/

$(Gamepad): $(OBJECTS)
	@printf "\e[1;32m LINK\e[m $@\n"
	@$(AR) $(ARFLAGS) $@ $^
# @$(CC) $^ $(LDFLAGS) -o $@

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
	@rm -rf $(OBJDIR)

