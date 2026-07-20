@desk-add
Feature: desk add
  Add new desk within environment

  Scenario: Add new desk
    When I run "desk add deskadd01"
    Then the exit code should be 0

  Scenario: Add desk to specific environment
    Given an env "dskaddenv" exists
    When I run "desk add -e dskaddenv deskadd02"
    Then the exit code should be 0

  Scenario: Add desk to nonexistent environment
    When I run "desk add -e nonexistent deskadd03"
    Then the exit code should not be 0

  Scenario: Add desk with -n flag (no toggle)
    When I run "desk add -n deskadd04"
    Then the exit code should be 0

  Scenario: Add desk with no name fails
    When I run "desk add"
    Then the exit code should be 1
    And stderr should be
      """
      tec: desk name is required
      """

  Scenario: Add with -q suppresses the duplicate desk error
    Given a desk "deskadd05" exists
    When I run "desk add -q deskadd05"
    Then the exit code should be 1
    And stderr should be
      """
      """

  Scenario: Add with -N neither switches desk nor directory
    When I run "desk add -N deskadd06"
    Then the exit code should be 0
    And the PWD file should be empty

  Scenario: Add with a custom description via -D
    When I run "desk add -D 'custom desk desc' deskadd07"
    Then the exit code should be 0
    And stdout should be
      """
      """
    When I run "desk cat -k desc deskadd07"
    Then stdout should be
      """
      custom desk desc
      """
