// $Id: solverManager.hpp 24 2007-05-07 12:38:20Z fraser $
//

#if !defined(INCOLLECTIONSOLVER_HEADER)
  #define INCOLLECTIONSOLVER_HEADER
  #include <inCollectionSolver.hpp>
#endif

#if !defined(UNIQUEINCOLLECTIONSOLVER_HEADER)
  #define UNIQUEINCOLLECTIONSOLVER_HEADER
  #include <uniqueInCollectionSolver.hpp>
#endif

#if !defined(COMPLEXMATCHESSOLVER_HEADER)
  #define COMPLEXMATCHESSOLVER_HEADER
  #include <complexMatchesSolver.hpp>
#endif

#if !defined(COORDS_HEADER)
  #define COORDS_HEADER
  #include <coords.hpp>
#endif

class SolverManager
{
  public:
    SolverManager() {};
    SolverManager(Board * boardToUse);
    ~SolverManager();
    bool Solve();

  private:
    static const int NoSolvers = 3;
    Board * _theBoard;
    Solver * _solver[NoSolvers];
    bool WorkOn(Coords coords);
};
