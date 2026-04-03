@env-set
Feature: env set
  Set environment properties

  @TestEnvSet1
  Scenario: Set environment description
    Given TestEnvSet1 Environment 'envtest5' exists
    When TestEnvSet1 I run "env set -D 'Test environment' envtest5"
    Then TestEnvSet1 Return code should be 0

  @TestEnvSet2
  Scenario: Set environment description for nonexistent environment
    When TestEnvSet2 I run "env set -D 'Test' nonexistent"
    Then TestEnvSet2 Return code should not be 0

  @TestEnvSet3
  Scenario: Set environment description with desk option
    Given TestEnvSet3 Environment 'envtest5b' with desk 'desktest5b' exists
    When TestEnvSet3 I run "env set -D 'Test env' -d desktest5b envtest5b"
    Then TestEnvSet3 Return code should be 0
