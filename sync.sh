#!/bin/bash

# 获取所有 upstream 分支
git fetch upstream

# 列出所有 upstream 分支，循环处理
for branch in $(git branch -r | grep 'upstream/' | sed 's/upstream\///'); do
    echo "Processing branch: $branch"
    # 检查本地是否存在分支，如果不存在则创建
    if git show-ref --verify --quiet refs/heads/$branch; then
        # 如果分支已存在，切换到该分支
        git checkout $branch
    else
        # 如果分支不存在，基于 upstream 创建新的本地分支
        git checkout -b $branch upstream/$branch
    fi
    # 将 upstream 的更改合并到当前分支
    git merge upstream/$branch
    # 将更改推送到 origin
    git push origin $branch
done

# 回到主分支
git checkout main
