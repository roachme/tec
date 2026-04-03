import os
import common
import subprocess
from behave import given, when, then

# TestEnvCat1 - Show environment properties
@given("TestEnvCat1 Environment '{env}' exists")
def step_envcat1_setup(context, env):
    subprocess.run(common.cmd + ['env', 'add', env])

@when("TestEnvCat1 I run \"env cat {env}\"")
def step_envcat1_run(context, env):
    res = subprocess.run(
        common.cmd + ['env', 'cat', env],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestEnvCat1 Return code should be {expected:d}")
def step_envcat1_check(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"

# TestEnvCat2 - Show nonexistent environment
@when("TestEnvCat2 I run \"env cat {env}\"")
def step_envcat2_run(context, env):
    res = subprocess.run(
        common.cmd + ['env', 'cat', env],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestEnvCat2 Return code should not be {expected:d}")
def step_envcat2_check(context, expected):
    assert context.returncode != expected, \
        f"Expected return code to not be {expected}, but got {context.returncode}"

# TestEnvCat3 - Show environment with desk
@given("TestEnvCat3 Environment '{env}' with desk '{desk}' exists")
def step_envcat3_setup(context, env, desk):
    subprocess.run(common.cmd + ['env', 'add', env])
    subprocess.run(common.cmd + ['desk', 'add', '-e', env, desk])

@when("TestEnvCat3 I run \"env cat -d {desk} {env}\"")
def step_envcat3_run(context, desk, env):
    res = subprocess.run(
        common.cmd + ['env', 'cat', '-d', desk, env],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestEnvCat3 Return code should be {expected:d}")
def step_envcat3_check(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"
