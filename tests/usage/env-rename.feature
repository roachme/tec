@env-rename
Feature: env rename
  Rename environment

  @TestEnvRename1
  Scenario: Rename environment
    Given TestEnvRename1 Environment 'envtest6' exists
    When TestEnvRename1 I run "env rename envtest6 envtest6new"
    Then TestEnvRename1 Return code should be 0

  @TestEnvRename2
  Scenario: Rename nonexistent environment
    When TestEnvRename2 I run "env rename nonexistent newname"
    Then TestEnvRename2 Return code should not be 0

