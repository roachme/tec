@add
Feature: add
  Scenario: TestAdd1 Add single valid task ID
    Given TestAdd1 Single task ID 'task1'
    When TestAdd1 Add single valid task ID 'task1'
    Then TestAdd1 Return value is '0'
    Then TestAdd1 There empty stderr
    Then TestAdd1 PWD changes to task created 'task1'
    Then TestAdd1 Task is added to toggles

  Scenario: TestAdd3 Add multiple valid task ID
    Given TestAdd3 Single multiple task IDs 'task2' 'task3'
    When TestAdd3 Add multiple valid task IDs 'task2' 'task3'
    Then TestAdd3 Return value is '0'
    And TestAdd3 There is empty stderr
    And TestAdd3 Last task is added to toggles

  Scenario Outline: Validate bunch of task IDs
    Given Bunch of task IDs "<taskid>"
    Then Bunch of task IDs result should be "<result>" and "<error>" and empty "<pwd>"
    Examples: Invalid task IDs
      | taskid         | result | pwd | error                                                      |
      | task1          | 1      | 0   | tec: cannot add task 'task1': object already exists        |
      | toolongtaskid  | 1      | 0   | tec: cannot add task 'toolongtaskid': task ID is too long  |
      | @illegal       | 1      | 0   | tec: cannot add task '@illegal': illegal object name       |
