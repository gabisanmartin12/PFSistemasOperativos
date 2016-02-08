#!/bin/sh
# Script to add the current work directory to the ~/.bashrc file.
# This way we can execute the minishell anywhere.
echo "export PATH=$PATH:$PWD" >> ~/.bashrc
