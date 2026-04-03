import os
import common
import subprocess
from behave import given, when, then

# TestDeskSet1 - Set desk description
@given("TestDeskSet1 Desk '{desk}' exists")
def step_deskset1_setup(context, desk):
    subprocess.run(common.cmd + ['desk', 'add', desk])

@when("TestDeskSet1 I run \"desk set -D '{desc}' {desk}\"")
def step_deskset1_run(context, desc, desk):
    res = subprocess.run(
        common.cmd + ['desk', 'set', '-D', desc, desk],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestDeskSet1 Return code should be {expected:d}")
def step_deskset1_check(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"

# TestDeskSet2 - Set desk description for nonexistent desk
@when("TestDeskSet2 I run \"desk set -D '{desc}' {desk}\"")
def step_deskset2_run(context, desc, desk):
    res = subprocess.run(
        common.cmd + ['desk', 'set', '-D', desc, desk],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestDeskSet2 Return code should not be {expected:d}")
def step_deskset2_check(context, expected):
    assert context.returncode != expected, \
        f"Expected return code to not be {expected}, but got {context.returncode}"

# TestDeskSet3 - Set desk description in specific environment
@given("TestDeskSet3 Environment '{env}' with desk '{desk}' exists")
def step_deskset3_setup(context, env, desk):
    subprocess.run(common.cmd + ['env', 'add', env])
    subprocess.run(common.cmd + ['desk', 'add', '-e', env, desk])

@when("TestDeskSet3 I run \"desk set -D '{desc}' {desk}\"")
def step_deskset3_run(context, desc, desk):
    res = subprocess.run(
        common.cmd + ['desk', 'set', '-D', desc, desk],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestDeskSet3 Return code should be {expected:d}")
def step_deskset3_check(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"
