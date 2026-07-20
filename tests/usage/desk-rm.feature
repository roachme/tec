@desk-rm
Feature: desk rm
  Remove desk

  Scenario: Remove desk with force flag
    Given a desk "desktest5" exists
    When I run "desk rm -f desktest5"
    Then the exit code should be 0

  Scenario: Remove desk with verbose flag
    Given a desk "desktest5b" exists
    When I run "desk rm -fv desktest5b"
    Then the exit code should be 0

  Scenario: Remove nonexistent desk
    When I run "desk rm -f nonexistent"
    Then the exit code should not be 0

  Scenario: Remove desk from specific environment
    Given an env "envtest5c" with desk "desktest5c" exists
    When I run "desk rm -f desktest5c"
    Then the exit code should be 0

  Scenario: Remove with -q suppresses the error for a nonexistent desk
    When I run "desk rm -q nonexistent"
    Then the exit code should be 1
    And stderr should be
      """
      """

  Scenario: Remove with -i prompts and removes on "y"
    Given a desk "desktest5d" exists
    When I answer "y" when running "desk rm -i desktest5d"
    Then the exit code should be 0
    When I run "desk cat desktest5d"
    Then the exit code should not be 0

  Scenario: Remove with -i prompts and keeps the desk on "n"
    Given a desk "desktest5e" exists
    When I answer "n" when running "desk rm -i desktest5e"
    Then the exit code should be 0
    When I run "desk cat desktest5e"
    Then the exit code should be 0

  Scenario: Remove with -I prompts once for more than three desks and accepts
    Given a desk "desktest5f" exists
    And a desk "desktest5g" exists
    And a desk "desktest5h" exists
    And a desk "desktest5i" exists
    When I answer "y" when running "desk rm -I desktest5f desktest5g desktest5h desktest5i"
    Then the exit code should be 0
    And stderr should contain "remove 4 desks? [y/N]"
    When I run "desk cat desktest5f"
    Then the exit code should not be 0

  Scenario: Remove with -I prompts once for more than three desks and declines
    Given a desk "desktest5j" exists
    And a desk "desktest5k" exists
    And a desk "desktest5l" exists
    And a desk "desktest5m" exists
    When I answer "n" when running "desk rm -I desktest5j desktest5k desktest5l desktest5m"
    Then the exit code should be 0
    When I run "desk cat desktest5j"
    Then the exit code should be 0
