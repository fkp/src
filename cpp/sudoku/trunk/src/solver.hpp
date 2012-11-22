// $Id: solver.hpp 19 2007-05-02 21:31:02Z fraser $
//

#if !defined(BOARD_HEADER)
  #define BOARD_HEADER
  #include <board.hpp>
#endif

#if !defined(COORDS_HEADER)
  #define COORDS_HEADER
  #include <coords.hpp>
#endif

class Solver
{
  protected:
    Board * _theBoard;
    map<string, int> _stats;

  public:
    Solver(Board * boardToUse);
    const map<string, int> & SolverStats();

    std::string RowToken;
    std::string ColToken;
    std::string AreaToken;

    // Abstract methods for inherited classes to implement
    virtual bool SolveSquare(Coords coords) =0;
    virtual string SolverName() =0;
};
