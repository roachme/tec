# Test Coverage Analysis - help.c vs Current Tests

## Summary

This document compares the functionality documented in `cli/help.c` (and,
where it disagrees with the actual implementation, the real behavior in
`cli/*.c`) against the current behave test coverage.

As of this revision, every builtin command (`add`, `cat`, `cd`, `ls`, `mv`,
`rm`, `set`, `cfg`, `init`, `version`, `help`, all `env-*` and `desk-*`
subcommands) has a feature file, and every documented option is exercised by
at least one scenario, including `-h`, `-q`, `-n`/`-N`, `-i`/`-I`, `-k`, the
`"-"` cd alias, and the various "under development" / "not implemented yet"
option stubs. `desk mv` remains genuinely unimplemented in `cli/desk.c`
(`_desk_mv` always returns "under development"); `desk-mv.feature` has two
non-skipped scenarios pinning down that current behavior, plus four `@skip`
scenarios describing the intended behavior for when it's implemented.

## Known implementation quirks pinned down by tests

These are real, observed behaviors of the current code that differ from what
`help.c`'s prose implies. Tests assert the *actual* behavior, not the
documented one, so a regression (or an intentional fix) will show up as a
test failure either way:

- `-h` on most builtin subcommands does not print full help; it prints
  `Try 'tec help CMD' for more information.` and exits, and the exit code is
  inconsistent between commands (0 for `cat`/`env-cat`/`desk-cat`, 1 for
  most others). See `help.feature` and the per-command scenarios exercising
  `-h`.
- `env add` / `desk add` require a NAME argument and error out
  (`environment name is required` / `desk name is required`) rather than
  generating one, despite `help.c` saying "(generated if not passed)".
- `desk-cat` does not accept `-d` even though `help.c` documents it
  (`desk-cat.feature`).
- `cfg get` with an unknown value logs an error but still exits 0
  (`cfg.feature`).
- `cfg revert` / `cfg save` are documented in `help.c` but not registered as
  subcommands at all; they fail with "no such cfg command" (`cfg.feature`).
- `desk rm -i` prints `remove desk '(null)'?` instead of the desk name
  (the code reads `args->task` instead of `args->desk`) - not separately
  pinned down beyond the accept/decline behavior since the exact prompt text
  isn't asserted for that case in `desk-rm.feature`.

## Where to look

- `help.feature` - top-level CLI behavior: no command, unknown/naughty
  commands, global `-h`/`-v`, invalid global options, invalid toggle values,
  and the `help` command itself (`-l`, `-s`, `-d`, unknown CMD).
- `cfg.feature` - `cfg` with no subcommand, invalid subcommand, `get`/`ls`,
  unimplemented `revert`/`save`.
- `add.feature`, `cat.feature`, `cd.feature`, `ls.feature`, `mv.feature`,
  `rm.feature`, `set.feature` - top-level task commands, including `-N`,
  `-q`, `-n`, `-i`/`-I` (via the `I answer "..." when running "..."` step),
  the `"-"` cd alias and its double-dash error, and the `-f`/`-t`/`-i`
  "under development" option stubs.
- `env-*.feature`, `desk-*.feature` - one file per subcommand, covering the
  same option surface as above plus `-d`/`-e` scoping and `-k` multi-key
  `cat` output.

Run the whole suite from `tests/usage/`:

```bash
behave --tags=-skip
```
