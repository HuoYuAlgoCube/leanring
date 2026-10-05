# _learning_ — C 语言机器学习 / 自然语言处理学习项目

这是一个用 **C 语言**从零手写实现的**机器学习与自然语言处理学习范例集合**。
项目不依赖任何机器学习框架，所有算法（卷积、Q-Learning、N-Gram 等）均以最朴素的
C 代码手工实现，目的是通过"重新造轮子"来理解算法的底层原理。

代码以 Sublime Text 为编辑器，每个子项目配有独立的 `.sublime-project` / `.sublime-workspace`
工程文件，并带有中文的目录分组（源代码 / 资源 / 图形库 / 矩阵库等）。

> 说明：本 README 为**介绍性质**，只描述项目"做了什么、怎么组织、如何运行"，
> 不对代码的正确性、健壮性或完备性做任何保证。部分文件是尚未完成的草稿。

---

## 一、目录结构

```
_learning_/
├── learning.sublime-project      # 顶层 Sublime 工程（同时挂载两个子项目）
├── learning.sublime-workspace    # Sublime 工作区状态
│
├── machine-learning-demo/        # 机器学习学习项目
│   ├── machine-learning-demo.sublime-project
│   ├── machine-learning-demo.sublime-workspace
│   ├── current.exe               # 编译产物
│   ├── resp.txt                  # 编译命令 / 链接参数备注
│   ├── res/                      # 资源目录
│   │   ├── lfh.png               # 卷积示例使用的图片
│   │   ├── q-learning-data.txt   # Q-Learning 状态/子节点数据（30 行）
│   │   └── q-learning-data2.txt  # Q-Learning 数据（500 行）
│   └── src/                      # 源代码
│       ├── matrix.c              # 矩阵库雏形
│       ├── convolve.c            # 图像卷积（基于 TIGR）
│       ├── q-learning-demo.c     # Q-Learning 走迷宫示例
│       ├── search.c              # Q-Learning 网格寻路示例
│       ├── demo2-2.c             # 权重评分示例
│       └── demo2-3.c             # 空模板
│
└── natural-language-processing/  # 自然语言处理学习项目
    ├── natural-language-processing.sublime-project
    ├── natural-language-processing.sublime-workspace
    ├── current.exe               # 编译产物
    └── src/
        └── ngram.c               # N-Gram 切片示例
```

---

## 二、子项目详解

### 1. matrix.c —— 矩阵库雏形

一个最小化的浮点矩阵封装，用 `struct` 把指针、行数、列数打包在一起：

```c
typedef struct {
    float* p;   // 一维数组存储，行优先
    int r;      // 行数
    int c;      // 列数
} Matrix;
```

提供的能力：

| 函数 | 作用 |
| --- | --- |
| `matAlloc(r, c)` | 申请 `r × c` 矩阵 |
| `matFree(m)` | 释放内存 |
| `matAssert(m)` | 校验矩阵是否合法 |
| `matInit(m, lower, upper)` | 用 `[lower, upper]` 区间内的随机数初始化 |
| `matPrint(m)` | 以 `| ... |` 形式打印矩阵 |
| `rnd(lower, upper)` | 区间随机数（基于 `rand()`） |

`main` 中创建一个 2×2 矩阵、随机初始化并打印，是整个项目的"张量"基础设施起点。

---

### 2. convolve.c —— 图像卷积

使用 **TIGR** 图形库做图像窗口显示，并在 CPU 上手工实现 2D 卷积：

```c
void convolve(Tigr* dest, Tigr* src, float kernel[], int dx[], int dy[], int size);
```

- 遍历源图像每个像素，对 `size × size` 的邻域做加权求和；
- 边界采用**钳制（clamp）**处理：越界的坐标被压回图像边缘；
- 结果按通道（R/G/B）累加后裁剪到 `[0, 255]`；
- `main` 中使用一个 3×3 锐化核：

  ```
   0  1  0
  -1  5 -1
   0  1  0
  ```

  并**连续卷积 20 次**后显示在 800×800 的窗口中。

这是理解 CNN 卷积层原理的最直观示例。依赖项在 `res/resp.txt` 中注明：

```
-Wall -O2
"src\convolve.c"
"D:\libs\Cpp\tigr\tigr\tigr.c"
-I"D:\libs\Cpp\tigr\tigr"
-lopengl32 -lgdi32
```

即需要本地 TIGR 库（默认路径 `D:\libs\Cpp\tigr`）以及 Windows 的 OpenGL / GDI 链接库。

---

### 3. q-learning-demo.c —— Q-Learning 网格世界

一个 **表格型 Q-Learning** 演示。把 10×10 网格编码为状态 `0..99`：

```
state = x * 10 + y
```

关键定义：

| 宏 | 值 | 含义 |
| --- | --- | --- |
| `Q_SIZE` | 100 | 状态数 |
| `SUB_SIZE` | 4 | 每个状态的邻居数 |
| `EPSILON` | 0.3 | 随机探索概率 |
| `ALPHA` | 0.1 | 学习率 |
| `GAMMA` | 0.9 | 折扣因子 |
| `REWARD` | 1000 | 到达/前进奖励 |
| `COUNT` | 1000 | 训练回合数 |

流程：

1. `initQValues` —— 随机初始化 Q 表；
2. `readData` —— 从标准输入读取每个状态的 `action` 与 4 个 `subNodes`（邻居）；
3. 训练 `COUNT` 轮，每轮从状态 0 出发，边 `reward()` 更新 Q 值边 `switchState()` 转移，
   直到状态 99（终点）；
