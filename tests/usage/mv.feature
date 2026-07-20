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

  Scenario: Move multiple tasks to another desk at once
    Given tasks "mvm1" "mvm2" "mvm3" exist
    And a desk "mvdsk2" exists without switching to it
    When I run "mv mvm1 mvm2 mvm3 mvdsk2/"
    Then the exit code should be 0
    And stdout should be
      """
      """

  Scenario: Rename current task using "." shorthand
    Given a task "mvdot1" exists
    And I run "cd mvdot1"
    When I run "mv . mvdotrn"
    Then the exit code should be 0
    And the task "mvdotrn" should exist

  Scenario: mv with no arguments fails with a missing source error
    When I run "mv"
    Then the exit code should not be 0
    And stderr should be
      """
      tec: source task is missing
      """

  Scenario: mv with only a source fails with a missing destination error
    Given a task "mvnodst" exists
    When I run "mv mvnodst"
    Then the exit code should not be 0
    And stderr should be
      """
      tec: destination is missing
      """

  Scenario: mv -f is under development
    Given a task "mvftest" exists
    When I run "mv -f mvftest mvfdst"
    Then the exit code should not be 0
    And stderr should be
      """
      tec: option `-f' under development
      """

  Scenario: mv -t is under development
    Given a task "mvttest" exists
    When I run "mv -t somedesk mvttest mvtdst"
    Then the exit code should not be 0
    And stderr should be
      """
      tec: option `-t' under development
      """
