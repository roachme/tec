@env-cd
Feature: env cd
  Change to environment

  @TestEnvCd1
  Scenario: Change to environment
    Given TestEnvCd1 Environment 'envtest2' exists
    When TestEnvCd1 I run "env cd envtest2"
    Then TestEnvCd1 Return code should be 0
    And TestEnvCd1 PWD changes to env 'envtest2'

  @TestEnvCd2
  Scenario: Change to environment with desk option
    Given TestEnvCd2 Environment 'envtest2b' with desk 'desktest2b' exists
    When TestEnvCd2 I run "env cd -d desktest2b envtest2b"
    Then TestEnvCd2 Return code should be 0
    And TestEnvCd2 PWD changes to desk 'desktest2b' in env 'envtest2b'

  @TestEnvCd3
  Scenario: Change to nonexistent environment
    When TestEnvCd3 I run "env cd nonexistent"
    Then TestEnvCd3 Return code should not be 0

  @TestEnvCd4
  Scenario: Change to environment with -n flag (no toggle)
    Given TestEnvCd4 Environment 'envtest2c' exists
    When TestEnvCd4 I run "env cd -n envtest2c"
    Then TestEnvCd4 Return code should be 0
    And TestEnvCd4 PWD changes to env 'envtest2c'

  @TestEnvCd5
  Scenario: Change to previous environment using "-" alias
    Given TestEnvCd5 Environments 'envprev1' and 'envprev2' exist
    And TestEnvCd5 I switch to 'envprev1' then 'envprev2'
    When TestEnvCd5 I run "env cd -"
    Then TestEnvCd5 Return code should be 0
    And TestEnvCd5 PWD changes to env 'envprev1'
