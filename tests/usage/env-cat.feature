@env-cat
Feature: env cat
  Show environment properties

  Scenario: Show environment properties
    Given an env "envtest3" exists
    When I run "env cat envtest3"
    Then the exit code should be 0

  Scenario: Show nonexistent environment properties
    When I run "env cat nonexistent"
    Then the exit code should not be 0

  Scenario: Show environment with desk properties
    Given an env "envtest3c" with desk "desktest3c" exists
    When I run "env cat -d desktest3c envtest3c"
    Then the exit code should be 0

  Scenario: Show a single key of an environment
    Given an env "envtest3d" exists
    When I run "env cat -k desc envtest3d"
    Then the exit code should be 0
    And stdout should contain "Generated desciption for environment envtest3d"

  Scenario: Show multiple keys of an environment
    Given an env "envtest3e" exists
    When I run "env cat -k id -k desc envtest3e"
    Then stdout should be
      """
      envtest3e
      Generated desciption for environment envtest3e
      """

  Scenario: -q suppresses the error for a nonexistent environment
    When I run "env cat -q nonexistent"
    Then the exit code should not be 0
    And stderr should be
      """
      """
