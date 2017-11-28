SCC_ROOT_DIR = ../..

# comment/uncomment the -D flags below to test
SCC_OVERRIDE_FLAGS = \
                     # -DSCC_OVERRIDE_HAVE_MACROS \
                     # -DHAVE_SDL_IMAGE \
                     # -DHAVE_SDL_TTF \
                     # -DHAVE_SDL_MIXER
export SCC_OVERRIDE_FLAGS

CXX = g++
LDLIBS = -lSDL2

CXXFLAGS = -I/usr/include/SDL2 -I$(SCC_ROOT_DIR)/include -std=c++11 $(SCC_OVERRIDE_FLAGS)

OBJ = main.o
BIN = configTest

$(BIN) : $(OBJ) $(TESTOBJ)
	$(CXX) -o $@ $^ $(LDLIBS)

%.o : %.c
	$(CXX) -c -o $@ $^ $(CXXFLAGS)

clean:
	rm -f $(OBJ)

.PHONY: clean
