@env-rename
Feature: env rename
  Rename environment

  Scenario: Rename environment
    Given an env "envtest6" exists
    When I run "env rename envtest6 envtest6new"
    Then the exit code should be 0

  Scenario: Rename nonexistent environment
    When I run "env rename nonexistent newname"
    Then the exit code should not be 0
