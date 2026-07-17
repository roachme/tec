@init
Feature: init
  Initialize task database structure

  Scenario: Init creates task database
    Given no task database exists
    When I run "init"
    Then the exit code should be 0
    And the task database directory should exist

  Scenario: Init when database already exists
    Given a task database exists
    When I run "init"
    Then the exit code should be 0
