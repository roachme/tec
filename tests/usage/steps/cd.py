import os
import common
import subprocess
from behave import given, when, then
from togg import Togg

@given("TestCd1 Task ID '{taskid}' exists")
def step_cd1_setup(context, taskid):
    res = subprocess.run(
        common.cmd + ['add', taskid],
        capture_output=True,
        text=True,
    )
    context.taskid = taskid

@when("TestCd1 I run \"cd {taskid}\"")
def step_cd1_run(context, taskid):
    res = subprocess.run(
        common.cmd + ['cd', taskid],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestCd1 Return code should be {expected:d}")
def step_cd1_check_returncode(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"

@then("TestCd1 PWD changes to task '{taskid}'")
def step_cd1_check_pwd(context, taskid):
    expected = common.get_expected_path(common.TEST_ENV, common.TEST_DESK, taskid)
    common.verify_pwd(expected)

@then("TestCd1 Task '{taskid}' is set as current toggle")
def step_cd1_check_toggle(context, taskid):
    togg = Togg(common.taskbase, {'env': common.TEST_ENV, 'desk': common.TEST_DESK, 'task': taskid})
    toggs = togg.gettask()
    assert toggs['curr'] == taskid, \
        f"Expected current task '{taskid}', but got '{toggs['curr']}'"

@given("TestCd2 Task IDs '{taskid1}' and '{taskid2}' exist")
def step_cd2_setup(context, taskid1, taskid2):
    subprocess.run(common.cmd + ['add', taskid1, taskid2])
    context.taskid1 = taskid1
    context.taskid2 = taskid2

@given("TestCd2 I cd to '{taskid}'")
def step_cd2_cd_to(context, taskid):
    subprocess.run(common.cmd + ['cd', taskid])

@when("TestCd2 I run \"cd -\"")
def step_cd2_run(context):
    res = subprocess.run(
        common.cmd + ['cd', '-'],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestCd2 Return code should be {expected:d}")
def step_cd2_check_returncode(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"

@then("TestCd2 PWD changes to task '{taskid}'")
def step_cd2_check_pwd(context, taskid):
    expected = common.get_expected_path(common.TEST_ENV, common.TEST_DESK, taskid)
    common.verify_pwd(expected)

@given("TestCd3 Task '{taskid}' exists")
def step_cd3_setup(context, taskid):
    subprocess.run(common.cmd + ['add', taskid])
    context.taskid = taskid

@when("TestCd3 I run \"cd -d desk -e test {taskid}\"")
def step_cd3_run(context, taskid):
    res = subprocess.run(
        common.cmd + ['cd', '-d', 'desk', '-e', 'test', taskid],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestCd3 Return code should be {expected:d}")
def step_cd3_check_returncode(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"

@then("TestCd3 PWD changes to task '{taskid}'")
def step_cd3_check_pwd(context, taskid):
    expected = common.get_expected_path(common.TEST_ENV, common.TEST_DESK, taskid)
    common.verify_pwd(expected)

@given("TestCd4 No preconditions")
def step_cd4_setup(context):
    pass

@when("TestCd4 I run \"cd {taskid}\"")
def step_cd4_run(context, taskid):
    res = subprocess.run(
        common.cmd + ['cd', taskid],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestCd4 Return code should not be {expected:d}")
def step_cd4_check_returncode(context, expected):
    assert context.returncode != expected, \
        f"Expected return code != {expected}, but got {context.returncode}"

@then("TestCd4 Error message should contain \"{message}\"")
def step_cd4_check_error(context, message):
    assert message in context.stderr, \
        f"Expected error to contain '{message}', but got '{context.stderr}'"
