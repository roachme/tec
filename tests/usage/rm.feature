@rm
Feature: Remove
  Scenario: TestRemove1 Add multiple valid task IDs
    Given TestRemove1 Multiple task IDs 'task1' 'task2' 'task3' 'task4' 'task5'
    When TestRemove1 Add multiple valid task IDs 'task1' 'task2' 'task3' 'task4' 'task5'
    Then TestRemove1 Return value is '0'

  Scenario: TestRemove2 Remove single valid task ID
    Given TestRemove2 Single task ID 'task1'
    When TestRemove2 Remove single valid task ID 'task1'
    Then TestRemove2 Return value is '0'
    And TestRemove2 There empty stderr
    # Add PWD changes to task created

  Scenario: TestRemove3 Remove multiple valid task IDs
    Given TestRemove3 Multiple task IDs 'task2' 'task3'
    When TestRemove3 Remove multiple valid task IDs 'task2' 'task3'
    Then TestRemove3 Return value is '0'
    And TestRemove3 There empty stderr
    # Add PWD changes to task created

  Scenario: TestRemove4 Remove single non existing task ID
    Given TestRemove4 Single non existing task ID 'task100'
    When TestRemove4 Remove single non existing ID 'task100'
    Then TestRemove4 Return value is '1'
    And TestRemove4 There empty stderr:
        """
        tec: cannot remove task 'task100': no such object
        """
    # Add PWD changes to task created

  Scenario: TestRemove5 Remove previous task ID
    Given TestRemove5 Single previous task ID 'task4'
    When TestRemove5 Remove previous task ID 'task4'
    Then TestRemove5 Return value is '0'
    And TestRemove5 There empty stderr
    # Add PWD changes to task created

  Scenario: TestRemove6 Remove current task ID
    Given TestRemove6 Single current task ID 'task5'
    When TestRemove6 Remove current task ID 'task5'
    Then TestRemove6 Return value is '0'
    And TestRemove6 There empty stderr
    # Add PWD changes to task created
