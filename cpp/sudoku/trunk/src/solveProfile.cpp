// $Id: solveProfile.cpp 20 2007-05-05 17:38:20Z fraser $
//

#if !defined(COORDS_HEADER)
  #define COORDS_HEADER
  #include <coords.hpp>
#endif

#include<solveProfile.hpp>

class CannotCompareStatsException: public std::exception
{
  virtual const char* what() const throw()
  {
    return "Cannot compare stats as the components don't match";
  }
} CannotCompareStats;


SolveProfileTolerance::SolveProfileTolerance(int min, int max)
{
  _minActivity = min;
  _maxActivity = max;
}

const int SolveProfileTolerance::Min() const
{
  return (const int) _minActivity;
}

const int SolveProfileTolerance::Max() const
{
  return (const int) _maxActivity;
}



SolveProfile::SolveProfile(std::string solver, std::map<std::string,
  SolveProfileTolerance> items)
{
  _solverName = solver;
  _items = items;
}

bool SolveProfile::WithinTolerance(std::string solver,
  std::map<std::string, int> stats)
{
  if (solver == _solverName)
  {
    // Check each item in out profile is in the stats
    for(std::map<std::string, SolveProfileTolerance>::const_iterator thisIt =
      _items.begin(); thisIt!=_items.end(); thisIt++)
    {
      // Try to find this stat in those passed to us
      std::map<std::string, int>::const_iterator it = stats.find(thisIt->first);

      if(it==stats.end())
      {
        // The stats for this solver are different, this shouldn't happen
        throw CannotCompareStats;
      }
      else
      {
        int statsValue = it->second;

        if (statsValue < thisIt->second.Min() ||
            statsValue > thisIt->second.Max())
        {
          // Its not within this tolerance
          return false;
        }
      }
    }
  }
  else
  {
    return false;
  }

  return true;
}

