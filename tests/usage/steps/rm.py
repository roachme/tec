import os
import common
import subprocess
from behave import given, when, then

def run_command(context, cmdname, cmdopt, args):
    res = subprocess.run(
        common.cmd + [ cmdname, cmdopt ] + args,
        capture_output = True,
        text = True,
    )
    context.output = res.stdout.strip()
    context.stderr = res.stderr.strip()
    context.retcode = res.returncode

def stderr_empty(context, expected = ""):
    expected = expected
    actual = context.stderr
    assert expected == actual, \
        f"Expected\n'{expected}', but got\n'{actual}'"

def retcode_equal(context, expected):
    expected = int(expected)
    actual = context.retcode
    assert expected == actual, \
        f"Expected\n'{expected}', but got\n'{actual}'"



@given("TestRemove1 Multiple task IDs '{taskid1}' '{taskid2}' '{taskid3}' '{taskid4}' '{taskid5}'")
def step_add_single_args(context, taskid1, taskid2, taskid3, taskid4, taskid5):
    context.args = [taskid1, taskid2, taskid3, taskid4, taskid5]

@when("TestRemove1 Add multiple valid task IDs '{taskid1}' '{taskid2}' '{taskid3}' '{taskid4}' '{taskid5}'")
def step_add_single_run(context, taskid1, taskid2, taskid3, taskid4, taskid5):
    run_command(context, "add", "", context.args)

@then("TestRemove1 Return value is '{res}'")
def step_add_single_check(context, res):
    retcode_equal(context, res)



@given("TestRemove2 Single task ID '{taskid}'")
def step_remove_single_args(context, taskid):
    context.args = [ taskid ]

@when("TestRemove2 Remove single valid task ID '{taskid}'")
def step_remove_single_run(context, taskid):
    run_command(context, "rm", "-f", context.args)

@then("TestRemove2 Return value is '{res}'")
def step_remove_single_check_1(context, res):
    retcode_equal(context, res)

@then("TestRemove2 There empty stderr")
def step_remove_single_check_2(context):
    stderr_empty(context)



@given("TestRemove3 Multiple task IDs '{taskid1}' '{taskid2}'")
def step_remove_multiple_args(context, taskid1, taskid2):
    context.args = [ taskid1, taskid2 ]

@when("TestRemove3 Remove multiple valid task IDs '{taskid1}' '{taskid2}'")
def step_remove_multiple_run(context, taskid1, taskid2):
    run_command(context, "rm", "-f", context.args)

@then("TestRemove3 Return value is '{res}'")
def step_remove_multiple_check_1(context, res):
    retcode_equal(context, res)

@then("TestRemove3 There empty stderr")
def step_remove_multiple_check_2(context):
    stderr_empty(context)



@given("TestRemove4 Single non existing task ID '{taskid}'")
def step_rm_non_existing_args(context, taskid):
    context.args = [ taskid ]

@when("TestRemove4 Remove single non existing ID '{taskid}'")
def step_rm_non_existing_run(context, taskid):
    run_command(context, "rm", "-f", context.args)

@then("TestRemove4 Return value is '{res}'")
def step_rm_non_existing_check_1(context, res):
    retcode_equal(context, res)

@then("TestRemove4 There empty stderr")
def step_rm_non_existing_check_2(context):
    expected = context.text.strip()
    actual = context.stderr.strip()
    assert expected == actual, \
        f"Expected\n'{expected}',\n\nbut got:\n'{actual}'"



@given("TestRemove5 Single previous task ID '{taskid}'")
def step_rm_previous_args(context, taskid):
    context.args = [ taskid ]

@when("TestRemove5 Remove previous task ID '{taskid}'")
def step_rm_previous_run(context, taskid):
    run_command(context, "rm", "-f", context.args)

@then("TestRemove5 Return value is '{res}'")
def step_rm_previous_check_1(context, res):
    retcode_equal(context, res)

@then("TestRemove5 There empty stderr")
def step_rm_previous_check_2(context):
    stderr_empty(context)



@given("TestRemove6 Single current task ID '{taskid}'")
def step_rm_current_args(context, taskid):
    context.args = [taskid]

@when("TestRemove6 Remove current task ID '{taskid}'")
def step_rm_current_run(context, taskid):
    run_command(context, "rm", "-f", context.args)


@then("TestRemove6 Return value is '{res}'")
def step_rm_current_check_1(context, res):
    retcode_equal(context, res)

@then("TestRemove6 There empty stderr")
def step_rm_current_check_2(context):
    stderr_empty(context)
