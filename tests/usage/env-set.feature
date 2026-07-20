@env-set
Feature: env set
  Set environment properties

  Scenario: Set environment description
    Given an env "envtest5" exists
    When I run "env set -D 'Test environment' envtest5"
    Then the exit code should be 0

  Scenario: Set environment description for nonexistent environment
    When I run "env set -D 'Test' nonexistent"
    Then the exit code should not be 0

  Scenario: Set environment description with desk option
    Given an env "envtest5b" with desk "desktest5b" exists
    When I run "env set -D 'Test env' -d desktest5b envtest5b"
    Then the exit code should be 0

  Scenario: Set -q suppresses the error for a nonexistent environment
    When I run "env set -q -D 'Test' nonexistent"
    Then the exit code should not be 0
    And stderr should be
      """
      """

  Scenario: Set an invalid description is rejected
    Given an env "envtest5c" exists
    When I run "env set -D '!!!invalid!!!' envtest5c"
    Then the exit code should not be 0
    And stderr should contain "invalid description"

  Scenario: Set description on multiple environments at once
    Given an env "envtest5d" exists
    And an env "envtest5e" exists
    When I run "env set -D 'Shared desc' envtest5d envtest5e"
    Then the exit code should be 0
    When I run "env cat -k desc envtest5d"
    Then stdout should contain "Shared desc"
    When I run "env cat -k desc envtest5e"
    Then stdout should contain "Shared desc"
