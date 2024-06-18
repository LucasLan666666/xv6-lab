#!/bin/bash

# 获取所有upstream分支
git fetch upstream

# 列出所有upstream分支，循环处理
for branch in $(git branch -r | grep 'upstream/' | sed 's/upstream\///'); do
    echo "Processing branch: $branch"
    git checkout -b $branch upstream/$branch
    git push origin $branch
done
