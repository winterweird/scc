SCC_ROOT_DIR = ..
include $(SCC_ROOT_DIR)/makefile.common

INCLUDE_DIRS := -I$(SDL_INCLUDE_DIR) -I../include

CXXFLAGS := $(INCLUDE_DIRS) $(EXTRA_INCLUDE_DIRS) -std=c++11 $(DEBUG) \
	$(SCC_HAVE_FLAGS)

OBJ := $(addprefix $(OBJDIR)/, $(patsubst %.cpp, %.o, $(wildcard *.cpp)))

all : $(OBJ)

$(OBJDIR)/%.o : %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(OBJ) : | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -f $(OBJ)

.PHONY: clean all
