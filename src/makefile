SCC_ROOT_DIR = ..
include $(SCC_ROOT_DIR)/makefile.common
vpath %.cpp GL/

INCLUDE_DIRS := -I$(SDL_INCLUDE_DIR) -I../include

CXXFLAGS := $(INCLUDE_DIRS) $(EXTRA_INCLUDE_DIRS) -std=c++11 $(DEBUG) \
	$(SCC_HAVE_FLAGS)

OBJ := $(addprefix $(OBJDIR)/, $(patsubst %.cpp, %.o, $(wildcard *.cpp)))

GLOBJ := $(addprefix $(OBJDIR)/, $(patsubst %.cpp, %.o, \
	$(notdir $(wildcard GL/*.cpp))))

all : $(OBJ) $(GLOBJ)

$(OBJDIR)/%.o : %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(OBJ) : | $(OBJDIR)
$(GLOBJ) : | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -f $(OBJ) $(GLOBJ)

.PHONY: clean all
