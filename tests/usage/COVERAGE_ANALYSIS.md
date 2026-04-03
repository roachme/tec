# Test Coverage Analysis - help.c vs Current Tests

## Summary

This document compares the functionality documented in `cli/help.c` with the current behave test coverage.

## ENV Subcommands Coverage

### env-add ✓ COVERED
**Documented in help.c (lines 518-534):**
- Options: `-d DESK`, `-h`, `-n`, `-q`, `-N`
- Features: Multiple names, generated names

**Current Test Coverage:**
- ✓ Basic add (env-add.feature)
- ✓ Add single valid environment
- ✓ Add single invalid environment
- ✓ Add multiple environments
- ✓ Add existing environment (error case)
- ✓ Mixed valid/invalid environments

**Missing Tests:**
- ⚠ `-d` option (specify desk name when adding env)
- ⚠ `-n` option (no toggle)
- ⚠ `-q` option (quiet mode)
- ⚠ `-N` option (no switch to directory)

---

### env-ls ⚠ PARTIALLY COVERED
**Documented in help.c (lines 579-590):**
- Options: `-h`, `-q`, `-v`

**Current Test Coverage:**
- ✓ List all environments (env-ls.feature)
- ✓ List multiple environments
- ✓ List when empty

**Missing Tests:**
- ✗ `-q` option (quiet mode)
- ✗ `-v` option (verbose - under development)
- ✗ `-h` option (help)

---

### env-cd ⚠ PARTIALLY COVERED
**Documented in help.c (lines 554-576):**
- Options: `-d DESK`, `-h`, `-n`, `-q`, `-N`
- Special: `"-"` alias for previous environment

**Current Test Coverage:**
- ✓ Basic cd to environment (env-cd.feature)
- ✓ With `-d` desk option
- ✓ With `-n` flag (no toggle)
- ✓ Nonexistent environment (error)

**Missing Tests:**
- ✗ `"-"` alias (switch to previous environment)
- ✗ `-N` option (no switch to directory)
- ✗ `-q` option (quiet mode)
- ✗ `-h` option (help)
- ✗ Double `"-"` error case

---

### env-cat ⚠ PARTIALLY COVERED
**Documented in help.c (lines 537-551):**
- Options: `-h`, `-q`
- Note: Implementation also has `-d` option

**Current Test Coverage:**
- ✓ Show environment properties (env-cat.feature)
- ✓ Nonexistent environment (error)
- ✓ With `-d` desk option

**Missing Tests:**
- ✗ `-q` option (quiet mode)
- ✗ `-h` option (help)

---

### env-rm ⚠ PARTIALLY COVERED
**Documented in help.c (lines 610-627):**
- Options: `-f`, `-h`, `-i`, `-q`, `-v`, `-I`

**Current Test Coverage:**
- ✓ Remove with `-f` flag (env-rm.feature)
- ✓ Remove with `-fv` flags
- ✓ Nonexistent environment (error)
- ✓ With `-d` desk option

**Missing Tests:**
- ✗ `-i` option (interactive prompt before each removal)
- ✗ `-I` option (prompt once before removing multiple)
- ✗ `-q` option (quiet mode)
- ✗ `-h` option (help)
- ✗ Testing removal of multiple environments at once

---

### env-set ⚠ PARTIALLY COVERED
**Documented in help.c (lines 630-646):**
- Options: `-d DESK`, `-h`, `-i`, `-q`, `-D`

**Current Test Coverage:**
- ✓ Set description with `-D` (env-set.feature)
- ✓ Nonexistent environment (error)
- ✓ With `-d` desk option

**Missing Tests:**
- ✗ `-i` option (interactive mode - under development)
- ✗ `-q` option (quiet mode)
- ✗ `-h` option (help)

---

### env-rename ⚠ PARTIALLY COVERED
**Documented in help.c (lines 593-607):**
- Options: `-h`, `-q`
- Arguments: SRC DST

**Current Test Coverage:**
- ✓ Basic rename (env-rename.feature)
- ✓ Nonexistent environment (error)

**Missing Tests:**
- ✗ `-q` option (quiet mode)
- ✗ `-h` option (help)
- ✗ Rename to existing name (should fail)

---

## DESK Subcommands Coverage

### desk-add ⚠ PARTIALLY COVERED
**Documented in help.c (lines 354-371):**
- Options: `-e ENV`, `-h`, `-n`, `-q`, `-N`

**Current Test Coverage:**
- ✓ Basic add (desk-add.feature)
- ✓ Add to specific environment with `-e`
- ✓ Add to nonexistent environment (error)
- ✓ With `-n` flag

**Missing Tests:**
- ✗ `-q` option (quiet mode)
- ✗ `-N` option (no switch to directory)
- ✗ `-h` option (help)
- ✗ Generated desk name (no NAME argument)

---

### desk-ls ⚠ PARTIALLY COVERED
**Documented in help.c (lines 374-387):**
- Options: `-h`, `-q`

**Current Test Coverage:**
- ✓ List desks (desk-ls.feature)
- ✓ List multiple desks
- ✓ List when empty

**Missing Tests:**
- ✗ `-q` option (quiet mode)
- ✗ `-h` option (help)
- ✗ List desks for specific ENV argument

---

### desk-cd ⚠ PARTIALLY COVERED
**Documented in help.c (lines 466-489):**
- Options: `-e ENV`, `-h`, `-n`, `-q`, `-N`
- Special: `"-"` alias for previous desk

