@desk-ls
Feature: desk ls
  List all desks

  Scenario: List all desks
    Given a desk "desktest2" exists
    When I run "desk ls"
    Then the exit code should be 0
    And stdout should contain "desktest2"

  Scenario: List multiple desks
    Given a desk "desktest2a" exists
    And a desk "desktest2b" exists
    When I run "desk ls"
    Then the exit code should be 0
    And stdout should contain "desktest2a"
    And stdout should contain "desktest2b"

  Scenario: List when no desks exist
    When I run "desk ls"
    Then the exit code should be 0

  Scenario: List desks for a specific environment argument
    Given an env "envtest2c" with desk "desktest2c" exists
    When I run "desk ls envtest2c"
    Then the exit code should be 0
    And stdout should contain "desktest2c"

  Scenario: -q suppresses the error for a nonexistent environment
    When I run "desk ls -q bogusenv"
    Then the exit code should not be 0
    And stderr should be
      """
      """
