@desk-cd
Feature: desk cd
  Change to desk

  @TestDeskCd1
  Scenario: Change to desk
    Given TestDeskCd1 Desk 'desktest3' exists
    When TestDeskCd1 I run "desk cd desktest3"
    Then TestDeskCd1 Return code should be 0
    And TestDeskCd1 PWD changes to desk 'desktest3'

  @TestDeskCd2
  Scenario: Change to desk in specific environment
    Given TestDeskCd2 Environment 'envtest3b' with desk 'desktest3b' exists
    When TestDeskCd2 I run "desk cd -e envtest3b desktest3b"
    Then TestDeskCd2 Return code should be 0
    And TestDeskCd2 PWD changes to desk 'desktest3b' in env 'envtest3b'

  @TestDeskCd3
  Scenario: Change to nonexistent desk
    When TestDeskCd3 I run "desk cd nonexistent"
    Then TestDeskCd3 Return code should not be 0

  @TestDeskCd4
  Scenario: Change to desk with -n flag (no toggle)
    Given TestDeskCd4 Desk 'desktest3c' exists
    When TestDeskCd4 I run "desk cd -n desktest3c"
    Then TestDeskCd4 Return code should be 0
    And TestDeskCd4 PWD changes to desk 'desktest3c'

  @TestDeskCd5
  Scenario: Change to previous desk using "-" alias
    Given TestDeskCd5 Desks 'deskprev1' and 'deskprev2' exist
    And TestDeskCd5 I switch to 'deskprev1' then 'deskprev2'
    When TestDeskCd5 I run "desk cd -"
    Then TestDeskCd5 Return code should be 0
    And TestDeskCd5 PWD changes to desk 'deskprev1'
