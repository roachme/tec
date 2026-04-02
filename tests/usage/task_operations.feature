Feature: Task Operations
  As a developer using tec
  I want to modify and remove tasks
  So that I can maintain my task workspace

  Scenario: Remove an existing task
    Given I run tec command "add task-to-remove"
    When I run tec command "rm task-to-remove"
    Then the command should succeed
    When I run tec command "ls"
    Then the output should not contain "task-to-remove"

  Scenario: Remove non-existent task fails
    When I run tec command "rm nonexistent-task"
    Then the command should fail

  Scenario: Move/rename a task
    Given I run tec command "add oldname"
    When I run tec command "mv oldname newname"
    Then the command should succeed
    When I run tec command "ls"
    Then the output should contain "newname"
    And the output should not contain "oldname"

  Scenario: Move to existing task name fails
    Given I run tec command "add task1"
    And I run tec command "add task2"
    When I run tec command "mv task1 task2"
    Then the command should fail

  Scenario: Set task property
    Given I run tec command "add mytask"
    When I run tec command "set mytask priority high"
    Then the command should succeed
    When I run tec command "cat mytask"
    Then the output should contain "priority"
    And the output should contain "high"

  Scenario: Set multiple properties on task
    Given I run tec command "add project123"
    When I run tec command "set project123 status active"
    And I run tec command "set project123 owner alice"
    Then the command should succeed
    When I run tec command "cat project123"
    Then the output should contain "status"
    And the output should contain "active"
    And the output should contain "owner"
    And the output should contain "alice"

  Scenario: Set property on non-existent task fails
    When I run tec command "set nonexistent key value"
    Then the command should fail

  Scenario: Remove multiple tasks
    Given I run tec command "add temp1"
    And I run tec command "add temp2"
    And I run tec command "add temp3"
    When I run tec command "rm temp1"
    And I run tec command "rm temp2"
    And I run tec command "rm temp3"
    Then the command should succeed
    When I run tec command "ls"
    Then the output should not contain "temp1"
    And the output should not contain "temp2"
    And the output should not contain "temp3"
