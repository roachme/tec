import os
import common
import subprocess
from behave import given, when, then

# TestDeskMv1 - Rename desk
@given("TestDeskMv1 Desk '{desk}' exists")
def step_deskmv1_setup(context, desk):
    subprocess.run(common.cmd + ['desk', 'add', desk])

@when("TestDeskMv1 I run \"desk mv {olddesk} {newdesk}\"")
def step_deskmv1_run(context, olddesk, newdesk):
    res = subprocess.run(
        common.cmd + ['desk', 'mv', olddesk, newdesk],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestDeskMv1 Return code should be {expected:d}")
def step_deskmv1_check(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"

# TestDeskMv2 - Rename desk in specific environment
@given("TestDeskMv2 Environment '{env}' with desk '{desk}' exists")
def step_deskmv2_setup(context, env, desk):
    subprocess.run(common.cmd + ['env', 'add', env])
    subprocess.run(common.cmd + ['desk', 'add', '-e', env, desk])

@when("TestDeskMv2 I run \"desk mv -e {env} {olddesk} {newdesk}\"")
def step_deskmv2_run(context, env, olddesk, newdesk):
    res = subprocess.run(
        common.cmd + ['desk', 'mv', '-e', env, olddesk, newdesk],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestDeskMv2 Return code should be {expected:d}")
def step_deskmv2_check(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"

# TestDeskMv3 - Rename nonexistent desk
@when("TestDeskMv3 I run \"desk mv {olddesk} {newdesk}\"")
def step_deskmv3_run(context, olddesk, newdesk):
    res = subprocess.run(
        common.cmd + ['desk', 'mv', olddesk, newdesk],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestDeskMv3 Return code should not be {expected:d}")
def step_deskmv3_check(context, expected):
    assert context.returncode != expected, \
        f"Expected return code to not be {expected}, but got {context.returncode}"

# TestDeskMv4 - Rename to existing desk name
@given("TestDeskMv4 Desks '{desk1}' and '{desk2}' exist")
def step_deskmv4_setup(context, desk1, desk2):
    subprocess.run(common.cmd + ['desk', 'add', desk1])
    subprocess.run(common.cmd + ['desk', 'add', desk2])

@when("TestDeskMv4 I run \"desk mv {olddesk} {newdesk}\"")
def step_deskmv4_run(context, olddesk, newdesk):
    res = subprocess.run(
        common.cmd + ['desk', 'mv', olddesk, newdesk],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestDeskMv4 Return code should not be {expected:d}")
def step_deskmv4_check(context, expected):
    assert context.returncode != expected, \
        f"Expected return code to not be {expected}, but got {context.returncode}"
