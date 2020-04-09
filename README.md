# SpyroGame
# Build steps
1. Install required nuget packages on all projects
2. Set game project to startup project
3. Set the build to either debug or release x64
4. Do an initial build
5. Run the batch script in the root folder twice (it doesn't always work the first time for some reason)
6. Projects should now run

## Useful commands
## Prune large file from repo after resizing to <100mb to avoid using lfs
```
git filter-branch --index-filter "git rm -rf --cached --ignore-unmatch <filepath>" HEAD
```