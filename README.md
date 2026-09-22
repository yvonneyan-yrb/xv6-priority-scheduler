\# xv6 Priority Scheduler \& System Call Extensions

\# xv6 优先级调度器与系统调用扩展



> A COMPSCI 340 (Operating Systems, University of Auckland) assignment: extending the xv6 (RISC-V) teaching kernel with new system calls, user-level programs, and a priority-based CPU scheduler.

>

> 奥克兰大学 COMPSCI 340（操作系统）课程作业：为 xv6（RISC-V）教学内核扩展新的系统调用、用户态程序，并实现基于优先级的 CPU 调度器。



\---



\## 📌 Project Overview | 项目简介



\*\*English\*\*

This project modifies the xv6 kernel (C version, RISC-V architecture) in two parts:



\- \*\*Part 1 — xv6 Basics\*\*: four user-level programs and one new system call, covering argument handling, inter-process communication with pipes, and the full path of adding a system call (user stub → kernel dispatch → implementation).

\- \*\*Part 2 — Priority Scheduler\*\*: replacing xv6's default round-robin scheduler with a priority-based one, plus two new system calls (`setpri`, `getpinfo`) for controlling and inspecting process priority and CPU usage.



\*\*中文\*\*

本项目基于 xv6 内核（C 版本，RISC-V 架构），分为两部分：



\- \*\*Part 1 — xv6 基础\*\*：四个用户态程序和一个新系统调用，覆盖命令行参数处理、基于管道的进程间通信，以及添加一个系统调用的完整链路（用户态桩 → 内核分发 → 内核实现）。

\- \*\*Part 2 — 优先级调度器\*\*：将 xv6 默认的 Round-Robin 调度器替换为基于优先级的调度器，并新增两个系统调用（`setpri`、`getpinfo`），用于设置和查询进程优先级及 CPU 使用情况。



\---



\## ✨ Features | 实现内容



\### Part 1: xv6 Basics (user-level)



| Program | Description |

|---------|-------------|

| `user/touch.c` | `touch \[file]` — creates an empty file; prints an error when no argument is given |

| `user/pingpong.c` | Forks two children; the parent sends `"ping"` through pipes, each child prints `<pid>: pong` and replies `"ACK"`; the parent waits for both ACKs and both children to exit |

| `user/findppid.c` | `findppid \[num]` — new `getppid` system call; spawns a linear chain of `num` child processes, each printing its PID and its parent's PID up to the root |

| `user/primes.c` | `primes \[num]` — concurrent prime sieve using `fork()` + `pipe()`: a chain of filtering processes, each removing multiples of its prime from the stream (supports num ≤ 200) |



| 程序 | 说明 |

|------|------|

| `user/touch.c` | `touch \[file]` — 创建空文件；缺少参数时打印错误信息 |

| `user/pingpong.c` | fork 两个子进程；父进程通过管道发送 `"ping"`，每个子进程打印 `<pid>: pong` 并回传 `"ACK"`；父进程等待两条 ACK 并回收全部子进程后退出 |

| `user/findppid.c` | `findppid \[num]` — 新增 `getppid` 系统调用；以线性链方式创建 num 个子进程，由最深层子进程开始逐级向上打印各自 PID 与父进程 PID |

| `user/primes.c` | `primes \[num]` — 基于 `fork()` + `pipe()` 的并发素数筛：一条过滤进程链，每个进程从数据流中筛去自身素数的倍数（支持 num ≤ 200） |



\### Part 2: Priority Scheduler (kernel)



\*\*English\*\*

\- Replaced the default scheduler in `kernel/proc.c` with a \*\*priority-based scheduler\*\*:

&#x20; - Two priority levels: `1` (low, default) and `2` (high)

&#x20; - Every process starts at priority 1

&#x20; - High-priority processes always preempt low-priority ones; a level-1 process never runs while a runnable level-2 process exists

&#x20; - Same-priority processes are scheduled \*\*round-robin\*\*

\- New system calls:

&#x20; - `int setpri(int num)` — sets the calling process's priority; returns `0` on success, `-1` for any value other than 1 or 2

&#x20; - `int getpinfo(struct pinfo \*)` — fills in per-process info (PID and CPU ticks consumed at each priority level); returns `-1` on a bad pointer, `0` on success. Defined in `kernel/pinfo.h`.

\- Changed `CPUS := 3` to `CPUS := 1` in the `Makefile` (single-core scheduling as required)



\*\*中文\*\*

\- 将 `kernel/proc.c` 中的默认调度器替换为\*\*基于优先级的调度器\*\*：

&#x20; - 两个优先级：`1`（低优先级，默认值）和 `2`（高优先级）

&#x20; - 所有进程创建时默认为优先级 1

&#x20; - 高优先级进程始终优先运行；只要存在可运行的高优先级进程，低优先级进程就不会被调度

&#x20; - 同优先级进程之间采用 \*\*Round-Robin\*\* 轮转调度

\- 新增系统调用：

&#x20; - `int setpri(int num)` — 设置调用进程的优先级；成功返回 `0`，参数非 1/2 返回 `-1`

&#x20; - `int getpinfo(struct pinfo \*)` — 填充每个进程的信息（PID 及其在各优先级上消耗的 CPU 时钟周期数）；指针非法返回 `-1`，成功返回 `0`。结构体定义在 `kernel/pinfo.h`

\- 按作业要求将 `Makefile` 中 `CPUS := 3` 改为 `CPUS := 1`（单核调度）



\---



\## 🔧 Key Files Modified | 关键改动文件



```

kernel/proc.c      # priority scheduler  优先级调度器

kernel/proc.h      # per-process priority state  进程优先级状态

kernel/pinfo.h     # struct pinfo definition  pinfo 结构体定义

kernel/syscall.h   # syscall numbers  系统调用号

kernel/syscall.c   # syscall dispatch  系统调用分发

kernel/sysproc.c   # setpri / getpinfo / getppid implementation  系统调用实现

user/touch.c       # Task 1

user/pingpong.c    # Task 2

user/findppid.c    # Task 3

user/primes.c      # Task 4

Makefile           # CPUS := 1; UPROGS additions  单核配置；新增用户程序

```



\---



\## 🚀 Build \& Run | 构建与运行



\*\*Environment | 环境\*\*: Ubuntu 24.04 LTS (tested on QEMU RISC-V)



\### 1. Install Dependencies | 安装依赖



```sh

sudo apt update

sudo apt install -y build-essential git qemu-system-riscv64 gcc-riscv64-linux-gnu

```



\### 2. Build and Launch xv6 | 编译并启动



```sh

make qemu

```



\### 3. Try It Out | 功能验证



```sh

$ touch test                  # create a file  创建文件

$ ls                          # verify it exists  确认文件已创建

$ pingpong                    # pipe IPC between 3 processes  三进程管道通信

$ findppid 5                  # process chain of 5  5 层进程链

$ primes 20                   # concurrent sieve  并发素数筛

```



To exit QEMU, press `Ctrl` + `a` and then `x`.

退出 QEMU：先按 `Ctrl` + `a`，再按 `x`。



\---



\## 📝 Notes | 说明



\- Completed as coursework for COMPSCI 340/SE370 at the University of Auckland (2026 S2). Shared here for portfolio purposes — please do not copy it if you are currently taking the course.



\- 本项目为奥克兰大学 COMPSCI 340/SE370 课程作业（2026 学年第二学期），仅作为个人作品集展示。如果你正在修读该课程，请不要直接拷贝代码。



