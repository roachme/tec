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
