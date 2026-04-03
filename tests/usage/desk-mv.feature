@desk-mv @wip
Feature: desk mv
  Move or rename desk
  Note: desk mv is documented but may not be fully implemented yet

  @TestDeskMv1 @skip
  Scenario: Rename desk
    Given TestDeskMv1 Desk 'deskold' exists
    When TestDeskMv1 I run "desk mv deskold desknew"
    Then TestDeskMv1 Return code should be 0

  @TestDeskMv2 @skip
  Scenario: Rename desk in specific environment
    Given TestDeskMv2 Environment 'envmv2' with desk 'deskold2' exists
    When TestDeskMv2 I run "desk mv -e envmv2 deskold2 desknew2"
    Then TestDeskMv2 Return code should be 0

  @TestDeskMv3 @skip
  Scenario: Rename nonexistent desk
    When TestDeskMv3 I run "desk mv nonexistent newname"
    Then TestDeskMv3 Return code should not be 0

  @TestDeskMv4 @skip
  Scenario: Rename to existing desk name
    Given TestDeskMv4 Desks 'desk1' and 'desk2' exist
    When TestDeskMv4 I run "desk mv desk1 desk2"
    Then TestDeskMv4 Return code should not be 0
