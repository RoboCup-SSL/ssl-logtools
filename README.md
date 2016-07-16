ssl-logtools
============

Log recording tools for RoboCup Small Size League games

Copyright (c) 2013 Robotics Erlangen e.V. <br>
http://www.robotics-erlangen.de/ <br>
info@robotics-erlangen.de <br>

This software was written by team members of ER-Force at
Robotics Erlangen e.V., Germany.

The ssl-logtools can record and playback the messages of SSL-Vision
(https://github.com/RoboCup-SSL/ssl-vision) and the SSL-RefBox
(https://github.com/RoboCup-SSL/ssl-refbox).

Compiling:
------------

All programs should compile on GNU/Linux, Windows and Mac OS X.

In order to build the ssl-logtools you will need:
  * cmake-2.8.2
  * g++-4.1
  * qt-gui-4.6.0
  * boost-program-options-1.42.0
  * zlib-1.2.7
  * protobuf-2.0.0

The recommended way of building a project with CMake is by doing an
out-of-source build. This can be done like this:

> mkdir build <br>
> cd build <br>
> cmake .. <br>
> make <br>

Binaries will be created in the subdirectory "bin" of the "build" folder.

Example Usage:
------------

Record log:
> logrecorder -o /tmp/test.log

Record log and write it to gzip compressed file:
> logrecorder --compress -o /tmp/test.log.gz

Automatically create a new log for each new game based on the referee commands:
> logdaemon -o /tmp/logs/

Print help message with all available command-line options:
> logrecorder --help

Commands can be aborted using Control-C.

Both uncompressed (*.log) and gzip compressed (*.log.gz) log files can be played back
using "logplayer".

Reading Log Files Using Your Own Software:
------------

If you want to read the log files using your own software, look at the class
"LogFile" in "src/common/logfile.h".

Alternatively a plain C++ example for reading log files with minimal dependecies
is provided in "src/examples".

Default Binary Format of the Log Files:
------------

The log files are created by default in format version 1.

Each log file starts with the following header:

> 1: String - File type ("SSL_LOG_FILE") <br>
> 2: Int32  - Log file format version <br>

Format version 1 encodes the protobuf messages in the following format:

> 1: Int64  - Receiver timestamp in ns <br>
> 2: Int32  - Message type <br>
> 3: Int32  - Size of binary protobuf message <br>
> 4: String - Binary protobuf message <br>

The message types are:

> MESSAGE_BLANK           = 0 (ignore message)<br>
> MESSAGE_UNKNOWN         = 1 (try to guess message type by parsing the data)<br>
> MESSAGE_SSL_VISION_2010 = 2<br>
> MESSAGE_SSL_REFBOX_2013 = 3<br>
> MESSAGE_SSL_VISION_2014 = 4<br>

To convert any supported log to file format version 1 run:
> logconvert -f 1 -i /tmp/in.log -o /tmp/out.log

License:
------------

The software is licensed to you under the GNU General Public License
Version 3. The license can be found in the file "LICENSE".

Source:
------------

The latest version of this software can be obtained at
https://github.com/RoboCup-SSL/ssl-logtools.
