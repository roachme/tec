@env-add
Feature: env add

  Scenario: Add a single valid environment
    When I run "env add bddtest"
    Then the exit code should be 0

  Scenario: Reject an environment name containing a space
    When I run "env add 'test me'"
    Then the exit code should be 1
    And stderr should be
      """
      tec: cannot add environment 'test me': illegal environment name
      """

  Scenario: Add multiple valid environments at once
    When I run "env add test1 test2 test3"
    Then the exit code should be 0

  Scenario: Reject a batch containing one invalid name
    When I run "env add test11 'test me' test33"
    Then the exit code should be 1
    And stderr should be
      """
      tec: cannot add environment 'test me': illegal environment name
      """

  Scenario: Reject an environment that already exists
    Given an env "bddtest" exists
    When I run "env add bddtest"
    Then the exit code should be 1

  Scenario: Reject a missing environment name
    When I run "env add"
    Then the exit code should be 1
    And stderr should be
      """
      tec: environment name is required
      """

  Scenario: Add an environment with a custom desk name via -d
    When I run "env add -d customdsk envwithd"
    Then the exit code should be 0
    And the PWD should be the desk "customdsk" in env "envwithd"

  Scenario: Add with -n switches directory but not the current environment
    Given an env "envnbase" exists
    And I run "env cd envnbase"
    When I run "env add -n envnflag"
    Then the exit code should be 0
    And the PWD should be the desk "desk" in env "envnflag"
    And the current environment should be "envnbase"

  Scenario: Add with -q suppresses the duplicate environment error
    Given an env "envqdup" exists
    When I run "env add -q envqdup"
    Then the exit code should be 1
    And stderr should be
      """
      """

  Scenario: Add with -N neither switches environment nor directory
    Given an env "envnbase2" exists
    And I run "env cd envnbase2"
    When I run "env add -N envntest"
    Then the exit code should be 0
    And the PWD file should be empty
    And the current environment should be "envnbase2"

  Scenario: -D is not implemented yet
    When I run "env add -D 'some desc' envdopt"
    Then the exit code should be 1
    And stderr should contain "not implemented yet"
