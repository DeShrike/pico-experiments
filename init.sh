#!/bin/bash

alias ll='ls -lh'
alias la='ls -A'
alias l='ls -l'

PS1='${debian_chroot:+($debian_chroot)}\[\033[01;33m\]\u@\h\[\033[00m\]:\[\033[01;35m\]\w\[\033[00m\] \033[1;36m\$\033[0m '
