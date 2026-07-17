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
