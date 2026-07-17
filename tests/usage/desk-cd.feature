@desk-cd
Feature: desk cd
  Change to desk

  Scenario: Change to desk
    Given a desk "desktest3" exists
    When I run "desk cd desktest3"
    Then the exit code should be 0
    And the PWD should be the desk "desktest3" in env "test"

  Scenario: Change to desk in specific environment
    Given an env "envtest3b" with desk "desktest3b" exists
    When I run "desk cd -e envtest3b desktest3b"
    Then the exit code should be 0
    And the PWD should be the desk "desktest3b" in env "envtest3b"

  Scenario: Change to nonexistent desk
    When I run "desk cd nonexistent"
    Then the exit code should not be 0

  Scenario: Change to desk with -n flag (no toggle)
    Given a desk "desktest3c" exists
    When I run "desk cd -n desktest3c"
    Then the exit code should be 0
    And the PWD should be the desk "desktest3c" in env "test"

  Scenario: Change to previous desk using "-" alias
    Given a desk "deskprev1" exists
    And a desk "deskprev2" exists
    And I run "desk cd deskprev1"
    And I run "desk cd deskprev2"
    When I run "desk cd -"
    Then the exit code should be 0
    And the PWD should be the desk "deskprev1" in env "test"
