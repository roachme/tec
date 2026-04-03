@ls
Feature: ls
  List tasks in current or specified environment/desk

  @TestLs1
  Scenario: List all tasks in current environment
    Given TestLs1 Tasks 'lstest1' 'lstest2' 'lstest3' exist
    When TestLs1 I run "ls"
    Then TestLs1 Return code should be 0
    And TestLs1 Output should contain "lstest1"
    And TestLs1 Output should contain "lstest2"
    And TestLs1 Output should contain "lstest3"

  @TestLs2
  Scenario: List tasks with toggles only
    Given TestLs2 Tasks 'lstest4' 'lstest5' exist
    And TestLs2 I cd to 'lstest4'
    When TestLs2 I run "ls -t"
    Then TestLs2 Return code should be 0
    And TestLs2 Output should contain "lstest4"

  @TestLs3
  Scenario: List tasks in specific desk
    Given TestLs3 Tasks 'lstest6' 'lstest7' exist
    When TestLs3 I run "ls -d desk"
    Then TestLs3 Return code should be 0
    And TestLs3 Output should contain "lstest6"

  @TestLs4
  Scenario: List tasks with headers
    Given TestLs4 Tasks 'lstest8' exist
    When TestLs4 I run "ls -H"
    Then TestLs4 Return code should be 0
