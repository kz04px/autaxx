CC         = g++
CFLAGS     = -std=c++17 -Wall -Wextra -Wshadow
RFLAGS     = -O3 -flto -march=native -DNDEBUG
DFLAGS     = -g
INC        = -I./libs/libataxx/src

LINKER     = g++
LFLAGS     = -L./libs/libataxx/build/static/ -pthread -lataxx -O3 -flto

TARGET     = main
SRCDIR     = src
OBJDIR     = obj
BINDIR     = bin

SOURCES := $(shell find $(SRCDIR) -type f -name *.cpp)
OBJECTS := $(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(SOURCES:.cpp=.o))

$(BINDIR)/$(TARGET): $(BINDIR) $(OBJDIR) $(OBJECTS)
	@echo "Linking "$<
	@$(LINKER) -o $@ $(OBJECTS) $(LFLAGS)

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	@echo "Compiling "$<
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@

release:
	$(MAKE) CFLAGS="$(CFLAGS) $(RFLAGS)"

debug:
	$(MAKE) CFLAGS="$(CFLAGS) $(DFLAGS)" TARGET="$(TARGET)-debug"

bin:
	mkdir -p $(BINDIR)

obj:
	mkdir -p $(OBJDIR)

clean:
	rm -r $(OBJDIR) $(BINDIR)/$(TARGET) $(BINDIR)/$(TARGET)-debug

.PHONY: clean
