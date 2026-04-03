@desk-cat
Feature: desk cat
  Show desk properties

  @TestDeskCat1
  Scenario: Show desk properties
    Given TestDeskCat1 Desk 'desktest4' exists
    When TestDeskCat1 I run "desk cat desktest4"
    Then TestDeskCat1 Return code should be 0

  @TestDeskCat2
  Scenario: Show nonexistent desk properties
    When TestDeskCat2 I run "desk cat nonexistent"
    Then TestDeskCat2 Return code should not be 0

  @TestDeskCat3
  Scenario: Show desk properties in specific environment
    Given TestDeskCat3 Environment 'envtest4b' with desk 'desktest4b' exists
    When TestDeskCat3 I run "desk cat desktest4b"
    Then TestDeskCat3 Return code should be 0
