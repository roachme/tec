## [Unreleased]

### Added

- `cfg`: Add `ls` subcommand for command `cfg`
- `cfg`: Add `get` subcommand for command `cfg`

### Changed

- `_tec`: Rename binary program name to `_tec`

### Fixed

- `cli/pgn`: Style code
- `cli/tec`: Fix segmentation fault for argvec
- `cli`: Rename structure members
- `cli/aux/hook`: Fix hook option generator
- `cli/set`: Fix unintialized retcode

### Deleted


## [0.3.0] - (2026-02-09)

### Added

- `cli/cat`: Add support for multiple keys
- `cli`: Use const argv for cli arguments

### Changed

- `cli`: Remove column support

### Fixed

- `lib/errmod`: Fix array boundary check
- `cli/aux/toggle`: Fix buffer boundary
- `cli/rm`: Fix toggle removal
- `cli/env`: Fix env renaming function
- `cli`: Fix error format messages
- `cli/rm`: Refactor code
- `cli/aux/toggle`: Fix memory leaks
- `cli/add`: Fix overlapping memory regions
- `cli`: Fix alias check for `cd` commands
- `cli/pgn`: Separate hook and pgn module

### Deleted


## [0.2.2] - (2026-01-30)

### Added

- `cli/set`: Add skeleton for interactive option
- `cli/help`: Add new option for command `help`

### Changed

- `cli/column`: Rename subcommands

### Fixed

- `completion/_tec.zsh`: Update command list
- `cli/help`: Fix exit code
- `cli`: Add missing help options
- `cli/aux/toggle`: Fix toggle logic if no toggle found
- `cli/cd`: Fix alias mixed with task IDs
- `cli/cd`: Fix return code
- `cli`: Remove board and project namings
- `cli/cd`: Style code
- `cli/desk`: Style code
- `cli/env`: Style code
- `cli`: Fix `env` and `desk` cat commands
- `cli/add`: Style code
- `cli/rm`: Style code
- `cli/set`: Style code
- `cli/env`: Style code
- `cli/desk`: Style code

### Deleted

- `cli`: Delete `env` and `desk` subcommands `prev`


## [0.2.1] - (2026-01-25)

### Added

- `cli/env`: Add support for alias "-"
- `cli/desk`: Add support for alias "-"

### Changed

- `cli/env`: Rename command `env` subcommand `sync` to `cd`
- `cli/desk`: Rename command `desk` subcommand `sync` to `cd`
- `cli/tec`: Fix memleak in case of invalid command
- `cli/set`: Remove strict at least one key set
- `tec`: Change option `-p` to `-e`
- `tec`: Change option `-b` to `-d`

### Fixed

- `cli/help`: Update help messages
- `cli`: Fix command renaming issues
- `cli/desk`: Update help messages
- `tec`: Remove all notions of project prefixes
- `tec`: Remove all notions of board prefixes
- `cli`: Fix help messages
- `cli`: Fix command `ls` option check
- `cli/cd`: Fix multiple task IDs
- `cli/cd`: Fix task ID invalid loop
- `cli/ls`: Fix logic for options
- `cli/aux/hook`: Fix invalid desk option

### Deleted

- `cli/version`: Delete command version
- `cli/aux/hook`: Delete unnecessary option `-P` to hooks


## [0.2.0] - (2026-01-10)

### Added

- `cli`: Add skeleton for plugin output
- `cli/sync`: Add support for alias "-"
- `cli/move`: Implement move command
- `cli/rm`: Add verbose option

### Changed

