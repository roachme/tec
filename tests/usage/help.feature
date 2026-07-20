@help
Feature: help
  Show help for commands and top-level CLI behavior

  Scenario: No command lists all commands and fails
    When I run with no arguments
    Then the exit code should not be 0
    And stdout should contain "Usage: tec [OPTION]... COMMAND|PLUGIN"
    And stdout should contain "Basic:"

  Scenario: help with no CMD lists all commands
    When I run "help"
    Then the exit code should be 0
    And stdout should contain "Usage: tec [OPTION]... COMMAND|PLUGIN"
    And stdout should contain "System:"
    And stdout should contain "Basic:"
    And stdout should contain "Object:"

  Scenario: help for a specific command shows full usage
    When I run "help add"
    Then the exit code should be 0
    And stdout should contain "Usage: tec add [OPTION]... [ID]..."
    And stdout should contain "Add a new task to environment."

  Scenario: help for an object subcommand
    When I run "help env-add"
    Then the exit code should be 0
    And stdout should contain "Usage: tec env add [OPTION]... [NAME]..."

  Scenario: help for an unknown command fails
    When I run "help bogus"
    Then the exit code should be 1
    And stderr should contain "no such builtin command"

  Scenario: help -l lists all commands with short description
    When I run "help -l"
    Then the exit code should be 0
    And stdout should contain "add "
    And stdout should contain "Add a new task to environment."

  Scenario: help -s shows only the synopsis
    When I run "help -s add"
    Then the exit code should be 0
    And stdout should be
      """
      Usage: tec add [OPTION]... [ID]...
      """

  Scenario: help -d shows only the short description
    When I run "help -d add"
    Then the exit code should be 0
    And stdout should be
      """
      add - Add a new task to environment.
      """

  Scenario: help with multiple commands, one unknown
    # Exit status reflects only the last CMD looked up, not the whole batch.
    When I run "help add bogus1"
    Then the exit code should be 1
    And stderr should contain "'bogus1': no such builtin command"
    And stdout should contain "Usage: tec add [OPTION]... [ID]..."

  Scenario: global -h flag shows help and succeeds
    When I run "-h"
    Then the exit code should be 0
    And stdout should contain "Usage: tec [OPTION]... COMMAND|PLUGIN"

  Scenario: global -v flag shows version and succeeds
    When I run "-v"
    Then the exit code should be 0
    And stdout should contain "tec version"

  Scenario: unknown command fails
    When I run "bogus"
    Then the exit code should be 1
    And stderr should be
      """
      tec: 'bogus': no such command, alias or plugin
      """

  Scenario: command starting with a non-letter is rejected
    When I run "123bad"
    Then the exit code should be 1
    And stderr should contain "naughty command"

  Scenario: invalid global option fails
    When I run "-Z"
    Then the exit code should be 1
    And stderr should contain "invalid option"

  Scenario Outline: invalid toggle value for global boolean options fails
    When I run "-<opt> maybe help"
    Then the exit code should be 1
    And stderr should contain "accepts either 'on' or 'off'"

    Examples: Toggle options
      | opt |
      | C   |
      | D   |
      | H   |
