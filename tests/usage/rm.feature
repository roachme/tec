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
