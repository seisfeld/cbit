# cbit change log

## r27
* extends axl api matrix
* updating README.md
* 2018 updates

## r26
* refining csv delimiter treatment
* changing the year in copyright line in all files

## r25
* updating README.md
* renaming cbit_config.h to cbit_config
* changing the year in copyright line in all files
* adding newline at the end of all files

## r24
* adding summary output to log file
* adding job run time statistics to log file
* adding csv delimiter override option when csv delimiter missmatch is detected
* minor log file tweaks
* minor documentation changes
* changing version numbering to release numbers (rXX)
* code cleanup

## v1.23.0
* simplifying functions ansi(), curl_error_text() and is_cucmdb_version_supported()
* adding display of configured csv delimiter to -c option
* adding csv delimiter check on startup
* minor interface tweaks

## v1.22.0
* adding curl module detection on startup
* adding detailed curl error messages (on php versions >= 5.5.0)
* removing link to explain curl errors when showing curls error code again
* code cleanup

## v1.21.1
* fixed bug in get_cucm_version()

## v1.21.0
* fixed bug in name space declaration of getCCMVersion request
* made cucm version detection more robust
* added debug info at certain exit points
* added "_" (underscore) and "-" (dash/hyphen) to the list of allowed characters for variable names
* added check for duplicates in csv header
* added check for cbitId in csv header
* added display of password back to -c option
* code cleanup

## v1.20.2
* added a link to explain curl errors when showing curls error code

## v1.20.1
* added job run time statistics

## v1.20.0
* added support for empty data cells in csv file (use 'null')
* code cleanup

## v1.19.1
* code cleanup

## v1.19.0
* improved windows support
* code cleanup

## v1.18.0
* switched versioning to x.y.z
* removed test mode
* updated -h output
* rewrote CHANGES file with markdown
* rewrote LICENSE file with markdown
* updated documentation
* code cleanup

## v1.17
* removed -i option in favor of a complete rewrite of the template versioning
* updated documentation

## v1.16
* added -i option to disable check of template version against cucm version
* added -b option to run a simple latency benchmark

## v1.15
* added MIT license
* published code on github

## v1.14
* added outgoing interface parameter to cucm profiles
* added display of outgoing interface to -c option
* removed display of password from -c option

## v1.13
* added check if the csv file contains only the header but no data
* added check if the csv file is empty
* added display number of data rows to process
* reworded some log file entries
* code cleanup

## v1.12
* ignore newlines and remove empty data rows when reading the csv input file
* added various sanity checks when reading the csv input file
* added unix timestamp to log file name
* added more comments

## v1.11
* added more info about request status to log file
* code cleanup

## v1.10
* added unknown variable detection in templates
* reworded some log file entries
* code cleanup

## v1.9
* added -s option to disable skipping subsequent jobs if a previous one failed
* added -h for help
* changed curl timeout to 3 seconds
* made log file more readable
* added template store to cache template data
* code cleanup

## v1.8
* added template versioning support
* added exception handling for main loop
* code cleanup

## v1.7
* added summary output
* code cleanup

## v1.6
* added missing templates to log file
* made log file more readable
* code cleanup

## v1.5
* added check if templates really exist on disk
* reworded log file entries

## v1.4
* added logfile renaming
* made log file more readable
* code cleanup

## v1.3
* added cucm cluster profiles

## v1.2
* added -c option to dump config
* code cleanup

## v1.1
* fixed typos
* code cleanup

## v1.0
* initial release