**Current Test Coverage:**
- ✓ Basic cd to desk (desk-cd.feature)
- ✓ With `-e` environment option
- ✓ With `-n` flag
- ✓ Nonexistent desk (error)

**Missing Tests:**
- ✗ `"-"` alias (switch to previous desk)
- ✗ `-N` option (no switch to directory)
- ✗ `-q` option (quiet mode)
- ✗ `-h` option (help)
- ✗ Double `"-"` error case

---

### desk-cat ⚠ PARTIALLY COVERED
**Documented in help.c (lines 444-463):**
- Options: `-d DESK`, `-e ENV`, `-h`, `-k KEY`, `-q`

**Current Test Coverage:**
- ✓ Show desk properties (desk-cat.feature)
- ✓ Nonexistent desk (error)
- ✓ In specific environment

**Missing Tests:**
- ✗ `-k KEY` option (show specific key)
- ✗ `-d` option (specify desk name)
- ✗ `-q` option (quiet mode)
- ✗ `-h` option (help)
- ✗ Multiple `-k` options

---

### desk-rm ⚠ PARTIALLY COVERED
**Documented in help.c (lines 404-422):**
- Options: `-e ENV`, `-f`, `-h`, `-i`, `-q`, `-v`, `-I`

**Current Test Coverage:**
- ✓ Remove with `-f` flag (desk-rm.feature)
- ✓ Remove with `-fv` flags
- ✓ Nonexistent desk (error)
- ✓ From specific environment

**Missing Tests:**
- ✗ `-i` option (interactive prompt)
- ✗ `-I` option (prompt once)
- ✗ `-q` option (quiet mode)
- ✗ `-h` option (help)
- ✗ Remove multiple desks at once

---

### desk-set ⚠ PARTIALLY COVERED
**Documented in help.c (lines 425-441):**
- Options: `-e ENV`, `-h`, `-q`, `-D`

**Current Test Coverage:**
- ✓ Set description with `-D` (desk-set.feature)
- ✓ Nonexistent desk (error)
- ✓ In specific environment

**Missing Tests:**
- ✗ `-q` option (quiet mode)
- ✗ `-h` option (help)

---

### desk-mv ✗ NOT COVERED AT ALL
**Documented in help.c (lines 390-401):**
- Options: `-e ENV`, `-h`, `-q`
- Arguments: SRC DST

**Current Test Coverage:**
- ✗ No tests exist for desk-mv

**Missing Tests:**
- ✗ Basic move/rename desk
- ✗ With `-e` option
- ✗ Error cases (nonexistent source, existing destination)
- ✗ All command options

---

## Coverage Summary

### Overall Statistics
- **ENV Subcommands:** 6 tested, all have gaps
  - Fully covered: 0/6 (0%)
  - Partially covered: 6/6 (100%)
  - Not covered: 0/6 (0%)

- **DESK Subcommands:** 5/6 tested, 1 missing entirely
  - Fully covered: 0/6 (0%)
  - Partially covered: 5/6 (83%)
  - Not covered: 1/6 (17%) - desk-mv

### Most Critical Missing Tests

#### High Priority (Core Functionality)
1. **desk-mv** - Entire subcommand not tested
2. **"-" alias** - Previous env/desk switching (env-cd, desk-cd)
3. **-k KEY option** - Show specific keys (desk-cat, task cat)
4. **-i/-I options** - Interactive removal prompts (env-rm, desk-rm)

#### Medium Priority (Common Options)
5. **-q option** - Quiet mode across all commands
6. **-N option** - No directory switch (env-add, env-cd, desk-add, desk-cd)
7. **-h option** - Help output for all commands

#### Low Priority (Edge Cases)
8. Multiple arguments (remove multiple envs/desks at once)
9. Double "-" error cases
10. Generated names (env-add, desk-add with no NAME)

### Options Coverage Matrix

| Option | Purpose | Tested In | Missing In |
|--------|---------|-----------|------------|
| `-d` | Desk name | env-cd, env-cat, env-rm, env-set | env-add, desk-cat |
| `-e` | Environment name | desk-add, desk-cd, desk-cat, desk-rm, desk-set | - |
| `-f` | Force (no prompt) | env-rm, desk-rm | - |
| `-h` | Help | - | ALL commands |
| `-i` | Interactive prompt | - | env-rm, desk-rm |
| `-I` | Prompt once | - | env-rm, desk-rm |
| `-k` | Show specific key | - | desk-cat |
| `-n` | No toggle update | env-cd, desk-cd, desk-add | env-add |
| `-N` | No dir switch | - | env-add, env-cd, desk-add, desk-cd |
| `-q` | Quiet mode | - | ALL commands |
| `-v` | Verbose | env-rm, desk-rm | env-ls |
| `-D` | Description | env-set, desk-set | - |
| `"-"` | Previous env/desk | - | env-cd, desk-cd |

## Recommendations

### Phase 1: Critical Coverage (Immediate)
1. Add **desk-mv** tests (complete subcommand)
2. Add **"-" alias** tests for env-cd and desk-cd
3. Add **-k KEY** tests for cat commands

### Phase 2: Common Options (Soon)
4. Add **-q** (quiet) tests for all commands
5. Add **-N** (no directory switch) tests
6. Add **-i/-I** (interactive) tests for rm commands

### Phase 3: Completeness (Later)
7. Add **-h** (help) tests for all commands
8. Add multiple argument tests
9. Add edge case tests
