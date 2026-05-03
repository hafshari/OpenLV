# OpenLV — guidance for AI assistants

## Git branches

Work tied to a GitHub issue should use:

`feature/issue-<issue-number>-<short-kebab-description>`

Example: `feature/issue-2-git-lfs-labview-hygiene`.

Bugfixes tied to an issue may use `fix/issue-<number>-<description>`. Full convention: `.cursor/rules/git-branch-conventions.mdc`.

## Why not a Cursor “skill”?

Repo-specific workflow lives here (rules + this file). Global Cursor skills under `~/.cursor/skills-cursor/` are for reusable patterns across projects, not this repository’s branch naming.
