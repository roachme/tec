@desk-mv @wip
Feature: desk mv
  Move or rename desk
  Note: desk mv is documented but may not be fully implemented yet

  @skip
  Scenario: Rename desk
    Given a desk "deskold" exists
    When I run "desk mv deskold desknew"
    Then the exit code should be 0

  @skip
  Scenario: Rename desk in specific environment
    Given an env "envmv2" with desk "deskold2" exists
    When I run "desk mv -e envmv2 deskold2 desknew2"
    Then the exit code should be 0

  @skip
  Scenario: Rename nonexistent desk
    When I run "desk mv nonexistent newname"
    Then the exit code should not be 0

  @skip
  Scenario: Rename to existing desk name
    Given a desk "desk1" exists
    And a desk "desk2" exists
    When I run "desk mv desk1 desk2"
    Then the exit code should not be 0
