@env-rm
Feature: env rm
  Remove environment

  Scenario: Remove environment with force flag
    Given an env "envtest4" exists
    When I run "env rm -f envtest4"
    Then the exit code should be 0

  Scenario: Remove environment with verbose flag
    Given an env "envtest4b" exists
    When I run "env rm -fv envtest4b"
    Then the exit code should be 0

  Scenario: Remove nonexistent environment
    When I run "env rm -f nonexistent"
    Then the exit code should not be 0

  Scenario: Remove environment with desk specified
    Given an env "envtest4c" with desk "desktest4c" exists
    When I run "env rm -f -d desktest4c envtest4c"
    Then the exit code should be 0

  Scenario: Remove with -q suppresses the error for a nonexistent environment
    When I run "env rm -q nonexistent"
    Then the exit code should be 1
    And stderr should be
      """
      """

  Scenario: Remove with -i prompts and removes on "y"
    Given an env "envtest4d" exists
    When I answer "y" when running "env rm -i envtest4d"
    Then the exit code should be 0
    And stderr should contain "remove environment 'envtest4d'? [y/N]"
    When I run "env cat envtest4d"
    Then the exit code should not be 0

  Scenario: Remove with -i prompts and keeps the environment on "n"
    Given an env "envtest4e" exists
    When I answer "n" when running "env rm -i envtest4e"
    Then the exit code should be 0
    When I run "env cat envtest4e"
    Then the exit code should be 0

  Scenario: Remove with -I prompts once for more than three environments and accepts
    Given an env "envtest4f" exists
    And an env "envtest4g" exists
    And an env "envtest4h" exists
    And an env "envtest4i" exists
    When I answer "y" when running "env rm -I envtest4f envtest4g envtest4h envtest4i"
    Then the exit code should be 0
    And stderr should contain "remove 4 environments? [y/N]"
    When I run "env cat envtest4f"
    Then the exit code should not be 0

  Scenario: Remove with -I prompts once for more than three environments and declines
    Given an env "envtest4j" exists
    And an env "envtest4k" exists
    And an env "envtest4l" exists
    And an env "envtest4m" exists
    When I answer "n" when running "env rm -I envtest4j envtest4k envtest4l envtest4m"
    Then the exit code should be 0
    When I run "env cat envtest4j"
    Then the exit code should be 0
