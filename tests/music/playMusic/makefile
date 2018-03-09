# FIXME change this mess to use makefile.tests

SCC_ROOT_DIR = ../../..
SCC_HAVE_FLAGS = -DHAVE_SDL_MIXER
export SCC_HAVE_FLAGS

DEBUG = -g
export DEBUG

CXX = g++
CXXFLAGS = -I/usr/include/SDL2 -I$(SCC_ROOT_DIR)/include $(DEBUG) -std=c++11 \
	$(SCC_HAVE_FLAGS)
LDLIBS = -lSDL2 -lSDL2_mixer

TESTOBJ_COMMON = common.o
TESTOBJ_OGG = ogg.o
TESTOBJ_MIDI = midi.o
TESTOBJ_MP3 = mp3.o
TESTOBJ_FLAC = flac.o


BIN_OGG = playOgg
BIN_MIDI = playMidi
BIN_MP3 = playMp3
BIN_FLAC = playFlac

what :
	@echo 'make what?'

ogg : $(TESTOBJ_OGG) $(TESTOBJ_COMMON)
	$(CXX) -o $(BIN_OGG) $^ $(LDLIBS)
midi : $(TESTOBJ_MIDI) $(TESTOBJ_COMMON)
	$(CXX) -o $(BIN_MIDI) $^ $(LDLIBS)
mp3 : $(TESTOBJ_MP3) $(TESTOBJ_COMMON)
	$(CXX) -o $(BIN_MP3) $^ $(LDLIBS)
flac : $(TESTOBJ_FLAC) $(TESTOBJ_COMMON)
	$(CXX) -o $(BIN_FLAC) $^ $(LDLIBS)
all : ogg midi mp3 flac

$(TESTOBJ_OGG) : $(TESTOBJ_OGG:.o=.cpp)
	$(CXX) -c -o $@ $^ $(CXXFLAGS)
$(TESTOBJ_MIDI) : $(TESTOBJ_MIDI:.o=.cpp)
	$(CXX) -c -o $@ $^ $(CXXFLAGS)
$(TESTOBJ_MP3) : $(TESTOBJ_MP3:.o=.cpp)
	$(CXX) -c -o $@ $^ $(CXXFLAGS)
$(TESTOBJ_FLAC) : $(TESTOBJ_FLAC:.o=.cpp)
	$(CXX) -c -o $@ $^ $(CXXFLAGS)

clean:
	rm -f $(TESTOBJ_OGG) $(TESTOBJ_MIDI) $(TESTOBJ_MP3) $(TESTOBJ_FLAC) $(TESTOBJ_COMMON)

.PHONY: clean all what
