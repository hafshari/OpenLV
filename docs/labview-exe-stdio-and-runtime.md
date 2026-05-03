# LabVIEW EXE: stdio, native helpers, and runtime — executive summary

This document summarizes a cross-platform investigation (Windows, Linux, macOS) for OpenLV: **no-UI** builds, **stdout/stderr**, **DLL/shared-library** helpers, and **LabVIEW Run-Time Engine (RTE)** deployment. It refines the idea “put all console I/O in a DLL and call it from LabVIEW” and records **how practitioners usually solve** the same problems.

---

## Executive summary

1. **LabVIEW does not provide `std::cout` / `cerr`-class streams** for application-built executables. Patterns differ by OS (pipes on Linux, Win32 / subsystem / hacks on Windows, bundles + wrappers on macOS).

2. **A small native library (`.dll` / `.so` / `.dylib`) that calls `printf` / `write(1|2)`** and is loaded through **Call Library Function** is a **reasonable** way to centralize “write a line of text” — but it is **not automatically portable** in behavior: the library runs **in the LabVIEW process** and **inherits that process’s standard handles** (or lack of them). See [Caveat: DLLs and stdio](#caveat-dlls-inherit-the-labview-process) below.

3. **RTE is not “taken care of” automatically on every target machine.** The Application Builder produces an **EXE/.app** (and optional **installer**). **You** must ship the **matching RTE** and any **drivers/toolkits** using NI-supported means (Windows installer with **Additional Installers**, Linux **apt/dnf** packages, macOS **frameworks in the bundle** or separate installs). LabVIEW does not silently install dependencies on end-user PCs when you copy only the EXE.

---

## Caveat: DLLs inherit the LabVIEW process

A dynamic library has **no stdout of its own**; it uses the **host process** file descriptors (POSIX) or **console handles** (Windows).

### Why `printf` / `fprintf(stdout|stderr)` in a DLL often “does nothing” (especially on Windows)

- A **DLL is not its own program.** It is loaded into **`LabVIEW.exe`** or your built **EXE**. Calls such as **`printf`**, **`fprintf(stdout, …)`**, and **`fprintf(stderr, …)`** use the C runtime’s **`stdout` / `stderr`**, which are wired to whatever **standard handles** that **same process** already has.
- **`stdout` and `stderr` are not magical pipes inside the DLL** — they are **`FILE*` wrappers** backed by the process’s standard-output / standard-error handles (Windows) or **FD 1 / FD 2** (POSIX).
- On **Windows**, a typical LabVIEW-built app is a **GUI subsystem** process. The OS **does not** attach a console session to it the way it does for a normal **`main()` console program**, so **stdin / stdout / stderr may be missing or useless** ([`GetStdHandle` / subsystem behavior](https://learn.microsoft.com/en-us/windows/console/getstdhandle)). Then **`printf` → nowhere you can see** — not because the DLL is broken, but because **the LabVIEW process never received real console streams**.
- The same logic applies to **`stderr`**: if the process has no proper **stderr** handle for a CLI session, **`fprintf(stderr, …)`** often **shows nothing** even though the call may return normally.
- **When it works better:** On **Linux** (and often **macOS Terminal**), starting the app **from a shell** usually gives **valid FD 1 and 2**, so **`printf` / `write(1|2)`** can appear in that terminal. On **Windows**, output tends to appear only after you **create or attach a console**, change **subsystem**, or use **Win32** APIs with valid handles — same as for any GUI-hosted DLL ([NI: DLLs inherit GUI LabVIEW’s stdio](https://forums.ni.com/t5/LabVIEW/Capturing-STOUT-STDERR-from-C-DLLs/td-p/3682488/page/2)).

**One-line takeaway:** the DLL doesn’t fail — **`stdout`/`stderr` mean “the LabVIEW process’s streams,”** and a **Windows GUI LabVIEW process often has no real console streams** unless you add them.

### Windows (most friction)

- A default LabVIEW **.exe** is a **Windows GUI** (not **console**) process. The OS **does not** give such a process useful **stdin/stdout/stderr** the way a C `main()` console app does ([`GetStdHandle` behavior for GUI vs console](https://learn.microsoft.com/en-us/windows/console/getstdhandle)).
- Therefore **`printf` / `fprintf(stdout, …)` inside a DLL** loaded by LabVIEW often goes **nowhere visible**, unless you:
  - **Attach or allocate a console** (e.g. `AttachConsole`, `AllocConsole`) — **side effects** include a new console window and shared lifetime with LabVIEW ([forum: DLL and console](https://forums.ni.com/t5/LabVIEW/Calling-a-DLL-and-writing-to-console-window/td-p/3120665)), or
  - Use **Win32** (`WriteConsole`, etc.) with valid handles, or
  - Change the **PE subsystem** (e.g. post-build **`editbin /SUBSYSTEM:CONSOLE`**) so the process starts as a **console** app — **tradeoffs** for GUI apps.
- Community experience: **redirecting** stdout from DLLs into **pipes** that LabVIEW reads is a common pattern when you need **capture inside LV** ([redirecting stdout/stderr in DLLs](https://forums.ni.com/t5/LabVIEW/Redirecting-stdout-stderr-stdin-to-from-LabVIEW-in-dlls/td-p/1768662)).

### Linux

- If the user starts your app **from a shell**, **FD 1 and 2** are often valid; **pipe VIs** / writing to **fd 1 (stdout)** and **fd 2 (stderr)** are widely described ([example: fd mixups vs redirection](https://forums.ni.com/t5/LabVIEW/Linux-Pipe-to-stdout-displays-on-the-command-line-but-does-not/td-p/2605399)).
- **Headless servers** (no X): a plain LabVIEW **EXE** may still require a **display stack**; NI documents **shared library + embedded “dark” RTE + C wrapper** for **no-GUI** targets ([Running a LabVIEW Application on Linux Without a Graphical User Interface](https://knowledge.ni.com/KnowledgeArticleDetails?id=kA00Z0000019RYlSAM)). A **DLL** that only does `printf` does **not** remove that constraint by itself.

### macOS

- **POSIX-like** behavior can apply when the process has a sane stdio setup; **`.app`** launches from Finder may **not** behave like a terminal-attached CLI.
- **Product reality:** professional LabVIEW for macOS effectively stopped at **2023 Q3**; later options are largely **Community** ([NI macOS compatibility](https://www.ni.com/en/support/documentation/compatibility/18/labview-and-macos-compatibility.html)). Treat macOS as **verify-on-target**.

**Bottom line:** a **thin native “console write” DLL** is a good **single place** for `fprintf`/`write`, but on **Windows** you often still need **console attachment or subsystem strategy**; on **Linux** terminal-launched apps behave more predictably; on **macOS** validate with your exact launch path and LV edition.

---

## RTE and dependencies: what LabVIEW does *not* do for you

- Executables need the **LabVIEW Run-Time Engine** version that matches the build ([EXE vs installer](https://knowledge.ni.com/KnowledgeArticleDetails?id=kA00Z0000019YLjSAM&l=en-US)).
- **Windows:** build an **installer** and tick **LabVIEW Run-Time Engine** under **Additional Installers** ([include RTE](https://knowledge.ni.com/KnowledgeArticleDetails?id=kA03q000000YGvpCAG&l=)); copying RTE folders without running installers is **unsupported** ([run standalone apps](https://knowledge.ni.com/KnowledgeArticleDetails?id=kA00Z0000019OWNSA2)).
- **Linux:** install **`labview-…-rte`** via **apt/dnf** after adding NI feeds ([Install LabVIEW on Linux](https://knowledge.ni.com/KnowledgeArticleDetails?id=kA03q000000YGwsCAG&l=en-CA)).
- **macOS:** developers sometimes **embed** `LabVIEW … Runtime.framework` into the **`.app`** via **post-build** scripts ([community: Apple Silicon / Support folder](https://forums.ni.com/t5/LabVIEW/LabVIEW-Standalone-Application-for-Apple-Silicon-arm64-using/m-p/4354016/highlight/true)); **drivers** (e.g. VISA) may still need **separate** installs.

---

## How others do it (community and tooling)

| Approach | Idea | Links |
|----------|------|--------|
| **G-CLI** | **`g-cli`** proxy on **TCP**; **`OUTP`** → real console. Requires **G-CLI VI library** in your app (not automatic). | [GitHub](https://github.com/JamesMc86/G-CLI), [docs](https://g-cli.github.io/G-CLI/), [protocol wiki](https://github-wiki-see.page/m/JamesMc86/G-CLI/wiki_index), [v2.1](https://forums.ni.com/t5/Continuous-Integration/G-CLI-v2-1-Release/td-p/3947866) |
| **NI LabVIEW CLI (`LabVIEWCLI`)** | Shipped with LabVIEW (2018+); automation via **VI Server**; Jenkins docs reference **VI Server** port **3363**. | [Jenkins LabVIEW tutorial](https://www.jenkins.io/doc/tutorials/build-a-labview-app/), [Jenkins + LabVIEWCLI / user account issues](https://forums.ni.com/t5/Continuous-Integration/Setup-CI-and-Jenkins-LabVIEWCLI-G-CLI-not-working-yet/td-p/4223012) |
| **Wiresmith “LabVIEW CLI”** | **C# console** app + TCP to LabVIEW — early pattern for **exit codes** and stdout for **Jenkins**. | [Wiresmith article](https://www.wiresmithtech.com/devs/bringing-command-line-interface-labview/) |
| **JKI .NET System Exec** | **.NET Process** wrapper: **redirect** stdout/stderr to LabVIEW **events** — great for **calling existing CLI tools**, not for LV-as-CLI. | [VIPM package](https://www.vipm.io/package/jki_lib_dotnet_system_exec/), [GitHub](https://github.com/jvr-ruiz/JKI-.NET-System-Exec) |
| **JKI (historical CI)** | **Batch files + text files** to pass results when native stdin/out were insufficient (cited in Wiresmith article). | (same Wiresmith link) |
| **NI forums: DLL + pipes** | **Redirect** DLL stdout/stderr through **pipes** read by LabVIEW; **`fflush`** often required; **GUI process** still limits naive `printf`. | [STDIO / DLL thread](https://forums.ni.com/t5/LabVIEW/Redirecting-stdout-stderr-stdin-to-from-LabVIEW-in-dlls/td-p/1768662), [capture from C DLLs](https://forums.ni.com/t5/LabVIEW/Capturing-STOUT-STDERR-from-C-DLLs/td-p/3682488/page/2) |
| **Linux: Pipe VIs** | Write to **stdout/stderr** via correct **file descriptors** when run from a terminal. | [Linux pipe / redirection](https://forums.ni.com/t5/LabVIEW/Linux-Pipe-to-stdout-displays-on-the-command-line-but-does-not/td-p/2605399) |

**Pattern:** tools like **G-CLI** and **LabVIEWCLI** **do not** depend on LabVIEW being a well-behaved POSIX/Win32 console binary; they use a **tiny native CLI front-end** + **TCP or VI Server** so CI and shells get **predictable** output and exit status.

### G-CLI: the LabVIEW side must opt in

The TCP stream is **not** something built-in VIs do automatically. You need:

- The **G-CLI VI package** installed for each LabVIEW version you use ([VIPM](https://g-cli.github.io/G-CLI/) / [releases](https://github.com/JamesMc86/G-CLI/releases)).
- Application logic that calls the **G-CLI library** to receive startup metadata (**arguments**, **cwd**, etc.) and to send **output** and **exit code** messages that the proxy turns into real **stdout** and a proper **process exit status**.
- Launch via **`g-cli …`** (proxy on `PATH`) so the **console-attached process** is `g-cli`, not raw `LabVIEW.exe`.

A stock VI opened from Explorer does **not** speak this protocol. **`LabVIEWCLI`** (NI) is separate: it expects **VI Server** enabled (default TCP **3363**) and NI’s operations — again **explicit** setup, not implicit LabVIEW behavior ([Jenkins tutorial](https://www.jenkins.io/doc/tutorials/build-a-labview-app/)).

---

## What to do if we still need stdout and stderr

Pick a strategy by **who consumes the output** and **OS**:

| Goal | What to implement |
|------|-------------------|
| **CI / Jenkins / GitLab runner** (Windows-centric automation) | Prefer **`g-cli`** + G-CLI library in your tool VI/EXE, or **NI `LabVIEWCLI`** with VI Server and NI-supported operations. Ship **`g-cli`** (and RTE) on agents; document launch command. |
| **Shell scripting** (`cmd`, PowerShell) calling **your** LV-built EXE | **Windows:** Either run through **`g-cli`**, or fix the **process** stdio: **Win32** in a DLL (`AttachConsole` / `AllocConsole` / `WriteConsole`), **`editbin /SUBSYSTEM:CONSOLE`**, or community “Write StdOut” patterns ([NI forum](https://forums.ni.com/t5/LabVIEW/Write-to-STDOUT-from-Labview-EXE-Ran-from-Console/td-p/3297020)). **Linux:** Use **pipe / FD 1 & 2** from NI’s pipe APIs when users run `./your_exe` from a terminal ([fd / redirection](https://forums.ni.com/t5/LabVIEW/Linux-Pipe-to-stdout-displays-on-the-command-line-but-does-not/td-p/2605399)). **macOS:** Prefer **`g-cli`** or a tiny **native wrapper**; verify **Community vs Pro** and launch context (Terminal vs `.app`). |
| **Separate “true” stderr vs stdout** | **G-CLI:** follow library support for error vs normal lines (check current G-CLI API). **DLL:** use **`fprintf(stderr, …)`** vs **`stdout`** only if handles are valid; **Windows GUI** may require the same console fixes for **both** streams. **Linux:** separate writes to **fd 2** vs **fd 1**. |
| **No protocol / no extra launcher** | **Log file** or **syslog / Windows Event Log** (`Write to System Log`) — always reliable; callers tail the file. |
| **In-process only** (no `g-cli`) | **DLL** with **`write(2)`** / **`fprintf`** + **`fflush`**; on Windows GUI EXEs, plan **AllocConsole** or pipe redirection ([DLL threads](https://forums.ni.com/t5/LabVIEW/Redirecting-stdout-stderr-stdin-to-from-LabVIEW-in-dlls/td-p/1768662)). |

**Deployment reminder:** whichever path you choose, the target still needs the **matching RTE** (and **G-CLI** binaries if you standardize on G-CLI); bundle via **installer**, **Linux packages**, or **documented** installs — see [RTE](#rte-and-dependencies-what-labview-does-not-do-for-you) above.

---

## Suggested direction for OpenLV

- For **maximum portability** of “echo” for humans and CI: treat **G-CLI–style proxy** or **file-based** logging as first-class, and treat **in-process DLL `printf`** as an **optimization** that must be **validated per OS** (especially **Windows GUI EXE**).
- For **deployment**, document the **exact RTE** and **install path** (installer vs Linux packages vs embedded macOS frameworks).

Related issues: [#4](https://github.com/hafshari/OpenLV/issues/4), [#5](https://github.com/hafshari/OpenLV/issues/5).
