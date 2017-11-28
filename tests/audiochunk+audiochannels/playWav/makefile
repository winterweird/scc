SCC_ROOT_DIR = ../../..
SCC_OVERRIDE_FLAGS = -DSCC_OVERRIDE_HAVE_MACROS -DHAVE_SDL_MIXER
export SCC_OVERRIDE_FLAGS

DEBUG = -g
export DEBUG

CXX = g++
CXXFLAGS = -I/usr/include/SDL2 -I$(SCC_ROOT_DIR)/include $(DEBUG) -std=c++11 \
	$(SCC_OVERRIDE_FLAGS)
LDLIBS = -lSDL2 -lSDL2_mixer

TESTOBJ = main.o
SRCDIR = $(SCC_ROOT_DIR)/src
OBJ = audiochunk.o
BIN = playWav

$(BIN) : $(OBJ) $(TESTOBJ)
	$(CXX) -o $(BIN) $^ $(LDLIBS)

$(TESTOBJ) : $(TESTOBJ:.o=.cpp)
	$(CXX) -c -o $@ $^ $(CXXFLAGS)

$(OBJ) :
	$(MAKE) -C $(SRCDIR) $@
	mv $(addprefix $(SRCDIR)/, $@) ./

clean:
	rm -f $(TESTOBJ) $(OBJ)

.PHONY: clean
