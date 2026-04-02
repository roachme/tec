import os
import shutil
import tempfile
import subprocess

def before_all(context):
    """Set up test environment before all tests."""
    context.tec_binary = "/home/roach/utils/tec/_tec"
    context.original_cwd = os.getcwd()

def before_scenario(context, scenario):
    """Set up before each scenario."""
    context.temp_dir = tempfile.mkdtemp(prefix="tec_test_")
    context.test_workspace = os.path.join(context.temp_dir, "workspace")
    os.makedirs(context.test_workspace, exist_ok=True)

    result = subprocess.run(
        [context.tec_binary, '-T', context.test_workspace, 'init'],
        capture_output=True,
        text=True
    )

    context.created_tasks = []
    context.process_result = None

def after_scenario(context, scenario):
    """Clean up after each scenario."""
    if hasattr(context, 'temp_dir') and os.path.exists(context.temp_dir):
        try:
            shutil.rmtree(context.temp_dir)
        except Exception as e:
            print(f"Warning: Failed to clean up temp directory {context.temp_dir}: {e}")

def after_all(context):
    """Clean up after all tests."""
    os.chdir(context.original_cwd)
