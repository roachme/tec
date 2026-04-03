import os
import common
import subprocess
from behave import given, when, then

# TODO:
# 2. Check PWD file /tmp/tecpwd

@given("TestCat1 Task ID '{taskid}'")
def step_get_numbers(context, taskid):
    context.args = [taskid]

@when("TestCat1 Program show task ID '{taskid}'")
def step_tec_cat_no_hook_single_taskID(context, taskid):
    res = subprocess.run(
        common.cmd + [ '-H', 'off', 'cat' ] + list(context.args),
        capture_output = True,
        text = True,
    )
    context.output = res.stdout.strip()

@then("TestCat1 Result is")
def step_tec_cat_no_hook_single_taskID_check_result(context):
    passed = True
    expected = context.text.strip().split('\n')
    actual = context.output.strip().split('\n')

    diffs = []
    for i, (line1, line2) in enumerate(zip(expected, actual), 1):
        if line1 != line2:
            # Exception for 'date', cuz it's different every time
            if not 'date' in line1:
                diffs.append((i, line1, line2))
                passed = False

    assert passed, \
        f"Expected\n'{expected}',\n\nbut got:\n'{actual}'"


@given("TestCat2 This Task ID '{taskid}' new")
def step_get_numbers_2(context, taskid):
    context.args = [ taskid ]

@when("TestCat2 This Program show task ID '{taskid}' with keys '{id}' and '{desc}'")
def step_tec_cat_no_hook_single_taskID2(context, taskid, id, desc):
    res = subprocess.run(
        common.cmd + [ 'cat', '-k', id, '-k', desc ] + list(context.args),
        capture_output = True,
        text = True,
    )
    context.output = res.stdout.strip()

@then("TestCat2 Result is is")
def step_tec_cat_no_hook_single_taskID_check_result_2(context):
    expected = context.text.strip()
    actual = context.output.strip()
    assert expected == actual, \
        f"Expected\n'{expected}',\n\nbut got:\n'{actual}'"


@given("TestCat3 Non-existing task ID '{taskid}'")
def step_get_numbers_3(context, taskid):
    context.args = [taskid]

@when("TestCat3 The return should accept task ID '{taskid}'")
def step_tec_cat_no_hook_single_taskID_3(context, taskid):
    res = subprocess.run(
        common.cmd + [ 'cat' ] + list(context.args),
        capture_output = True,
        text = True,
    )
    context.output = res.stdout.strip()
    context.res_stderr = res.stderr.strip()

@then('TestCat3 Result is')
def step_tec_cat_no_hook_single_taskID_check_result_3(context):
    expected = context.text.strip()
    actual = context.res_stderr.strip()
    assert expected == actual, \
        f"Expected\n'{expected}',\n\nbut got:\n'{actual}'"



@given("TestCat4 Task IDs '{taskid1}' '{taskid2}'")
def step_multipl_valid_tasks_prep(context, taskid1, taskid2):
    context.args = [taskid1, taskid2]

@when("TestCat4 Program show task IDs '{taskid1}' '{taskid2}'")
def step_multipl_valid_tasks_run(context, taskid1, taskid2):
    res = subprocess.run(
        common.cmd + [ 'cat' ] + list(context.args),
        capture_output = True,
        text = True,
    )
    context.output = res.stdout.strip()

@then("TestCat4 Result is")
def step_multipl_valid_tasks_check(context):
    passed = True
    expected = context.text.strip().split('\n')
    actual = context.output.strip().split('\n')

    diffs = []
    for i, (line1, line2) in enumerate(zip(expected, actual), 1):
        if line1 != line2:
            # Exception for 'date', cuz it's different every time
            if not 'date' in line1:
                diffs.append((i, line1, line2))
                passed = False

    assert passed, \
        f"Expected\n'{expected}',\n\nbut got:\n'{actual}'"
