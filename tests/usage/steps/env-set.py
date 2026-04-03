import os
import common
import subprocess
from behave import given, when, then

# TestEnvSet1 - Set environment description
@given("TestEnvSet1 Environment '{env}' exists")
def step_envset1_setup(context, env):
    subprocess.run(common.cmd + ['env', 'add', env])

@when("TestEnvSet1 I run \"env set -D '{desc}' {env}\"")
def step_envset1_run(context, desc, env):
    res = subprocess.run(
        common.cmd + ['env', 'set', '-D', desc, env],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestEnvSet1 Return code should be {expected:d}")
def step_envset1_check(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"

# TestEnvSet2 - Set description for nonexistent environment
@when("TestEnvSet2 I run \"env set -D '{desc}' {env}\"")
def step_envset2_run(context, desc, env):
    res = subprocess.run(
        common.cmd + ['env', 'set', '-D', desc, env],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestEnvSet2 Return code should not be {expected:d}")
def step_envset2_check(context, expected):
    assert context.returncode != expected, \
        f"Expected return code to not be {expected}, but got {context.returncode}"

# TestEnvSet3 - Set environment description with desk option
@given("TestEnvSet3 Environment '{env}' with desk '{desk}' exists")
def step_envset3_setup(context, env, desk):
    subprocess.run(common.cmd + ['env', 'add', env])
    subprocess.run(common.cmd + ['desk', 'add', '-e', env, desk])

@when("TestEnvSet3 I run \"env set -D '{desc}' -d {desk} {env}\"")
def step_envset3_run(context, desc, desk, env):
    res = subprocess.run(
        common.cmd + ['env', 'set', '-D', desc, '-d', desk, env],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestEnvSet3 Return code should be {expected:d}")
def step_envset3_check(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"
