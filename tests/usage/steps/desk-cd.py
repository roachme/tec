import os
import common
import subprocess
from behave import given, when, then

# TestDeskCd1 - Change to desk
@given("TestDeskCd1 Desk '{desk}' exists")
def step_deskcd1_setup(context, desk):
    # Ensure we're in the test environment first
    subprocess.run(common.cmd + ['env', 'cd', common.TEST_ENV])
    subprocess.run(common.cmd + ['desk', 'add', desk])

@when("TestDeskCd1 I run \"desk cd {desk}\"")
def step_deskcd1_run(context, desk):
    res = subprocess.run(
        common.cmd + ['desk', 'cd', desk],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()
    context.desk = desk

@then("TestDeskCd1 Return code should be {expected:d}")
def step_deskcd1_check_returncode(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"

@then("TestDeskCd1 PWD changes to desk '{desk}'")
def step_deskcd1_check_pwd(context, desk):
    expected = common.get_expected_path(common.TEST_ENV, desk)
    common.verify_pwd(expected)

# TestDeskCd2 - Change to desk in specific environment
@given("TestDeskCd2 Environment '{env}' with desk '{desk}' exists")
def step_deskcd2_setup(context, env, desk):
    subprocess.run(common.cmd + ['env', 'add', env])
    subprocess.run(common.cmd + ['desk', 'add', '-e', env, desk])

@when("TestDeskCd2 I run \"desk cd -e {env} {desk}\"")
def step_deskcd2_run(context, env, desk):
    res = subprocess.run(
        common.cmd + ['desk', 'cd', '-e', env, desk],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()
    context.env = env
    context.desk = desk

@then("TestDeskCd2 Return code should be {expected:d}")
def step_deskcd2_check_returncode(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"

@then("TestDeskCd2 PWD changes to desk '{desk}' in env '{env}'")
def step_deskcd2_check_pwd(context, desk, env):
    expected = common.get_expected_path(env, desk)
    common.verify_pwd(expected)

# TestDeskCd3 - Change to nonexistent desk
@when("TestDeskCd3 I run \"desk cd {desk}\"")
def step_deskcd3_run(context, desk):
    res = subprocess.run(
        common.cmd + ['desk', 'cd', desk],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestDeskCd3 Return code should not be {expected:d}")
def step_deskcd3_check(context, expected):
    assert context.returncode != expected, \
        f"Expected return code to not be {expected}, but got {context.returncode}"

# TestDeskCd4 - Change to desk with -n flag
@given("TestDeskCd4 Desk '{desk}' exists")
def step_deskcd4_setup(context, desk):
    # Ensure we're in the test environment first
    subprocess.run(common.cmd + ['env', 'cd', common.TEST_ENV])
    subprocess.run(common.cmd + ['desk', 'add', desk])

@when("TestDeskCd4 I run \"desk cd -n {desk}\"")
def step_deskcd4_run(context, desk):
    res = subprocess.run(
        common.cmd + ['desk', 'cd', '-n', desk],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()
    context.desk = desk

@then("TestDeskCd4 Return code should be {expected:d}")
def step_deskcd4_check_returncode(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"

@then("TestDeskCd4 PWD changes to desk '{desk}'")
def step_deskcd4_check_pwd(context, desk):
    expected = common.get_expected_path(common.TEST_ENV, desk)
    common.verify_pwd(expected)

# TestDeskCd5 - Change to previous desk using "-" alias
@given("TestDeskCd5 Desks '{desk1}' and '{desk2}' exist")
def step_deskcd5_setup(context, desk1, desk2):
    # Ensure we're in the test environment first
    subprocess.run(common.cmd + ['env', 'cd', common.TEST_ENV])
    subprocess.run(common.cmd + ['desk', 'add', desk1])
    subprocess.run(common.cmd + ['desk', 'add', desk2])

@given("TestDeskCd5 I switch to '{desk1}' then '{desk2}'")
def step_deskcd5_setup_switches(context, desk1, desk2):
    subprocess.run(common.cmd + ['desk', 'cd', desk1])
    subprocess.run(common.cmd + ['desk', 'cd', desk2])

@when("TestDeskCd5 I run \"desk cd -\"")
def step_deskcd5_run(context):
    res = subprocess.run(
        common.cmd + ['desk', 'cd', '-'],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestDeskCd5 Return code should be {expected:d}")
def step_deskcd5_check_returncode(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"

@then("TestDeskCd5 PWD changes to desk '{desk}'")
def step_deskcd5_check_pwd(context, desk):
    expected = common.get_expected_path(common.TEST_ENV, desk)
    common.verify_pwd(expected)
