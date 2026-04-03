@desk-add
Feature: desk add
  Add new desk within environment

  @TestDeskAdd1
  Scenario: Add new desk
    Given TestDeskAdd1 Environment exists
    When TestDeskAdd1 I run "desk add deskadd01"
    Then TestDeskAdd1 Return code should be 0

  @TestDeskAdd2
  Scenario: Add desk to specific environment
    Given TestDeskAdd2 Environment 'dskaddenv' exists
    When TestDeskAdd2 I run "desk add -e dskaddenv deskadd02"
    Then TestDeskAdd2 Return code should be 0

  @TestDeskAdd3
  Scenario: Add desk to nonexistent environment
    When TestDeskAdd3 I run "desk add -e nonexistent deskadd03"
    Then TestDeskAdd3 Return code should not be 0

  @TestDeskAdd4
  Scenario: Add desk with -n flag (no toggle)
    Given TestDeskAdd4 Environment exists
    When TestDeskAdd4 I run "desk add -n deskadd04"
    Then TestDeskAdd4 Return code should be 0
