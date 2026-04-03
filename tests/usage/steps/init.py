import os
import shutil
import common
import subprocess
from behave import given, when, then

@given("TestInit1 No task database exists")
def step_init1_setup(context):
    # Remove task database if it exists
    if os.path.exists(common.taskbase):
        shutil.rmtree(common.taskbase)

@when("TestInit1 I run \"init\"")
def step_init1_run(context):
    res = subprocess.run(
        common.cmd + ['init'],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestInit1 Return code should be {expected:d}")
def step_init1_check_returncode(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"

@then("TestInit1 Task database directory should exist")
def step_init1_check_exists(context):
    assert os.path.exists(common.taskbase), \
        f"Task database directory should exist at {common.taskbase}"

@given("TestInit2 Task database exists")
def step_init2_setup(context):
    # Ensure task database exists
    if not os.path.exists(common.taskbase):
        subprocess.run(common.cmd + ['init'])

@when("TestInit2 I run \"init\"")
def step_init2_run(context):
    res = subprocess.run(
        common.cmd + ['init'],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestInit2 Return code should be {expected:d}")
def step_init2_check(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"
