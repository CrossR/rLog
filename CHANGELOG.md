## v1.1.0

#### Updates

   - Bumped all dependencies up, fixed  up build.
   - Incorporate other changes into new release.

## v1.0.2

#### Features

   - Add `-l` flag to link to older log files. (#35)
   - Add `list` command, to list previous runs. (#37)

## v1.0.1

#### Features

   - Format execution time into `HH:MM:SS.xxxx` format. (#30)

#### Fixes

   - Fix issue around midnight where log files would be split over 2 days. (#32)

## v1.0.0

#### Features

   - Add force flag for CLI, to force the making of link files. (#23)
   - Add recursive flag to `link` (`-R`), so that folders can be linked to recursively. (#27)

#### Fixes

   - Fix error code capture. (#20)
   - Fix link file issues. (#21)
   - Fix storing output of first link command. (#22)
   - Use the local system time, rather than UTC (#25)

## v0.1.2

#### Features

   - Add `link` command, to link the last `rLog run` run to any given output files.
     This also lets commands be ran at the end of the process. This will be easier
     once I add variable syntax. (#15)
   - Add variable syntax, to replace variables in commands with in process variables. (#16)

#### Fixes

   - Refactor and tidy up. (#14)

## v0.1.1

#### Features

   - Add `-s` as a silent flag, to mute all output. (#8)
   - Expand metadata for commands, to add time (#10)

#### Fixes

  - Fix config generation, and checking of paths. (#7)
  - Fix config printing to be more accurate. (#9)
  - Fix large command output causing large slow down (#11)

## v0.1.0

First release!
