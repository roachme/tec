@env-rm
Feature: env rm
  Remove environment

  @TestEnvRm1
  Scenario: Remove environment with force flag
    Given TestEnvRm1 Environment 'envtest4' exists
    When TestEnvRm1 I run "env rm -f envtest4"
    Then TestEnvRm1 Return code should be 0

  @TestEnvRm2
  Scenario: Remove environment with verbose flag
    Given TestEnvRm2 Environment 'envtest4b' exists
    When TestEnvRm2 I run "env rm -fv envtest4b"
    Then TestEnvRm2 Return code should be 0

  @TestEnvRm3
  Scenario: Remove nonexistent environment
    When TestEnvRm3 I run "env rm -f nonexistent"
    Then TestEnvRm3 Return code should not be 0

  @TestEnvRm4
  Scenario: Remove environment with desk specified
    Given TestEnvRm4 Environment 'envtest4c' with desk 'desktest4c' exists
    When TestEnvRm4 I run "env rm -f -d desktest4c envtest4c"
    Then TestEnvRm4 Return code should be 0
