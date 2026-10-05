# _learning_ — C 语言编程 / 机器学习 / 图形与游戏学习项目

这是一个用 **C 语言**从零手写实现的**综合性学习范例集合**，涵盖三大方向：

1. **机器学习与自然语言处理** —— 手写卷积、矩阵库、Q-Learning、N-Gram、反向传播神经网络；
2. **图形与可视化** —— 基于 TIGR / raylib 的渲染、3D 投影、函数绘图、BFS 迷宫寻路、康威生命游戏；
3. **游戏编程** —— 精灵动画、状态机、碰撞检测、子弹系统与简单 AI。

项目不依赖任何机器学习框架，所有算法均以最朴素的 C 代码手工实现，目的是通过
"重新造轮子"来理解底层原理。代码以 Sublime Text 为编辑器，每个子项目配有独立的
`.sublime-project` / `.sublime-workspace` 工程文件（带中文目录分组），
每个子项目下通常还有一个 `resp.txt` 记录编译命令与链接参数。

> 说明：本 README 为**介绍性质**，只描述项目"做了什么、怎么组织、如何运行"，
> 不对代码的正确性、健壮性或完备性做任何保证。部分文件是尚未完成的草稿，
> 部分设计带有明显的学习探索痕迹（例如 conwaygame 的 `civilizationRule` 中
> 保留了大段关于"无法在本帧修改邻居"的注释推演）。

---

## 一、目录结构

```
_learning_/
├── learning.sublime-project              # 顶层 Sublime 工程（挂载全部子项目）
├── learning.sublime-workspace            # Sublime 工作区状态
├── README.md
│
├── machine-learning-demo/                # 【机器学习】项目
│   ├── res/                              # 图片与 Q-Learning 数据集
│   │   ├── lfh.png
│   │   ├── q-learning-data.txt           #  30 行
│   │   └── q-learning-data2.txt          # 500 行
│   └── src/
│       ├── matrix.c                      # 矩阵库雏形
│       ├── convolve.c                    # 图像卷积（TIGR）
│       ├── q-learning-demo.c             # Q-Learning 走迷宫
│       ├── search.c                      # Q-Learning 网格寻路
│       ├── demo2-2.c                     # 权重评分玩具
│       └── demo2-3.c                     # 空模板
│
├── natural-language-processing/          # 【自然语言处理】项目
│   └── src/ngram.c                       # N-Gram 切片示例
│
├── supervised-learning/                  # 【监督学习】完整 MLP 神经网络
│   └── src/
│       ├── matrix.h / matrix.c           # 完整矩阵运算库
│       ├── trainlib.h / trainlib.c       # 前向/反向传播与更新
│       └── main.c                        # 两层 MLP 拟合乘法表
│
├── 3dcube-demo/                          # 【3D 渲染】线框立方体（raylib）
│   └── src/main.c, render.h, render.c
│
├── bfs-demo/                             # 【算法可视化】BFS 迷宫寻路（raylib）
│   └── src/main.c, bfs.h, bfs.c
│
├── conwaygame-demo/                      # 【生命游戏】ConwayGame 2.0（raylib）
│   └── src/main.c, cell.h, cell.c
│
├── function-painter-demo/                # 【函数绘图】多项式曲线（TIGR）
│   ├── include/tigr.c, tigr.h            # 内置 TIGR 库
│   ├── out/                              # 编译产物
│   └── src/main.c
│
└── stickfigure/                          # 【游戏】火柴人格斗（raylib）
    ├── resources/                        # 精灵图资源
    │   ├── tco.png, fire.png, laser.png, laser1.png
    │   └── background.png
    └── src/main.c, game.h, game.c, player.h
```

---

## 二、机器学习 / NLP 部分

### 1. machine-learning-demo

#### 1.1 matrix.c —— 矩阵库雏形

一个最小化的浮点矩阵封装，用 `struct` 把指针、行数、列数打包在一起：

```c
typedef struct {
    float* p;   // 一维数组存储，行优先
    int r;      // 行数
    int c;      // 列数
} Matrix;
```

| 函数 | 作用 |
| --- | --- |
| `matAlloc(r, c)` | 申请 `r × c` 矩阵 |
| `matFree(m)` | 释放内存 |
| `matAssert(m)` | 校验矩阵是否合法 |
| `matInit(m, lower, upper)` | 用 `[lower, upper]` 区间内的随机数初始化 |
| `matPrint(m)` | 以 `| ... |` 形式打印矩阵 |
| `rnd(lower, upper)` | 区间随机数（基于 `rand()`） |

