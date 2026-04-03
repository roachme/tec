import os
import common
import subprocess
from behave import given, when, then

@given("TestLs1 Tasks '{taskid1}' '{taskid2}' '{taskid3}' exist")
def step_ls1_setup(context, taskid1, taskid2, taskid3):
    subprocess.run(common.cmd + ['add', taskid1, taskid2, taskid3])

@when("TestLs1 I run \"ls\"")
def step_ls1_run(context):
    res = subprocess.run(
        common.cmd + ['ls'],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestLs1 Return code should be {expected:d}")
def step_ls1_check_returncode(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"

@then("TestLs1 Output should contain \"{text}\"")
def step_ls1_check_output(context, text):
    assert text in context.stdout, \
        f"Expected output to contain '{text}', but got '{context.stdout}'"

@given("TestLs2 Tasks '{taskid1}' '{taskid2}' exist")
def step_ls2_setup(context, taskid1, taskid2):
    subprocess.run(common.cmd + ['add', taskid1, taskid2])

@given("TestLs2 I cd to '{taskid}'")
def step_ls2_cd(context, taskid):
    subprocess.run(common.cmd + ['cd', taskid])

@when("TestLs2 I run \"ls -t\"")
def step_ls2_run(context):
    res = subprocess.run(
        common.cmd + ['ls', '-t'],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestLs2 Return code should be {expected:d}")
def step_ls2_check_returncode(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"

@then("TestLs2 Output should contain \"{text}\"")
def step_ls2_check_output(context, text):
    assert text in context.stdout, \
        f"Expected output to contain '{text}', but got '{context.stdout}'"

@given("TestLs3 Tasks '{taskid1}' '{taskid2}' exist")
def step_ls3_setup(context, taskid1, taskid2):
    subprocess.run(common.cmd + ['add', taskid1, taskid2])

@when("TestLs3 I run \"ls -d {desk}\"")
def step_ls3_run(context, desk):
    res = subprocess.run(
        common.cmd + ['ls', '-d', desk],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestLs3 Return code should be {expected:d}")
def step_ls3_check_returncode(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"

@then("TestLs3 Output should contain \"{text}\"")
def step_ls3_check_output(context, text):
    assert text in context.stdout, \
        f"Expected output to contain '{text}', but got '{context.stdout}'"

@given("TestLs4 Tasks '{taskid}' exist")
def step_ls4_setup(context, taskid):
    subprocess.run(common.cmd + ['add', taskid])

@when("TestLs4 I run \"ls -H\"")
def step_ls4_run(context):
    res = subprocess.run(
        common.cmd + ['ls', '-H'],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestLs4 Return code should be {expected:d}")
def step_ls4_check_returncode(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"
