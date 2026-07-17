@cat

# TODO:
# 1. Add tests for multiple valid tasks with invalid one

Feature: cat

  Background:
    Given a task "test1" exists
    Given a task "test2" exists

  Scenario: Show a single task
    When I run "cat test1"
    Then stdout should be
      """
      id     : test1
      prio   : mid
      type   : task
      date   : 20260403
      desc   : Generated desciption for test1
      """

  Scenario: Show selected keys of a task
    When I run "cat -k id -k desc test1"
    Then stdout should be
      """
      test1
      Generated desciption for test1
      """

  Scenario: Show a non-existing task
    When I run "cat deadtask"
    Then the exit code should be 1
    And stderr should be
      """
      tec: cannot show task units 'deadtask': no such task ID
      """

  Scenario: Show multiple tasks
    When I run "cat test1 test2"
    Then stdout should be
      """
      id     : test1
      prio   : mid
      type   : task
      date   : 20260403
      desc   : Generated desciption for test1
      id     : test2
      prio   : mid
      type   : task
      date   : 20260403
      desc   : Generated desciption for test2
      """
