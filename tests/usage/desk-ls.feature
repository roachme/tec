@desk-ls
Feature: desk ls
  List all desks

  @TestDeskLs1
  Scenario: List all desks
    Given TestDeskLs1 Desk 'desktest2' exists
    When TestDeskLs1 I run "desk ls"
    Then TestDeskLs1 Return code should be 0
    And TestDeskLs1 Output should contain "desktest2"

  @TestDeskLs2
  Scenario: List multiple desks
    Given TestDeskLs2 Desks 'desktest2a' and 'desktest2b' exist
    When TestDeskLs2 I run "desk ls"
    Then TestDeskLs2 Return code should be 0
    And TestDeskLs2 Output should contain "desktest2a"
    And TestDeskLs2 Output should contain "desktest2b"

  @TestDeskLs3
  Scenario: List when no desks exist
    When TestDeskLs3 I run "desk ls"
    Then TestDeskLs3 Return code should be 0
