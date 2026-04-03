import os
import re
import common
import subprocess
from behave import given, when, then

@given("TestVersion1 No preconditions")
def step_version1_setup(context):
    pass

@when("TestVersion1 I run \"version\"")
def step_version1_run(context):
    res = subprocess.run(
        common.cmd + ['version'],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestVersion1 Return code should be {expected:d}")
def step_version1_check_returncode(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"

@then("TestVersion1 Output should contain \"{text}\"")
def step_version1_check_output(context, text):
    assert text in context.stdout, \
        f"Expected output to contain '{text}', but got '{context.stdout}'"

@given("TestVersion2 No preconditions")
def step_version2_setup(context):
    pass

@when("TestVersion2 I run \"version\"")
def step_version2_run(context):
    res = subprocess.run(
        common.cmd + ['version'],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestVersion2 Return code should be {expected:d}")
def step_version2_check_returncode(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"

@then("TestVersion2 Output should match version pattern")
def step_version2_check_pattern(context):
    # Check for pattern like "tec version vX.X.X" or "tec v0.10.0"
    pattern = r'tec(\s+version)?\s+v\d+\.\d+\.\d+'
    assert re.search(pattern, context.stdout), \
        f"Expected version pattern 'tec [version] vX.X.X', but got '{context.stdout}'"
