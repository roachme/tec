@cat

# TODO:
# 1. Add tests for multiple valid tasks
# 2. Add tests for multiple tasks with invalid one

Feature: cat
  Scenario: Test this shit
    Given TestCat1 Task ID 'test1'
    When TestCat1 Program show task ID 'test1'
    Then TestCat1 Result is:
        """
        id     : test1
        prio   : mid
        type   : task
        date   : 20260403
        desc   : Generated desciption for test1
        """

  Scenario: Test new shit
    Given TestCat2 This Task ID 'test1' new
    When TestCat2 This Program show task ID 'test1' with keys 'id' and 'desc'
    Then TestCat2 Result is is:
    """
    test1
    Generated desciption for test1
    """

  Scenario: TestCat3 ducking new shit
    Given TestCat3 Non-existing task ID 'deadtask'
    When TestCat3 The return should accept task ID 'deadtask'
    Then TestCat3 Result is:
        """
        tec: cannot show units 'deadtask': no such object
        """

  Scenario: TestCat4 Test this shit
    Given TestCat4 Task IDs 'test1' 'test2'
    When TestCat4 Program show task IDs 'test1' 'test2'
    Then TestCat4 Result is:
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
