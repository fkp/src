// $Id: square.cpp 22 2007-05-06 21:12:01Z fraser $
//

#include<square.hpp>
#include<common.hpp>
#include<string>
#include<iostream>
using namespace std;

class CouldNotSolveException: public exception
{
  virtual const char* what() const throw()
  {
    return "Could not find value as a possibilty in this square";
  }
} CouldNotSolve;

Square::Square()
{
  // Add all the possibilities to the list
  for (int i=1; i<= Common::NoNumbers; i++)
  {
    _possibilities.push_back(i);
  }
}

// Copy constructor
Square::Square(const Square * squareToCopy)
{
  // Copy all of the possibilities into our class
  _possibilities.assign(squareToCopy->_possibilities.begin(),
    squareToCopy->_possibilities.end());
}

Square::Square(int resolved)
{
  _possibilities.push_back(resolved);
}

void Square::SolveAs(int solveValue)
{
  if (SolvedValue())
  {
    throw CouldNotSolve;
  }

  if (!CouldBe(solveValue))
  {
    throw CouldNotSolve;
  }
  else
  {
    _possibilities.clear();
    _possibilities.push_back(solveValue);
  }
}

void Square::RemovePossibility(int possibility)
{
  if (SolvedValue())
  {
    throw CouldNotSolve;
  }

  vector<int>::iterator it = find(_possibilities.begin(), _possibilities.end(),
    possibility);

  if (it == _possibilities.end())
  {
    throw CouldNotSolve;
  }
  else
  {
    _possibilities.erase(it);
  }
}

const vector<int> Square::Values() const
{
  return (const vector<int>)_possibilities;
}

string Square::ToString() const
{
  string toReturn = "[";
  char temp[2];

  for (vector<int>::const_iterator it = _possibilities.begin();
  it!=_possibilities.end(); ++it)
  {
    if (toReturn.length() > 1)
    {
      toReturn.append(",");
    }

    sprintf(temp, "%i", *it);
    toReturn.append(temp);
  }
  toReturn.append("]");

  return toReturn;
}

int Square::SolvedValue() const
{
  if (_possibilities.size() == 1)
  {
    return _possibilities[0];
  }
  else
  {
    return 0;
  }
}

bool Square::CouldBe(int value) const
{
  return Common::VectorIncludes<int> (_possibilities, value);
}

bool Square::ValuesEqual(const Square * other) const
{
  const vector<int> otherVals = other->Values();
  const vector<int> theseVals = (const vector<int>) _possibilities;

  for(vector<int>::const_iterator it=otherVals.begin(); it!=otherVals.end();
    it ++)
  {
    if (!CouldBe(*it))
    {
      // This square has a value which we don't
      return false;
    }
  }

  for(vector<int>::const_iterator it=theseVals.begin(); it!=theseVals.end();
    it++)
  {
    if (!other->CouldBe(*it))
    {
      // We have a value this square doesn't
      return false;
    }
  }

  // Both squares seem to have the same values
  return true;
}
