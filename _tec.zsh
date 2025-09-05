#compdef tec

local -a subcommands global_opts

global_opts=(
    '(-C)'{-C,--color}'[enable colors]:toggle:(on off)'
    '(-D)'{-D,--debug}'[enable debug mode]:toggle:(on off)'
    '(-h)'{-h,--help}'[show help and exit]'
    '(-F)'{-F,--config}'[path to config file]:directory:_files'
    '(-H)'{-H,--hooks}'[enable hooks]:toggle:(on off)'
    '(-P)'{-P,--elugins}'[plugins directory]:directory:_files'
    '(-T)'{-T,--tasks}'[tasks directory]:directory:_files'
    '(-V)'{-V,--version}'[show version and exit]'
)

_subcommands() {
    subcommands=(
        'help:Show help for commands'
        'init:Init directory structure'
        'add:Add a new task to env'
        'del:Delete task from env'
        'list:List env tasks'
        'move:Move (rename) tasks'
        'prev:Switch to previous task'
        'set:Set task unit values'
        'show:Show task unit values'
        'sync:Synchronize (with) task'
        'desk:Manage and show desks'
        'column:Manage and show columns'
        'env:Manage and show envs'
    )
}

_tec_help() {
    _arguments \
        '(-d)'{-d,--desc}'[output short description for each topic]' \
        '(-s)'{-s,--synopsis}'[output only a short usage synopsis]' \
        ':command:->command'

    case $state in
        command)
            local -a help_topics
            help_topics=(${(k)_tec_subcommands} ${(k)_tec_desk_subcommands} ${(k)_tec_column_subcommands} ${(k)_tec_env_subcommands})
            _describe 'help topic' help_topics
            ;;
    esac
}

_tec_add() {
    _arguments \
        '(-d)'{-d,--desk}'[desk name]:desk:_tec_desks' \
        '(-h)'{-h,--help}'[show help and exit]' \
        '(-n)'{-n,--no-switch}'[do not switch to task]' \
        '(-e)'{-e,--env}'[env name]:env:_tec_envs' \
        '(-q)'{-q,--quiet}'[do not write to stderr]' \
        '(-N)'{-N,--no-switch-dir}'[neither switch to task nor to task directory]' \
        '*:task ID:'
}

_tec_del() {
    _arguments \
        '(-d)'{-d,--desk}'[desk name]:desk:_tec_desks' \
        '(-h)'{-h,--help}'[show help and exit]' \
        '(-e)'{-e,--env}'[env name]:env:_tec_envs' \
        '(-q)'{-q,--quiet}'[do not write to stderr]' \
        '(-y)'{-y,--yes}'[delete without confirmation]' \
        '*:task ID:_tec_tasks'
}

_tec_list() {
    _arguments \
        '(-a)'{-a,--all}'[list all tasks]' \
        '(-d)'{-d,--desk}'[desk name]:desk:_tec_desks' \
        '(-h)'{-h,--help}'[show help and exit]' \
        '(-q)'{-q,--quiet}'[do not write to stderr]' \
        '(-v)'{-v,--verbose}'[show more verbose output]' \
        '(-t)'{-t,--toggles}'[show only toggle switches]' \
        '(-H)'{-H,--headers}'[show headers]' \
        '*:env:_tec_envs'
}

_tec_move() {
    _arguments \
        '(-d)'{-d,--destination}'[destination env]:env:_tec_envs' \
        '(-f)'{-f,--force}'[overwrite destination task]' \
        '(-s)'{-s,--source}'[source env]:env:_tec_envs' \
        '(-T)'{-T,--tasks}'[treat all IDs as source to move to env]' \
        '*:task ID:_tec_tasks'
}

_tec_prev() {
    _arguments \
        '(-d)'{-d,--desk}'[desk name]:desk:_tec_desks' \
        '(-h)'{-h,--help}'[show help and exit]' \
        '(-e)'{-e,--env}'[env name]:env:_tec_envs' \
        '(-q)'{-q,--quiet}'[do not write to stderr]'
}

_tec_set() {
    _arguments \
        '(-d)'{-d,--desk}'[desk name]:desk:_tec_desks' \
        '(-d)'{-d,--description}'[task description]' \
        '(-h)'{-h,--help}'[show help and exit]' \
        '(-e)'{-e,--env}'[env name]:env:_tec_envs' \
        '(-q)'{-q,--quiet}'[do not write to stderr]' \
        '(-t)'{-t,--type}'[task type]:type:(task bugfix feature hotfix)' \
        '(-P)'{-P,--eriority}'[task priority]:priority:(lowest low mid high highest)' \
        '*:task ID:_tec_tasks'
}

_tec_show() {
    _arguments \
        '(-d)'{-d,--desk}'[desk name]:desk:_tec_desks' \
        '(-h)'{-h,--help}'[show help and exit]' \
        '(-k)'{-k,--key}'[key to show]:key' \
        '(-e)'{-e,--env}'[env name]:env:_tec_envs' \
        '(-q)'{-q,--quiet}'[do not write to stderr]' \
        '*:task ID:_tec_tasks'
}

