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

  Scenario: Add with -N neither switches task nor task directory
    Given a task "addnbase" exists
    And I run "cd addnbase"
    When I run "add -N addntest"
    Then the exit code should be 0
    And the PWD file should be empty
    And the current task should be "addnbase"

  Scenario: Add with -n does not update the current task toggle
    Given a task "addnfl1" exists
    When I run "add -n addnfl2"
    Then the exit code should be 0
    And the current task should be "addnfl1"

  Scenario: Add with -q suppresses the duplicate task error
    Given a task "addqtest" exists
    When I run "add -q addqtest"
    Then the exit code should be 1
    And stderr should be
      """
      """

  Scenario: Add task to a specific desk
    Given a desk "otherdsk" exists
    When I run "add -d otherdsk adddsk1"
    Then the exit code should be 0
    And the PWD should be the task "adddsk1" in desk "otherdsk" in env "test"

  Scenario: Add task to a specific environment
    Given an env "otherenv" exists
    When I run "add -e otherenv addenv1"
    Then the exit code should be 0
    And the PWD should be the task "addenv1" in desk "desk" in env "otherenv"
