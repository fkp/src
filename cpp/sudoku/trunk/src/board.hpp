// $Id: board.hpp 41 2008-03-10 20:10:20Z fraser $
//

#include <libxml/xpathInternals.h>
#include <map>
#include <gtkmm.h>

#if !defined(SQUARE_HEADER)
  #define COMMON_HEADER
  #include <common.hpp>
#endif

#if !defined(SQUARE_HEADER)
  #define SQUARE_HEADER
  #include <square.hpp>
#endif

#if !defined(COORDS_HEADER)
  #define COORDS_HEADER
  #include <coords.hpp>
#endif

class Board : public Gtk::Window
{
  protected:
    Square * _theSquares[Common::NoNumbers][Common::NoNumbers];

    // Signal handlers
    virtual void on_button_clicked();

    // Member widgets
    Gtk::Button * _Button;
    Gtk::Table * _Table;
    void BuildTable();

  private:
    std::map<int, Square **> _rowMap;
    std::map<int, Square **> _colMap;
    std::map<int, Square **> _areaMap;
    bool ProcessNodes(xmlNodeSetPtr nodes, FILE* output);
    void IncrementCoords(int& x, int& y);
    int GetAreaIdForCoords(Coords coords);
    int GetAreaSideSize();
    bool IsSquareValid(Coords coords, int solvedValue);
    bool ContainsSolvedValue(Square ** coll, int val, int ignoreIndex);

  public:
    Board();
    Board(const Board * boardToCopy);
    ~Board();
    bool InitFromXMLFile(const char* filename);
    void DisplayContents();
    Square * GetSquare(Coords coords);
    Square ** GetRow(Coords coords);
    Square ** GetCol(Coords coords);
    Square ** GetArea(Coords coords);
    int IndexInAreaMap(Coords coords);
    bool BoardValid();
    bool IsSolved();
};