_tec_sync() {
    _arguments \
        '(-d)'{-d,--desk}'[desk name]:desk:_tec_desks' \
        '(-n)'{-n,--no-update}'[do not update toggles]' \
        '(-h)'{-h,--help}'[show help and exit]' \
        '(-e)'{-e,--env}'[env name]:env:_tec_envs' \
        '(-q)'{-q,--quiet}'[do not write to stderr]' \
        '(-N)'{-N,--no-switch-dir}'[neither update toggles nor switch to task directory]' \
        '*:task ID:_tec_tasks'
}

# Desk subcommands
_tec_desk() {
    local curcontext="$curcontext" state line
    typeset -A opt_args

    _arguments -C \
        '(-h)'{-h,--help}'[show help and exit]' \
        ':subcommand:->subcmd' \
        '*:: :->args'

    case $state in
        subcmd)
            local -a desk_subcommands
            desk_subcommands=(
                'add:Add a new desk'
                'del:Delete desk with all tasks'
                'list:List desks'
                'move:Move (rename) desks'
                'prev:Switch to previous desk'
                'set:Set desk values'
                'show:Show desk info'
                'sync:Switch to or synchronize (with) desk'
            )
            _describe -t commands 'desk subcommand' desk_subcommands
            ;;
        args)
            case $line[1] in
                add) _tec_desk_add ;;
                del) _tec_desk_del ;;
                list) _tec_desk_list ;;
                move) _tec_desk_move ;;
                prev) _tec_desk_prev ;;
                set) _tec_desk_set ;;
                show) _tec_desk_show ;;
                sync) _tec_desk_sync ;;
            esac
            ;;
    esac
}

_tec_desk_add() {
    _arguments \
        '(-h)'{-h,--help}'[show help and exit]' \
        '(-n)'{-n,--no-switch}'[do not switch to new desk]' \
        '(-e)'{-e,--env}'[env name]:env:_tec_envs' \
        '(-q)'{-q,--quiet}'[do not write to stderr]' \
        '*:desk name:'
}

_tec_desk_del() {
    _arguments \
        '(-h)'{-h,--help}'[show help and exit]' \
        '(-n)'{-n,--no-confirm}'[delete without confirmation]' \
        '(-e)'{-e,--env}'[env name]:env:_tec_envs' \
        '(-q)'{-q,--quiet}'[do not write to stderr]' \
        '*:desk name:_tec_desks'
}

_tec_desk_list() {
    _arguments \
        '(-h)'{-h,--help}'[show help and exit]' \
        '(-q)'{-q,--quiet}'[do not write to stderr]' \
        ':env:_tec_envs'
}

_tec_desk_move() {
    _arguments \
        '(-h)'{-h,--help}'[show help and exit]' \
        '(-e)'{-e,--env}'[env name]:env:_tec_envs' \
        '(-q)'{-q,--quiet}'[do not write to stderr]' \
        ':source desk:_tec_desks' \
        ':destination desk:'
}

_tec_desk_prev() {
    _arguments \
        '(-h)'{-h,--help}'[show help and exit]' \
        '(-e)'{-e,--env}'[env name]:env:_tec_envs' \
        '(-q)'{-q,--quiet}'[do not write to stderr]'
}

_tec_desk_set() {
    _arguments \
        '(-d)'{-d,--description}'[desk description]' \
        '(-h)'{-h,--help}'[show help and exit]' \
        '(-e)'{-e,--env}'[env name]:env:_tec_envs' \
        '(-q)'{-q,--quiet}'[do not write to stderr]' \
        '*:desk name:_tec_desks'
}

_tec_desk_show() {
    _arguments \
        '(-d)'{-d,--desk}'[desk name]:desk:_tec_desks' \
        '(-h)'{-h,--help}'[show help and exit]' \
        '(-k)'{-k,--key}'[key to show]:key' \
        '(-e)'{-e,--env}'[env name]:env:_tec_envs' \
        '(-q)'{-q,--quiet}'[do not write to stderr]' \
        '*:desk name:_tec_desks'
}

_tec_desk_sync() {
    _arguments \
        '(-h)'{-h,--help}'[show help and exit]' \
        '(-n)'{-n,--no-switch}'[do not switch to desk]' \
        '(-e)'{-e,--env}'[env name]:env:_tec_envs' \
        '(-q)'{-q,--quiet}'[do not write to stderr]' \
        '*:desk name:_tec_desks'
}

# Column subcommands
_tec_column() {
    local curcontext="$curcontext" state line
    typeset -A opt_args

    _arguments -C \
        '(-h)'{-h,--help}'[show help and exit]' \
        ':subcommand:->subcmd' \
        '*:: :->args'

    case $state in
        subcmd)
            local -a column_subcommands
            column_subcommands=(
                'list:List columns'
                'move:Move task to column'
            )
            _describe -t commands 'column subcommand' column_subcommands
            ;;
        args)
            case $line[1] in
                list) _tec_column_list ;;
                move) _tec_column_move ;;
            esac
            ;;
    esac
}

_tec_column_list() {
    _arguments \
        '(-h)'{-h,--help}'[show help and exit]'
}

