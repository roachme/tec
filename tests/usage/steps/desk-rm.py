import os
import common
import subprocess
from behave import given, when, then

# TestDeskRm1 - Remove desk with force flag
@given("TestDeskRm1 Desk '{desk}' exists")
def step_deskrm1_setup(context, desk):
    subprocess.run(common.cmd + ['desk', 'add', desk])

@when("TestDeskRm1 I run \"desk rm -f {desk}\"")
def step_deskrm1_run(context, desk):
    res = subprocess.run(
        common.cmd + ['desk', 'rm', '-f', desk],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestDeskRm1 Return code should be {expected:d}")
def step_deskrm1_check(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"

# TestDeskRm2 - Remove desk with verbose flag
@given("TestDeskRm2 Desk '{desk}' exists")
def step_deskrm2_setup(context, desk):
    subprocess.run(common.cmd + ['desk', 'add', desk])

@when("TestDeskRm2 I run \"desk rm -fv {desk}\"")
def step_deskrm2_run(context, desk):
    res = subprocess.run(
        common.cmd + ['desk', 'rm', '-fv', desk],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestDeskRm2 Return code should be {expected:d}")
def step_deskrm2_check(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"

# TestDeskRm3 - Remove nonexistent desk
@when("TestDeskRm3 I run \"desk rm -f {desk}\"")
def step_deskrm3_run(context, desk):
    res = subprocess.run(
        common.cmd + ['desk', 'rm', '-f', desk],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestDeskRm3 Return code should not be {expected:d}")
def step_deskrm3_check(context, expected):
    assert context.returncode != expected, \
        f"Expected return code to not be {expected}, but got {context.returncode}"

# TestDeskRm4 - Remove desk from specific environment
@given("TestDeskRm4 Environment '{env}' with desk '{desk}' exists")
def step_deskrm4_setup(context, env, desk):
    subprocess.run(common.cmd + ['env', 'add', env])
    subprocess.run(common.cmd + ['desk', 'add', '-e', env, desk])

@when("TestDeskRm4 I run \"desk rm -f {desk}\"")
def step_deskrm4_run(context, desk):
    res = subprocess.run(
        common.cmd + ['desk', 'rm', '-f', desk],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestDeskRm4 Return code should be {expected:d}")
def step_deskrm4_check(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"