`main` 中创建一个 2×2 矩阵、随机初始化并打印，是整个项目"张量"基础设施的起点。

#### 1.2 convolve.c —— 图像卷积

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

这是理解 CNN 卷积层原理的最直观示例。依赖项在 `res/resp.txt` 中注明
（需要本地 TIGR 库及 Windows 的 OpenGL / GDI 链接库）。

#### 1.3 q-learning-demo.c —— Q-Learning 网格世界

一个 **表格型 Q-Learning** 演示。把 10×10 网格编码为状态 `0..99`：

```
state = x * 10 + y
```

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

邻居数据由 `res/q-learning-data*.txt` 提供。

#### 1.4 search.c —— Q-Learning 网格寻路

与上一个示例思想相同，但改用 **`ROW × COL = 10 × 10` 的二维浮点 Q 表**，
并叠加了**曼哈顿距离启发式**，目标是从 `(0,0)` 寻路到 `(9,9)`：

- `select()` —— ε-贪心动作选择，`flag = 1` 时允许随机探索，`flag = 0` 时纯贪心；
- `reward()` —— 对四个邻格做 Q 值更新，若邻格到目标的曼哈顿距离更小则给 `REWARD`（100）；
- `maxQValue()` —— 取邻域最大 Q 值作为 bootstrap 目标。

参数：`EPSILON = 0.2`、`ALPHA = 0.2`、`GAMMA = 0.8`、训练 20 轮。

#### 1.5 demo2-2.c / demo2-3.c

- `demo2-2.c` —— "权重匹配打分"玩具：用户输入样本特征与标签，随机生成权重向量
  （取值 0/1/2，其中 `2` 视为通配），`getScore()` 逐维比对后统计得分。
  是分类/感知机的极度简化雏形。
- `demo2-3.c` —— 仅输出 `Hello!` 的占位模板。

### 2. natural-language-processing / ngram.c

实现最基础的 **N-Gram 滑窗切分**：

1. 先读入整数 `n`（N-Gram 窗口大小）；
2. 再读入一行文本到 `buf`；
3. 对文本中每个位置 `i`，连续输出从 `i` 开始的 `n` 个字符。

辅助函数 `is2Bype()` 用于判断某个字节是否属于双字节字符（GBK 编码范围
`0x81–0x9F` 或 `0xE0–0xEF`），为处理中文等多字节文本预留。

示例输入 `2` + `abcde`，输出 `ab / bc / cd / de`。

### 3. supervised-learning —— 从零实现的两层神经网络

这是全集里**完成度最高的机器学习项目**：一个完整的矩阵库 + 训练库 + 训练脚本。

#### 3.1 matrix.h / matrix.c —— 完整矩阵库

`Matrix` 使用**值语义**（按值传递，内部持有 `float*`）：

```c
typedef struct {
    int rows, cols;
    float* p;
} Matrix;
```

提供三类接口：

- **构造/销毁**：`mat_alloc`、`mat_get`、`mat_getrand`、`mat_getfill`、
  `mat_getcopy`、`mat_free`、`matp_free`（批量释放矩阵数组）；
- **运算**：`mat_dot`（矩阵乘）、`mat_sum` / `mat_minus`（加减）、`mat_mul`
  （逐元素乘）、`mat_scale`、`mat_bias`（广播加偏置）、`mat_tsp`（转置）、
  `mat_sumcols`（按列求和，用于 bias 梯度）、`mat_sub`（取子块）、`mat_row`；
- **激活/工具**：`sigmoidf`（带 ±15 饱和保护的 sigmoid）、`mat_sigmoid`、
  `mat_sigmoid_deriv`、`my_rand`、`mat_print`、`isempty`、`mat_size`。

几乎所有函数都带 `assert` 做维度检查，是一份很好的"防御式 C 编程"参考。

#### 3.2 trainlib.h / trainlib.c —— 训练核心