- `tec`: Rename project to `tec`
- `cli`: Use C standard boolean header
- `tec`: Rename main file from `cli` to `tec`
- `tec`: Change version output and CLI option
- `lib`: Make libtec part of the main project
- `cli/plugin`: Let the plugin to find current arguments
- `cli/tec`: Look for plugin first then builtins
- `cli/tec.h`: Shrink task ID to 8
- `cli/rm`: Rename command `del` to `rm`
- `cli/ls`: Rename command `list` to `ls`
- `cli/cat`: Rename command `show` to `cat`
- `cli/mv`: Rename command `move` to `mv`
- `cli/env`: Rename command `project` to `env`
- `cli/desk`: Rename command `board` to `desk`
- `cli/cd`: Rename command `sync` to `cd`
- `cli`: Rename option `-v` to `-V` to show version
- `cli/env`: Rename command `env` subcommand `list` to `ls`
- `cli/column`: Rename command `column` subcommand `list` to `ls`
- `cli/desk`: Rename command `desk` subcommand `list` to `ls`
- `cli/desk`: Rename command `desk` subcommand `del` to `rm`
- `cli/env`: Rename command `env` subcommand `del` to `rm`
- `cli/desk`: Rename command `desk` subcommand `show` to `cat`
- `cli/env`: Rename command `env` subcommand `show` to `cat`
- `cli/desk`: Rename command `desk` subcommand `move` to `mv`

### Fixed

- `cli/tec`: Fix padding for objects
- `lib`: Fix API MACRO name
- `cli/aux/plugin`: Add error messages
- `cli`: Style code
- `cli`: Move command `version` to new file

### Deleted

- `cli`: Detele command `prev`


## [0.1.2] - (2025-08-06)

### Added

- `cli`: Add new command `column`


## [0.1.1] - (2025-08-02)

### Added

- `cli/flow`: Add workflow subcommands
- `cli/sync`: Add option not to switch to directory
- `cli/add`: Add option not to switch to directory
- `src`: Add support to check board feature usage

### Changed

- `Makefile`: Change default target to user build
- `cli/cli`: cli/cli: Use getopt to parse options

### Fixed

- `cli/set`: Fix memory leak
- `cli/help`: Update help message in case of error

### Deleted

- `cli/plugin`: Remove check for input arguments
- `cli/fsck`: Delete command `fsck` from builtin commands
- `cli/cfg`: Delete command `cfg` from builtin commands
- `cli`: Delete command `flow` from builtin commands


## [0.0.22] - (2025-07-12)

### Added

- `cli/board`: Add a skeleton
- `cli`: Add skeleton for board functions

### Changed

- `cli/fsck`: Rename builtin command name
- `tman`: Unify column module

### Fixed

- `cli`: Simplify hook logic
- `cli/hook`: Delete access to private structure in library
- `cli/flow`: Fix support to move to columns

### Deleted

- `cli/add`: Delete unused option for ID generation
- `src/libtman`: Delete id member from `tman_ctx_t`


## [0.0.21] - (2025-06-29)

### Added

- `cli`: Move hooks from libtman
- `cli/hook`: Add plugin command generation

### Changed

- `README`: Update README.md
- `cli`: Update libtman SHA commit

### Fixed

- `src`: Fix access to private structures


## [0.0.20] - (2025-06-26)

### Added

- `cli/plugin`: Add option `-P`
- `cli/pgm`: Add command to install plugin
- `scripts/release`: Add script for release
- `tests/usage`: Add usage tests against regression
- `cli/add`: Add support to generate task units
- `cli/prj`: Add support to generate project units
- `cli/col`: Add column name to `struct tman_context`

### Changed

- `scripts`: Update README.md generator
- `lib`: Bump to version 0.0.20
- `scripts/genreadme`: Update README.md
- `cli/cli`: Move ispgn() from lib part

### Fixed

- `cli/config`: Fix memleak for plugins in config
- `cli/add`: Fix task ID generation
- `cli/cli`: Fix PWD in case of invalid command
- `cli/add`: Fix task ID iteration in case of error
- `cli`: Fix plugin option parser

### Deleted

- `pgn`: Move plugins to separate project
- `cli`: Move plugin manager `pgm` to plugin part


## [0.0.19] - (2025-06-20)

### Added

