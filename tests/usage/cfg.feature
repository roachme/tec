@cfg
Feature: cfg
  Manage and query configuration settings

  Scenario: List all configuration
    When I run "cfg ls"
    Then the exit code should be 0

  Scenario: Get taskbase configuration value
    When I run "cfg get taskbase"
    Then the exit code should be 0
    And stdout should contain "tectask"

  Scenario: Get nested opts.color value
    When I run "cfg get opts.color"
    Then the exit code should be 0

  Scenario: Get multiple configuration keys
    When I run "cfg get taskbase pgnbase"
    Then the exit code should be 0
    And stdout should contain "tectask"
