Feature: Basic Commands
  As a developer using tec
  I want to access help and version information
  So that I can learn how to use the tool

  Scenario: Show version information
    When I run tec command "version"
    Then the command should succeed
    And the output should contain "0.5"

  Scenario: Show help information
    When I run tec command "help"
    Then the command should succeed
    And the output should contain "tec"

  Scenario: Show help for specific command
    When I run tec command "help add"
    Then the command should succeed
    And the output should contain "add"

  Scenario: Run tec without arguments shows help
    When I run tec with no arguments
    Then the command should fail
    And the output should contain "Usage:"

  Scenario: Invalid command shows error
    When I run tec command "invalidcommand"
    Then the command should fail
