@desk-set
Feature: desk set
  Set desk properties

  @TestDeskSet1
  Scenario: Set desk description
    Given TestDeskSet1 Desk 'desktest6' exists
    When TestDeskSet1 I run "desk set -D 'Test desk description' desktest6"
    Then TestDeskSet1 Return code should be 0

  @TestDeskSet2
  Scenario: Set desk description for nonexistent desk
    When TestDeskSet2 I run "desk set -D 'Test' nonexistent"
    Then TestDeskSet2 Return code should not be 0

  @TestDeskSet3
  Scenario: Set desk description in specific environment
    Given TestDeskSet3 Environment 'envtest6b' with desk 'desktest6b' exists
    When TestDeskSet3 I run "desk set -D 'Test desk' desktest6b"
    Then TestDeskSet3 Return code should be 0
