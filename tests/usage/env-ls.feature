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

  Scenario: List only current and previous environments with -t
    Given an env "envtest1c" exists
    And an env "envtest1d" exists
    And an env "envtest1e" exists
    And I run "env cd envtest1c"
    And I run "env cd envtest1d"
    When I run "env ls -t"
    Then the exit code should be 0
    And stdout should contain "envtest1c"
    And stdout should contain "envtest1d"
    And stdout should not contain "envtest1e"

  Scenario: -v is under development
    When I run "env ls -v"
    Then the exit code should not be 0
    And stderr should contain "under development"
