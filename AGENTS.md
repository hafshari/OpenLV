# OpenLV — guidance for AI assistants

## Git branches

Work tied to a GitHub issue should use:

`feature/issue-<issue-number>-<short-kebab-description>`

Example: `feature/issue-2-git-lfs-labview-hygiene`.

Bugfixes tied to an issue may use `fix/issue-<number>-<description>`. Full convention: `.cursor/rules/git-branch-conventions.mdc`.

## Research (LabVIEW / NI / OS behavior)

For topics that vary by platform (runtime, EXE/console, deployment, paths): **investigate and document Windows, Linux, and macOS** — see `.cursor/rules/cross-platform-investigation.mdc`. Do not infer one OS from another without checking.

## Why not a Cursor “skill”?

Repo-specific workflow lives here (rules + this file). Global Cursor skills under `~/.cursor/skills-cursor/` are for reusable patterns across projects, not this repository’s branch naming.
