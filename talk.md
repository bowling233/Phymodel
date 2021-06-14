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

* 循环全部使用迭代器
* 全部改成智能指针

遗留问题：

* 返回根的问题（通过反向运行判断，但未检验实用性）
* 步长的问题，留到事件实现中讨论
* 测试小球和墙的碰撞（如果不行还要修复）

## 论文部分

### 1.水分子模型

* 水分子直径0.38nm
* 水的密度0.99987×103 kg/m3（标况）
* 水的摩尔质量18.01524 g·mol⁻¹

>     printf("%e",pow(18.01524e-3/0.99987e3/6.02214076e23,1.0/3.0));

* 立方体模型：水分子间距3.104430e-10即0.31nm
* 球体模型则有3.851669e-10即0.385nm

计算得出水分子间距小于水分子直径，或者很接近，计算难度高，需要斟酌如何处理

间距0.385nm的球体模型是可用的，使用的数据为标准状况。立方体模型求出值很离谱，需要问物理老师。

## 代码部分
