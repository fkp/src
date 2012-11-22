# $Id: Makefile.am 40 2008-02-17 13:29:43Z fraser $
#

bin_PROGRAMS = sudoku

AM_CXXFLAGS = $(SUDOKU_GTMM_CFLAGS) $(SUDOKU_LIBXML_CFLAGS)

sudoku_SOURCES = common.hpp \
  coord.hpp coords.cpp \
  square.hpp square.cpp \
  solveProfile.hpp solveProfile.cpp \
  board.hpp board.cpp \
  boardBuilder.hpp boardBuilder.cpp \
  solverManager.hpp solverManager.cpp \
  puzzleGenerator.hpp puzzleGenerator.cpp \
  solver.hpp solver.cpp \
  inCollectionSolver.hpp inCollectionSolver.cpp \
  uniqueInCollectionSolver.hpp uniqueInCollectionSolver.cpp \
  complexMatchesSolver.hpp complexMatchesSolver.cpp \
  main.cpp 
sudoku_LDADD = $(SUDOKU_GTMM_LIBS) $(SUDOKU_LIBXML_LIBS)
