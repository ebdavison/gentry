# Gentry v0.2.0

This program fits a need of mine for what I have found lacking for quite
some time in the database realm.  That need, simply put, is data entry.
But the data entry needed to be generic enough that it could accomodate
different tables.

So, I wrote this program as part of my attempt to improve my skills in C
and GTK+.  And, as i have been an active believer and consumer of free
programs, I felt I should return the favor and make this program available
for public consumption.

## Reqirements

This program requires MySQL client libraries and development headers as 
that is the program of choice for my database work.

Also, this requires GTK Version 2.0 and its development headers.

## Screenshots

Example entry screen:

![screenshot 1](gentry1.png)

## Contributions

Feel free to modify the program or improve upon it and send the patches
or updates to me and I will be glad to include them.  For now, this program
has made some simple shortcuts in order to get it functional.

## Usage

To run the program, you must execute it with the database and table name
on the command line, as follows:

```
	gentry personal address
```

There are new command line switches that allow you to specify the username
and password on the command line as well:

```
	gentry personal address -u username -p password
```


There are three buttons.  

The first is Insert and it does exactly what it says, inserts the data into
the database.  If there is an error with the insert, the data will be left
on the screen and the MySQL error message displayed in the status bar.  If
the insert is successful, the data items will be cleared and you will be
ready to continue with the next entry.

The Clear button just clears the data from the entry boxes.

The Cancel button exits the application.  It was named Cancel as I felt the
data entry should be canceled to stop.  (Maybe Quit would be more
appropriate; any feedback on this?)

## Closing

That is all, I suppose.

Ed Davison

edavison@users.sourceforge.net

http://gentry.sourceforge.net
