@init
Feature: init
  Initialize task database structure

  @TestInit1
  Scenario: Init creates task database
    Given TestInit1 No task database exists
    When TestInit1 I run "init"
    Then TestInit1 Return code should be 0
    And TestInit1 Task database directory should exist

  @TestInit2
  Scenario: Init when database already exists
    Given TestInit2 Task database exists
    When TestInit2 I run "init"
    Then TestInit2 Return code should be 0
