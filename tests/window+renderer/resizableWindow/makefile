SCC_ROOT_DIR = ../../..
SCC_OVERRIDE_FLAGS = -DSCC_OVERRIDE_HAVE_MACROS
export SCC_OVERRIDE_FLAGS

CXX = g++
CXXFLAGS = -I/usr/include/SDL2 -I$(SCC_ROOT_DIR)/include -std=c++11 $(SCC_OVERRIDE_FLAGS)
LDLIBS = -lSDL2

TESTOBJ = main.o
SRCDIR = $(SCC_ROOT_DIR)/src
OBJ = window.o renderer.o texture.o rwops.o
BIN = resizableWindow

$(BIN) : $(OBJ) $(TESTOBJ)
	$(CXX) -o $(BIN) $^ $(LDLIBS)

$(TESTOBJ) : $(TESTOBJ:.o=.cpp)
	$(CXX) -c -o $@ $^ $(CXXFLAGS)

$(OBJ) :
	$(MAKE) -C $(SRCDIR) $@
	mv $(addprefix $(SRCDIR)/, $@) ./

clean :
	rm -f $(TESTOBJ) $(OBJ)

.PHONY: clean
