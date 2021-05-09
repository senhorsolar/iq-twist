#!/usr/bin/env bash

git filter-branch --commit-filter '
    GIT_COMMITTER_NAME="senhorsolar";
    GIT_AUTHOR_NAME="senhorsolar";
    GIT_COMMITTER_EMAIL="25336720+senhorsolar@users.noreply.github.com";
    GIT_AUTHOR_EMAIL="25336720+senhorsolar@users.noreply.github.com";
    git commit-tree "$@";
    ' HEAD

git push --force