_tec_column_move() {
    _arguments \
        '(-d)'{-d,--desk}'[desk name]:desk:_tec_desks' \
        '(-c)'{-c,--column}'[column to move to]:column:_tec_columns' \
        '(-h)'{-h,--help}'[show help and exit]' \
        '(-q)'{-q,--quiet}'[do not write to stderr]' \
        '(-e)'{-e,--env}'[env name]:env:_tec_envs' \
        '*:task ID:_tec_tasks'
}

# Environment subcommands
_tec_env() {
    local curcontext="$curcontext" state line
    typeset -A opt_args

    _arguments -C \
        '(-h)'{-h,--help}'[show help and exit]' \
        ':subcommand:->subcmd' \
        '*:: :->args'

    case $state in
        subcmd)
            local -a env_subcommands
            env_subcommands=(
                'add:Add a new env'
                'del:Delete env with all desks and tasks'
                'list:List envs'
                'prev:Switch to previous env'
                'rename:Rename env'
                'set:Set env values'
                'show:Show env info'
                'sync:Switch to or synchronize (with) env'
            )
            _describe -t commands 'env subcommand' env_subcommands
            ;;
        args)
            case $line[1] in
                add) _tec_env_add ;;
                del) _tec_env_del ;;
                list) _tec_env_list ;;
                prev) _tec_env_prev ;;
                rename) _tec_env_rename ;;
                set) _tec_env_set ;;
                show) _tec_env_show ;;
                sync) _tec_env_sync ;;
            esac
            ;;
    esac
}

_tec_env_add() {
    _arguments \
        '(-d)'{-d,--desk}'[desk name]:desk:_tec_desks' \
        '(-h)'{-h,--help}'[show help and exit]' \
        '(-n)'{-n,--no-switch}'[do not switch to new env]' \
        '(-q)'{-q,--quiet}'[do not write to stderr]' \
        '*:env name:'
}

_tec_env_del() {
    _arguments \
        '(-h)'{-h,--help}'[show help and exit]' \
        '(-n)'{-n,--no-confirm}'[delete without confirmation]' \
        '(-q)'{-q,--quiet}'[do not write to stderr]' \
        '*:env name:_tec_envs'
}

_tec_env_list() {
    _arguments \
        '(-h)'{-h,--help}'[show help and exit]' \
        '(-q)'{-q,--quiet}'[do not write to stderr]' \
        '(-v)'{-v,--verbose}'[show more info]'
}

_tec_env_prev() {
    _arguments \
        '(-h)'{-h,--help}'[show help and exit]'
}

_tec_env_rename() {
    _arguments \
        '(-h)'{-h,--help}'[show help and exit]' \
        '(-q)'{-q,--quiet}'[do not write to stderr]' \
        ':source env:_tec_envs' \
        ':destination env:'
}

_tec_env_set() {
    _arguments \
        '(-d)'{-d,--description}'[env description]' \
        '(-h)'{-h,--help}'[show help and exit]' \
        '(-q)'{-q,--quiet}'[do not write to stderr]' \
        '*:env name:_tec_envs'
}

_tec_env_show() {
    _arguments \
        '(-h)'{-h,--help}'[show help and exit]' \
        '(-q)'{-q,--quiet}'[do not write to stderr]' \
        '*:env name:_tec_envs'
}

_tec_env_sync() {
    _arguments \
        '(-h)'{-h,--help}'[show help and exit]' \
        '(-n)'{-n,--no-switch}'[do not switch to env]' \
        '(-q)'{-q,--quiet}'[do not write to stderr]' \
        '*:env name:_tec_envs'
}

# Helper functions
_tec_envs() {
    local -a envs
    # This would ideally query the actual envs, but for completion we'll just provide a stub
    envs=('env1' 'env2' 'env3')
    _describe 'env' envs
}

_tec_desks() {
    local -a desks
    # Similarly, this would query the actual desks
    desks=('desk1' 'desk2' 'desk3')
    _describe 'desk' desks
}

_tec_columns() {
    local -a columns
    columns=('todo' 'in-erogress' 'done' 'review')
    _describe 'column' columns
}

_tec_tasks() {
    local -a tasks
    # This would query the actual tasks
    tasks=('task1' 'task2' 'task3')
    _describe 'task' tasks
}

# Main completion function
_tec() {
    local curcontext="$curcontext" state line
    typeset -A opt_args

    _arguments -C -s \
        $global_opts \
        ':subcommand:->subcmd' \
        '*:: :->args'

    case $state in
        subcmd)
            _subcommands
            _describe -t commands 'tec command' subcommands
            ;;
        args)
            case $line[1] in
                help) _tec_help ;;
                init) ;;
                add) _tec_add ;;
                del) _tec_del ;;
                list) _tec_list ;;
                move) _tec_move ;;
                prev) _tec_prev ;;
                set) _tec_set ;;
                show) _tec_show ;;
                sync) _tec_sync ;;
                desk) _tec_desk ;;
                column) _tec_column ;;
                env) _tec_env ;;
            esac
            ;;
    esac
}

_tec "$@"
