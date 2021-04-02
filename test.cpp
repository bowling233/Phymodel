#include <vector>
#include <iostream>
using namespace std;
int main()
{
    vector<int> nums;
    for (int i = 0; i != 10; i++)
        nums.push_back(i);
    int &p = nums[2];
    cout << p << endl;
    nums[2] = 9;
    cout << p << endl;
    cout << nums[2] << endl;
    return 0;
}