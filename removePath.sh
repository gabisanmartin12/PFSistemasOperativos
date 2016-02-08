#!/bin/sh
# Script to remove the current work directory from $PATH environment variable.
var="${PWD##*/}"
sed -i "/$var/ d" ~/.bashrc

