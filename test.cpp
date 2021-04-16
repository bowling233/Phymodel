#include <vector>
#include <iostream>
using namespace std;
class Subunion
{
public:
    Subunion() = default;
    Subunion(int i, float j) : subnum(i), subflo(j) {}
    Subunion(float j) : Subunion(1, j) {}
    int subnum = 0;
    float subflo = 0.0f;
};
class originclass
{
protected:
    int origin = 0;
};
class subclass1 : public originclass
{
public:
    subclass() = default;

protected:
    int sub1 = 1;
};
class subclass2 : public originclass
{
public:
    subclass2() = default;

protected:
    int sub2 = 2;
};
class Test : public subclass1, public subclass2
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
    int sub01() { return sub1; }
    int sub02() { return sub2; }
    int ori() { return origin; }
};
int main()
{
    Test test1(1);
    Test test2(1.0f);
    cout << test1.sub01() << endl;
    cout << test2.sub02() << endl;
    return 0;
}