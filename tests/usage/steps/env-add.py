import os
import common
import subprocess
from behave import given, when, then

@given("Test1 Single environment name '{env}'")
def step_env_add_valid(context, env):
    context.args = [env]

#When  Add single valid environment name 'test'
@when("Test1 Add single valid environment name '{env}'")
def step_env_add_valid(context, env):
    res = subprocess.run(
        common.cmd + [ 'env', 'add' ] + list(context.args),
        capture_output = True,
        text = True
    )
    context.output = res.stdout.strip()
    context.returncode = res.returncode

@then("Test1 Return value is '{res}'")
def step_env_add_valid_check(context, res):
    expected = int(res)
    actual = context.returncode
    assert expected == actual, \
        f"Expected '{expected}', but got '{actual}'"



@given("Test2 Single invalid environment name '{env}'")
def step_env_add_invalid(context, env):
    context.args = [env]

@when("Test2 Add single environment name '{env}'")
def step_env_add_invalid_check(context, env):
    res = subprocess.run(
        common.cmd + [ 'env', 'add' ] + list(context.args),
        capture_output = True,
        text = True
    )
    context.output = res.stdout.strip()
    context.stderr = res.stderr.strip()
    context.returncode = res.returncode

@then("Test2 Return value is '{res}'")
def step_env_add_invalid_check(context, res):
    expected = int(res)
    actual = context.returncode
    assert expected == actual, \
        f"Expected '{expected}', but got '{actual}'"

# FIXME: for some reasons stderr message gets duplicated
@then("Test2 Get error message")
def step_env_add_invalid_check_2(context):
    expected = context.text.strip()
    actual = context.stderr.strip()
    assert expected == actual, \
        f"Expected\n'{expected}',\n\nbut got:\n'{actual}'"


@given("Test3 Multiple environment name '{env1}' '{env2}' '{env3}'")
def step_env_add_mulitple_valid(context, env1, env2, env3):
    context.args = [ env1, env2, env3 ]

@when("Test3 Add single valid environment name '{env1}' '{env2}' '{env3}'")
def step_env_add_mulitple_run(context, env1, env2, env3):
    res = subprocess.run(
        common.cmd + [ 'env', 'add' ] + list(context.args),
        capture_output = True,
        text = True
    )
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()
    context.returncode = res.returncode

@then("Test3 Return value is '{res}'")
def step_env_add_mulitple_check(context, res):
    expected = int(res)
    actual = context.returncode
    assert expected == actual, \
        f"Expected '{expected}', but got '{actual}'"


@given("Test4 Multiple valid and invalid environments names '{env1}' '{env2}' '{env3}'")
def step_env_add_mulitple_valid_and_invalid_setup(context, env1, env2, env3):
    context.args = [ env1, env2, env3 ]

#When  Test4 Add multiple valid and invalid valid environment names 'test1' 'test2' 'test3'
@when("Test4 Add multiple valid and invalid valid environment names '{env1}' '{env2}' '{env3}'")
def step_env_add_mulitple_valid_and_invalid_run(context, env1, env2, env3):
    res = subprocess.run(
        common.cmd + [ 'env', 'add' ] + list(context.args),
        capture_output = True,
        text = True
    )
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()
    context.returncode = res.returncode

@then("Test4 Return value is '{res}'")
def step_env_add_mulitple_valid_and_invalid_check(context, res):
    expected = int(res)
    actual = context.returncode
    assert expected == actual, \
        f"Expected '{expected}', but got '{actual}'"

@then("Test4 Get error message")
def step_env_add_mulitple_valid_and_invalid_check(context):
    expected = context.text.strip()
    actual = context.stderr.strip()
    assert expected == actual, \
        f"Expected\n'{expected}',\n\nbut got:\n'{actual}'"


@given("Test5 Single already existing environment name '{env}'")
def step_env_add_valid_existing(context, env):
    context.args = [ env ]

@when("Test5 Add single valid environment name '{env}'")
def step_env_add_valid_existing_run(context, env):
    res = subprocess.run(
        common.cmd + [ 'env', 'add' ] + list(context.args),
        capture_output = True,
        text = True
    )
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()
    context.returncode = res.returncode

@then("Test5 Return value is '{res}'")
def step_env_add_valid_existing_check(context, res):
    expected = int(res)
    actual = context.returncode
    assert expected == actual, \
        f"Expected '{expected}', but got '{actual}'"


@given("Test6 Multiple environment names '{env1}' '{env2}' '{env3}' '{env4}' '{env5}' '{env6}'")
def step_env_remove_setup(context, env1, env2, env3, env4, env5, env6):
    context.args = [env1, env2, env3, env4, env5, env6, ]

# TODO: add the case when user have to press 'Y' to confirm remove
@when("Test6 Remove environments '{env1}' '{env2}' '{env3}' '{env4}' '{env5}' '{env6}'")
def step_env_remove_run(context, env1, env2, env3, env4, env5, env6):
    res = subprocess.run(
        common.cmd + [ 'env', 'rm', '-f' ] + list(context.args),
        capture_output = True,
        text = True
    )
    context.stdout = res.stdout.strip()
    context.stderr = res.stderr.strip()
    context.returncode = res.returncode

@then("Test6 Return value is '{res}'")
def step_env_remove_check(context, res):
    expected = int(res)
    actual = context.returncode
    assert expected == actual, \
        f"Expected '{expected}', but got '{actual}'"
