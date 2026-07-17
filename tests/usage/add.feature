@add
Feature: add
  Scenario: Add a single new task
    When I run "add task1"
    Then the exit code should be 0
    And stderr should be
      """
      """
    And the PWD should be the task "task1"
    And the current task should be "task1"

  Scenario: Add multiple new tasks at once
    When I run "add task2 task3"
    Then the exit code should be 0
    And stderr should be
      """
      """
    And the current task should be "task3"

  Scenario Outline: Reject invalid task IDs
    Given a task "task1" exists
    When I run "add <taskid>"
    Then the exit code should be <exitcode>
    And stderr should be
      """
      <error>
      """
    And the PWD file should be empty

    Examples: Invalid task IDs
      | taskid        | exitcode | error                                                      |
      | task1         | 1        | tec: cannot add task 'task1': task ID already exists       |
      | toolongtaskid | 1        | tec: cannot add task 'toolongtaskid': task ID is too long  |
      | @illegal      | 1        | tec: cannot add task '@illegal': illegal task ID           |
