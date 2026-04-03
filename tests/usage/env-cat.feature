@env-cat
Feature: env cat
  Show environment properties

  @TestEnvCat1
  Scenario: Show environment properties
    Given TestEnvCat1 Environment 'envtest3' exists
    When TestEnvCat1 I run "env cat envtest3"
    Then TestEnvCat1 Return code should be 0

  @TestEnvCat2
  Scenario: Show nonexistent environment properties
    When TestEnvCat2 I run "env cat nonexistent"
    Then TestEnvCat2 Return code should not be 0

  @TestEnvCat3
  Scenario: Show environment with desk properties
    Given TestEnvCat3 Environment 'envtest3c' with desk 'desktest3c' exists
    When TestEnvCat3 I run "env cat -d desktest3c envtest3c"
    Then TestEnvCat3 Return code should be 0
