import os
import common
import subprocess
from behave import given, when, then

# TestDeskCat1 - Show desk properties
@given("TestDeskCat1 Desk '{desk}' exists")
def step_deskcat1_setup(context, desk):
    subprocess.run(common.cmd + ['desk', 'add', desk])

@when("TestDeskCat1 I run \"desk cat {desk}\"")
def step_deskcat1_run(context, desk):
    res = subprocess.run(
        common.cmd + ['desk', 'cat', desk],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestDeskCat1 Return code should be {expected:d}")
def step_deskcat1_check(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"

# TestDeskCat2 - Show nonexistent desk properties
@when("TestDeskCat2 I run \"desk cat {desk}\"")
def step_deskcat2_run(context, desk):
    res = subprocess.run(
        common.cmd + ['desk', 'cat', desk],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestDeskCat2 Return code should not be {expected:d}")
def step_deskcat2_check(context, expected):
    assert context.returncode != expected, \
        f"Expected return code to not be {expected}, but got {context.returncode}"

# TestDeskCat3 - Show desk properties in specific environment
@given("TestDeskCat3 Environment '{env}' with desk '{desk}' exists")
def step_deskcat3_setup(context, env, desk):
    subprocess.run(common.cmd + ['env', 'add', env])
    subprocess.run(common.cmd + ['desk', 'add', '-e', env, desk])

@when("TestDeskCat3 I run \"desk cat {desk}\"")
def step_deskcat3_run(context, desk):
    res = subprocess.run(
        common.cmd + ['desk', 'cat', desk],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestDeskCat3 Return code should be {expected:d}")
def step_deskcat3_check(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"
