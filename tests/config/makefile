SCC_ROOT_DIR := ../..

# uncomment according to which headers have been included in main.cpp
# NOTE: the compiler might complain about those being redefined,
# but they're necessary to work with makefile.tests,
# otherwise you'd get a linking error.
SCC_HAVE_FLAGS := \
                     #-DHAVE_SDL_IMAGE \
                     #-DHAVE_SDL_TTF \
                     #-DHAVE_SDL_MIXER
TESTOBJ := main.o
BIN := configTest

include $(SCC_ROOT_DIR)/tests/makefile.tests
