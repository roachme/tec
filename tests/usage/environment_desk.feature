Feature: Environment and Desk Management
  As a developer using tec
  I want to manage environments and desks
  So that I can organize tasks into different contexts

  Scenario: List environments
    When I run tec command "env"
    Then the command should succeed

  Scenario: Add a new environment
    When I run tec command "env add development"
    Then the command should succeed
    When I run tec command "env"
    Then the output should contain "development"

  Scenario: Remove an environment
    Given I run tec command "env add temp-env"
    When I run tec command "env rm temp-env"
    Then the command should succeed
    When I run tec command "env"
    Then the output should not contain "temp-env"

  Scenario: List desks
    When I run tec command "desk"
    Then the command should succeed

  Scenario: Add a new desk
    When I run tec command "desk add my-desk"
    Then the command should succeed
    When I run tec command "desk"
    Then the output should contain "my-desk"

  Scenario: Remove a desk
    Given I run tec command "desk add temp-desk"
    When I run tec command "desk rm temp-desk"
    Then the command should succeed
    When I run tec command "desk"
    Then the output should not contain "temp-desk"

  Scenario: Cannot add duplicate environment
    Given I run tec command "env add duplicate"
    When I run tec command "env add duplicate"
    Then the command should fail

  Scenario: Cannot add duplicate desk
    Given I run tec command "desk add duplicate"
    When I run tec command "desk add duplicate"
    Then the command should fail
