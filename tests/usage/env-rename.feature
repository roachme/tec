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

  Scenario: Rename to an existing environment name fails
    Given an env "renamea" exists
    And an env "renameb" exists
    When I run "env rename renamea renameb"
    Then the exit code should not be 0
    And stderr should contain "destination env already exists"

  Scenario: Rename with -q suppresses the error output
    When I run "env rename -q nonexistent newname"
    Then the exit code should not be 0
    And stderr should be
      """
      """

  Scenario: Rename switches the current environment to the new name
    Given an env "renc" exists
    When I run "env rename renc rencnew"
    Then the exit code should be 0
    And the PWD should be the env "rencnew"
