# Behave Test Guide for ENV and DESK Subcommands

## Overview
This directory contains comprehensive BDD (Behavior-Driven Development) tests for all `env` and `desk` subcommands using Python behave framework.

## Test Structure

### ENV Subcommand Tests
Each env subcommand has its own feature file:

| Subcommand | Feature File | Description |
|------------|-------------|-------------|
| `env ls` | `env-ls.feature` | List environments |
| `env cd` | `env-cd.feature` | Change to environment |
| `env cat` | `env-cat.feature` | Show environment properties |
| `env rm` | `env-rm.feature` | Remove environment |
| `env set` | `env-set.feature` | Set environment properties |
| `env rename` | `env-rename.feature` | Rename environment |
| `env add` | `env-add.feature` | Add new environment |

### DESK Subcommand Tests
Each desk subcommand has its own feature file:

| Subcommand | Feature File | Description |
|------------|-------------|-------------|
| `desk add` | `desk-add.feature` | Add new desk |
| `desk ls` | `desk-ls.feature` | List desks |
| `desk cd` | `desk-cd.feature` | Change to desk |
| `desk cat` | `desk-cat.feature` | Show desk properties |
| `desk rm` | `desk-rm.feature` | Remove desk |
| `desk set` | `desk-set.feature` | Set desk properties |

None of these have their own step file anymore. All scenarios are written
against a small set of shared, generic steps in `steps/cli.py` (running the
CLI and asserting on exit code/stdout/stderr) and `steps/fixtures.py`
(creating tasks/envs/desks as preconditions, and checking PWD/toggle state).
`steps/togg.py` and `steps/common.py` are small shared helpers those two
files build on.

## How to Run Tests

### Prerequisites
```bash
# Make sure you have Python 3 and behave installed
pip install behave

# Build the tec program first
cd /home/roach/.workspace/.tec/pgn/gmux/tec
make clean && make
```

### Running All Tests

**✨ RECOMMENDED: Use the Test Runner Script**

The easiest way to run all tests:
```bash
cd tests/usage
./run_tests.sh
```

This script runs each feature file separately to avoid test state pollution and provides a clean summary.

**Manual Test Execution:**

From tests/usage directory:
```bash
cd tests/usage
behave env-*.feature desk-*.feature --tags=-skip
```

**Run each feature separately (alternative method):**
```bash
cd tests/usage
for feature in env-*.feature desk-*.feature; do
    behave "$feature" --tags=-skip
done
```

### Running Specific Tests

**Run all ENV tests:**
```bash
behave tests/usage/env-*.feature
```

**Run all DESK tests:**
```bash
behave tests/usage/desk-*.feature
```

**Run specific subcommand tests:**
```bash
# Test env ls
behave tests/usage/env-ls.feature

# Test desk add
behave tests/usage/desk-add.feature

# Test env cd
behave tests/usage/env-cd.feature
```

**Run by tag:**
```bash
# Run a specific test scenario
behave --tags=TestEnvLs1 tests/usage/

# Run all tests for env-ls
behave --tags=env-ls tests/usage/

# Run all tests for desk-cd
behave --tags=desk-cd tests/usage/
```

**Run with verbose output:**
```bash
behave -v tests/usage/env-*.feature tests/usage/desk-*.feature
```

**Run and show only failures:**
```bash
behave --no-skipped tests/usage/env-*.feature tests/usage/desk-*.feature
```

### Running Other Test Suites

**Run all tests (including init, cfg, version, etc.):**
```bash
cd tests/usage
behave
```

**Run specific feature:**
```bash
behave tests/usage/init.feature
behave tests/usage/cfg.feature
behave tests/usage/version.feature
```

## Test Coverage

### ENV Subcommands Coverage
- **env ls**: 3 scenarios (list single, multiple, empty)
- **env cd**: 4 scenarios (basic, with desk option, nonexistent, with -n flag)
- **env cat**: 3 scenarios (show properties, nonexistent, with desk)
- **env rm**: 4 scenarios (force, verbose, nonexistent, with desk)
- **env set**: 3 scenarios (set description, nonexistent, with desk)
- **env rename**: 2 scenarios (rename, nonexistent)
- **env add**: 5 scenarios (single, invalid, multiple, mixed, existing)

