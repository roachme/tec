import os
import common
import subprocess
from behave import given, when, then

# TestDeskAdd1 - Add new desk
@given("TestDeskAdd1 Environment exists")
def step_deskadd1_setup(context):
    pass  # Environment already initialized in before_feature

@when("TestDeskAdd1 I run \"desk add {desk}\"")
def step_deskadd1_run(context, desk):
    res = subprocess.run(
        common.cmd + ['desk', 'add', desk],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestDeskAdd1 Return code should be {expected:d}")
def step_deskadd1_check(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"

# TestDeskAdd2 - Add desk to specific environment
@given("TestDeskAdd2 Environment '{env}' exists")
def step_deskadd2_setup(context, env):
    subprocess.run(common.cmd + ['env', 'add', env])

@when("TestDeskAdd2 I run \"desk add -e {env} {desk}\"")
def step_deskadd2_run(context, env, desk):
    res = subprocess.run(
        common.cmd + ['desk', 'add', '-e', env, desk],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestDeskAdd2 Return code should be {expected:d}")
def step_deskadd2_check(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"

# TestDeskAdd3 - Add desk to nonexistent environment
@when("TestDeskAdd3 I run \"desk add -e {env} {desk}\"")
def step_deskadd3_run(context, env, desk):
    res = subprocess.run(
        common.cmd + ['desk', 'add', '-e', env, desk],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestDeskAdd3 Return code should not be {expected:d}")
def step_deskadd3_check(context, expected):
    assert context.returncode != expected, \
        f"Expected return code to not be {expected}, but got {context.returncode}"

# TestDeskAdd4 - Add desk with -n flag
@given("TestDeskAdd4 Environment exists")
def step_deskadd4_setup(context):
    pass  # Environment already initialized in before_feature

@when("TestDeskAdd4 I run \"desk add -n {desk}\"")
def step_deskadd4_run(context, desk):
    res = subprocess.run(
        common.cmd + ['desk', 'add', '-n', desk],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestDeskAdd4 Return code should be {expected:d}")
def step_deskadd4_check(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"
