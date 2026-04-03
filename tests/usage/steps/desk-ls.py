import os
import common
import subprocess
from behave import given, when, then

# TestDeskLs1 - List all desks
@given("TestDeskLs1 Desk '{desk}' exists")
def step_deskls1_setup(context, desk):
    subprocess.run(common.cmd + ['desk', 'add', desk])

@when("TestDeskLs1 I run \"desk ls\"")
def step_deskls1_run(context):
    res = subprocess.run(
        common.cmd + ['desk', 'ls'],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestDeskLs1 Return code should be {expected:d}")
def step_deskls1_check_returncode(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"

@then("TestDeskLs1 Output should contain \"{text}\"")
def step_deskls1_check_output(context, text):
    assert text in context.stdout, \
        f"Expected output to contain '{text}', but got '{context.stdout}'"

# TestDeskLs2 - List multiple desks
@given("TestDeskLs2 Desks '{desk1}' and '{desk2}' exist")
def step_deskls2_setup(context, desk1, desk2):
    subprocess.run(common.cmd + ['desk', 'add', desk1])
    subprocess.run(common.cmd + ['desk', 'add', desk2])

@when("TestDeskLs2 I run \"desk ls\"")
def step_deskls2_run(context):
    res = subprocess.run(
        common.cmd + ['desk', 'ls'],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestDeskLs2 Return code should be {expected:d}")
def step_deskls2_check_returncode(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"

@then("TestDeskLs2 Output should contain \"{text}\"")
def step_deskls2_check_output(context, text):
    assert text in context.stdout, \
        f"Expected output to contain '{text}', but got '{context.stdout}'"

# TestDeskLs3 - List when no desks exist
@when("TestDeskLs3 I run \"desk ls\"")
def step_deskls3_run(context):
    res = subprocess.run(
        common.cmd + ['desk', 'ls'],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestDeskLs3 Return code should be {expected:d}")
def step_deskls3_check_returncode(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"
