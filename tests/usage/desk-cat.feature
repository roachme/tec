@desk-cat
Feature: desk cat
  Show desk properties

  Scenario: Show desk properties
    Given a desk "desktest4" exists
    When I run "desk cat desktest4"
    Then the exit code should be 0

  Scenario: Show nonexistent desk properties
    When I run "desk cat nonexistent"
    Then the exit code should not be 0

  Scenario: Show desk properties in specific environment
    Given an env "envtest4b" with desk "desktest4b" exists
    When I run "desk cat desktest4b"
    Then the exit code should be 0

  Scenario: Show a single key of a desk
    Given a desk "desktest4c" exists
    When I run "desk cat -k desc desktest4c"
    Then stdout should be
      """
      Generated desciption for desktest4c
      """

  Scenario: Show multiple keys of a desk
    Given a desk "desktest4d" exists
    When I run "desk cat -k id -k desc desktest4d"
    Then stdout should be
      """
      desktest4d
      Generated desciption for desktest4d
      """

  Scenario: -q suppresses the error for a nonexistent desk
    When I run "desk cat -q nonexistent"
    Then the exit code should not be 0
    And stderr should be
      """
      """

  Scenario: -d is not a valid option for desk cat
    When I run "desk cat -d somedesk"
    Then the exit code should not be 0
    And stderr should contain "invalid option"
