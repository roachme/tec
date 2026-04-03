@version
Feature: version
  Display program version information

  @TestVersion1
  Scenario: Version displays program name and version
    Given TestVersion1 No preconditions
    When TestVersion1 I run "version"
    Then TestVersion1 Return code should be 0
    And TestVersion1 Output should contain "tec"
    And TestVersion1 Output should contain "v"

  @TestVersion2
  Scenario: Version output format matches pattern
    Given TestVersion2 No preconditions
    When TestVersion2 I run "version"
    Then TestVersion2 Return code should be 0
    And TestVersion2 Output should match version pattern
