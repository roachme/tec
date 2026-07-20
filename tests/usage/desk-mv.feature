@desk-mv @wip
Feature: desk mv
  Move or rename desk
  Note: desk mv is documented but not implemented yet - it currently always
  fails with "under development", regardless of arguments. The scenarios
  below pin down that actual current behavior; the @skip scenarios further
  down describe the intended (not yet implemented) behavior for when it
  eventually lands.

  Scenario: desk mv is under development and always fails
    Given a desk "deskmv1" exists
    When I run "desk mv deskmv1 deskmv2"
    Then the exit code should not be 0
    And stderr should contain "under development"

  Scenario: desk mv with no arguments is also under development
    When I run "desk mv"
    Then the exit code should not be 0
    And stderr should contain "under development"

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
