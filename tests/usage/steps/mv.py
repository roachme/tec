import os
import common
import subprocess
from behave import given, when, then
from togg import Togg

@given("TestMv1 Task '{taskid}' exists")
def step_mv1_setup(context, taskid):
    subprocess.run(common.cmd + ['add', taskid])

@when("TestMv1 I run \"mv {src} {dst}\"")
def step_mv1_run(context, src, dst):
    res = subprocess.run(
        common.cmd + ['mv', src, dst],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()
    context.dst = dst

@then("TestMv1 Return code should be {expected:d}")
def step_mv1_check_returncode(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"

@then("TestMv1 Task '{taskid}' should exist")
def step_mv1_check_exists(context, taskid):
    res = subprocess.run(
        common.cmd + ['cat', taskid],
        capture_output=True,
        text=True,
    )
    assert res.returncode == 0, \
        f"Task '{taskid}' should exist but cat command failed. stderr: {res.stderr}"

@given("TestMv2 Task '{taskid}' exists")
def step_mv2_setup_task(context, taskid):
    subprocess.run(common.cmd + ['add', taskid])

@given("TestMv2 Desk '{desk}' exists")
def step_mv2_setup_desk(context, desk):
    # Use -N flag to not change directory/toggle
    subprocess.run(common.cmd + ['desk', 'add', '-N', desk])

@when("TestMv2 I run \"mv {src} {dst}\"")
def step_mv2_run(context, src, dst):
    res = subprocess.run(
        common.cmd + ['mv', src, dst],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestMv2 Return code should be {expected:d}")
def step_mv2_check_returncode(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}. stderr: {context.stderr}"

@given("TestMv3 Task '{taskid}' exists")
def step_mv3_setup(context, taskid):
    subprocess.run(common.cmd + ['add', taskid])

@when("TestMv3 I run \"mv {src} {dst}\"")
def step_mv3_run(context, src, dst):
    res = subprocess.run(
        common.cmd + ['mv', src, dst],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestMv3 Return code should not be {expected:d}")
def step_mv3_check_returncode(context, expected):
    assert context.returncode != expected, \
        f"Expected return code != {expected}, but got {context.returncode}"

@given("TestMv4 Task '{taskid}' exists")
def step_mv4_setup(context, taskid):
    subprocess.run(common.cmd + ['add', taskid])
    context.taskid = taskid

@given("TestMv4 I cd to '{taskid}'")
def step_mv4_cd(context, taskid):
    res = subprocess.run(common.cmd + ['cd', taskid], capture_output=True, text=True)
    # Ensure cd succeeded
    if res.returncode != 0:
        print(f"CD failed: {res.stderr}")

@when("TestMv4 I run \"mv {src} {dst}\"")
def step_mv4_run(context, src, dst):
    res = subprocess.run(
        common.cmd + ['mv', src, dst],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()
    context.new_taskid = dst

@then("TestMv4 Return code should be {expected:d}")
def step_mv4_check_returncode(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"

@then("TestMv4 Current toggle should be '{taskid}'")
def step_mv4_check_toggle(context, taskid):
    import time
    time.sleep(0.1)  # Small delay to ensure file is written
    togg = Togg(common.taskbase, {'env': common.TEST_ENV, 'desk': common.TEST_DESK, 'task': taskid})
    toggs = togg.gettask()
    assert toggs['curr'] == taskid, \
        f"Expected current toggle '{taskid}', but got '{toggs['curr']}'. Full toggles: {toggs}"
