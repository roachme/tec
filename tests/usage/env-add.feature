@env-add
Feature: env
  Scenario: Test1 Add single valid environment
    Given Test1 Single environment name 'bddtest'
    When Test1 Add single valid environment name 'bddtest'
    Then Test1 Return value is '0'
    # Add PWD changes to env created

  Scenario: Test2 Add single invalid environment
    Given Test2 Single invalid environment name 'test me'
    When Test2 Add single environment name 'test me'
    Then Test2 Return value is '1'
    And Test2 Get error message:
        """
        tec: cannot add env 'test me': illegal object name
        """
    #And check that PWD value does not change

  Scenario: Test3 Add multiple valid environments
    Given Test3 Multiple environment name 'test1' 'test2' 'test3'
    When Test3 Add single valid environment name 'test1' 'test2' 'test3'
    Then Test3 Return value is '0'
    # Add PWD changes to last env created

  Scenario: Test4 Add multiple valid and invalid environments
    Given Test4 Multiple valid and invalid environments names 'test11' 'test me' 'test33'
    When Test4 Add multiple valid and invalid valid environment names 'test1' 'test2' 'test3'
    Then Test4 Return value is '1'
    And Test4 Get error message:
        """
        tec: cannot add env 'test me': illegal object name
        """
    # Add PWD changes to last env created

  Scenario: Test5 Add single already existing environment
    Given Test5 Single already existing environment name 'bddtest'
    When Test5 Add single valid environment name 'bddtest'
    Then Test5 Return value is '1'
    # Add PWD does not change to env created

  # Clean-up phase
  Scenario: Test6 Remove all created environments
    Given Test6 Multiple environment names 'bddtest' 'test1' 'test2' 'test3' 'test11' 'test33'
    When Test6 Remove environments 'bddtest' 'test1' 'test2' 'test3' 'test11' 'test33'
    Then Test6 Return value is '0'
