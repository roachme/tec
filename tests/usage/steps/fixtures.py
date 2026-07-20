import os
import re
import shutil
import subprocess
import common
from togg import Togg
from behave import given, then

# Generic task fixtures + state assertions, shared by every feature file
# whose scenarios need a pre-existing task (cat, mv, set, cd, ...).

@given('a task "{taskid}" exists')
def step_task_exists(context, taskid):
    res = subprocess.run(
        common.cmd + [ 'add', taskid ],
        capture_output = True,
        text = True,
    )
    assert res.returncode == 0, \
        f"Fixture setup failed: could not add task '{taskid}': {res.stderr.strip()}"

@given('tasks {taskids} exist')
def step_tasks_exist(context, taskids):
    ids = re.findall(r'"([^"]+)"', taskids)
    res = subprocess.run(
        common.cmd + [ 'add' ] + ids,
        capture_output = True,
        text = True,
    )
    assert res.returncode == 0, \
        f"Fixture setup failed: could not add tasks {ids}: {res.stderr.strip()}"

@given('an env "{env:w}" exists')
def step_env_exists(context, env):
    res = subprocess.run(
        common.cmd + [ 'env', 'add', env ],
        capture_output = True,
        text = True,
    )
    assert res.returncode == 0, \
        f"Fixture setup failed: could not add env '{env}': {res.stderr.strip()}"

@given('an env "{env}" with desk "{desk}" exists')
def step_env_with_desk_exists(context, env, desk):
    res = subprocess.run(common.cmd + [ 'env', 'add', env ], capture_output=True, text=True)
    assert res.returncode == 0, \
        f"Fixture setup failed: could not add env '{env}': {res.stderr.strip()}"
    res = subprocess.run(common.cmd + [ 'desk', 'add', '-e', env, desk ], capture_output=True, text=True)
    assert res.returncode == 0, \
        f"Fixture setup failed: could not add desk '{desk}' in env '{env}': {res.stderr.strip()}"

@then('the PWD should be the env "{env}"')
def step_pwd_is_env(context, env):
    common.verify_pwd(common.get_expected_path(env))

@then('the PWD should be the desk "{desk}" in env "{env}"')
def step_pwd_is_desk(context, desk, env):
    common.verify_pwd(common.get_expected_path(env, desk))

@then('the PWD should be the task "{taskid}" in desk "{desk}" in env "{env}"')
def step_pwd_is_task_full(context, taskid, desk, env):
    common.verify_pwd(common.get_expected_path(env, desk, taskid))

@given('a desk "{desk}" exists')
def step_desk_exists(context, desk):
    res = subprocess.run(
        common.cmd + [ 'desk', 'add', desk ],
        capture_output = True,
        text = True,
    )
    assert res.returncode == 0, \
        f"Fixture setup failed: could not add desk '{desk}': {res.stderr.strip()}"

@given('a desk "{desk}" exists without switching to it')
def step_desk_exists_no_switch(context, desk):
    # -N: register the desk without moving the current toggle/PWD there.
    # Note: 'desk add -N' leaves the desk's unit values unreadable (a real
    # tec bug) - only use this where the test doesn't need to cat/ls it.
    res = subprocess.run(
        common.cmd + [ 'desk', 'add', '-N', desk ],
        capture_output = True,
        text = True,
    )
    assert res.returncode == 0, \
        f"Fixture setup failed: could not add desk '{desk}': {res.stderr.strip()}"

@then('the task "{taskid}" should exist')
def step_task_exists_check(context, taskid):
    res = subprocess.run(
        common.cmd + [ 'cat', taskid ],
        capture_output = True,
        text = True,
    )
    assert res.returncode == 0, \
        f"Expected task '{taskid}' to exist, but cat failed: {res.stderr.strip()}"

@then('task "{taskid}" {key} should be "{value}"')
def step_task_key_is(context, taskid, key, value):
    res = subprocess.run(
        common.cmd + [ 'cat', '-k', key, taskid ],
        capture_output = True,
        text = True,
    )
    assert value in res.stdout, \
        f"Expected task '{taskid}' {key} to be '{value}', but got '{res.stdout.strip()}'"

@then('the current task should be "{taskid}"')
def step_current_task_is(context, taskid):
    togg = Togg(common.taskbase, {
        'env': common.TEST_ENV,
        'desk': common.TEST_DESK,
        'task': taskid,
    })
    actual = togg.gettask()['curr']
    assert actual == taskid, \
        f"Expected current task '{taskid}', but got '{actual}'"

@then('the current environment should be "{env}"')
def step_current_env_is(context, env):
    togg = Togg(common.taskbase, {})
    actual = togg.getenv()['curr']
    assert actual == env, \
        f"Expected current environment '{env}', but got '{actual}'"

@then('the current desk should be "{desk}" in env "{env}"')
def step_current_desk_is(context, desk, env):
    togg = Togg(common.taskbase, { 'env': env })
    actual = togg.getdesk()['curr']
    assert actual == desk, \
        f"Expected current desk '{desk}', but got '{actual}'"

@then('the PWD should be the task "{taskid}"')
def step_pwd_is_task(context, taskid):
    expected = common.get_expected_path(common.TEST_ENV, common.TEST_DESK, taskid)
    common.verify_pwd(expected)

@then('the PWD file should be empty')
def step_pwd_is_empty(context):
    assert os.path.getsize(common.pwdfile) == 0, \
        "Expected PWD file to be empty, but it has content"

@given('no task database exists')
def step_no_task_database(context):
    shutil.rmtree(common.taskbase, ignore_errors=True)

@given('a task database exists')
def step_task_database_exists(context):
    if not os.path.exists(common.taskbase):
        subprocess.run(common.cmd + [ 'init' ])

@then('the task database directory should exist')
def step_task_database_dir_exists(context):
    assert os.path.exists(common.taskbase), \
        f"Task database directory should exist at {common.taskbase}"
