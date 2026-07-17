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
