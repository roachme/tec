import os
import common
import subprocess
from behave import given, when, then

# TestEnvCd1 - Change to environment
@given("TestEnvCd1 Environment '{env}' exists")
def step_envcd1_setup(context, env):
    subprocess.run(common.cmd + ['env', 'add', env])

@when("TestEnvCd1 I run \"env cd {env}\"")
def step_envcd1_run(context, env):
    res = subprocess.run(
        common.cmd + ['env', 'cd', env],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()
    context.env = env

@then("TestEnvCd1 Return code should be {expected:d}")
def step_envcd1_check_returncode(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"

@then("TestEnvCd1 PWD changes to env '{env}'")
def step_envcd1_check_pwd(context, env):
    expected = common.get_expected_path(env)
    common.verify_pwd(expected)

# TestEnvCd2 - Change to environment with desk
@given("TestEnvCd2 Environment '{env}' with desk '{desk}' exists")
def step_envcd2_setup(context, env, desk):
    subprocess.run(common.cmd + ['env', 'add', env])
    subprocess.run(common.cmd + ['desk', 'add', '-e', env, desk])

@when("TestEnvCd2 I run \"env cd -d {desk} {env}\"")
def step_envcd2_run(context, desk, env):
    res = subprocess.run(
        common.cmd + ['env', 'cd', '-d', desk, env],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()
    context.env = env
    context.desk = desk

@then("TestEnvCd2 Return code should be {expected:d}")
def step_envcd2_check_returncode(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"

@then("TestEnvCd2 PWD changes to desk '{desk}' in env '{env}'")
def step_envcd2_check_pwd(context, desk, env):
    expected = common.get_expected_path(env, desk)
    common.verify_pwd(expected)

# TestEnvCd3 - Change to nonexistent environment
@when("TestEnvCd3 I run \"env cd {env}\"")
def step_envcd3_run(context, env):
    res = subprocess.run(
        common.cmd + ['env', 'cd', env],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestEnvCd3 Return code should not be {expected:d}")
def step_envcd3_check(context, expected):
    assert context.returncode != expected, \
        f"Expected return code to not be {expected}, but got {context.returncode}"

# TestEnvCd4 - Change to environment with -n flag
@given("TestEnvCd4 Environment '{env}' exists")
def step_envcd4_setup(context, env):
    subprocess.run(common.cmd + ['env', 'add', env])

@when("TestEnvCd4 I run \"env cd -n {env}\"")
def step_envcd4_run(context, env):
    res = subprocess.run(
        common.cmd + ['env', 'cd', '-n', env],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()
    context.env = env

@then("TestEnvCd4 Return code should be {expected:d}")
def step_envcd4_check_returncode(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"

@then("TestEnvCd4 PWD changes to env '{env}'")
def step_envcd4_check_pwd(context, env):
    expected = common.get_expected_path(env)
    common.verify_pwd(expected)

# TestEnvCd5 - Change to previous environment using "-" alias
@given("TestEnvCd5 Environments '{env1}' and '{env2}' exist")
def step_envcd5_setup(context, env1, env2):
    subprocess.run(common.cmd + ['env', 'add', env1])
    subprocess.run(common.cmd + ['env', 'add', env2])

@given("TestEnvCd5 I switch to '{env1}' then '{env2}'")
def step_envcd5_setup_switches(context, env1, env2):
    subprocess.run(common.cmd + ['env', 'cd', env1])
    subprocess.run(common.cmd + ['env', 'cd', env2])

@when("TestEnvCd5 I run \"env cd -\"")
def step_envcd5_run(context):
    res = subprocess.run(
        common.cmd + ['env', 'cd', '-'],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestEnvCd5 Return code should be {expected:d}")
def step_envcd5_check_returncode(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"

@then("TestEnvCd5 PWD changes to env '{env}'")
def step_envcd5_check_pwd(context, env):
    expected = common.get_expected_path(env)
    common.verify_pwd(expected)
