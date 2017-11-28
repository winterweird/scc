SCC_ROOT_DIR = ../../..
CXX = g++
CXXFLAGS = -I/usr/include/SDL2 -I$(SCC_ROOT_DIR)/include -std=c++11 \
           -DSCC_OVERRIDE_HAVE_MACROS
LDLIBS = -lSDL2

TESTOBJ = main.o
SRCDIR = $(SCC_ROOT_DIR)/src
OBJ = window.o renderer.o
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
