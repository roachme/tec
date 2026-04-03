import os
import common
import subprocess
from behave import given, when, then

# TestEnvRm1 - Remove environment with force flag
@given("TestEnvRm1 Environment '{env}' exists")
def step_envrm1_setup(context, env):
    subprocess.run(common.cmd + ['env', 'add', env])

@when("TestEnvRm1 I run \"env rm -f {env}\"")
def step_envrm1_run(context, env):
    res = subprocess.run(
        common.cmd + ['env', 'rm', '-f', env],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestEnvRm1 Return code should be {expected:d}")
def step_envrm1_check(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"

# TestEnvRm2 - Remove with verbose flag
@given("TestEnvRm2 Environment '{env}' exists")
def step_envrm2_setup(context, env):
    subprocess.run(common.cmd + ['env', 'add', env])

@when("TestEnvRm2 I run \"env rm -fv {env}\"")
def step_envrm2_run(context, env):
    res = subprocess.run(
        common.cmd + ['env', 'rm', '-fv', env],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestEnvRm2 Return code should be {expected:d}")
def step_envrm2_check(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"

# TestEnvRm3 - Remove nonexistent environment
@when("TestEnvRm3 I run \"env rm -f {env}\"")
def step_envrm3_run(context, env):
    res = subprocess.run(
        common.cmd + ['env', 'rm', '-f', env],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestEnvRm3 Return code should not be {expected:d}")
def step_envrm3_check(context, expected):
    assert context.returncode != expected, \
        f"Expected return code to not be {expected}, but got {context.returncode}"

# TestEnvRm4 - Remove environment with desk specified
@given("TestEnvRm4 Environment '{env}' with desk '{desk}' exists")
def step_envrm4_setup(context, env, desk):
    subprocess.run(common.cmd + ['env', 'add', env])
    subprocess.run(common.cmd + ['desk', 'add', '-e', env, desk])

@when("TestEnvRm4 I run \"env rm -f -d {desk} {env}\"")
def step_envrm4_run(context, desk, env):
    res = subprocess.run(
        common.cmd + ['env', 'rm', '-f', '-d', desk, env],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestEnvRm4 Return code should be {expected:d}")
def step_envrm4_check(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"
