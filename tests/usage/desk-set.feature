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

  Scenario: Set -q suppresses the error for a nonexistent desk
    When I run "desk set -q -D 'Test' nonexistent"
    Then the exit code should not be 0
    And stderr should be
      """
      """

  Scenario: Set an invalid description is rejected
    Given a desk "desktest6c" exists
    When I run "desk set -D '!!!invalid!!!' desktest6c"
    Then the exit code should not be 0
    And stderr should contain "invalid description"

  Scenario: Set description on multiple desks at once
    Given a desk "desktest6d" exists
    And a desk "desktest6e" exists
    When I run "desk set -D 'Shared desc' desktest6d desktest6e"
    Then the exit code should be 0
    When I run "desk cat -k desc desktest6d"
    Then stdout should contain "Shared desc"
    When I run "desk cat -k desc desktest6e"
    Then stdout should contain "Shared desc"
