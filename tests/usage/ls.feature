@ls
Feature: ls
  List tasks in current or specified environment/desk

  Scenario: List all tasks in current environment
    Given tasks "lstest1" "lstest2" "lstest3" exist
    When I run "ls"
    Then the exit code should be 0
    And stdout should contain "lstest1"
    And stdout should contain "lstest2"
    And stdout should contain "lstest3"

  Scenario: List tasks with toggles only
    Given tasks "lstest4" "lstest5" exist
    And I run "cd lstest4"
    When I run "ls -t"
    Then the exit code should be 0
    And stdout should contain "lstest4"

  Scenario: List tasks in specific desk
    Given tasks "lstest6" "lstest7" exist
    When I run "ls -d desk"
    Then the exit code should be 0
    And stdout should contain "lstest6"

  Scenario: List tasks with headers
    Given a task "lstest8" exists
    When I run "ls -H"
    Then the exit code should be 0

  Scenario: List all tasks including done
    Given a task "lstest9" exists
    When I run "ls -a"
    Then the exit code should be 0
    And stdout should contain "lstest9"

  Scenario: -a and -t are not compatible
    When I run "ls -a -t"
    Then the exit code should not be 0
    And stderr should contain "are not compatible"

  Scenario: -q suppresses errors for a non-existent env
    When I run "ls -q bogusenv"
    Then the exit code should not be 0
    And stderr should be
      """
      """

  Scenario: -v is under development
    When I run "ls -v"
    Then the exit code should not be 0
    And stderr should contain "under development"
