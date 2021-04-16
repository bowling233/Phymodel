#include <iostream>
#include <array>
using namespace std;
class Test
{
    public:
        Test(int i, int j) : nums{i, j} { cout << "created" << endl; }
        array<int, 2> nums;
};
int main()
{
    Test test1(1,2);

    cout << test1.nums[0] << endl;
    return 0;
}