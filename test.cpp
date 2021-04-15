#include <vector>
#include <iostream>
using namespace std;
class Subunion
{
public:
    Subunion() = default;
    Subunion(int i,float j):subnum(i),subflo(j){}
    Subunion(float j):Subunion(1,j){}
    int subnum = 0;
    float subflo = 0.0f;
};
class Test
{
public:
    Test() = default;
    Test(const int i) : num(i) {}
    Test(const float j) : num2(j) {}
    union
    {
        int num;
        Subunion num2;
    };
};
int main()
{
    Test test1(1);
    Test test2(1.0f);
    cout << test1.num2.subflo << endl;
    cout << test2.num2.subflo << endl;
    return 0;
}