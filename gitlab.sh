#!/bin/bash

last_commit_message=$(git log -1 --pretty=%B)
cd /home/gruvw/SynologyDrive/EPFL/BA5/SEM/tic-tac-tile
git add .
git commit -m "$last_commit_message"
