@set
Feature: set
  Set task properties like description, type, and priority

  @TestSet1
  Scenario: Set task description
    Given TestSet1 Task 'settest1' exists
    When TestSet1 I run "set -D 'New description' settest1"
    Then TestSet1 Return code should be 0
    And TestSet1 Task 'settest1' description should be 'New description'

  @TestSet2
  Scenario: Set task type to bugfix
    Given TestSet2 Task 'settest2' exists
    When TestSet2 I run "set -T bugfix settest2"
    Then TestSet2 Return code should be 0
    And TestSet2 Task 'settest2' type should be 'bugfix'

  @TestSet3
  Scenario: Set task priority to high
    Given TestSet3 Task 'settest3' exists
    When TestSet3 I run "set -P high settest3"
    Then TestSet3 Return code should be 0
    And TestSet3 Task 'settest3' priority should be 'high'

  @TestSet4
  Scenario: Set invalid type returns error
    Given TestSet4 Task 'settest4' exists
    When TestSet4 I run "set -T invalidtype settest4"
    Then TestSet4 Return code should not be 0
    And TestSet4 Error should contain "invalid"
