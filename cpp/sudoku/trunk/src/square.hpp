// $Id: square.hpp 40 2008-02-17 13:29:43Z fraser $
//

#include <vector>
#include <algorithm>
#include <gtkmm.h>

using namespace std;

class Square : public Gtk::Label
{
  private:
    vector<int> _possibilities;
  public:
    Square();
    Square(const Square * squareToCopy);
    Square(int resolved);
    string ToString() const;
    void SolveAs(int value);
    void RemovePossibility(int value);
    const vector<int> Values() const;
    int SolvedValue() const;
    bool CouldBe(int value) const;
    bool ValuesEqual(const Square * other) const;
};
