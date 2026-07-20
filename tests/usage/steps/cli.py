import re
import shlex
import subprocess
import common
from behave import step, then

# Generic "run the tec CLI and assert on its output" steps, shared by every
# feature file. Keeps each feature's step file free of one-off subprocess.run
# boilerplate per scenario.

# @step (not @when): some scenarios use "I run ..." as a Given/And setup
# action (e.g. cd'ing somewhere before the real When), and behave matches
# Given/When/Then steps by keyword type, not just text.
@step('I run "{args}"')
def step_run(context, args):
    context.result = subprocess.run(
        common.cmd + shlex.split(args),
        capture_output = True,
        text = True,
    )

@step('I run with no arguments')
def step_run_no_args(context):
    context.result = subprocess.run(
        common.cmd,
        capture_output = True,
        text = True,
    )

# For commands with interactive [y/N] prompts (-i/-I on rm/env-rm/desk-rm):
# feeds a line on stdin instead of leaving the child blocked waiting for input.
@step('I answer "{stdin}" when running "{args}"')
def step_run_with_input(context, args, stdin):
    context.result = subprocess.run(
        common.cmd + shlex.split(args),
        input = stdin + "\n",
        capture_output = True,
        text = True,
    )

@then('the exit code should be {code:d}')
def step_exit_code(context, code):
    actual = context.result.returncode
    assert actual == code, \
        f"Expected exit code {code}, but got {actual}\nstderr: {context.result.stderr}"

@then('the exit code should not be {code:d}')
def step_exit_code_not(context, code):
    actual = context.result.returncode
    assert actual != code, \
        f"Expected exit code != {code}, but got {actual}"

@then('stdout should be')
def step_stdout_is(context):
    _assert_multiline(context.text, context.result.stdout)

@then('stderr should be')
def step_stderr_is(context):
    expected = (context.text or "").strip()
    actual = context.result.stderr.strip()
    assert expected == actual, \
        f"Expected stderr:\n'{expected}',\n\nbut got:\n'{actual}'"

@then('stdout should contain "{substr}"')
def step_stdout_contains(context, substr):
    assert substr in context.result.stdout, \
        f"Expected stdout to contain '{substr}', but got:\n'{context.result.stdout}'"

@then('stderr should contain "{substr}"')
def step_stderr_contains(context, substr):
    assert substr in context.result.stderr, \
        f"Expected stderr to contain '{substr}', but got:\n'{context.result.stderr}'"

@then('stdout should not contain "{substr}"')
def step_stdout_not_contains(context, substr):
    assert substr not in context.result.stdout, \
        f"Expected stdout not to contain '{substr}', but got:\n'{context.result.stdout}'"

@then('stdout should match "{pattern}"')
def step_stdout_matches(context, pattern):
    assert re.search(pattern, context.result.stdout), \
        f"Expected stdout to match /{pattern}/, but got:\n'{context.result.stdout}'"

def _assert_multiline(expected_text, actual_text):
    expected = (expected_text or "").strip().split('\n')
    actual = actual_text.strip().split('\n')
    assert len(expected) == len(actual), \
        f"Expected {len(expected)} line(s), got {len(actual)}\n" \
        f"Expected:\n{expected_text.strip()}\n\nActual:\n{actual_text.strip()}"

    # 'date' lines are excluded since the value is generated at runtime.
    diffs = [
        (i, e, a) for i, (e, a) in enumerate(zip(expected, actual), 1)
        if e != a and 'date' not in e
    ]
    assert not diffs, \
        f"Line mismatches {diffs}\n\nExpected:\n{expected_text.strip()}\n\nActual:\n{actual_text.strip()}"