| 函数 | 作用 |
| --- | --- |
| `forward(X, Y, W, b, num_layers, Z, A)` | 全连接前向传播，隐藏层用 **Leaky ReLU**，输出层线性 |
| `backward(...)` | 反向传播，链式求导得到 `dW`、`db`，并按 batch 归一化 |
| `update(...)` | 梯度下降更新：`W -= lr * dW`，`b -= lr * db` |
| `mat_mse(pred, target)` | 均方误差 |
| `mat_leaky_relu` / `mat_leaky_relu_deriv` | Leaky ReLU（α = 0.01）及其导数 |

#### 3.3 main.c —— 拟合乘法表

- **任务**：输入 `(x, y)`，输出 `x * y`。数据集为 `0..9` 全组合共 100 组
  `x_data` / `y_data`；
- **网络结构**：`layers = 2`，`inp_size = 2`，`hidden_size = 20`，`out_size = 1`；
- **初始化**：He 初始化，`limit = sqrt(2 / fan_in)`；
- **训练**：`10000000` 步，学习率 `1e-4`，每 `10000` 步打印一次 loss；
- **推理**：训练后用单样本 `(5, 6)` 做一次前向，验证输出是否为 30。

这是**反向传播 + 梯度下降**在纯 C 下的完整实现，是理解深度学习框架内部机制的核心示例。

---

## 三、图形 / 可视化 / 游戏部分

> 除 `function-painter-demo`、`machine-learning-demo/convolve.c` 使用 **TIGR** 外，
> 图形类项目（3dcube、bfs、conway、stickfigure）统一使用 **raylib**，
> 依赖路径见各自的 `resp.txt`（默认 `d:/libs/Cpp/raylib`）。

### 1. 3dcube-demo —— 线框立方体

一个**不依赖硬件 3D 管线**的手写 3D 渲染示例：

- `main.c` 定义 8 个顶点（边长 `edge = 0.2`），用 A/D/W/S/J/K 六键平移原点；
- `render.c` 是渲染核心：
  - `to2D()` —— **透视投影**：`(x/z, y/z)`，`z == 0` 时用 `1e-3` 防除零；
  - `toScreen()` —— 把 NDC 的 `[-1,1]` 映射到屏幕像素（y 轴翻转）；
  - `point()` —— 用 `DrawRectangle` 画方块顶点；
  - `model()` —— 遍历顶点做投影并连线，用 `DrawLine` 绘制线框。

是理解"世界坐标 → 投影 → 屏幕坐标"管线的最小化示例。

### 2. bfs-demo —— BFS 迷宫寻路可视化

20×20 网格上的**广度优先搜索**可视化：

- `Grid` 结构体带 `row/col/isWall` 与 4 个方向的 `neighbor` 指针；
- `isWall()` —— 以 `RATE`（0.3）随机生成墙，但起点/终点周围
  `START_AREA` / `END_AREA` 范围强制保留空地；
- `initMaze()` —— 用显式栈做 DFS 遍历，为每个格子建立邻居指针；
- `bfs()` —— 标准队列 BFS，用 `parentRow` / `parentCol` 记录父节点，
  再由终点回溯出路径并翻转存入 `path[]`；
- `main.c` —— 玩家用 WASD 控制绿色终点方块，**每次移动都重新跑一次 BFS**，
  红色路径以每秒约 4.3 格的速度逐帧动画延伸；`E` 键重新生成迷宫。

是"算法 + 实时可视化"结合的入门范例。

### 3. conwaygame-demo —— ConwayGame 2.0

康威生命游戏的**可扩展版本**，核心抽象是**元胞 + 规则函数指针**：

```c
typedef struct {
    int weight[MAX_VERTEX];   // 多属性权重（存活态、人口、资源、文化……）
    int vertex;               // 该元胞启用的属性维度
} Cell;

typedef Cell (*Rule)(Cell* cur, Cell* nb, Settings* set);
```

`Settings` 统管网格尺寸、邻居偏移表（`dtRow` / `dtCol`）、演化速率与各属性上限。
`update()` 是通用演化驱动：为每个元胞收集邻居（越界一律置零），
调用规则函数，最后 `memcpy` 回写（双缓冲，保证同步更新）。

内置三条规则：

| 规则 | 说明 |
| --- | --- |
| `conway` | 经典康威规则：存活且邻居 2–3 存活则存活；死亡且邻居恰为 3 则复活 |
| `colorLife` | 彩色生命：在康威规则基础上，复活时取邻居中出现最多的颜色（`weight[1] % 16`） |
| `civilizationRule` | 文明模拟：引入人口 / 资源 / 文化三维属性，实现资源再生、人口增长与消耗、文化发展、衰亡等 |

