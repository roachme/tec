@env-ls
Feature: env ls
  List all environments

  @TestEnvLs1
  Scenario: List all environments
    Given TestEnvLs1 Environment 'envtest1' exists
    When TestEnvLs1 I run "env ls"
    Then TestEnvLs1 Return code should be 0
    And TestEnvLs1 Output should contain "envtest1"

  @TestEnvLs2
  Scenario: List multiple environments
    Given TestEnvLs2 Environments 'envtest1a' and 'envtest1b' exist
    When TestEnvLs2 I run "env ls"
    Then TestEnvLs2 Return code should be 0
    And TestEnvLs2 Output should contain "envtest1a"
    And TestEnvLs2 Output should contain "envtest1b"

  @TestEnvLs3
  Scenario: List when no custom environments exist
    When TestEnvLs3 I run "env ls"
    Then TestEnvLs3 Return code should be 0
