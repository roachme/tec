Feature: Show task units
  This is a dumb description

  Scenario: Show units of existing task
    Given Create task ID "00000004"
    When I read the file "/tmp/tecpwd"
    Then the file should contain "/home/roach/tectask/test/desk/00000004"
