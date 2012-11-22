// $Id: board.cpp 40 2008-02-17 13:29:43Z fraser $
//

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#include <board.hpp>
#include <iostream>

static const char* XPathExpression = "/board/square";

Board::Board()
{
}

// Copy constructor
Board::Board(const Board * boardToCopy)
{
  // All we need to do is copy each of the squares in this board to our own
  for(int y=0; y<Common::NoNumbers; y++)
  {
    for(int x=0; x<Common::NoNumbers; x++)
    {
      _theSquares[x][y] = new Square(boardToCopy->_theSquares[x][y]);
    }
  }

  BuildTable();
}

Board::~Board()
{
  for(int x=0; x<Common::NoNumbers; x++)
  {
    for(int y=0; y<Common::NoNumbers; y++)
    {
      delete _theSquares[x][y];
    }
  }
}

bool Board::InitFromXMLFile(const char * fileName)
{
  bool toReturn = false;
  xmlDocPtr doc;
  xmlXPathContextPtr xpathCtx; 
  xmlXPathObjectPtr xpathObj; 
  int ret;

  // Init libxml
  xmlInitParser();
  LIBXML_TEST_VERSION

  assert(fileName);
  assert(XPathExpression);

  // Load XML document
  doc = xmlParseFile(fileName);

  if (doc == NULL)
  {
    cout << "Error: unable to parse file " << fileName << endl;
    return false;
  }

  // Create xpath evaluation context
  xpathCtx = xmlXPathNewContext(doc);
  if(xpathCtx == NULL)
  {
    cout << "Error: unable to create new XPath context" << endl;
    xmlFreeDoc(doc); 
    return false;
  }
    
  // Evaluate xpath expression
  xpathObj = xmlXPathEvalExpression(BAD_CAST XPathExpression, xpathCtx);
  if(xpathObj == NULL)
  {
    cout << "Error: unable to evaluate xpath expression " << XPathExpression << endl;
    xmlXPathFreeContext(xpathCtx); 
    xmlFreeDoc(doc); 
    return false;
  }

  // Process the nodes in the file
  toReturn = ProcessNodes(xpathObj->nodesetval, stdout);

  // Cleanup
  xmlXPathFreeObject(xpathObj);
  xmlXPathFreeContext(xpathCtx); 
  xmlFreeDoc(doc); 
    
  // Shutdown libxml
  xmlCleanupParser();
    
  // This is to debug memory for regression tests
  xmlMemoryDump();

  // Build the UI table with the squares we have
  BuildTable();

  return toReturn;
}

bool Board::ProcessNodes(xmlNodeSetPtr nodes, FILE* output)
{
  xmlNodePtr cur;
  int size;
  int i;
  int curX = 0;
  int curY = 0;
    
  assert(output);
  size = (nodes) ? nodes->nodeNr : 0;
    
  cout << "Found (" << size << " nodes)" << endl;

  if (size != Common::NoNumbers * Common::NoNumbers)
  {
    cout << "For a board size of " << Common::NoNumbers << ", expected " <<
      Common::NoNumbers * Common::NoNumbers << " entries in file." <<
      endl;
    return false;
  }

  for(i = 0; i < size; ++i)
  {
    assert(nodes->nodeTab[i]);

    if(nodes->nodeTab[i]->type == XML_NAMESPACE_DECL)
    {
      // We aren't really too interested in namespace declrations
    }
    else if(nodes->nodeTab[i]->type == XML_ELEMENT_NODE)
    {
      cur = nodes->nodeTab[i];
      xmlChar* nodeValue = xmlGetProp(cur, BAD_CAST "value");

      if(cur->ns)
      { 
        // We aren't really interested in namespaces here
      }
      else
      {
        if (xmlStrlen(nodeValue) == 0)
        {
          _theSquares[curX][curY] = new Square();
        }
        else
        {
          _theSquares[curX][curY] = new Square(atoi((char*)nodeValue));
        }
      }
    }
    else
    {
      cout << "Encountered an unknown node with name: " << cur->name <<
        " and type: " << cur->type << endl;
      cur = nodes->nodeTab[i];    
    }

    IncrementCoords(curX, curY);
  }

  // Note that the x coord should now be zero and y the number
  // of squares
  if (curX != 0 || curY != Common::NoNumbers)
  {
     cout << "Error, didn't populate all squares" << endl;
     return false;
  }

  // Everything seems fine
  return true;
}


void Board::IncrementCoords(int& x, int& y)
{
  if (x+1 < Common::NoNumbers)
  {
    x++;
  }
  else
  {
    y++;
    x=0;
  }
}

Square * Board::GetSquare(Coords coords)
{
  return _theSquares[coords.XCoord()][coords.YCoord()];
}

Square ** Board::GetRow(Coords coords)
{
  std::map<int, Square **>::iterator it;

  it = _rowMap.find(coords.YCoord());

  if(it == _rowMap.end())
  {
    // Initialise a new row map
    Square** val = new Square*[Common::NoNumbers];

    for (int thisX=0; thisX<Common::NoNumbers; thisX++)
    {
      val[thisX] = _theSquares[thisX][coords.YCoord()];
    }

    _rowMap[coords.YCoord()] = val;
  }

  return _rowMap[coords.YCoord()];
}

