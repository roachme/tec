@env-ls
Feature: env ls
  List all environments

  Scenario: List all environments
    Given an env "envtest1" exists
    When I run "env ls"
    Then the exit code should be 0
    And stdout should contain "envtest1"

  Scenario: List multiple environments
    Given an env "envtest1a" exists
    And an env "envtest1b" exists
    When I run "env ls"
    Then the exit code should be 0
    And stdout should contain "envtest1a"
    And stdout should contain "envtest1b"

  Scenario: List when no custom environments exist
    When I run "env ls"
    Then the exit code should be 0