4. 训练结束后用**贪心策略**（`epsilon = 0`）回放路径并打印。

Q 值更新遵循经典公式：

```
Q(s,a) ← Q(s,a) + α [ r + γ·maxQ(s') − Q(s,a) ]
```

其中前进方向的邻居（坐标变大）给予 `REWARD`，其余按折扣递推。
邻居数据由 `res/q-learning-data*.txt` 提供（分别对应 30 行与 500 行规模的数据集）。

---

### 4. search.c —— Q-Learning 网格寻路

与上一个示例思想相同，但改用 **`ROW × COL = 10 × 10` 的二维浮点 Q 表**，
并叠加了**曼哈顿距离启发式**，目标是从 `(0,0)` 寻路到 `(9,9)`：

- `select()` —— ε-贪心动作选择，`flag = 1` 时允许随机探索，
  `flag = 0` 时纯贪心；每次只考虑上下左右 4 个邻格；
- `reward()` —— 对四个邻格做 Q 值更新，若邻格到目标的曼哈顿距离更小则给 `REWARD`（100），
  否则奖励为 0；
- `maxQValue()` —— 取邻域中的最大 Q 值作为 bootstrap 目标。

参数：`EPSILON = 0.2`、`ALPHA = 0.2`、`GAMMA = 0.8`、训练 20 轮。
训练完成后打印从起点到终点的完整路径坐标。

---

### 5. demo2-2.c —— 权重评分玩具

一个用于理解 **"权重匹配打分"** 的小例子：

- 用户按 `batch_size = 3`、`data_size = 3` 输入样本特征与预测标签 `pred`；
- 随机生成权重向量 `weight`（取值 0/1/2）；
- `getScore()` 对每个样本逐维比对：
  - 权重为 `2` 视为"通配"，直接得分；
  - 否则要求 `weight[j] == inp[i][j]`；
  - 全部命中且 `pred == 1`，或未全部命中且 `pred == 0`，则整体计 1 分。

它是分类/感知机的极度简化雏形，用来观察"参数如何决定得分"。

---

### 6. demo2-3.c —— 空模板

仅输出 `Hello!` 的占位文件，用于后续实验。

---

### 7. ngram.c —— N-Gram 切片

自然语言处理项目中的唯一示例，实现最基础的 **N-Gram 滑窗切分**：

1. 先读入整数 `n`（N-Gram 的窗口大小）；
2. 再读入一行文本到 `buf`；
3. 对文本中每个位置 `i`，连续输出从 `i` 开始的 `n` 个字符，即得到一个 N-Gram。

辅助函数 `is2Bype()` 用于判断某个字节是否属于双字节字符（GBK 编码范围
`0x81–0x9F` 或 `0xE0–0xEF`），这是为处理中文等多字节文本预留的判断逻辑。

示例输入：

```
2
abcde
```

输出：

```
ab
bc
cd
de
```

这是分词、语言模型（统计语言模型）等任务的第一步。

---

## 三、环境与运行

**依赖**

- C 编译器（MSVC / MinGW / GCC 均可）
- 图形部分额外需要：[TIGR](https://github.com/erkkah/tigr) 单文件图形库，
  以及 Windows 的 `opengl32`、`gdi32`
- `matrix.c`（以及 TIGR 工程中的矩阵库）默认引用本地路径 `D:/libs/Cpp/matrix`，
  见 `machine-learning-demo.sublime-project`

**编译示例（GCC / MinGW）**

```bash
# 矩阵示例
gcc machine-learning-demo/src/matrix.c -o matrix.exe && ./matrix.exe

# Q-Learning 网格世界（数据从标准输入读入）
gcc machine-learning-demo/src/q-learning-demo.c -o ql.exe \
  && ./ql.exe < machine-learning-demo/res/q-learning-data2.txt

# Q-Learning 寻路
gcc machine-learning-demo/src/search.c -lm -o search.exe && ./search.exe

# 权重评分
gcc machine-learning-demo/src/demo2-2.c -o demo22.exe && ./demo22.exe

# N-Gram
gcc natural-language-processing/src/ngram.c -o ngram.exe && ./ngram.exe

# 图像卷积（需 TIGR）
gcc -Wall -O2 machine-learning-demo/src/convolve.c \
    -I"D:/libs/Cpp/tigr/tigr" \
    -lopengl32 -lgdi32 -o convolve.exe && ./convolve.exe
```

编译产物统一命名为 `current.exe`（顶层工程约定）。在 Sublime Text 中可直接
打开对应的 `.sublime-project` 使用其构建系统编译运行。

---

## 四、学习路线（阅读建议）

```
matrix.c        基础数据结构：矩阵/张量
   │
   ├── convolve.c       → 卷积运算（CNN 的前身）
   ├── demo2-2.c        → 权重与打分（感知机雏形）
   ├── q-learning-demo.c → 强化学习：Q-Learning（一维状态表）
   ├── search.c          → 强化学习：Q-Learning（二维网格 + 启发式寻路）
   └── (demo2-3.c)       → 待续
ngram.c        自然语言处理：N-Gram 切分
```

---

## 五、说明

- 本仓库是**个人学习笔记式的代码集合**，各文件相对独立、各自带有 `main()`，
  并非一个统一的应用程序。
- 代码以"能讲清原理"为首要目标，命名、注释与结构都保留了学习过程的痕迹。
- 一切以理解算法为目的，欢迎在此基础上继续实验和改写。
