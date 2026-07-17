@cd
Feature: cd
  Change directory to task, desk, or environment

  Scenario: CD to valid task
    Given a task "cdtest1" exists
    When I run "cd cdtest1"
    Then the exit code should be 0
    And the PWD should be the task "cdtest1"
    And the current task should be "cdtest1"

  Scenario: CD with dash alias to previous task
    Given tasks "cdtest2" "cdtest3" exist
    And I run "cd cdtest2"
    And I run "cd cdtest3"
    When I run "cd -"
    Then the exit code should be 0
    And the PWD should be the task "cdtest2"

  Scenario: CD to task in specific desk using options
    Given a task "cdtest4" exists
    When I run "cd -d desk -e test cdtest4"
    Then the exit code should be 0
    And the PWD should be the task "cdtest4"

  Scenario: CD to non-existent task returns error
    When I run "cd badtask"
    Then the exit code should not be 0
    And stderr should contain "no such task ID"
