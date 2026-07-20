@rm
Feature: Remove

  Background:
    Given a task "task1" exists

  Scenario: Remove a single existing task
    When I run "rm -f task1"
    Then the exit code should be 0
    And stderr should be
      """
      """

  Scenario: Remove multiple existing tasks
    Given a task "task2" exists
    When I run "rm -f task1 task2"
    Then the exit code should be 0
    And stderr should be
      """
      """

  Scenario: Remove a non-existing task
    When I run "rm -f task100"
    Then the exit code should be 1
    And stderr should be
      """
      tec: cannot remove task 'task100': no such task ID
      """

  Scenario: Remove with -q suppresses the error for a non-existing task
    When I run "rm -q task100"
    Then the exit code should be 1
    And stderr should be
      """
      """

  Scenario: Remove with -v explains what was removed
    When I run "rm -fv task1"
    Then the exit code should be 0
    And stdout should contain "removed task 'task1'"

  Scenario: Remove with -i prompts and removes on "y"
    When I answer "y" when running "rm -i task1"
    Then the exit code should be 0
    And stderr should contain "remove task 'task1'? [y/N]"

  Scenario: Remove with -i prompts and keeps the task on "n"
    When I answer "n" when running "rm -i task1"
    Then the exit code should be 0
    When I run "cat task1"
    Then the exit code should be 0

  Scenario: Remove with -I prompts once for more than three tasks and accepts
    Given tasks "task2" "task3" "task4" exist
    When I answer "y" when running "rm -I task1 task2 task3 task4"
    Then the exit code should be 0
    And stderr should contain "remove 4 tasks? [y/N]"
    When I run "cat task1"
    Then the exit code should not be 0

  Scenario: Remove with -I prompts once for more than three tasks and declines
    Given tasks "task2" "task3" "task4" exist
    When I answer "n" when running "rm -I task1 task2 task3 task4"
    Then the exit code should be 0
    When I run "cat task1"
    Then the exit code should be 0

  Scenario: Remove with -I does not prompt for three or fewer tasks
    Given tasks "task2" "task3" exist
    When I run "rm -I task1 task2 task3"
    Then the exit code should be 0
    And stderr should be
      """
      """
