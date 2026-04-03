import os
import common
import subprocess
from behave import given, when, then

# TestEnvRename1 - Rename environment
@given("TestEnvRename1 Environment '{env}' exists")
def step_envrename1_setup(context, env):
    subprocess.run(common.cmd + ['env', 'add', env])

@when("TestEnvRename1 I run \"env rename {oldenv} {newenv}\"")
def step_envrename1_run(context, oldenv, newenv):
    res = subprocess.run(
        common.cmd + ['env', 'rename', oldenv, newenv],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestEnvRename1 Return code should be {expected:d}")
def step_envrename1_check(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"

# TestEnvRename2 - Rename nonexistent environment
@when("TestEnvRename2 I run \"env rename {oldenv} {newenv}\"")
def step_envrename2_run(context, oldenv, newenv):
    res = subprocess.run(
        common.cmd + ['env', 'rename', oldenv, newenv],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestEnvRename2 Return code should not be {expected:d}")
def step_envrename2_check(context, expected):
    assert context.returncode != expected, \
        f"Expected return code to not be {expected}, but got {context.returncode}"

