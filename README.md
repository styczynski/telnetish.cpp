# Telnetish :rocket:

Simple C++ implementation of telnet server.


**Contents:**
* **About**
* **Building**
* **Running**
* **Implementation**
  * **Directory structure**
  * **About**

## About

This project was coded for networking course at Warsaw University (Faculty of Mathematics, Informatics and Mechanics).<br>
It's purpose was to design and implement simple telnet server serving basic menu with selectable options.
    
## Building

To build the source please enter the following sequence of commands to your terminal:

```
$ cd <main_folder_of_telnetish>
$ mkdir build && cd build
$ cmake ..
$ make

```

The make command should generate `telnet_server` binary.

## Running

The server usage looks like follows:
```
./telnet_server server <port> [flags]
```

Where there exist the following flags:

| Switch                           | Description                                                                                                                                          |
|----------------------------------|------------------------------------------------------------------------------------------------------------------------------------------------------|
| `--local` <br> *Alias*: `-l`     | Run the server in local mode. In that mode server is not really a server but a local program that runs the implemented ncurses app in your terminal. |
| `--nosplash`                     | Disable initial splash screen animation                                                                                                              |
| `--nocolors` <br> *Alias*: `-nc` | Disable any ANSI color sequences                                                                                                                     |
| `--verbose` <br> *Alias*: `-v`   | Verbosive output mode                                                                                                                                |

## Implementation

### Directory structure

```
telnetish
 |- include  (telnetish header files)
 |- src      (telnetish implemnetations of header files functions)
 \- server   (server entrypoint and ncurses application)
```

### About

The implementation is a bit unusual.<br>
The telnet server itself is implemented in `server/server.cpp` using `include/telnet-server.h` and `include/stimple-telnet-server.h`.

The basic idea is that the clients connects to the server synchronically (each client is served after the current one quits application,
disconnects or is timed out).

The server (see `include/term-program.h`) forks itself and asynchronically runs ncurses applciation (see `src/renderer.h`).<br>
The input and output is transferred from client to the forked process via pipes (and vice-versa).

It enables us to support any terminal type that is supported by ncurses and easilly implement console application via ncurses popular library.<br>
Additionally the server can be started with `--local` switch to run as normal ncurses application without any network communication (useful when
you want to locally test your application).
