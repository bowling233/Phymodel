# c++ primer



## Chapter7 类

### 7.5 构造函数再探

* 习惯上定义变量时立即初始化，这事关底层效率问题
* 构造函数初始值列表有时必不可少（特别是成员是const或引用时，或成员属于某种类类型且该类没有定义默认构造函数时，必须初始化）
* 初始化const或引用类型数据成员唯一机会就是构造函数初始值
* 最好避免使用成员初始化其他成员，使用成员初始化成员时注意成员初始化的顺序
* 

### 7.6 类的静态成员

* 有静态数据成员和静态函数成员

* 存在于任何对象之外，对象中没有静态成员

* 静态函数成员不包含this指针，不能声明成const的
* 用作用域运算符直接访问静态成员
* 可以使用类的对象、引用或指针访问静态成员
* 成员函数直接使用静态成员
* static关键字只出现在类内部的声明语句，外部定义时不要重复
* 必须在类外部定义和初始化每个静态成员
* 可以为静态成员提供const整数类型的类内初始值
* 有些普通成员无法使用的场景可以使用静态成员：静态数据成员可以是不完全类型，可以被用作默认实参

## Chapter 8 IO库

一些设施：

istream, ostream , cin , cout,cerr , >>, <<, getline

### 8.1 IO类

IO库类型和头文件

* iostream基类
  * istream, ostream, iostream
* fstream文件
  * i\~,o\~,f\~
* sstream字符串IO流
  * istringstream, o\~, 
* w前缀处理宽字符流

IO类型间的关系

#### 8.1.1 IO对象无拷贝和赋值

#### 8.1.2 条件状态

IO库条件状态

* 一个流一旦发生错误，其后所有IO操作全部失败。仅无错状态才可读写。
* 使用流前通常应该检查：最好将其当作一个条件来使用

查询流的状态

* iostate类型表达流状态
  * badbit：不可恢复错误，流无法再使用，此时fail也返回true
  * failbit：可以修正错误
  * 文件结尾eofbit和failbit都会置位，goodbit=0
  * good在均无出错情况下返回true，而bad、fail、eof在对应错误置位时返回true

管理流条件状态

#### 8.1.3 管理输出缓冲

* 导致缓冲刷新的原因：
  * 正常结束：return
  * 缓冲区满
  * 操作符如endl
  * 输出后使用unitbuf设置流内部状态清空缓冲区（cerr默认设置unitbuf，因此cerr内容都是立即刷新的）
  * 流可能关联到另一个流，读写被关联的流时，关联到的流的缓冲区会刷新（默认cin和cerr都关联到cout）

刷新输出缓冲区

* flush不输出
* ends插入空字符然后刷新

unitbuf操纵符

关联输入和输出流

### 8.2 文件输入输出

三个类型：ifstream,ofstream,fstream

操作与cin和cout一样

特有操作：

```
#include<fstream>
fstream fstrm;//未绑定文件流
fstream fstrms(s);//打开名为s的文件
fstream fstrm(s,mode);
fstrm.open(s);
fstrm.close();
fstrm.is_open();
```
#### 8.2.1 使用文件流对象

##### 用fstream代替iostream&

要求使用基类对象的地方可以用继承类型替代

##### 成员函数open和close

已打开文件流调用open失败，failbit置位。检测open是否成功是个好习惯

##### 自动构造和析构

当fstream销毁时，自动调用close

#### 8.2.2 文件模式

in读,out写,app每次写定位到尾,ate打开后定位到尾,trunc截断,binary

默认out截断。为了保留必须同时指定app

ate和binary可以和其他任意组合，用于任何文件

##### 以out模式打开文件会丢弃已有数据

ofstream::out|ofstream::app

##### 每次调用open时都会确定文件模式

### 8.3 string流

## Chapter 17 ?

### 17.5 IO库再探

#### 17.5.1 格式化输入与输出

##### 指定打印精度

precision成员，不接受参数返回精度值，接受参数设置精度

setprecision操纵符

##### 浮点数计数法

scidntific科学计数，fixed定点十进制（精度控制小数点后位数）

##### 输出补白

setw下一数字或字符串最小空间，left左对齐，right右对齐（默认），internal负数符号位置