绘制风格同样可切换（`drawCells` 黑白 / `drawCellsColor` HSV 彩色），
并通过 `RenderTexture2D` 渲染到纹理再缩放贴屏。

**交互**（启动时需输入行列数，1–1000）：

```
1 / 2 / 3     切换规则（康威 / 彩色生命 / 文明）
T / Y         切换黑白 / 彩色绘制
Z + 滚轮      调整演化速率（0.01s ~ 10s）
X + 滚轮      以鼠标为锚点缩放视图
E             暂停 / 继续
R             随机重置
C             清空
鼠标左键拖动   平移视图
鼠标右键       绘制活细胞
```

### 4. function-painter-demo —— 多项式函数绘图

用 TIGR 绘制多项式曲线的最小示例：

```c
typedef struct {
    float l, r;        // 绘制区间
    float step;
    TPixel color;
    float* w;          // 多项式系数
    int s;             // 最高次数
    float rate;        // 纵轴缩放
} Function;
```

`paint()` 在 `[l, r]` 上以 `step` 为步长，用 `Σ w[i]·x^i`（`pow()`）计算 y，
再经过平移/翻转映射到屏幕坐标后 `tigrPlot()` 打点。
示例参数 `W = {-2, 12, -40}`（二次多项式）、区间 `[-200, 200]`、步长 `0.1`、品红色。
注意本目录 `include/` 中**内置了一份 tigr.c / tigr.h**，无需外部依赖。

### 5. stickfigure —— 火柴人格斗游戏

项目里最完整的**游戏框架示例**（`game.c` 约 370 行），实现了：

**数据模型（player.h）**

- `AnimFlip` —— 帧动画：帧序列、每帧时长、位移、攻击判定、受击判定、
  取消动画、默认动画、循环标志等；
- `States` —— 状态枚举：`STAND / RUN / FLY / ATTACK / HIT / LASER`；
- `Player` —— 纹理、动画组、碰撞盒、朝向、状态、重力/飞行参数、
  攻击计时、血量、可见性等。

**系统（game.c）**

| 模块 | 函数 |
| --- | --- |
| 帧动画 | `updateFrame`（按 `duration` 推进帧、循环与位移累积）、`drawAnim`（按朝向翻转绘制） |
| 物理 | `gravity`、`fly` |
| 战斗 | `checkAttack`（近战判定 + 计时 + 击退/扣血）、`ai`（敌人行为） |
| 子弹 | `initBullet`、`startBullet`、`updateBullets`、`drawBullets`、`checkBulletAttack`、`freeBullet` |
| 资源 | `initTco`（加载精灵表并切帧）、`setFrames` / `fillr` / `fillf` 等辅助 |

`main.c` 中：`A/D` 控制玩家移动，`F` 近战攻击，`SPACE` 飞行，`E` 发射激光；
方向键控制敌人；双方均有重力、边界钳制、血量显示与简单 AI。
资源来自 `resources/`（精灵表 `tco.png`、`fire.png`、`laser.png`、背景图等）。

---

## 四、环境与运行

**依赖**

| 依赖 | 用途 | 默认路径（见各 `resp.txt`） |
| --- | --- | --- |
| C 编译器 | 全部 | MSVC / MinGW / GCC |
| **raylib** | 3dcube / bfs / conway / stickfigure | `d:/libs/Cpp/raylib` |
| **TIGR** | convolve / function-painter | `D:/libs/Cpp/tigr`（function-painter 已内置） |
| `opengl32`、`gdi32`、`winmm` | 图形链接库 | Windows 系统库 |

`resp.txt` 中记录的是**构建参数清单**（编译选项 + 源文件 + 包含/库路径 + 链接库），
例如 `3dcube-demo/resp.txt`：

```
-Wall -O2
src/main.c
src/render.c
-o main
-I"d:/libs/Cpp/raylib/include"
-L"d:/libs/Cpp/raylib/lib"
-lraylib -lopengl32 -lgdi32 -lwinmm
```

**编译示例（GCC / MinGW）**

