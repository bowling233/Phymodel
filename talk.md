# 讨论区

这里储存废弃的文字、代码段，储存待讨论的想法。

* 一些常用工具：
  * [Latex公式编辑器](https://www.codecogs.com/latex/eqneditor.php)
  * [Latex公式编辑器2](https://latex.91maths.com/)
  * [参考文献生成器](http://wenxian.aazz.cn/)
  * [物理化学实验数据参考(Handbook of Chemistry and Physics)](https://hbcp.chemnetbase.com/faces/contents/ContentsSearch.xhtml)
  * [Wikipedia镜像站](https://chi.jinzhao.wiki/)
  * [Wikipedia镜像站2](https://zhwiki.netlify.app/)
  * [Wikipedia英文镜像](https://en.jinzhao.wiki/wiki/Main_Page)
  * [物理学常数(NIST Physics Data)](https://www.nist.gov/pml/productsservices/physical-reference-data)

* 参考文献请使用`参考文献生成器`生成统一格式的参考文献附于文件结尾参考文献部分。参考文献请按在文中的引用顺序放置，`[]`部分留空，截稿时再填写序号。
* 当文献在文中引用时，请在引用文献处添加标记：`<ref>`，转换为word文档时重新编号
* 使用Typora可以更好地看清文章结构

## todo

索引优先队列设计方法：
1.首先遍历，每个球拥有自己最近的一次碰撞
2.接下来处理最近的一次碰撞。处理后进行预测：储存本小球最近一次新的碰撞，副小球同样
3.删无效事件，比如和刚刚那两个小球碰撞过的。此时它们也应当被重新预测，否则碰撞就无了

暑假计划：一周程序框架+算法；一周实验+完善

* 论文布局调整：模仿论文模板，调整研究方向：主要在算法。
  * 算法：重点研究：优先队列、八叉树两个算法，重在做实验、结果比较、阐明道理

[-] 重写：循环使用迭代器
[-] 重写：使用制表符

* 重写：智能指针
* 目前创建事件时没有限制时间，可能造成队列爆掉，增长数据时记得添加限制
* 重写：物体类型计数器
* 检验：事件队列逻辑
* 检验：小球预测正确性
* 测试：btw碰撞

遗留问题：

* 返回根（通过反向运行判断，但未检验实用性）
* 反向检查是否有用？
* 步长讨论

## 这几天遇到过的问题（7.8）

* 注意Object的基类虚函数！
   尝试从抽象基类调用纯虚函数。 纯虚函数没有基类实现。 若要解决此问题，请确保已实现所有调用的虚拟函数。LNK2001
* 文件流中的路径需要转义`\\`

### 名称修饰以及无参构造函数

这本来是一件很简单的事情：默认构造一个对象的时候不应该带上函数调用运算符

```C++
Wall wall1();//error
Wall wall2;//pass
```

Visual Studio编译器报错：[LNK2001](https://docs.microsoft.com/zh-cn/cpp/error-messages/tool-errors/linker-tools-error-lnk2001)

对比链接器信息发现涉及：使用修饰名查找错误，再次refer到[名称修饰](https://docs.microsoft.com/zh-cn/cpp/error-messages/tool-errors/name-decoration)查找相关信息：

> 具有不匹配的参数的函数原型也会导致此错误。 名称修饰将函数的参数合并到最终修饰的函数名中。

于是仔细检查构造函数。最后去掉输出测试行，发现生成时编译器警告：
> 未调用原型函数 (是有意用变量定义的吗？ )

搜索后才知道大部分是因为无参调用构造函数出错，才明白`名称修饰`中信息的意思：无参构造函数引发歧义，编译器识别为函数原型，并且引发了名称修饰的冲突

> MSDN解释：编译器检测到未使用的函数原型。 如果有意将该原型作为变量声明，则移除左/右括号。

## 论文部分

> 所有的算法都是先有API，然后实现，再是证明，最后是数据

### 数据汇总整理

* 默认标准状态： (273.15 K, 100 kPa)
* [理想气体摩尔体积$V_m$](https://physics.nist.gov/cgi-bin/cuu/Value?mvol)V_m=22.710 954 64...  x 10-3 m3 mol-1（molar volume of ideal gas）【来自】
* 氧气分子直径d=0.346nm（常用气体分子直径表）
* [阿伏伽德罗常数$N_A$](https://physics.nist.gov/cgi-bin/cuu/Value?na)N_A=6.022 140 76  x 1023 mol-1
* [玻尔兹曼常数$k_B$](https://physics.nist.gov/cgi-bin/cuu/Value?k)=k_B1.380 649  x 10-23 J K-1
* 氧气分子质量m=5.3 x 10^-26 kg

* 标准状态下气体分子间距$\sqrt[3]{\frac{V_m}{N_A}}$=3.353473e-09m

> printf("%e",pow(22.71095464e-3/6.02214076e23,1.0/3.0));

* 一微米长度上可排列气体分子数2.981983e+02取为298个

> printf("%e",1e-6/pow(22.71095464e-3/6.02214076e23,1.0/3.0));

* 方差

> printf("%e",sqrt(1.380649e-23*273.15/5.3e-26));

理想气体模型被广泛应用于研究气体性质。理想气体模型中，气体分子被理想化为质点或刚性小球，与其他分子无相互作用。理想气体分子在各个方向上的运动速率服从正态分布：

$$p(v_t)=\frac{1}{\sqrt{2\pi \sigma ^2}}\exp[-\frac{(v_t-\mu)^2}{2\sigma_t^2}]$$

## 代码部分
