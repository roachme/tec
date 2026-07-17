@desk-rm
Feature: desk rm
  Remove desk

  Scenario: Remove desk with force flag
    Given a desk "desktest5" exists
    When I run "desk rm -f desktest5"
    Then the exit code should be 0

  Scenario: Remove desk with verbose flag
    Given a desk "desktest5b" exists
    When I run "desk rm -fv desktest5b"
    Then the exit code should be 0

  Scenario: Remove nonexistent desk
    When I run "desk rm -f nonexistent"
    Then the exit code should not be 0

  Scenario: Remove desk from specific environment
    Given an env "envtest5c" with desk "desktest5c" exists
    When I run "desk rm -f desktest5c"
    Then the exit code should be 0
