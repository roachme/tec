@cd
Feature: cd
  Change directory to task, desk, or environment

  @TestCd1
  Scenario: CD to valid task
    Given TestCd1 Task ID 'cdtest1' exists
    When TestCd1 I run "cd cdtest1"
    Then TestCd1 Return code should be 0
    And TestCd1 PWD changes to task 'cdtest1'
    And TestCd1 Task 'cdtest1' is set as current toggle

  @TestCd2
  Scenario: CD with dash alias to previous task
    Given TestCd2 Task IDs 'cdtest2' and 'cdtest3' exist
    And TestCd2 I cd to 'cdtest2'
    And TestCd2 I cd to 'cdtest3'
    When TestCd2 I run "cd -"
    Then TestCd2 Return code should be 0
    And TestCd2 PWD changes to task 'cdtest2'

  @TestCd3
  Scenario: CD to task in specific desk using options
    Given TestCd3 Task 'cdtest4' exists
    When TestCd3 I run "cd -d desk -e test cdtest4"
    Then TestCd3 Return code should be 0
    And TestCd3 PWD changes to task 'cdtest4'

  @TestCd4
  Scenario: CD to non-existent task returns error
    Given TestCd4 No preconditions
    When TestCd4 I run "cd badtask"
    Then TestCd4 Return code should not be 0
    And TestCd4 Error message should contain "no such object"
