@cfg
Feature: cfg
  Manage and query configuration settings

  @TestCfg1
  Scenario: List all configuration
    Given TestCfg1 Configuration exists
    When TestCfg1 I run "cfg ls"
    Then TestCfg1 Return code should be 0

  @TestCfg2
  Scenario: Get taskbase configuration value
    Given TestCfg2 Configuration exists
    When TestCfg2 I run "cfg get taskbase"
    Then TestCfg2 Return code should be 0
    And TestCfg2 Output should contain "tectask"

  @TestCfg3
  Scenario: Get nested opts.color value
    Given TestCfg3 Configuration exists
    When TestCfg3 I run "cfg get opts.color"
    Then TestCfg3 Return code should be 0

  @TestCfg4
  Scenario: Get multiple configuration keys
    Given TestCfg4 Configuration exists
    When TestCfg4 I run "cfg get taskbase pgnbase"
    Then TestCfg4 Return code should be 0
    And TestCfg4 Output should contain "tectask"