- `cli/pgm`: Add support for plugin description
- `cli`: Add debug option
- `tman`: Add Git LFS tracking for large files

### Changed

- `lib`: Move library part to submodule

### Fixed

- `cli/prj`: Fix status code when project is deleted
- `cli/pgm`: Remove lib dependency
- `CICD`: Fix broken pipeline

### Deleted

- `cli`: Delete command `flow` from list


## [0.0.18] - (2025-06-17)

### Added

- `cli/config`: Add default values for config
- `cli/plugin`: Add support to pass plugin common options
- `scripts/genshell`: Add generator to generate tman.sh file
- `cli/cfg`: Add show subcommand to cfg

### Changed

- `README`: Update README.md file
- `pgn`: Update SHA commit to pgn
- `cli`: Move PWD logic to cli part
- `cli/del`: Add confirmation for every task

### Fixed

- `lib/unit`: Fix memory heap corruption
- `cli`: Fix directory switch for commands
- `lib/task`: Fix memleak when task ID is equal to limit


## [0.0.17] - (2025-06-11)

### Added

- `cli/set`: Add hook support for command `set`

### Changed

- `Makefile`: Refactor some logic
- `tman`: Use tmp file for pwd
- `cli/plugin`: Refactor plugin API for options

### Fixed

- `cli/color`: Color output only if it is a TTY


## [0.0.16] - (2025-06-08)

### Added

- `cli/config`: Add default values
- `cli/config`: Use libconfig package for config
- `script/genreadme`: Add generator to generate README.md file

### Changed

- `cli/link`: revert "Add skeleton to link tasks"
- `pgn`: Move plugins to submodule

### Fixed

- `pgn`: Update option for task directory
- `cli/show`: Hotfix show hooks
- `CICD`: Fix CICD dependency

### Deleted

- `cli`: Delete link command skeletons


## [0.0.15] - (2025-06-02)

### Added

- `pgn/find`: Add plugin to find in database
- `cli/prj`: Add command `set` to project object
- `cli/prj`: Add command `show` to project object
- `scripts/updver`: Add help message command

### Changed

- `tman`: Refactor unit module
- `lib/unit`: Update unit module's API

### Fixed

- `cli/help`: Update help message
- `cli/plugin`: Fix plugin options parser
- `cli/cli`: Fix config options
- `cli`: Fix missing usehooks option


## [0.0.14] - (2025-05-24)

### Added

- `cli/move`: Add option `-h` for help
- `cli/config`: Add support for environment variable `HOME` in path
- `scripts/install`: Add skeleton for installation script
- `cli/list`: Add option to show project name header

### Changed

- `cli/config`: Change depricated hook names

### Fixed

- `scripts`: Fix a typo in directory name
- `cli/set`: Fix options mismatching with help message


## [0.0.13] - (2025-05-15)

### Added

- `scripts/makevideo`: Add a script to produce video of how to use tman
- `cli/prj`: Add prompt before deleting a project
- `cli`: Pass system config values via option

### Changed

- `lib`: Delete depricated function
- `cli/help`: Update help messages
- `lib/tman`: Rename structure member
- `lib`: Add prefix `LIB` to library macros

### Fixed

- `scripts/updver`: Fix tag update
- `lib/tman`: Get new current arguments when switch to previous task
- `cli/prj`: Fix option `-h` for project subcommands

### Deleted

- `tman`: Delete unused header files


## [0.0.12] - (2025-05-10)

### Added

- `cli/help`: Add notion about new option
- `lib/tman`: Add skeleton to check board name
- `lib/task`: Add support to move current task to any column

### Changed

- `cli/help`: Update command `add` help message
- `scripts/updver`: Update some of the logic
- `lib/tman`: Move name checks to common function
- `scripts/updver`: Delete `v` from version in CHANGELOG.md
- `lib`: Rename confusing private function names

### Fixed

