import os
import subprocess
from steps import common

class Task:
    def __init__(self):
        self.cmd = [ common.program, '-H', 'off', '-T', common.taskbase ]

    def task_init(self):
        res = subprocess.run(self.cmd + [ 'init' ])
        res = subprocess.run(self.cmd + [ 'env', 'add', 'test' ])

    def add_before(self):
        pass

    def add_after(self):
        res = subprocess.run(self.cmd + [ 'env', 'rm', '-f', 'test' ])

    def cat_before(self):
        tasks = [ 'test1', 'test2', 'test3' ]
        res = subprocess.run( self.cmd + [ 'add' ] + tasks)

    def cat_after(self):
        tasks = [ 'test1', 'test2', 'test3' ]
        res = subprocess.run(self.cmd + [ 'rm', '-f' ] + tasks)

    def cd_before(self):
        pass
    def cd_after(self):
        pass

    def ls_before(self):
        pass
    def ls_after(self):
        pass

    def mv_before(self):
        pass
    def mv_after(self):
        pass

    def rm_before(self):
        pass
    def rm_after(self):
        pass

    def set_before(self):
        pass
    def set_after(self):
        pass

class Desk:
    pass

class Env:
    pass

class Cfg:
    pass



"""
feature members:
    feature.name
    feature.filename
    feature.tags
"""
def before_feature(context, feature):
    task = Task()
    desk = Desk()
    env = Env()
    tags = [ "add", "cat", "cd", "ls", "mv", "rm", "set", "desk", "env", "env-add",
             "env-ls", "env-cd", "env-cat", "env-rm", "env-set", "env-rename",
             "desk-add", "desk-ls", "desk-cd", "desk-cat", "desk-rm", "desk-set", "desk-mv",
             "cfg", "init", "version" ]

    tag = feature.tags[0] if feature.tags else None

    if tag == "add":
        task.task_init()
        task.add_before()
    elif tag == "cat":
        task.task_init()
        task.cat_before()
    elif tag == "cd":
        task.task_init()
        task.cd_before()
    elif tag == "ls":
        task.task_init()
        task.ls_before()
    elif tag == "mv":
        task.task_init()
        task.mv_before()
    elif tag == "rm":
        task.task_init()
        task.rm_before()
    elif tag == "set":
        task.task_init()
        task.set_before()
    elif tag in ["desk", "env", "env-add", "env-ls", "env-cd", "env-cat", "env-rm", "env-set", "env-rename",
                 "desk-add", "desk-ls", "desk-cd", "desk-cat", "desk-rm", "desk-set", "desk-mv"]:
        task.task_init()
    elif tag == "cfg":
        task.task_init()
    elif tag == "init":
        pass  # No initialization needed for init tests
    elif tag == "version":
        pass  # No initialization needed for version tests

def after_feature(context, feature):
    task = Task()

    tag = feature.tags[0] if feature.tags else None

    if tag == "add":
        task.add_after()
    elif tag == "cat":
        task.cat_after()
    elif tag in ["desk", "env", "env-add", "env-ls", "env-cd", "env-cat", "env-rm", "env-set", "env-rename",
                 "desk-add", "desk-ls", "desk-cd", "desk-cat", "desk-rm", "desk-set", "desk-mv",
                 "cfg", "cd", "ls", "mv", "set", "rm"]:
        # Cleanup test environment
        task.add_after()
