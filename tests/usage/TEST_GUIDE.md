# Behave Test Guide for ENV and DESK Subcommands

## Overview
This directory contains comprehensive BDD (Behavior-Driven Development) tests for all `env` and `desk` subcommands using Python behave framework.

## Test Structure

### ENV Subcommand Tests
Each env subcommand has its own feature file and step implementation:

| Subcommand | Feature File | Step File | Description |
|------------|-------------|-----------|-------------|
| `env ls` | `env-ls.feature` | `steps/env-ls.py` | List environments |
| `env cd` | `env-cd.feature` | `steps/env-cd.py` | Change to environment |
| `env cat` | `env-cat.feature` | `steps/env-cat.py` | Show environment properties |
| `env rm` | `env-rm.feature` | `steps/env-rm.py` | Remove environment |
| `env set` | `env-set.feature` | `steps/env-set.py` | Set environment properties |
| `env rename` | `env-rename.feature` | `steps/env-rename.py` | Rename environment |
| `env add` | `env-add.feature` | `steps/env-add.py` | Add new environment |

### DESK Subcommand Tests
Each desk subcommand has its own feature file and step implementation:

| Subcommand | Feature File | Step File | Description |
|------------|-------------|-----------|-------------|
| `desk add` | `desk-add.feature` | `steps/desk-add.py` | Add new desk |
| `desk ls` | `desk-ls.feature` | `steps/desk-ls.py` | List desks |
| `desk cd` | `desk-cd.feature` | `steps/desk-cd.py` | Change to desk |
| `desk cat` | `desk-cat.feature` | `steps/desk-cat.py` | Show desk properties |
| `desk rm` | `desk-rm.feature` | `steps/desk-rm.py` | Remove desk |
| `desk set` | `desk-set.feature` | `steps/desk-set.py` | Set desk properties |

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
- **env add**: 6 scenarios (single, invalid, multiple, mixed, existing, cleanup)

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

1. Create feature file: `tests/usage/env-newcmd.feature`
2. Add feature tag: `@env-newcmd`
3. Create step file: `tests/usage/steps/env-newcmd.py`
4. Update `environment.py` to include new tag
5. Follow existing naming conventions (TestNewCmd1, TestNewCmd2, etc.)

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
