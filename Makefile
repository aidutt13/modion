CXX := clang++
CXXFLAGS := -std=c++20 -Wall -Wextra -Werror -pedantic

EXT := cpp

SRCDIR := src
BUILDDIR := build
INCLUDEDIRS := include;
TARGET := bin/modion

LIBS := -lcurl -lcurlpp

INCLUDES := $(patsubst %;,-I%,$(INCLUDEDIRS))

SRC := $(wildcard $(SRCDIR)/*.$(EXT)) $(wildcard $(SRCDIR)/*/*.$(EXT))
OBJ := $(patsubst $(SRCDIR)/%.$(EXT),$(BUILDDIR)/%.o,$(SRC))
JSONS := $(subst /,_,$(OBJ))

$(TARGET): $(OBJ)
	@mkdir -p $(@D)
	$(shell sed -e '1s/^/[\n/' -e '$$s/,$$/\n]/' $(BUILDDIR)/json/*.o.json > $(BUILDDIR)/compile_commands.json)
	$(CXX) $(CXXFLAGS) $(LIBS) -o $@ $^

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(EXT)
	@mkdir -p $(@D) $(BUILDDIR)/json
	$(CXX) -MJ $(patsubst %.o,$(BUILDDIR)/json/%.o.json,$(subst /,_,$@)) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

.PHONY: clean debug release run

clean:
	rm -f $(BUILDDIR)/*.o $(BUILDDIR)/*/*.o $(BUILDDIR)/json/*.json $(TARGET)

debug: CXXFLAGS += -g
debug: $(TARGET)

release: CXXFLAGS += -O3
release: $(TARGET)

run: $(TARGET)
	./$(TARGET)
