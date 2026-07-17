@set
Feature: set
  Set task properties like description, type, and priority

  Scenario: Set task description
    Given a task "settest1" exists
    When I run "set -D 'New description' settest1"
    Then the exit code should be 0
    And task "settest1" desc should be "New description"

  Scenario: Set task type to bugfix
    Given a task "settest2" exists
    When I run "set -T bugfix settest2"
    Then the exit code should be 0
    And task "settest2" type should be "bugfix"

  Scenario: Set task priority to high
    Given a task "settest3" exists
    When I run "set -P high settest3"
    Then the exit code should be 0
    And task "settest3" prio should be "high"

  Scenario: Set invalid type returns error
    Given a task "settest4" exists
    When I run "set -T invalidtype settest4"
    Then the exit code should not be 0
    And stderr should contain "invalid"
