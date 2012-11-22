// $Id: main.cpp 41 2008-03-10 20:10:20Z fraser $
//

#include <gtkmm/main.h>
#include <iostream>

#if !defined(SOLVERMANAGER_HEADER)
  #define SOLVERMANAGER_HEADER
  #include <solverManager.hpp>
#endif

#if !defined(PUZZLEGENERATOR_HEADER)
  #define PUZZLEGENERATOR_HEADER
  #include <puzzleGenerator.hpp>
#endif

#if !defined(BOARDBUILDER_HEADER)
  #define BOARDBUILDER_HEADER
  #include <boardBuilder.hpp>
#endif

int DisplayUsage(char * argv[]);
int SolveFromFile(const char* fileName);
int GeneratePuzzle();

int main(int argc, char * argv[])
{
  // We need to initialise the Gtk stuff here in order to use the widgets
  Gtk::Main kit(argc, argv);

  Board * myBoard = new Board();
  int result;

  if (argc <2)
  {
    result = DisplayUsage(argv);
  }
  else if (argc <3)
  {
    if (strcmp(argv[1],"generate")==0)
    {
      result = GeneratePuzzle();
    }
    else if (strcmp(argv[1], "solve")==0)
    {
      // Can't have a solve without a filename
      result = DisplayUsage(argv);
    }
    else
    {
      // Assume that we want to solve from a filename
      result = SolveFromFile(argv[1]);
    }
  }
  else if (argc <4)
  {
    if (strcmp(argv[1], "solve")!=0)
    {
      // This combination doesn't make sense
      result = DisplayUsage(argv);
    }
    else
    {
      result = SolveFromFile(argv[2]);
    }
  }
  else
  {
    result = DisplayUsage(argv);
  }

  // Show the main window
  Gtk::Main::run(*myBoard);

  return result;
}

int DisplayUsage(char * argv[])
{
  cout << argv[0] << ": [COMMAND] FILENAME" << endl;
  return -1;
}

int SolveFromFile(const char* fileName)
{
  Board * myBoard = new Board();

  cout << "Filename: " << fileName << endl;

  if (myBoard->InitFromXMLFile(fileName))
  {
    myBoard->DisplayContents();

    SolverManager sManager(myBoard);
    if (sManager.Solve())
    {
      cout << "Solved!" << endl;
    }
    else
    {
      cout << "Not solved.." << endl;
    }

    myBoard->DisplayContents();
    delete myBoard;
    return 0;
  }
  else
  {
    cout << "Could not initialise the board" << endl;
    return -1;
  }
}

int GeneratePuzzle()
{
  BoardBuilder * myBoard = new BoardBuilder();
  myBoard->Init();

  PuzzleGenerator generator(myBoard);
  generator.Generate();

  if(!myBoard->BoardValid())
  {
    cout << "Board invalid.." << endl;
  }

  delete myBoard;
  return 0;
}
