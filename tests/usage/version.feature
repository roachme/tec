@version
Feature: version
  Display program version information

  Scenario: Version displays program name and version
    When I run "version"
    Then the exit code should be 0
    And stdout should contain "tec"
    And stdout should contain "v"

  Scenario: Version output format matches pattern
    When I run "version"
    Then the exit code should be 0
    And stdout should match "tec(\s+version)?\s+v\d+\.\d+\.\d+"
