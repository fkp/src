// $Id: solver.cpp 19 2007-05-02 21:31:02Z fraser $
//

#include<iostream>
#include<solver.hpp>

Solver::Solver(Board * boardToUse)
{
  _theBoard = boardToUse;

  RowToken = "Row";
  ColToken = "Col";
  AreaToken = "Area";

  _stats[AreaToken] = 0;
  _stats[RowToken] = 0;
  _stats[ColToken] = 0;
}

const map<string, int> & Solver::SolverStats()
{
  return _stats;
}

