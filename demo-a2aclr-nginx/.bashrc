# ~/.bashrc: executed by bash(1) for non-login shells.

# Note: PS1 and umask are already set in /etc/profile. You should not
# need this unless you want different defaults for root.
# PS1='${debian_chroot:+($debian_chroot)}\h:\w\$ '
# umask 022

# You may uncomment the following lines if you want `ls' to be colorized:
# export LS_OPTIONS='--color=auto'
# eval "`dircolors`"
# alias ls='ls $LS_OPTIONS'
# alias ll='ls $LS_OPTIONS -l'
# alias l='ls $LS_OPTIONS -lA'
#
# Some more alias to avoid making mistakes:
# alias rm='rm -i'
# alias cp='cp -i'
# alias mv='mv -i'

alias ll='ls -al'
alias lr='ls -altr'
alias gl='git log'
alias gs='git status'
alias gd='git diff'
alias fgr='fgrep -RI'
alias fgri='fgrep -iRI'
alias f='find . -name'
alias vl='cd /var/log'
alias tf='tail -n40 -f'
alias lint='cc -Wall -Wextra -pedantic -Wno-unused-parameter -fsyntax-only'

set -o vi
