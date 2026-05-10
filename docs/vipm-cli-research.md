# VIPM CLI — research notes (OpenLV)

Supports [#12](https://github.com/hafshari/OpenLV/issues/12) (research), [#13](https://github.com/hafshari/OpenLV/issues/13) (feature: scripted workflows), and [#14](https://github.com/hafshari/OpenLV/issues/14) (request macOS Preview parity). Primary GitHub discussion: [issue #12, consolidated comment](https://github.com/hafshari/OpenLV/issues/12#issuecomment-4414777816).

This document records **what we looked up**, **what we verified locally**, and **what we filed upstream** so it survives chat history and issue threads.

---

## Official VIPM CLI documentation

- **Overview:** [Using VIPM via CLI](https://docs.vipm.io/latest/cli/) — automation and CI/CD entry point; links to the command reference, getting started, Docker, and GitHub Actions.
- **Command reference:** [VIPM CLI Command Reference](https://docs.vipm.io/latest/cli/command-reference/) — global options (`--refresh`, `--labview-version`, `--labview-bitness`, `--color-mode`, `-h` / `--help`), and commands such as `install`, `uninstall`, `list`, `search`, `package-list-refresh`, `activate`, `build`, `version`, `about`. Preview **`vipm.toml`** flows are documented separately: [vipm.toml getting started](https://docs.vipm.io/latest/vipm-toml/getting-started/).
- **Getting started:** [Getting Started with VIPM CLI](https://docs.vipm.io/latest/cli/getting-started/) — requires **VIPM Desktop** with CLI support; check with `vipm --version`; PATH examples include **`C:\Program Files\JKI\VIPM`** (Windows) and **`/usr/local/jki/vipm`** (Linux / macOS in docs).

---

## VIPM Preview installers and macOS

- **[VIPM Preview — Installation](https://docs.vipm.io/preview/preview/)** lists **Windows** and **Linux** installers (including one-line install snippets). **macOS** is **not** listed there as of our review.
- **[VIPM Desktop Versions](https://www.vipm.io/desktop/versions/)** shows **Mac** releases trailing **Windows** (stable Mac line vs current Windows builds).

**Implication:** The **documented Preview CLI** story aligns with **Windows + Linux** Preview installs today; **macOS** users on older stable Mac builds may **not** get the same **`vipm`** CLI entry point until JKI publishes a matching Mac Preview or newer Mac installer.

---

## NI LabVIEW on macOS (why macOS VIPM parity matters)

Per NI **[LabVIEW and macOS Compatibility](https://www.ni.com/en/support/documentation/compatibility/18/labview-and-macos-compatibility.html)** (February 2026):

- LabVIEW on macOS is offered again in **[Community Edition](https://www.ni.com/en/shop/labview/select-edition/labview-community-edition.html)** starting **LabVIEW 2025 Q3**.
- The **final LabVIEW Professional** release for macOS was **2023 Q3**; NI documents **no** macOS releases between **2023 Q3** and **2025 Q3**.

Treat **commercial vs Community** licensing separately from tooling; the compatibility page is the authoritative NI summary.

---

## What we did in OpenLV (tracking)

| Action | Link / detail |
|--------|----------------|
| Research ticket | [#12 — VIPM CLI evaluation](https://github.com/hafshari/OpenLV/issues/12) |
| Feature ticket (scripts/docs after research) | [#13 — VIPM CLI scripted workflows](https://github.com/hafshari/OpenLV/issues/13) |
| macOS Preview request (OpenLV tracker) | [#14 — VIPM Preview installer for macOS](https://github.com/hafshari/OpenLV/issues/14) |
| Upstream request (JKI / VIPM repo) | [vipm-io/vipm-desktop-issues#124](https://github.com/vipm-io/vipm-desktop-issues/issues/124) |
| Working branch | `feature/issue-13-vipm-cli-workflows` |

Consolidated findings were posted as a **single comment** on **#12**, with **#13** and **#14** linking back to that comment for traceability.

---

## Local verification (example: maintainer Mac)

On one developer machine used for OpenLV:

- Installed application: **`/Applications/VI Package Manager.app`**, bundle version **2020.3.0.2540** (`CFBundleShortVersionString` in `Contents/Info.plist`).
- Shell: **`vipm`** was **not** on `PATH` (`command not found`).
- No separate **`vipm`** CLI binary was found beside the GUI executable under **`Contents/MacOS/`**; **`/usr/local/jki/vipm`** was not present.

**Conclusion for that host:** **GUI VIPM (2020.x)** is installed; the **modern unified `vipm` CLI** described in current **docs.vipm.io** CLI pages **was not available** without a **newer VIPM for macOS** that ships that binary (see Preview / Mac release gap above). Other machines should re-check with `which vipm`, `vipm --version`, and `vipm version` after upgrading VIPM.

---

## Homebrew

- **Homebrew** manages general macOS/Linux software; it is **not** a replacement for **VIPM’s** VI Package catalog.
- **`brew search`** requires at least **one** text or regex argument (e.g. `brew search --cask vipm`). Invoking `brew search --cask` with **no** argument fails; piping that into `rg` does not help.
- **`brew search --cask vipm`** did not surface an official **VIPM** cask; results may include **unrelated** fuzzy matches (e.g. **`vimy`**). Expect installation via **JKI’s** installer, not `brew install vipm`.

---

## Docker, executables, and OS-specific builds

- **Docker** images used day-to-day are overwhelmingly **Linux**. Docker Desktop on macOS/Windows runs Linux containers via a VM/WSL layer — **not** “Windows + Linux + macOS builds in one generic container.”
- **LabVIEW Application Builder** outputs are **native per OS** (`.exe`/DLL ecosystem on Windows, ELF/`.so` on Linux, `.app`/frameworks on macOS). Plan **separate** build environments or CI jobs per OS for **release-grade EXE/app** artifacts.
- **macOS** builds typically require **macOS** runners or hardware (for example GitHub **`macos-*`**). **Apple’s Virtualization.framework** targets **macOS as a guest on Apple hardware** for developer/VM scenarios — it does **not** replace Linux Docker with “macOS in a container on Linux” for arbitrary CI hosts.

---

## Related reading

- OpenLV executive summary (EXE, stdio, RTE): [labview-exe-stdio-and-runtime.md](labview-exe-stdio-and-runtime.md)
