Feature: Task Management
  As a developer using tec
  I want to create, list, and view tasks
  So that I can organize my work

  Scenario: Add a new task with valid ID
    When I run tec command "add 00000001"
    Then the command should succeed

  Scenario: Add multiple tasks
    When I run tec command "add 00000001"
    And I run tec command "add 00000002"
    And I run tec command "add 00000003"
    Then the command should succeed

  Scenario: List tasks shows added tasks
    Given I run tec command "add task001"
    And I run tec command "add task002"
    When I run tec command "ls"
    Then the command should succeed
    And the output should contain "task001"
    And the output should contain "task002"

  Scenario: Cannot add duplicate task
    Given I run tec command "add 00000001"
    When I run tec command "add 00000001"
    Then the command should fail

  Scenario: List tasks when no tasks exist
    When I run tec command "ls"
    Then the command should succeed

  Scenario: Cat command shows task information
    Given I run tec command "add mytask"
    When I run tec command "cat mytask"
    Then the command should succeed

  Scenario: Cat non-existent task fails
    When I run tec command "cat nonexistent"
    Then the command should fail

  Scenario: Add task with description
    When I run tec command "add testtask -d 'This is a test task'"
    Then the command should succeed
    When I run tec command "cat testtask"
    Then the output should contain "This is a test task"

  Scenario: Task IDs can be alphanumeric
    When I run tec command "add bug-fix-123"
    Then the command should succeed
    When I run tec command "ls"
    Then the output should contain "bug-fix-123"
