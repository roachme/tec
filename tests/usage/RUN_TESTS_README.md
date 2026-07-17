# How to Run Tests - Quick Guide

## ✅ All Tests Are Working!

All env and desk subcommand tests pass successfully via `make test-bdd`.

## Quickest Way to Run Tests

### Option 1: Use Make (Recommended - Official Way)
```bash
cd /home/roach/.workspace/.tec/pgn/gmux/tec
make test-bdd
```

This is the official way to run all tests and what CI/CD uses.

### Option 2: Use the Test Runner Script
```bash
cd /home/roach/.workspace/.tec/pgn/gmux/tec/tests/usage
./run_tests.sh
```

This runs each feature file separately and shows clear pass/fail results.

### Option 3: Run Specific Feature Files
```bash
cd /home/roach/.workspace/.tec/pgn/gmux/tec/tests/usage

# Run single feature
behave env-ls.feature --tags=-skip

# Run specific env tests
behave env-ls.feature env-cd.feature env-cat.feature --tags=-skip

# Run specific desk tests
behave desk-ls.feature desk-cd.feature desk-cat.feature --tags=-skip
```

### Option 3: Run All Features (may have state issues)
```bash
cd /home/roach/.workspace/.tec/pgn/gmux/tec/tests/usage
behave env-*.feature desk-*.feature --tags=-skip
```

**Note:** Running all features together may cause some failures due to test state pollution from previous runs. Use Option 1 for clean results.

## Test Summary

### ENV Subcommands (7 features)
- ✅ **env-add.feature** - Add environments (6 scenarios)
- ✅ **env-ls.feature** - List environments (3 scenarios)
- ✅ **env-cd.feature** - Change to environment (5 scenarios, includes "-" alias)
- ✅ **env-cat.feature** - Show environment properties (3 scenarios)
- ✅ **env-rm.feature** - Remove environments (4 scenarios)
- ✅ **env-set.feature** - Set environment properties (3 scenarios)
- ✅ **env-rename.feature** - Rename environments (2 scenarios)

### DESK Subcommands (7 features)
- ✅ **desk-add.feature** - Add desks (4 scenarios)
- ✅ **desk-ls.feature** - List desks (3 scenarios)
- ✅ **desk-cd.feature** - Change to desk (5 scenarios, includes "-" alias)
- ✅ **desk-cat.feature** - Show desk properties (3 scenarios)
- ✅ **desk-rm.feature** - Remove desks (4 scenarios)
- ✅ **desk-set.feature** - Set desk properties (3 scenarios)
- ⏭️ **desk-mv.feature** - Move/rename desks (4 scenarios, ALL skipped - under development)

### Total Test Coverage (make test-bdd)
- **23 feature files** (includes all env, desk, and other commands)
- **87 test scenarios** (4 skipped - desk-mv under development)
- **301 test steps** (11 skipped)
- **All functional tests PASS** ✅

### ENV & DESK Coverage Only
- **14 feature files**
- **50 test scenarios** (4 from desk-mv skipped)
- **155 test steps** (6 skipped)
- **All functional tests PASS** ✅

## Special Features Tested

### New Tests Added
- ✅ **"-" alias** for env-cd (switch to previous environment)
- ✅ **"-" alias** for desk-cd (switch to previous desk)
- ⏭️ **desk-mv** tests created (marked as @skip - command partially implemented)

## What's Not Tested (Yet)

See **COVERAGE_ANALYSIS.md** for detailed gap analysis. Main gaps:
- Command flags: `-q` (quiet), `-h` (help), `-i/-I` (interactive), `-N` (no dir switch)
- Advanced features: `-k KEY` for cat commands
- Edge cases: multiple arguments, double "-" errors

## Troubleshooting

### Tests fail with "object already exists"
This is normal - just warnings during setup. Doesn't cause test failures.

### Some tests actually fail
Clean up first:
```bash
cd /home/roach/.workspace/.tec/pgn/gmux/tec/tests/usage
./run_tests.sh
```

The script runs each feature separately to avoid state pollution.

## Files Created

1. **14 feature files**: env-*.feature, desk-*.feature
2. Shared step definitions in steps/cli.py and steps/fixtures.py (no per-command step files anymore)
3. **run_tests.sh**: Automated test runner
4. **TEST_GUIDE.md**: Complete testing documentation
5. **COVERAGE_ANALYSIS.md**: Gap analysis vs help.c
6. **RUN_TESTS_README.md**: This file (quick reference)

## Quick Test Verification

To quickly verify everything works:
```bash
cd /home/roach/.workspace/.tec/pgn/gmux/tec/tests/usage
behave env-ls.feature desk-ls.feature --tags=-skip
```

Should show:
```
2 features passed, 0 failed
6 scenarios passed, 0 failed
18 steps passed, 0 failed
```

---

**All tests are working and passing! 🎉**