- `scripts/updver`: Tag from master branch
- `cli/show`: Fix option parser
- `cli`: Fix linter warnings
- `tman`: Fix linter warnings
- `cli/del`: Fix project name check
- `lib/task`: Fix move task to previous column if current not set
- `cli/show`: Fix invalid option specification

### Optimized

- `lib/tman`: Optimize switch to previous task


## [v0.0.11] - (2025-05-07)

### Added

- `cli/help`: Delete command grep from manual
- `cli`: Add ID generation to cli part
- `cli`: Add fixed padding to output
- `cli/help`: Add help message for util itself
- `scripts/updver`: Refactor logic
- `cli`: Move version to file

### Changed

- `cli/help`: Amend help message for command `prj`
- `tman`: Rename lib API names from id to task

### Fixed

- `lib`: Fix minor linter warnings
- `cli`: Move project name check outta loop

### Deleted

- `cli/help`: Delete option '-h'
- `cli/add`: Delete option for task ID generation from manual


## [0.0.10] - (2025-04-20)

### Added

- `cli/cli`: Add program name before error messages
- `Makefile`: Add release build
- `lib/col`: Show list of project columns
- `tman`: Add command to rename project

### Changed

- `Makefile`: Rename executable filename to `_tmancli`
- `README`: Update shell script
- `lib`: Make check function's return value boolean
- `README`: Update README.md
- `cli/help`: Amend help for prj-rename command

### Fixed

- `tman`: Fix linter warnings
- `pgn/time`: Fix invalid option
- `lib/tman`: Fix wrong error code

### Deleted

- `cli/cli`: Delete command ver (replace it with option)
- `lib/common`: Delete unused function declarations


## [0.0.9] - (2025-04-09)

### Changed

- `cli`: Change API option `-b` to `-B`
- `pgn`: Make pgn submodule part of codebase

### Fixed

- `cli/add`: Fix task ID length check


## [0.0.8] - (2025-03-31)

### Added

- `cli/show`: Add option `-k' to command show
- `cli/move`: Add support to move and rename tasks

### Changed

- `tman`: Use unified structure for options
- `tman`: Move hooks to cli part
- `tman`: Move config to cli part
- `tman`: Rename struct tman_args to struct tman_arg
- `lib`: Rename preprocessor directive check

### Fixed

- `cli`: Fix a memory leak in config module
- `cli`: Fix linter warnings
- `lib/tman`: Fix build warning about overflow

### Deleted

- `cli/cli`: Delete command find
- `lib/tman`: Delete task API functions getcurr() and getprev()
- `lib/tman`: Delete project API functions getcurr() and getprev()


## [0.0.7] - (2025-03-24)

### Added

- `scripts`: Refactor logic for script updver.sh
- `cli/find`: Add support for project name
- `tman`: Add linux kernel style (trade spaces for tabs)

### Changed

- `cli/list`: Show plugin braces only there's plugin output
- `lib/tman`: Change return value of function tman_deinit()
- `lib/unit`: Change return value of function unit_delpgn()
- `lib/tree`: Change return value of function tree_free()
- `tman`: Use unified structure for input arguments

### Fixed

### Deleted

- `lib`: Remove variable PROGRAM from lib part
- `lib`: Remove logging functions
- `lib/tman`: Delete library functions' return code
- `lib/tman`: Delete fprintf() from library calls
- `cli/cli`: Delete command list as default


## [0.0.6] - (2025-03-14)

### Added

- `Makefile`: Turn on compiler warning messages
- `docs`: Add a usage video


## [0.0.5] - (2025-03-03)

### Added

- `CHANGELOG`: Add a structure to CHANGELOG logs
- `tman`: Add support to not switch to newly created project
- `cli`: Add extra space between task/project tag and description
- `lib/dir`: Add support to create project hidden directory
- `cli/prev`: Add option quiet to suppress error messages
- `cli/list`: Add filter options for task list

### Changed

- `cli/prj`: Delete extra space between columns
