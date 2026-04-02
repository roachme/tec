from behave import given, when, then
import subprocess
import os

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
        timeout=5
    )
    context.process_result = result
    context.stdout = result.stdout
    context.stderr = result.stderr
    context.returncode = result.returncode
    return result

@given('Create task ID "{task_id}"')
def step_create_task(context, task_id):
    """Creates a new task with the specified ID."""
    result = run_tec_command(context, "add", task_id)
    context.last_task_id = task_id
    assert result.returncode == 0, f"Failed to create task {task_id}: {result.stderr}"

@given('the configuration file "{filename}" exists')
def step_config_exists(context, filename):
    """Verifies that a configuration file exists."""
    assert os.path.exists(filename), f"Configuration file '{filename}' does not exist"
    context.filename = filename

@when('I read the file "{filename}"')
def step_read_file(context, filename):
    """Reads a file and stores its content in the context."""
    try:
        with open(filename, 'r') as f:
            context.file_content = f.read()
    except FileNotFoundError:
        assert False, f"The file '{filename}' was not found."

@then('the file should contain "{text}"')
def step_file_contains(context, text):
    """Verifies that the file content contains the specified text."""
    assert text in context.file_content, \
        f"Expected to find '{text}' in the file, but it was not there."
