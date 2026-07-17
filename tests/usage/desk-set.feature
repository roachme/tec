@desk-set
Feature: desk set
  Set desk properties

  Scenario: Set desk description
    Given a desk "desktest6" exists
    When I run "desk set -D 'Test desk description' desktest6"
    Then the exit code should be 0

  Scenario: Set desk description for nonexistent desk
    When I run "desk set -D 'Test' nonexistent"
    Then the exit code should not be 0

  Scenario: Set desk description in specific environment
    Given an env "envtest6b" with desk "desktest6b" exists
    When I run "desk set -D 'Test desk' desktest6b"
    Then the exit code should be 0
