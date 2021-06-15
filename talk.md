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

已经完成球与球之间的碰撞

等待做的：

* 重写：循环使用迭代器
* 重写：智能指针
* 重写：物体类型计数器
* 检验：事件队列逻辑
* 检验：小球预测正确性
* 测试：btw碰撞

遗留问题：

* 返回根（通过反向运行判断，但未检验实用性）
* 步长讨论

## 论文部分

### 1.水分子模型

* 水分子直径0.38nm
* 水的密度0.99987×103 kg/m3（标况）
* 水的摩尔质量18.01524 g·mol⁻¹

>     printf("%e",pow(18.01524e-3/0.99987e3/6.02214076e23,1.0/3.0));

* 立方体模型：水分子间距3.104430e-10即0.31nm
* 球体模型则有3.851669e-10即0.385nm

计算得出水分子间距小于水分子直径，或者很接近，计算难度高，需要斟酌如何处理

*间距0.385nm的球体模型是可用的*，使用的数据为标准状况。立方体模型求出值很离谱，需要问物理老师。

数据汇总整理：

* 默认标准状态： (273.15 K, 100 kPa)
* [理想气体摩尔体积$V_m$](https://physics.nist.gov/cgi-bin/cuu/Value?mvol)=22.710 954 64...  x 10-3 m3 mol-1（molar volume of ideal gas）【来自】
* 氧气分子直径d=0.346nm（常用气体分子直径表）
* [阿伏伽德罗常数$N_A$](https://physics.nist.gov/cgi-bin/cuu/Value?na)=6.022 140 76  x 1023 mol-1
* [玻尔兹曼常数$k_B$](https://physics.nist.gov/cgi-bin/cuu/Value?k)=1.380 649  x 10-23 J K-1
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
