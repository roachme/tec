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
