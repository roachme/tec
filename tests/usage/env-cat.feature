@env-cat
Feature: env cat
  Show environment properties

  Scenario: Show environment properties
    Given an env "envtest3" exists
    When I run "env cat envtest3"
    Then the exit code should be 0

  Scenario: Show nonexistent environment properties
    When I run "env cat nonexistent"
    Then the exit code should not be 0

  Scenario: Show environment with desk properties
    Given an env "envtest3c" with desk "desktest3c" exists
    When I run "env cat -d desktest3c envtest3c"
    Then the exit code should be 0
