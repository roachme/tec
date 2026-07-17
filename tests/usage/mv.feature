@mv
Feature: mv
  Move or rename tasks between desks and environments

  Scenario: Rename task in same desk
    Given a task "mvtest1" exists
    When I run "mv mvtest1 mvren1"
    Then the exit code should be 0
    And the task "mvren1" should exist

  Scenario: Move task to different desk
    Given a task "mvtest2" exists
    And a desk "desk2" exists without switching to it
    When I run "mv mvtest2 desk2/"
    Then the exit code should be 0

  Scenario: Move to non-existent destination returns error
    Given a task "mvtest3" exists
    When I run "mv mvtest3 baddesk/"
    Then the exit code should not be 0

  Scenario: Verify toggles updated after rename
    Given a task "mvtest4" exists
    And I run "cd mvtest4"
    When I run "mv mvtest4 mvren4"
    Then the exit code should be 0
    And the current task should be "mvren4"
