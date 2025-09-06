#!/bin/env bash

install_program() {
    echo "under development"
}

install_completions() {
    # install zsh completions
    mkdir -p $HOME/.zsh/completions
    cp completions/_tec.zsh $HOME/.zsh/completions

    # TODO: add completions for bash
}


install_program
install_completions
