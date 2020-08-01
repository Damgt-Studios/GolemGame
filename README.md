# The Great Golem
# Build steps
1. Install required nuget packages on all projects
2. Set game project to startup project
3. Set the build to either debug or release x64
4. Do an initial build
5. Run the batch script in the root folder twice (it doesn't always work the first time for some reason)
    -   (Optional) Instead of running the batch script you can install the Nodejs LTS and run the watchman.bat script that will automatically keep the appx and local files folder in sync. It requires at least nodejs version 8 or later to function properly. Start the script once and leave it to run in the background.
6. Projects should now run

## Useful commands
## Prune large file from repo after resizing to <100mb to avoid using lfs
```
git filter-branch --index-filter "git rm -rf --cached --ignore-unmatch <filepath>" HEAD
```

Controls:
Control Pad Up – Switch Golem Selection Up.
Control Pad Down – Switch Golem Selection Down.
Control Pad Left - Consume a MInion to recover health.
Left Thumbstick – Move and control the giant.
Right Thumbstick – Direct the camera.  Mobs charge towards camera facing.  
Start – Pause Menu, Resume from any option in the Menu.
Select – Debug Menu for Developer Mode
A – Slam Peasants
X – Punch Structure
Y – Special Skill
B – Kick Attack
RB – Switch Player Mode Right
LB – Switch Player Mode Left
RT – Send Minions toward camera direction.
LT – Make Minions return to the player.
