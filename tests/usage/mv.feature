@mv
Feature: mv
  Move or rename tasks between desks and environments

  @TestMv1
  Scenario: Rename task in same desk
    Given TestMv1 Task 'mvtest1' exists
    When TestMv1 I run "mv mvtest1 mvren1"
    Then TestMv1 Return code should be 0
    And TestMv1 Task 'mvren1' should exist

  @TestMv2
  Scenario: Move task to different desk
    Given TestMv2 Task 'mvtest2' exists
    And TestMv2 Desk 'desk2' exists
    When TestMv2 I run "mv mvtest2 desk2/"
    Then TestMv2 Return code should be 0

  @TestMv3
  Scenario: Move to non-existent destination returns error
    Given TestMv3 Task 'mvtest3' exists
    When TestMv3 I run "mv mvtest3 baddesk/"
    Then TestMv3 Return code should not be 0

  @TestMv4
  Scenario: Verify toggles updated after rename
    Given TestMv4 Task 'mvtest4' exists
    And TestMv4 I cd to 'mvtest4'
    When TestMv4 I run "mv mvtest4 mvren4"
    Then TestMv4 Return code should be 0
    And TestMv4 Current toggle should be 'mvren4'
