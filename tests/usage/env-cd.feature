@env-cd
Feature: env cd
  Change to environment

  Scenario: Change to environment
    Given an env "envtest2" exists
    When I run "env cd envtest2"
    Then the exit code should be 0
    And the PWD should be the env "envtest2"

  Scenario: Change to environment with desk option
    Given an env "envtest2b" with desk "desktest2b" exists
    When I run "env cd -d desktest2b envtest2b"
    Then the exit code should be 0
    And the PWD should be the desk "desktest2b" in env "envtest2b"

  Scenario: Change to nonexistent environment
    When I run "env cd nonexistent"
    Then the exit code should not be 0

  Scenario: Change to environment with -n flag (no toggle)
    Given an env "envtest2c" exists
    When I run "env cd -n envtest2c"
    Then the exit code should be 0
    And the PWD should be the env "envtest2c"

  Scenario: Change to previous environment using "-" alias
    Given an env "envprev1" exists
    And an env "envprev2" exists
    And I run "env cd envprev1"
    And I run "env cd envprev2"
    When I run "env cd -"
    Then the exit code should be 0
    And the PWD should be the env "envprev1"
