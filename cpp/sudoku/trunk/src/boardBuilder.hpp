// $Id: boardBuilder.hpp 22 2007-05-06 21:12:01Z fraser $
//

#if !defined(BOARD_HEADER)
  #define BOARD_HEADER
  #include <board.hpp>
#endif

#if !defined(COORDS_HEADER)
  #define COORDS_HEADER
  #include <coords.hpp>
#endif

class BoardBuilder : public Board
{
  private:
    vector<Coords> unsolvedCoords;

  public:
    BoardBuilder() : Board() {};
    bool Init();
    bool SolveRandomLocation();
};

