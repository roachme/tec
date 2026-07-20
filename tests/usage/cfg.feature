@cfg
Feature: cfg
  Manage and query configuration settings

  Scenario: List all configuration
    When I run "cfg ls"
    Then the exit code should be 0

  Scenario: Get taskbase configuration value
    When I run "cfg get taskbase"
    Then the exit code should be 0
    And stdout should contain "tectask"

  Scenario: Get nested opts.color value
    When I run "cfg get opts.color"
    Then the exit code should be 0

  Scenario: Get multiple configuration keys
    When I run "cfg get taskbase pgnbase"
    Then the exit code should be 0
    And stdout should contain "tectask"

  Scenario: cfg with no subcommand defaults to ls
    When I run "cfg"
    Then the exit code should be 0
    And stdout should contain "Paths:"
    And stdout should contain "Options:"

  Scenario: cfg with an invalid subcommand fails
    When I run "cfg bogus"
    Then the exit code should be 1
    And stderr should be
      """
      tec: 'bogus': no such cfg command
      """

  Scenario Outline: documented but unimplemented cfg subcommands fail
    When I run "cfg <subcmd>"
    Then the exit code should be 1
    And stderr should contain "no such cfg command"

    Examples: Unimplemented subcommands
      | subcmd |
      | revert |
      | save   |

  Scenario: cfg get with an unknown value does not fail the exit code
    When I run "cfg get bogusvalue"
    Then the exit code should be 0
    And stderr should be
      """
      tec: 'bogusvalue': no such config value
      """

  Scenario: cfg get with a mix of valid and invalid values
    When I run "cfg get taskbase bogusvalue"
    Then the exit code should be 0
    And stdout should contain "tectask"
    And stderr should contain "'bogusvalue': no such config value"
