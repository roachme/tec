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

  Scenario: CD with -q suppresses the error output
    When I run "cd -q badtask"
    Then the exit code should not be 0
    And stderr should be
      """
      """

  Scenario: CD with -N neither switches directory nor updates the current task
    Given tasks "cdtest5" "cdtest6" exist
    And I run "cd cdtest5"
    When I run "cd -N cdtest6"
    Then the exit code should be 0
    And the PWD file should be empty
    And the current task should be "cdtest5"

  Scenario: CD with -n switches directory but not the current task
    Given tasks "cdtest7" "cdtest8" exist
    And I run "cd cdtest7"
    When I run "cd -n cdtest8"
    Then the exit code should be 0
    And the PWD should be the task "cdtest8"
    And the current task should be "cdtest7"

  Scenario: CD rejects the "-" alias combined with another task ID
    Given a task "cdtest9" exists
    When I run "cd cdtest9 -"
    Then the exit code should not be 0
    And stderr should contain "alias '-' is used alone"

  Scenario: CD rejects a double "-" alias
    When I run "cd - -"
    Then the exit code should not be 0
    And stderr should contain "alias '-' is used alone"