Square ** Board::GetCol(Coords coords)
{
  std::map<int, Square **>::iterator it;
  it = _colMap.find(coords.XCoord());

  if(it == _colMap.end())
  {
    // Initialise a new column map
    Square** val = _theSquares[coords.XCoord()];
    _colMap[coords.XCoord()] = val;
  }

  return _colMap[coords.XCoord()];
}

void Board::DisplayContents()
{
  for(int y=0; y<Common::NoNumbers; y++)
  {
    for(int x=0; x<Common::NoNumbers; x++)
    {
      cout << _theSquares[x][y]->ToString();
    }
    cout << endl;
  }
}

int Board::GetAreaSideSize()
{
  // Assume that the board is divided up into 3x3 areas
  return Common::NoNumbers / 3;
}

Square ** Board::GetArea(Coords coords)
{
  int areaId = GetAreaIdForCoords(coords);
  std::map<int, Square **>::const_iterator it;
  it = _areaMap.find(areaId);

  if(it == _areaMap.end())
  {
    // Initialise a new row map
    Square** val = new Square*[Common::NoNumbers];

    int sideSize = GetAreaSideSize();

    // Assumes that we always round fractions down when assigning to int
    int areaXmin = sideSize * (coords.XCoord() / sideSize);
    int areaXmax = areaXmin + sideSize;
    int areaYmin = sideSize * (coords.YCoord() / sideSize);
    int areaYmax = areaYmin + sideSize;
    int index = 0;

    for (int thisY=areaYmin; thisY<areaYmax; thisY++)
    {
      for (int thisX=areaXmin; thisX<areaXmax; thisX++)
      {
        val[index] = _theSquares[thisX][thisY];
        index++;
      }
    }

    _areaMap[areaId] = val;
  }

  return _areaMap[areaId];
}

int Board::IndexInAreaMap(Coords coords)
{
  // If we are using the boards area map, derive the index in the area
  // returned which signifies the square at the x/y coords. This assumes
  // that the squares in an area are populated by x coord, and then by
  // y.
  int areaSideSize = GetAreaSideSize();

  return coords.XCoord() % areaSideSize + (areaSideSize *
    (coords.YCoord() % areaSideSize));
}

// Make a mapping between some coordinates on the board an an integer
// representing the area it is part of. So for a 3x3 board this will
// look like:
//   000|111|222
//   000|111|222
//   000|111|222
//   -----------
//   333|444|555
//   333|444|555
//   333|444|555
//   -----------
//   666|777|888
//   666|777|888
//   666|777|888
//   
int Board::GetAreaIdForCoords(Coords coords)
{
  int areaSize = GetAreaSideSize();

  // Assumes we down fractions down when assigning to int
  int xComponent = coords.XCoord() / areaSize;
  int yComponent = coords.YCoord() / areaSize;

  return yComponent * areaSize + xComponent;
}

// Check that the current state of the board is valid
bool Board::BoardValid()
{
  int solvedValue;

  for (int y=0; y<Common::NoNumbers; y++)
  {
    for(int x=0; x<Common::NoNumbers; x++)
    {
      Coords coords(x, y);
      solvedValue = _theSquares[x][y]->SolvedValue();

      if(solvedValue && !IsSquareValid(coords, solvedValue))
      {
        return false;
      }
    }
  }

  return true;
}

bool Board::IsSquareValid(Coords coords, int solvedValue)
{
  // Check that the solved value of this square doesn't appear in any of
  // the rows, columns or areas it belongs to as a solved value too
  if (ContainsSolvedValue(GetRow(coords), solvedValue, coords.XCoord()) ||
    ContainsSolvedValue(GetCol(coords), solvedValue, coords.YCoord()) ||
    ContainsSolvedValue(GetArea(coords), solvedValue, IndexInAreaMap(coords)))
  {
    cout << "Found invalid repeated value of " << solvedValue << endl;
    return false;
  }

  return true;
}

bool Board::ContainsSolvedValue(Square ** coll, int val, int ignoreIndex)
{
  int solvedValue;

  for (int i=0; i<Common::NoNumbers; i++)
  {
    if (i!= ignoreIndex)
    {
      solvedValue = coll[i]->SolvedValue();

      if(solvedValue == val)
      {
        return true;
      }
    }
  }

  return false;
}

bool Board::IsSolved()
{
  for (int x=0; x<Common::NoNumbers; x++)
  { 
    for (int y=0; y<Common::NoNumbers; y++)
    {
      if (!_theSquares[x][y]->SolvedValue())
      {
        return false;
      }
    }
  }

  // All squares seem to be solved
  return true;
}

void Board::BuildTable()
{
  _Button = new Gtk::Button("Close");
  _Table = new Gtk::Table(Common::NoNumbers, Common::NoNumbers+1);

  // Sets the border width of the window.
  set_border_width(10);
  set_title("Sudoku");

  for (int x=0; x<Common::NoNumbers; x++)
  { 
    for (int y=0; y<Common::NoNumbers; y++)
    {
      _theSquares[x][y]->set_text(_theSquares[x][y]->ToString());
      _Table->attach(*_theSquares[x][y], x, x+1, y, y+1);
    }
  }
}

void Board::on_button_clicked()
{
  // do nothing
}
