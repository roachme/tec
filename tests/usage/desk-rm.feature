@desk-rm
Feature: desk rm
  Remove desk

  @TestDeskRm1
  Scenario: Remove desk with force flag
    Given TestDeskRm1 Desk 'desktest5' exists
    When TestDeskRm1 I run "desk rm -f desktest5"
    Then TestDeskRm1 Return code should be 0

  @TestDeskRm2
  Scenario: Remove desk with verbose flag
    Given TestDeskRm2 Desk 'desktest5b' exists
    When TestDeskRm2 I run "desk rm -fv desktest5b"
    Then TestDeskRm2 Return code should be 0

  @TestDeskRm3
  Scenario: Remove nonexistent desk
    When TestDeskRm3 I run "desk rm -f nonexistent"
    Then TestDeskRm3 Return code should not be 0

  @TestDeskRm4
  Scenario: Remove desk from specific environment
    Given TestDeskRm4 Environment 'envtest5c' with desk 'desktest5c' exists
    When TestDeskRm4 I run "desk rm -f desktest5c"
    Then TestDeskRm4 Return code should be 0
