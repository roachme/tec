import os
import common
import subprocess
from behave import given, when, then

# TestEnvLs1 - List all environments
@given("TestEnvLs1 Environment '{env}' exists")
def step_envls1_setup(context, env):
    subprocess.run(common.cmd + ['env', 'add', env])

@when("TestEnvLs1 I run \"env ls\"")
def step_envls1_run(context):
    res = subprocess.run(
        common.cmd + ['env', 'ls'],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestEnvLs1 Return code should be {expected:d}")
def step_envls1_check_returncode(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"

@then("TestEnvLs1 Output should contain \"{text}\"")
def step_envls1_check_output(context, text):
    assert text in context.stdout, \
        f"Expected output to contain '{text}', but got '{context.stdout}'"

# TestEnvLs2 - List multiple environments
@given("TestEnvLs2 Environments '{env1}' and '{env2}' exist")
def step_envls2_setup(context, env1, env2):
    subprocess.run(common.cmd + ['env', 'add', env1])
    subprocess.run(common.cmd + ['env', 'add', env2])

@when("TestEnvLs2 I run \"env ls\"")
def step_envls2_run(context):
    res = subprocess.run(
        common.cmd + ['env', 'ls'],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestEnvLs2 Return code should be {expected:d}")
def step_envls2_check_returncode(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"

@then("TestEnvLs2 Output should contain \"{text}\"")
def step_envls2_check_output(context, text):
    assert text in context.stdout, \
        f"Expected output to contain '{text}', but got '{context.stdout}'"

# TestEnvLs3 - List when no custom environments exist
@when("TestEnvLs3 I run \"env ls\"")
def step_envls3_run(context):
    res = subprocess.run(
        common.cmd + ['env', 'ls'],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestEnvLs3 Return code should be {expected:d}")
def step_envls3_check_returncode(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"
