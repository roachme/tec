import os
import shutil
import subprocess
from steps import common

class Task:
    def __init__(self):
        self.cmd = [ common.program, '-H', 'off', '-T', common.taskbase ]

    def task_init(self):
        res = subprocess.run(self.cmd + [ 'init' ], capture_output=True)
        res = subprocess.run(self.cmd + [ 'env', 'add', 'test' ], capture_output=True)

    def reset_test_env(self):
        """Drop and recreate the 'test' env, so each scenario starts from a
        clean slate regardless of what earlier scenarios in the feature did."""
        res = subprocess.run(self.cmd + [ 'env', 'rm', '-f', 'test' ], capture_output=True)
        res = subprocess.run(self.cmd + [ 'env', 'add', 'test' ], capture_output=True)

    def cleanup_test_env(self):
        # Best-effort: may legitimately fail (e.g. a @skip'd feature whose
        # scenarios never ran before_scenario, so 'test' was never created).
        # Never let this internal plumbing print to the terminal.
        res = subprocess.run(self.cmd + [ 'env', 'rm', '-f', 'test' ], capture_output=True)

    def full_reset(self):
        """Wipe the whole task database and re-init from scratch. 'tec init'
        on an existing database is a no-op (doesn't clear it - verified
        empirically), so tags that manage top-level envs themselves (env-*)
        need the directory removed outright to get a clean slate per scenario."""
        shutil.rmtree(common.taskbase, ignore_errors=True)
        res = subprocess.run(self.cmd + [ 'init' ], capture_output=True)

    def full_reset_with_test_env(self):
        """Same as full_reset(), but also recreates the 'test' env - for tags
        whose scenarios assume an ambient default env is already current
        (desk-* commands run against 'test' unless -e names another env)."""
        shutil.rmtree(common.taskbase, ignore_errors=True)
        self.task_init()

# Feature tags whose scenarios each get a freshly reset 'test' env, so
# scenarios (including Scenario Outline rows) never depend on state left
# behind by an earlier scenario in the same feature. Fixtures for these
# tags are declared per-scenario in the .feature file itself (see
# steps/fixtures.py) instead of being pre-seeded here.
PER_SCENARIO_RESET_TAGS = { "cat", "rm" }

# Feature tags that manage top-level envs themselves (env-add creates and
# removes arbitrary env names). These get a full task-database wipe before
# each scenario instead of just resetting the 'test' env.
FULL_RESET_TAGS = { "env-add", "env-ls", "env-cd", "env-cat", "env-rm", "env-set", "env-rename" }

# Feature tags whose fixtures mix 'test'-env desks with arbitrary custom envs
# (envtest4b, envmv2, ...). Same full wipe as FULL_RESET_TAGS, but also
# recreates 'test' as the ambient default env desk-* (and add's -e option)
# scenarios expect.
DESK_RESET_TAGS = { "add", "desk-add", "desk-cat", "desk-cd", "desk-ls", "desk-mv", "desk-rm", "desk-set" }

"""
feature members:
    feature.name
    feature.filename
    feature.tags
"""
def before_feature(context, feature):
    task = Task()

    tag = feature.tags[0] if feature.tags else None

    if tag in PER_SCENARIO_RESET_TAGS:
        task.task_init()
    elif tag in FULL_RESET_TAGS or tag in DESK_RESET_TAGS:
        pass  # before_scenario fully resets the database ahead of every scenario
    elif tag in ["cd", "ls", "mv", "set", "desk", "env", "cfg"]:
        task.task_init()
    elif tag == "init":
        pass  # No initialization needed for init tests
    elif tag == "version":
        pass  # No initialization needed for version tests

def before_scenario(context, scenario):
    tag = scenario.feature.tags[0] if scenario.feature.tags else None
    if tag in PER_SCENARIO_RESET_TAGS:
        Task().reset_test_env()
    elif tag in FULL_RESET_TAGS:
        Task().full_reset()
    elif tag in DESK_RESET_TAGS:
        Task().full_reset_with_test_env()

def after_feature(context, feature):
    task = Task()

    tag = feature.tags[0] if feature.tags else None

    if tag in PER_SCENARIO_RESET_TAGS or tag in DESK_RESET_TAGS:
        # DESK_RESET_TAGS scenarios recreate 'test' via full_reset_with_test_env().
        # Remove it here so the next feature's unconditional 'env add test'
        # (task_init) succeeds and re-switches into a fresh one, instead of
        # silently failing against a leftover 'test' and stranding the
        # ambient current-env/desk wherever this feature's last scenario left it.
        task.cleanup_test_env()
    elif tag in FULL_RESET_TAGS:
        pass  # full_reset() never creates 'test'; nothing to persist
    elif tag in ["desk", "env", "cfg", "cd", "ls", "mv", "set"]:
        # Cleanup test environment
        task.cleanup_test_env()
