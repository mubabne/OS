# OS БД1: what it is and how to do it (B242270045)

> Sources: `OS/Assignment/OS_Topic_and_Peer_Review_Allocation.pdf`, `OS_BD1_Guide.pdf`, and the teammate repo `OS/biedaalt1/II-4-1-UNIX-Shell` (github.com/bulgaa777/II-4-1-UNIX-Shell, only an initial commit so far).
>
> ⚠️ **Correction:** `Guide.MD` says you're in team II-4-2 (Kernel Module). That is wrong. The allocation table puts **B242270045 in II-4-1**, and your teammate's repo name agrees.

## Your team and topic
- **Code:** II-4-1 (Day II, 4th period, topic 1)
- **Topic 1: UNIX Shell** (textbook Ch.3 Processes, *Operating System Concepts* 2018 edition, Programming Project 1)
- **UI visualization:** a **process tree**
- **Team (6 people):** B180930028, B231940067, B241910015, B241940061, **B242270045 (you)**, B242270131
- **You review:** IV-2-4 (Topic 4, Kernel Data Structures) in week 10
- **You're reviewed by:** IV-2-8

## What it asks
**Textbook part, written in C on Linux.** A shell that prints `osh>` and:
1. Reads a command and runs it in a child process (`fork()` → `execvp()`, while the parent `wait()`s).
2. Ending a command with `&` runs it in the background, so the parent doesn't wait.
3. History: `!!` re-runs the last command and prints it, and gives "No commands in history." when there's none.
4. Redirection: `>` (stdout to a file) and `<` (stdin from a file), via `dup2()`.
5. A pipe between two commands, e.g. `ls -l | less`, via `pipe()` + `dup2()`.
6. `exit` quits the shell.

**Teacher's addition:** show the processes the shell creates as a **tree** of parent→child links with PID and state (running, finished, background, zombie, …). You must be able to change commands and inputs and test again.

**General rules:** no hard-coded test data, a reset or re-run option, clear errors for bad input (unknown command, missing file, `!!` with empty history, a bad pipe), your own extra tests, and it must be runnable on another computer by following the Manual.

## Deliverables (due week 9, so confirm the exact day)
- **Repo:** `README.md` (name, code, goal, Quick Start, any AI/library use declared), `src/`, `tests/` or `examples/`, `docs/`, and a **release tag**. No secrets.
- **`II-4-1_BD1_Report.pdf`:** cover, summary, 1 Intro, 2 Textbook requirements, 3 Theory, 4 Design, 5 Implementation, 6 Tests + UI, 7 Test method, 8 Results (real screenshots), 9 Discussion, 10 Conclusion, references. The report must also describe how you worked on GitHub.
- **`II-4-1_Manual.pdf`:** a user guide (purpose, environment, start/stop/reset, what the tree's colors and states mean, 2+ examples, common errors) and an operational guide (clone, OS/compiler versions, dependencies, build and run, test data, boundary/error tests, expected output, known issues).
- **One team post** in Teams "БД1 - Submissions and Reviews", titled `II-4-1 – UNIX Shell`.
- **Week 10:** review IV-2-4 and reply under their post with a Review PDF.

## Likely split for 6 people
| Piece | Notes |
|---|---|
| 1. Core shell: parse, fork/exec/wait, `&`, `exit` | the foundation, probably the repo owner |
| 2. `!!` history + `<` `>` redirection | small and self-contained |
| 3. Pipe `\|` + error handling | |
| 4. Process-tree UI | the shell logs events (pid, ppid, cmd, state) to a JSON/log file, and a UI (Python/web) draws the tree |
| 5. Tests (`tests/` scripts) + Manual | |
| 6. Report + Teams post | |

**What you should do:** ask the group today which number is yours. If nobody has assigned one, claim **#2 (history + redirection)** or **#4 (tree UI)**. You can start #2 as soon as the core loop exists, and #4 only needs an agreed log format.

## Step by step for you
1. **Get Linux:** you already have VirtualBox, so use an Ubuntu VM (WSL also works for this topic, since no kernel module is involved). Run `sudo apt install build-essential git`.
2. **Learn the core (1–2 h):** Ch.3 sections on `fork`, `exec`, `wait`, zombies/orphans, and pipes. Your Lab03 already has `zombie.c` and the pipe programs, so re-read those. Run `pstree -p $$` to see a process tree.
3. **Read the textbook project:** in `CourseBook.pdf`, end of Ch.3, "Project 1 — UNIX Shell". Note each of its 5 parts.
4. **After she pushes:** `git pull`, `make`, then run `./osh` and try `ls`, `sleep 5 &`, `!!`, `ls > out.txt`, `sort < out.txt`, `ls | wc -l`. Work on your own branch and open a PR, because the commit history goes into the report.
5. **Do your part**, and add 3–5 test cases to `tests/` (input → expected result).
6. **Write your section** of the Report and Manual with real screenshots, then help tag the release and make the post.
