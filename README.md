<div align="center">
   <!--<img align="center" width="128px" src="crates/gitbutler-tauri/icons/128x128@2x.png" /> -->
	<h1 align="center"><b>Tec</b></h1>
	<p align="center">
        Terminal project and task manager
    <br />
  </p>
</div>

<br/>


![command example](docs/tec.gif)


## ⇁  Problems
During my work I encounter many subtaks I gotta do on the daily basis.
Among them
1. Clone Git repos to my machine and create branches in multiple repos (usually different for each task)
2. Keep my task logs, sketches, notes and repos for each task somewhere. Structured and seperately
3. Often switch between task, which involves context, notes, sketches, and especially Git branches

## ⇁  Solution
Tec comes with 3 basic ideas
1. Structure your workspace for each task
2. Get you exactly where you want with fewest keystrokes
3. Automate stuff that you're tired of during the work. Here plugins are for

## ⇁  Structure
- Customize: Adjust the util for your workflow via config file.
- Extensible: Extend util with predefined or your own plugins. You can get full list of Tec plugins in ``` tec-pgn ```.

## ⇁  Dependencies
Plugins might have dependencies. Take a look at plugins README.md for more info.
Main program uses the some dependencies. To install them run commands below:
```
sudo apt install -y libconfig-dev
```


## ⇁  Build
Simply run the command below.
``` bash
make
```

## ⇁  Installation
1. Once you compiled successfully, put executable ` _tec ` into one of directories defined in env variable ` PATH ` . I put it in ` ~/.local/bin ` .
2. Copy the content of tec.sh into your shell rc file. It's ` ~/.bashrc ` , ` .zshrc ` , etc.

``` bash
#!/usr/bin/env bash

function tec()
{
    local tecstatus;
    local pwdfile="/tmp/tecpwd"

    _tec "$@"
    tecstatus="$?"

    test -s "$pwdfile" && cd "$(cat "$pwdfile")" || return "$tecstatus"
    return "$tecstatus"
}
```

3. Create basic Tec config file. Either in ` ~/.tec/tec.cfg ` or ` ~/.config/tec/tec.cfg ` and fill it with content below

```
base = {
    task = "$HOME/tectask";
    pgn = "$HOME/.local/lib/tec/pgn";
};

options = {
    hook = true;    /* enable hooks */
    color = true;   /* enable colors */
    debug = false;  /* disable debug info */
};

/* list of hooks for all environments */
hooks = {
    show = (
        { bincmd = "show"; pgname = "gmux"; pgncmd = "show" },
    );
    action = (
        { bincmd = "add"; pgname = "gmux"; pgncmd = "cd" },
    );
    list = ();
};

/* Alias can be used for plugin commands as well.
 * Tho for rn there is no support for nested aliases, i.e. alias
 * can be either builtin command or plugin.  */
alias = {
    dir = "ls";
    els = "env ls";
    dcat = "desk cat";
};
```

## ⇁  Tec builtin commands
```
Usage: tec [OPTION]... COMMAND|PLUGIN
    Run 'tec help tec' to get more info.
    Run 'tec help help' to get more info on command.

    System:
      help       - Show help for commands.
      init       - Init directory structure.
      version    - Display version information.

    Basic:
      add        - Add a new task to environment.
      cat        - Concatenate task unit values.
      cd         - Switch to task.
      ls         - List environment tasks.
      mv         - Move (rename) tasks.
      rm         - Remove task from environment.
      set        - Set task unit values.

    Object:
      cfg        - Manage and show configs.
      desk       - Manage and show desks.
      env        - Manage and show environments.

```


## ⇁  Plugins

There is a ` nine ` plugin manager to install plugins.

Here's some plugin to check out \
` nine ` - [Tec plugin manager](https://github.com/roachme/tec-nine.git) \
` gmux ` - [Manage bunch of git repos](https://github.com/roachme/tec-gmux.git) \
` find ` - [Find stuff in tasks](https://github.com/roachme/tec-find.git)


## ⇁  Install plugin manager

To install plugin manager run the code below
```
PGNDIR="$HOME/.local/lib/tec/pgn"
git clone https://github.com/roachme/tec-nine.git "$PGNDIR/nine"
```
Note: \
1. Make sure repo dirname has NO prefix ` tec- `. The same goes about any plugin to install \
2. ` "$PGNDIR" ` is the path set in ` tec.cfg `.
   Make sure ` PGNDIR ` is the same as ` base.pgn ` in ` tec.cfg ` shown above.


## ⇁  Basic workflow
1. To initalize util type in ` tec init `
2. Now you're ready to create a environment: ` tec env add test `
3. Once task environment is created you can fill it with tasks: ` tec add test1 `
3. Add one more task: ` tec add test2 `
4. List all your task in current environment: ` tec ls `
5. Show the content of task: ` tec cat test1 `
6. Sync with current task: ` tec cd `
7. Or quickly switch to previous task: ` tec cd - `


## ⇁  Tips
1. Use ` tec help ` to get list of commands.
2. Or ` tec help COMMAND ` to get help on specified command.
