// $Id: complexMatchesSolver.cpp 19 2007-05-02 21:31:02Z fraser $
//

#include<complexMatchesSolver.hpp>
#include<iostream>

using namespace std;

string ComplexMatchesSolver::SolverName()
{
  return "ComplexMatchesSolver";
}

const map<string, int> ComplexMatchesSolver::SolverStats()
{
  return _stats;
}

bool ComplexMatchesSolver::SolveSquare(Coords coords)
{
  bool madeProgress = false;
  // For a row/col/area
  //   check whether this one has the same potential solutions as another
  //     if so, exclude both solutions from all others in the area

  if (CheckForDupSolutions(_theBoard->GetArea(coords), coords,
    _theBoard->IndexInAreaMap(coords)))
  {
    _stats[AreaToken]++;
    madeProgress = true;
  }

  if (CheckForDupSolutions(_theBoard->GetCol(coords), coords, coords.YCoord()))
  {
    _stats[ColToken]++;
    madeProgress = true;
  }

  if (CheckForDupSolutions(_theBoard->GetRow(coords), coords, coords.XCoord()))
  {
    _stats[RowToken]++;
    madeProgress = true;
  }

  return madeProgress;
}

bool ComplexMatchesSolver::CheckForDupSolutions(Square ** squares,
  Coords theseCoords, int excludeIndex)
{
  Square * thisSquare = _theBoard->GetSquare(theseCoords);
  const vector<int> thisSquareValues = thisSquare->Values();
  bool toReturn = false;
  vector<int> matchedIndexes;

  // If we can match x duplicates in the collection where x is the number
  // of possibilities in the square we are trying to solve, then we can
  // remove each possibility in the square from all others which we didn't
  // match as duplicates
  for(int index=0; index<Common::NoNumbers; index++)
  {
    if(excludeIndex != index &&
      squares[index]->ValuesEqual(_theBoard->GetSquare(theseCoords)))
    {
      // This row in the collection matches completely the row we are trying
      // to solve. Record this index in the collection we are maintaining
      matchedIndexes.push_back(index);
    }
  }

  if (matchedIndexes.size() > 0 &&
    matchedIndexes.size() == thisSquareValues.size()-1)
  {
    // Ok, we have matched the right number of duplicates for the possibilities
    // left in this square. We can therefore rule out all the possibilities in
    // this square from other squares in the collection passed to us which are
    // not in the indexes we matched (assumming here that the collection is an
    // area, row or column
    for(int index=0; index<Common::NoNumbers; index++)
    {
      if (index != excludeIndex && !Common::VectorIncludes<int>
        (matchedIndexes, index))
      {
        for(vector<int>::const_iterator it=thisSquareValues.begin();
          it!=thisSquareValues.end(); it++)
        {
          if (squares[index]->CouldBe(*it))
          {
            squares[index]->RemovePossibility(*it);
            toReturn = true;
          }
        }
      }
    }
  }

  return toReturn;
}

