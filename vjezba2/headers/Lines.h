#include <utility>
#include <vector>

class Lines
{
private:
    void bresenhamFrom90(std::vector<std::pair<int, int>> &pravac, int, int, int, int);
    void bresenhamTo90(std::vector<std::pair<int, int>> &pravac, int, int, int, int);
public:
    void drawLine(std::vector<std::pair<int, int>> &pravac, int, int, int, int);
};