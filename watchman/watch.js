const fs = require("fs");
const path = require("path");
const ncp = require("ncp");
const del = require("delete");

// meta data
const watch_path = path.join(__dirname, "..", "files");
const dest_directories = [
    path.join(__dirname, "..", "x64", "Debug", "Game", "AppX", "files"),
    path.join(__dirname, "..", "x64", "Release", "Game", "AppX", "files"),
];

console.log("Whittington Watchman v1.0\n");
console.log("Watching path: ", watch_path);

console.log("Num of destination directories: ", dest_directories.length);
for(dest of dest_directories)
{
    console.log("Dest dir:", dest);
}

console.log();

// Initial copy
for(dest of dest_directories)
{
    ncp(watch_path, dest, err => 
        {
            if(err) console.log("Initial copy to ", dest, "failed");
            else console.log("Initial copy to ", dest, "was successful");
        });
}

// Copy event
fs.watch(watch_path, { recursive: true }, (event, filename) => 
{
    if(filename)
    {
        if(filename.toLowerCase() !== "new folder")
        {
            console.log("Change detected: ", filename);
            console.log("Copying files");

            // Delete everything first
            del.sync(dest_directories);

            let fail_count = 0;
            for(dest of dest_directories)
            {
                ncp(watch_path, dest, err => 
                {
                    if(err) 
                    {
                        fail_count++;
                        console.log("Copy to ", dest, "failed");
                    }
                    else console.log("Copy to ", dest, "was successful");
                });
            }

            if(fail_count > 0)
            {
                console.log("MESSAGE: ", "copy errors happen on file deletions, if you saw copy errors after deleting a file, this is fine");
            }
        }
    }
});