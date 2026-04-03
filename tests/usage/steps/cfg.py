import os
import common
import subprocess
from behave import given, when, then

@given("TestCfg1 Configuration exists")
def step_cfg1_setup(context):
    pass  # Configuration always exists

@when("TestCfg1 I run \"cfg ls\"")
def step_cfg1_run(context):
    res = subprocess.run(
        common.cmd + ['cfg', 'ls'],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestCfg1 Return code should be {expected:d}")
def step_cfg1_check(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"

@given("TestCfg2 Configuration exists")
def step_cfg2_setup(context):
    pass

@when("TestCfg2 I run \"cfg get {key}\"")
def step_cfg2_run(context, key):
    res = subprocess.run(
        common.cmd + ['cfg', 'get', key],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestCfg2 Return code should be {expected:d}")
def step_cfg2_check_returncode(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"

@then("TestCfg2 Output should contain \"{text}\"")
def step_cfg2_check_output(context, text):
    assert text in context.stdout, \
        f"Expected output to contain '{text}', but got '{context.stdout}'"

@given("TestCfg3 Configuration exists")
def step_cfg3_setup(context):
    pass

@when("TestCfg3 I run \"cfg get {key}\"")
def step_cfg3_run(context, key):
    res = subprocess.run(
        common.cmd + ['cfg', 'get', key],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestCfg3 Return code should be {expected:d}")
def step_cfg3_check(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"

@given("TestCfg4 Configuration exists")
def step_cfg4_setup(context):
    pass

@when("TestCfg4 I run \"cfg get {key1} {key2}\"")
def step_cfg4_run(context, key1, key2):
    res = subprocess.run(
        common.cmd + ['cfg', 'get', key1, key2],
        capture_output=True,
        text=True,
    )
    context.returncode = res.returncode
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()

@then("TestCfg4 Return code should be {expected:d}")
def step_cfg4_check_returncode(context, expected):
    assert context.returncode == expected, \
        f"Expected return code {expected}, but got {context.returncode}"

@then("TestCfg4 Output should contain \"{text}\"")
def step_cfg4_check_output(context, text):
    assert text in context.stdout, \
        f"Expected output to contain '{text}', but got '{context.stdout}'"
