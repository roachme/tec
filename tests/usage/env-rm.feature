@env-rm
Feature: env rm
  Remove environment

  Scenario: Remove environment with force flag
    Given an env "envtest4" exists
    When I run "env rm -f envtest4"
    Then the exit code should be 0

  Scenario: Remove environment with verbose flag
    Given an env "envtest4b" exists
    When I run "env rm -fv envtest4b"
    Then the exit code should be 0

  Scenario: Remove nonexistent environment
    When I run "env rm -f nonexistent"
    Then the exit code should not be 0

  Scenario: Remove environment with desk specified
    Given an env "envtest4c" with desk "desktest4c" exists
    When I run "env rm -f -d desktest4c envtest4c"
    Then the exit code should be 0
