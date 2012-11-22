// $Id: coords.hpp 19 2007-05-02 21:31:02Z fraser $
//

#include<string>

class Coords
{
  private:
    int _x;
    int _y;
  public:
    bool operator== (Coords) const;
    Coords(int x, int y);
    const int XCoord() const;
    const int YCoord() const;
    std::string ToString();
};
