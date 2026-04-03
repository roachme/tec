import os

# Get the path to the project root (two directories up from tests/usage/steps/)
project_root = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..', '..'))
program = os.path.join(project_root, "build", "_tec")
pwdfile = "/tmp/tecpwd"
taskbase = os.path.expanduser("~/tectask")
cmd = [program, '-H', 'off', '-T', taskbase, '-P', 'pgn' ]

# Test environment constants
TEST_ENV = "test"
TEST_DESK = "desk"

def read_pwd_file():
    """Read and return the contents of the PWD file."""
    try:
        with open(pwdfile) as f:
            return f.read().strip()
    except FileNotFoundError:
        return ""

def get_expected_path(env=None, desk=None, task=None):
    """Construct expected path for env/desk/task."""
    parts = [taskbase]
    if env:
        parts.append(env)
    if desk:
        parts.append(desk)
    if task:
        parts.append(task)
    return "/".join(parts)

def verify_pwd(expected_path):
    """Verify PWD file matches expected path."""
    actual = read_pwd_file().rstrip('/')
    expected = expected_path.rstrip('/')
    assert actual == expected, \
        f"Expected PWD:\n'{expected}',\n\nbut got:\n'{actual}'"

