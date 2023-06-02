# Makefile for STWCalc
# To build and run the application GTK+ v4 developer package is required to be installed:
# debian package: apt-get install libgtk-4-dev

# =========================== Main variables ==================================

CC=gcc
CPP=g++
CFLAGS= -g -Wall -Wextra -std=c11 -O2
CPPFLAGS = -g -Wall -Wextra -std=c++14 -Igoogletest-main/googletest/include/gtest
TEST_LDFLAGS = -Lgoogletest-main/build/lib -lgtest -lgtest_main
GTK_FLAGS = $(shell pkg-config --cflags gtk4) # gcc flags for gtk
GTK_LIBS = $(shell pkg-config --libs gtk4) # include libraries for gtk


# =========================== Main commands ===================================
.PHONY=all clean mathlib_tests engine_tests doc run

# builds the app
all: stwcalc

# cleans all binaries and generated documentation
clean:
	rm -f *.o *.so *.out stwcalc
	rm -f -r docs

# builds and runs tests for math library
mathlib_tests: mathlib_tests.out
	./mathlib_tests.out

# builds and runs tests for calculator's engine
engine_tests: engine_tests.out
	./engine_tests.out

# generates Doxygen documentation
doc:
	doxygen Doxyfile

# builds and runs the application
run: stwcalc
	export LD_LIBRARY_PATH=.:${LD_LIBRARY_PATH};./stwcalc


# =========================== Binary files ====================================
stwcalc: stwcalc.o engine.o libmath_library.so
	$(CC) stwcalc.o engine.o -o $@ -L. -lmath_library $(GTK_LIBS)

stwcalc.o: app.c engine.h
	$(CC) $(GTK_FLAGS) -DGDK_VERSION_MIN_REQUIRED=GDK_VERSION_4_2 -c $< -o $@

engine_io: engine_io.o engine.o math_library.o
	${CC} ${CFLAGS} $^ -o $@

engine_io.o: engine_io.c engine.h
	${CC} ${CFLAGS} -c $<

engine.o: engine.c engine.h math_library.h
	${CC} ${CFLAGS} -c $<

libmath_library.so: math_library.o
	$(CC) -shared -o $@ $^

math_library.o: math_library.c math_library.h 
	$(CC) $(CFLAGS) -fPIC -c $<

mathlib_tests.out: math_library.o mathlib_tests.o
	$(CPP) $(CPPFLAGS) -o $@ $^ $(TEST_LDFLAGS)

mathlib_tests.o: mathlib_tests.cpp math_library.h
	$(CPP) $(CPPFLAGS) -c $<

engine_tests.out: engine.o engine_tests.o math_library.o
	$(CPP) $(CPPFLAGS) -o $@ $^ $(TEST_LDFLAGS)

engine_tests.o: engine_tests.cpp engine.h
	$(CPP) $(CPPFLAGS) -c $<