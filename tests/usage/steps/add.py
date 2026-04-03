import os
import common
import subprocess
from behave import given, when, then
from togg import Togg

@given("TestAdd1 Single task ID '{taskid}'")
def step_add_single_args(context, taskid):
    context.args = [taskid]

@when("TestAdd1 Add single valid task ID '{taskid}'")
def step_add_single_run(context, taskid):
    res = subprocess.run(
        common.cmd + [ 'add' ] + list(context.args),
        capture_output = True,
        text = True,
    )
    context.output = res.stdout.strip()
    context.stderr = res.stderr.strip()
    context.retcode = res.returncode

@then("TestAdd1 Return value is '{res}'")
def step_add_single_check(context, res):
    expected = int(res)
    actual = context.retcode
    assert expected == actual, \
        f"Expected\n'{expected}', but got\n'{actual}'"

@then("TestAdd1 There empty stderr")
def step_add_single_check_2(context):
    expected = ""
    actual = context.stderr
    assert expected == actual, \
        f"Expected\n'{expected}', but got\n'{actual}'"

@then("TestAdd1 PWD changes to task created '{taskid}'")
def step_add_single_check_3(context, taskid):
    expected = common.get_expected_path(common.TEST_ENV, common.TEST_DESK, taskid)
    common.verify_pwd(expected)

@then("TestAdd1 Task is added to toggles")
def step_add_single_check_4(context):
    togg = Togg(common.taskbase, {'env': common.TEST_ENV, 'desk': common.TEST_DESK, 'task':context.args[0]})
    toggs = togg.gettask()
    actual = toggs['curr']
    expected = context.args[0]
    assert actual == expected, \
        f"Expected\n'{expected}', but got\n'{actual}'"

@given("TestAdd2 Single existing task ID '{taskid}'")
def step_add_existing_args(context, taskid):
    context.args = [taskid]

@when("TestAdd2 Add single existing valid task ID '{taskid}'")
def step_add_existing_run(context, taskid):
    res = subprocess.run(
        common.cmd + [ 'add' ] + list(context.args),
        capture_output = True,
        text = True,
    )
    context.output = res.stdout.strip()
    context.stderr = res.stderr.strip()
    context.retcode = res.returncode


@given("TestAdd3 Single multiple task IDs '{taskid1}' '{taskid2}'")
def step_add_multiple_valid_args(context, taskid1, taskid2):
    context.args = [taskid1, taskid2]

@when("TestAdd3 Add multiple valid task IDs '{taskid1}' '{taskid2}'")
def step_add_multiple_valid_run(context, taskid1, taskid2):
    res = subprocess.run(
        common.cmd + [ 'add' ] + list(context.args),
        capture_output = True,
        text = True,
    )
    context.output = res.stdout.strip()
    context.stderr = res.stderr.strip()
    context.retcode = res.returncode

@then("TestAdd3 Return value is '{res}'")
def step_add_multiple_valid_check_1(context, res):
    expected = int(res)
    actual = context.retcode
    assert expected == actual, \
        f"Expected\n'{expected}', but got\n'{actual}'"

@then("TestAdd3 There is empty stderr")
def step_add_multiple_valid_check_2(context):
    expected = ""
    actual = context.stderr
    assert expected == actual, \
        f"Expected\n'{expected}', but got\n'{actual}'"

@then("TestAdd3 Last task is added to toggles")
def step_add_multiple_valid_check_3(context):
    # When adding multiple tasks, the last one becomes current
    togg = Togg(common.taskbase, {'env': common.TEST_ENV, 'desk': common.TEST_DESK, 'task':context.args[-1]})
    toggs = togg.gettask()
    actual = toggs['curr']
    expected = context.args[-1]
    assert actual == expected, \
        f"Expected\n'{expected}', but got\n'{actual}'"



@given('Bunch of task IDs "{taskid}"')
def step_add_bunch_task_ids_init(context, taskid):
    context.args = [taskid]
    context.taskid = taskid

@then('Bunch of task IDs result should be "{result}" and "{error}" and empty "{pwd}"')
def step_add_bunch_task_ids_run(context, result, error, pwd):
    res = subprocess.run(
        common.cmd + [ 'add', context.taskid ],
        capture_output = True,
        text = True,
    )
    context.output = res.stdout.strip()
    context.stderr = res.stderr.strip()
    context.retcode = res.returncode

    assert context.retcode == int(result), \
        f"Expected return code:\n'{result}', but got\n'{context.retcode}'"
    assert context.stderr == error, \
        f"Expected error message:\n'{error}', but got\n'{context.stderr}'"
    assert os.path.getsize(common.pwdfile) == int(pwd), \
        f"Expected PWD empty', but got not empty'"
