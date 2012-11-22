// $Id: coords.cpp 19 2007-05-02 21:31:02Z fraser $
//

#include<coords.hpp>
#include<iostream>

Coords::Coords(int x, int y)
{
  _x = x;
  _y = y;
}

const int Coords::XCoord() const
{
  return _x;
}

const int Coords::YCoord() const
{
  return _y;
}

std::string Coords::ToString()
{
  std::string toReturn = "(";
  char x[2];
  char y[2];

  sprintf(x, "%i", _x);
  sprintf(y, "%i", _y);

  toReturn.append(x);
  toReturn.append(",");
  toReturn.append(y);
  toReturn.append(")");

  return toReturn;
}

bool Coords::operator== (Coords other) const
{
  return (other.XCoord() == XCoord() && other.YCoord() == YCoord());
}
