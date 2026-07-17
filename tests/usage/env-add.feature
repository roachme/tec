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
