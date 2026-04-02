from behave import given, when, then
import subprocess
import os
import re

TEC_BINARY = "/home/roach/utils/tec/_tec"

def run_tec_command(context, *args):
    """Executes the tec command with the provided arguments."""
    cmd_args = [TEC_BINARY]

    if hasattr(context, 'test_workspace'):
        cmd_args.extend(['-T', context.test_workspace])

    cmd_args.extend(list(args))

    result = subprocess.run(
        cmd_args,
        capture_output=True,
        text=True,
        timeout=5,
        env=os.environ.copy()
    )
    context.process_result = result
    context.stdout = result.stdout
    context.stderr = result.stderr
    context.returncode = result.returncode
    return result

@given('I run tec command "{command}"')
def step_run_tec_command(context, command):
    """Runs a tec command with arguments."""
    args = command.split() if command.strip() else []
    run_tec_command(context, *args)

@when('I run tec command "{command}"')
def step_when_run_tec_command(context, command):
    """Runs a tec command with arguments."""
    args = command.split() if command.strip() else []
    run_tec_command(context, *args)

@when('I run tec with no arguments')
def step_when_run_tec_no_args(context):
    """Runs tec with no arguments."""
    run_tec_command(context)

@when('I run tec with arguments')
def step_run_with_arguments(context):
    """Runs tec with arguments from the data table."""
    args = [row['arg'] for row in context.table]
    run_tec_command(context, *args)

@then('the command should succeed')
def step_command_succeeds(context):
    """Verifies that the last command succeeded."""
    assert context.returncode == 0, \
        f"Command failed with return code {context.returncode}\nSTDERR: {context.stderr}\nSTDOUT: {context.stdout}"

@then('the command should fail')
def step_command_fails(context):
    """Verifies that the last command failed."""
    assert context.returncode != 0, \
        f"Command succeeded when it should have failed\nSTDOUT: {context.stdout}"

@then('the command should fail with code {code:d}')
def step_command_fails_with_code(context, code):
    """Verifies that the command failed with a specific return code."""
    assert context.returncode == code, \
        f"Expected return code {code} but got {context.returncode}\nSTDERR: {context.stderr}"

@then('the output should contain "{text}"')
def step_output_contains(context, text):
    """Verifies that the command output contains the specified text."""
    assert text in context.stdout, \
        f"Expected to find '{text}' in output but got:\n{context.stdout}"

@then('the output should not contain "{text}"')
def step_output_not_contains(context, text):
    """Verifies that the command output does not contain the specified text."""
    assert text not in context.stdout, \
        f"Did not expect to find '{text}' in output but it was there:\n{context.stdout}"

@then('the output should match "{pattern}"')
def step_output_matches(context, pattern):
    """Verifies that the output matches a regex pattern."""
    assert re.search(pattern, context.stdout), \
        f"Output did not match pattern '{pattern}'\nGot: {context.stdout}"

@then('the error output should contain "{text}"')
def step_stderr_contains(context, text):
    """Verifies that the error output contains the specified text."""
    assert text in context.stderr, \
        f"Expected to find '{text}' in stderr but got:\n{context.stderr}"

@then('the output should be empty')
def step_output_empty(context):
    """Verifies that the output is empty."""
    assert context.stdout.strip() == "", \
        f"Expected empty output but got:\n{context.stdout}"
