# ticker

Displays file information with a scroll effect. Designed with xmobar in mind.

Expects a single line file with a format: index delimiter string (no whitespace)

A primer script should probably be used to determine the argument that will be passed to the ticker program.


## Usage

`user@host:~ $ ticker "SongTitle - Artist"`

ticker compares argument string with string in file
- if different: ticker will truncate the file and write new values
- if same: ticker will progress with the scrolling, incrementing index by one
