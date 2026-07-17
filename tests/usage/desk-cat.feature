@desk-cat
Feature: desk cat
  Show desk properties

  Scenario: Show desk properties
    Given a desk "desktest4" exists
    When I run "desk cat desktest4"
    Then the exit code should be 0

  Scenario: Show nonexistent desk properties
    When I run "desk cat nonexistent"
    Then the exit code should not be 0

  Scenario: Show desk properties in specific environment
    Given an env "envtest4b" with desk "desktest4b" exists
    When I run "desk cat desktest4b"
    Then the exit code should be 0