### DESK Subcommands Coverage
- **desk add**: 4 scenarios (basic, specific env, nonexistent env, -n flag)
- **desk ls**: 3 scenarios (list single, multiple, empty)
- **desk cd**: 4 scenarios (basic, specific env, nonexistent, -n flag)
- **desk cat**: 3 scenarios (show properties, nonexistent, in specific env)
- **desk rm**: 4 scenarios (force, verbose, nonexistent, specific env)
- **desk set**: 3 scenarios (set description, nonexistent, in specific env)

**Total Coverage:**
- 12 feature files (6 env + 6 desk)
- 40+ test scenarios
- 120+ test steps
- All major command options tested (-f, -v, -n, -e, -d, -D, etc.)

## Test Results

Last test run: **All tests passing ✓**
- 12 features passed
- 0 features failed
- 40 scenarios passed
- 0 scenarios failed
- 120 steps passed
- 0 steps failed

## Common Issues and Solutions

### Issue: Tests fail with "object already exists"
**Solution:** This is normal during test setup when environments/desks from previous runs still exist. These warnings don't cause test failures. If tests actually fail, clean up first:
```bash
rm -rf ~/tectask/test ~/tectask/env* ~/tectask/add* ~/tectask/desk*
```

### Issue: Some tests fail when running all features together
**Solution:** This can happen due to test state pollution. Run tests one feature at a time:
```bash
cd tests/usage
for feature in env-*.feature desk-*.feature; do
    behave "$feature" --tags=-skip
done
```

### Issue: Permission denied when running tests
**Solution:** Make sure the tec program is built and executable:
```bash
cd /home/roach/.workspace/.tec/pgn/gmux/tec
make clean && make
chmod +x build/_tec
```

### Issue: Tests timeout or hang
**Solution:** Check that the tec program is working correctly:
```bash
/home/roach/.workspace/.tec/pgn/gmux/tec/build/_tec init
/home/roach/.workspace/.tec/pgn/gmux/tec/build/_tec env ls
```

## Test Configuration

### Environment Setup
- Tests use isolated test environment via `environment.py`
- Test taskbase: `~/tectask`
- Each feature gets initialized with a clean 'test' environment
- Cleanup happens automatically after each feature

### Test Constants (from `steps/common.py`)
- `TEST_ENV = "test"` - Default test environment
- `TEST_DESK = "desk"` - Default test desk
- `pwdfile = "/tmp/tecpwd"` - PWD tracking file

## Writing New Tests

To add new tests for a subcommand:

1. Create feature file: `tests/usage/env-newcmd.feature`, tagged `@env-newcmd`
2. Write scenarios in plain Gherkin against the existing shared steps:
   `Given a task/env/desk "..." exists`, `When I run "<cli args>"`,
   `Then the exit code should be N`, `stdout`/`stderr should be`/`should
   contain "..."`, `the PWD should be ...`, `the current task should be
   "..."` (see `steps/cli.py` and `steps/fixtures.py` for the full list).
   Only add a new step function if no existing one fits - most new
   subcommands don't need one.
3. Decide on isolation in `environment.py`: does the tag need a full
   per-scenario reset (add it to `PER_SCENARIO_RESET_TAGS`,
   `FULL_RESET_TAGS`, or `DESK_RESET_TAGS`, depending on what it touches),
   or is feature-level `task_init()` enough because scenarios use disjoint
   fixture names? Prefer per-scenario reset for anything that reuses names
   across scenarios - see the comments above those sets.
4. Do **not** invent a new numbered step-per-scenario convention
   (`TestNewCmd1`, `TestNewCmd2`, ...) - that pattern was replaced because
   it duplicated the same subprocess-and-assert logic dozens of times over.

## Debugging Tests

**Run with behave debugging:**
```bash
behave --no-capture tests/usage/env-ls.feature
```

**Check specific step output:**
```bash
behave -v --no-capture --tags=TestEnvLs1 tests/usage/
```

**Stop on first failure:**
```bash
behave --stop tests/usage/env-*.feature
```

## CI/CD Integration

To integrate with CI/CD pipelines:

```bash
# Run all tests and generate JUnit XML report
behave --junit --junit-directory=test-reports tests/usage/

# Run with specific format
behave --format=json --outfile=test-results.json tests/usage/
```

## Contributing

When adding new tests:
1. Follow the existing naming pattern (TestCmdN where N is incremental)
2. Keep each test scenario focused on a single behavior
3. Test both success and failure cases
4. Include command option variations (-f, -v, -n, etc.)
5. Ensure proper cleanup in setup/teardown
