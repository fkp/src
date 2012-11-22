// $Id: solveProfile.hpp 20 2007-05-05 17:38:20Z fraser $
//

#include <algorithm>
#include <vector>
#include <map>

class SolveProfileTolerance
{
  private:
    int _minActivity;
    int _maxActivity;

  public:
    SolveProfileTolerance(int min, int max);
    const int Min() const;
    const int Max() const;
};

class SolveProfile
{
  private:
    std::string _solverName;
    std::map<std::string, SolveProfileTolerance> _items;

  public:
    SolveProfile(std::string solver, std::map<std::string,
      SolveProfileTolerance> items);
    bool WithinTolerance(std::string solver, std::map<std::string, int> stats);
};

