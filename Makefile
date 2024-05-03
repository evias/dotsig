# Makefile
EXECUTABLE_FILE = dotsig
TARGET_DIR = .
BUILD_DIR = ./build
SOURCES_DIR = ./src

TARGET = $(TARGET_DIR)/$(EXECUTABLE_FILE)

BOTAN_CC = `botan config cflags`
BOTAN_LIBS = `botan config libs`

CC = clang++ -arch x86_64
XX_FLAGS = -std=c++20
CXX = $(CC) $(XX_FLAGS) $(BOTAN_CC)

PLATFORM := $(shell uname -s)
SOURCES = $(shell find $(SOURCES_DIR) -type f -name *.cpp)
OBJECTS := $(patsubst $(SOURCES_DIR)/%,$(BUILD_DIR)/%,$(SOURCES:.cpp=.o))

MANPREFIX = /usr/local/share/man/man1
ifeq ($(PREFIX),)
	PREFIX := /usr/local
endif

.PHONY: install

${TARGET}: $(OBJECTS)
	@echo "Linking $(TARGET)"; $(CC) $(OBJECTS) -o $(TARGET) $(BOTAN_LIBS)
	@chmod a+x $(TARGET)
	@echo ""
	@echo "dotsig built successfully"
	@echo "To run dotsig, use: $(TARGET)"

$(BUILD_DIR)/%.o: $(SOURCES_DIR)/%.cpp
	@echo "Compiling $<..."; $(CXX) -c -o $@ $<

install: $(TARGET)
	@echo "Installing $(TARGET) in $(DESTDIR)$(PREFIX)/bin/"
	@install -d $(DESTDIR)$(PREFIX)/bin
	@install -m 0755 $(EXECUTABLE_FILE) $(DESTDIR)$(PREFIX)/bin/
	@echo "Installing manpage in $(DESTDIR)$(MANPREFIX)"
	@install -m 0755 docs/dotsig.1 $(DESTDIR)$(MANPREFIX)
	@gzip $(DESTDIR)$(MANPREFIX)/dotsig.1

clean:
	rm -f $(BUILD_DIR)/* $(TARGET)