```bash
# ---- 机器学习 / NLP（无图形依赖）----
gcc machine-learning-demo/src/matrix.c -o matrix.exe && ./matrix.exe
gcc machine-learning-demo/src/q-learning-demo.c -o ql.exe \
  && ./ql.exe < machine-learning-demo/res/q-learning-data2.txt
gcc machine-learning-demo/src/search.c -lm -o search.exe && ./search.exe
gcc machine-learning-demo/src/demo2-2.c -o demo22.exe && ./demo22.exe
gcc natural-language-processing/src/ngram.c -o ngram.exe && ./ngram.exe

# ---- 神经网络 ----
gcc -Wall -O2 supervised-learning/src/main.c supervised-learning/src/matrix.c \
    supervised-learning/src/trainlib.c -lm -o supervised.exe && ./supervised.exe

# ---- raylib 图形 ----
gcc -Wall -O2 3dcube-demo/src/main.c 3dcube-demo/src/render.c \
    -I"d:/libs/Cpp/raylib/include" -L"d:/libs/Cpp/raylib/lib" \
    -lraylib -lopengl32 -lgdi32 -lwinmm -o cube.exe && ./cube.exe

gcc -Wall -O2 bfs-demo/src/main.c bfs-demo/src/bfs.c \
    -I"d:/libs/Cpp/raylib/include" -L"d:/libs/Cpp/raylib/lib" \
    -lraylib -lopengl32 -lgdi32 -lwinmm -o bfs.exe && ./bfs.exe

gcc -Wall -O2 conwaygame-demo/src/main.c conwaygame-demo/src/cell.c \
    -I"d:/libs/Cpp/raylib/include" -L"d:/libs/Cpp/raylib/lib" \
    -lraylib -lopengl32 -lgdi32 -lwinmm -o conway.exe && ./conway.exe

gcc -Wall -O2 stickfigure/src/main.c stickfigure/src/game.c \
    -I"d:/libs/Cpp/raylib/include" -L"d:/libs/Cpp/raylib/lib" \
    -lraylib -lopengl32 -lgdi32 -lwinmm -o stick.exe && ./stick.exe   # 需在 stickfigure/ 下运行以找到 resources/

# ---- 图像卷积（需 TIGR）----
gcc -Wall -O2 machine-learning-demo/src/convolve.c \
    -I"D:/libs/Cpp/tigr/tigr" -lopengl32 -lgdi32 -o convolve.exe && ./convolve.exe

# ---- 函数绘图（TIGR 已内置）----
gcc -Wall -O2 function-painter-demo/src/main.c function-painter-demo/include/tigr.c \
    -I"function-painter-demo/include" -lopengl32 -lgdi32 \
    -o function_painter.exe && ./function_painter.exe
```

在 Sublime Text 中可直接打开对应的 `.sublime-project` 使用其构建系统编译运行；
编译产物在各子项目约定位置（`current.exe`、`main.exe`、`*.exe`、`out/` 等）。

---

## 五、学习路线（阅读建议）

```
【基础】
matrix.c（矩阵库雏形） ──► supervised-learning（完整矩阵库 + 反向传播 + MLP）
        │
【机器学习】
        ├── convolve.c         卷积运算（CNN 前身）
        ├── demo2-2.c          权重与打分（感知机雏形）
        ├── q-learning-demo.c  强化学习 · 一维状态表
        └── search.c           强化学习 · 二维网格 + 启发式寻路
【自然语言处理】
ngram.c                        N-Gram 切分
【图形 / 算法可视化】
function-painter-demo          函数绘图（TIGR 入门）
3dcube-demo                    手写 3D 投影与线框渲染
bfs-demo                       BFS 迷宫寻路可视化
conwaygame-demo                康威生命游戏 + 可插拔规则 / 文明模拟
stickfigure                    游戏框架：动画 / 物理 / 战斗 / 子弹 / AI
```

---

## 六、说明

- 本仓库是**个人学习笔记式的代码集合**，各子项目相对独立、各自带有 `main()`，
  并非一个统一的应用程序；`machine-learning-demo` 与 `natural-language-processing`
  是最早的两个子项目，其余为后续扩展。
- 代码以"能讲清原理"为首要目标，命名、注释与结构都保留了学习过程的痕迹
  （例如 `resp.txt` 这种"构建参数笔记"、以及规则函数里未完成的设计推演注释）。
- 编译产物（`*.exe`）与部分编辑器状态文件已纳入版本库，属于项目当前的现状，
  并非有意为之；`.gitignore` 已尽量不再追踪新增的构建产物。
- 一切以理解算法与编程为目的，欢迎在此基础上继续实验和改写。
