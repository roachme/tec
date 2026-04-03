import os
import common
import subprocess
from behave import given, when, then

@given("TestSet1 Task '{taskid}' exists")
def step_set1_setup(context, taskid):
    subprocess.run(common.cmd + ['add', taskid])
    context.taskid = taskid

@when("TestSet1 I run \"set -D '{desc}' {taskid}\"")
def step_set1_run(context, desc, taskid):
    res = subprocess.run(
        common.cmd + ['set', '-D', desc, taskid],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()
    context.expected_desc = desc

@then("TestSet1 Return code should be {expected:d}")
def step_set1_check_returncode(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"

@then("TestSet1 Task '{taskid}' description should be '{desc}'")
def step_set1_check_desc(context, taskid, desc):
    res = subprocess.run(
        common.cmd + ['cat', '-k', 'desc', taskid],
        capture_output=True,
        text=True,
    )
    assert desc in res.stdout, \
        f"Expected description '{desc}', but got '{res.stdout}'"

@given("TestSet2 Task '{taskid}' exists")
def step_set2_setup(context, taskid):
    subprocess.run(common.cmd + ['add', taskid])

@when("TestSet2 I run \"set -T {tasktype} {taskid}\"")
def step_set2_run(context, tasktype, taskid):
    res = subprocess.run(
        common.cmd + ['set', '-T', tasktype, taskid],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestSet2 Return code should be {expected:d}")
def step_set2_check_returncode(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"

@then("TestSet2 Task '{taskid}' type should be '{tasktype}'")
def step_set2_check_type(context, taskid, tasktype):
    res = subprocess.run(
        common.cmd + ['cat', '-k', 'type', taskid],
        capture_output=True,
        text=True,
    )
    assert tasktype in res.stdout, \
        f"Expected type '{tasktype}', but got '{res.stdout}'"

@given("TestSet3 Task '{taskid}' exists")
def step_set3_setup(context, taskid):
    subprocess.run(common.cmd + ['add', taskid])

@when("TestSet3 I run \"set -P {prio} {taskid}\"")
def step_set3_run(context, prio, taskid):
    res = subprocess.run(
        common.cmd + ['set', '-P', prio, taskid],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestSet3 Return code should be {expected:d}")
def step_set3_check_returncode(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"

@then("TestSet3 Task '{taskid}' priority should be '{prio}'")
def step_set3_check_prio(context, taskid, prio):
    res = subprocess.run(
        common.cmd + ['cat', '-k', 'prio', taskid],
        capture_output=True,
        text=True,
    )
    assert prio in res.stdout, \
        f"Expected priority '{prio}', but got '{res.stdout}'"

@given("TestSet4 Task '{taskid}' exists")
def step_set4_setup(context, taskid):
    subprocess.run(common.cmd + ['add', taskid])

@when("TestSet4 I run \"set -T {tasktype} {taskid}\"")
def step_set4_run(context, tasktype, taskid):
    res = subprocess.run(
        common.cmd + ['set', '-T', tasktype, taskid],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestSet4 Return code should not be {expected:d}")
def step_set4_check_returncode(context, expected):
    assert context.returncode != expected, \
        f"Expected return code != {expected}, but got {context.returncode}"

@then("TestSet4 Error should contain \"{message}\"")
def step_set4_check_error(context, message):
    assert message in context.stderr, \
        f"Expected error to contain '{message}', but got '{context.stderr}'"
