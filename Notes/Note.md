# Note:开发笔记

## 0.Visual Studio


### 程序文件编码

在visual studio中应当使用`Unicode (UTF-8 带签名) -代码页 65001`编码

## 1.Github

### 清理git历史记录减小项目体积

```bash
#克隆git仓库
git clone [URL] 
#进入git仓库
cd [仓库名] 

#创建一个名为 new_branch 新的空分支(不包含历史的分支)
git checkout --orphan  new_branch

#添加所有文件到new_branch分支，对new_branch分支做一次提交
git add -A
git commit -am '提交信息' 

#删除master分支
git branch -D master 
#将当前所在的new_branch分支重命名为master
git branch -m master
#将更改强制推送到github仓库
git push origin master --force
```

### [git远程覆盖本地](https://blog.csdn.net/sinat_36184075/article/details/80115000)

```bash
git fetch --all
//拉取所有更新，不同步；
git reset --hard origin/master
//本地代码同步线上最新版本(会覆盖本地所有与远程仓库上同名的文件)
git pull
//再更新一次（其实也可以不用，第二步命令做过了其实）
```

### 重新链接仓库

```shell
git init
git add .
git commit -m 'initial commit'
git remote add origin git@github.com:xx/aaaaa.git 
```

### 项目体积过大

解决：使用`.gitignore`文件

[.gitignore](https://github.com/github/gitignore/blob/master/VisualStudio.gitignore)

## 2.Coding(OpenGL)

### GLSL调试

要参考Log信息。解决习题3.1时GLSL矩阵代码写错，追踪Utils.c中的函数定义，找到出错位置，未定义标识符，说明是函数定义写错了。解决问题。

### GLSL只能使用一维数组

### OpenGL动画卡顿

改用独立显卡即可

### [链接器工具错误 LNK2019](https://docs.microsoft.com/zh-cn/cpp/error-messages/tool-errors/linker-tools-error-lnk2019?f1url=%3FappId%3DDev16IDEF1%26l%3DZH-CN%26k%3Dk(LNK2019)%26rd%3Dtrue&view=msvc-160)

没有包括书本cpp文件

### init卡住无输出

可能是着色器文件名输错了

## 3.Coding(C++)

### 结构体输入输出

编写球的结构，文件输入输出，处理复杂方程的问题时犯了很多小错误

1. 读数组没从零开始。导致产生数据溢出错误，自己改数据输出一样，仍没有发现是读取了同一内存导致。
2. 浮点数精度问题。在GDB调试过程中发现小数点后几位出现了浮动，是float的毛病，在处理大量数据的时候要注意。

### 测试文件输入编码

有一次重置所有项目文件编码为UTF8withBOM后，从文件流读取数据报错。自习检查文件流状态后发现可以打开，读入错误。查询后发现是文件格式原因

普通fstream无法读取UTF8withBOM编码，应当使用宽字符的文件流







## 4.配置过程中

### 1.按教程更改为x64位编码

[Book update](https://athena.ecs.csus.edu/~gordonvs/C1Elibraries.html)

### 2.编译书本示范程序时发生错误

[github issue](https://github.com/bincrafters/community/issues/126)

解决方案：my suggestion is to use /Z7 option instead of /Zi when building glfw as static library - this way debugging information is embedded into library and pdb files are not needed

[Stackoverflow answer](https://stackoverflow.com/questions/284778/what-are-the-implications-of-using-zi-vs-z7-for-visual-studio-c-projects)

[Microsoft Docs on Z7](https://docs.microsoft.com/en-us/cpp/build/reference/z7-zi-zi-debug-information-format?view=msvc-160)

### 3.从书本2.5程序开始输出全部红色

解决：书中代码部分漏掉了填充缓冲区的代码，需要补上：

```C
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
```



### 5.在教室电脑编译运行返回-1

原因：硬件OpenGL版本过老

解决：更改`glWindowsint()`使用更老版本。经测试`4.0.0`版本后的可以兼容

### 6.Visual Studio更改include路径出现错误

[Zhihu](https://zhuanlan.zhihu.com/p/73638902)

尝试过的解决方案：
* 使用`\ZW`，报错platform.wind

Visual Studio中不应该使用相对路径，无法找到库文件和头文件


### 8.编译出现.obj已定义错误

尚未查明原因，猜测是：更换了cpp文件而vs没有识别，于是复用了之前的.obj文件。进入Debug文件夹删除.obj后正常编译

