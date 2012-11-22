// $Id: common.hpp 19 2007-05-02 21:31:02Z fraser $
//

#include<vector>
#include<string>
using namespace std;

class Common
{
  public:
    static const int NoNumbers = 9;

    template <typename T> static bool VectorIncludes(vector<T> vec, T val)
    {
      typename  vector<T>::iterator it = find(vec.begin(), vec.end(), val);
      return it != vec.end();
    }
};

