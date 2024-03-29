# CPPChat, a C++ chat server and client

> CPPChat is a school project, and is not intended for production use.

This repository contains a simple chat server and client written in C++, using the Qt framework.
It is a example of how to use Qt's networking classes to create a simple chat server and client. All message are stored in a SQL database.

The server and the cpp client can be compiled on Windows, Linux and Mac OS X. ( We have not tested the server on Mac OS X, but it should work. )

Also, we have a web client, written in JavaScript, that uses websockets to connect to the server.

## Building

### Requirements

* Qt 5.2 or later
* CMake 2.8.11 or later

### Building the server

To build the server, run the following commands:

    mkdir build
    cd build
    cmake ..
    make

### Building the client

Same as the server, but run the following commands:

    mkdir build
    cd build
    cmake -DBUILD_CLIENT=ON ..
    make

### Building the web client
    Double click on the `webclient/index.html` file. x)

## What's next?

This is a very simple example of a chat server and client. There are many things that could be added to it, such as:

* A GUI for the server
* Asymmetric encryption
* A better protocol
* A better authentication system (maybe using SSL)
* A better way to handle the client list
* Add cool features, like private messages, markdown on messages, command system ( like Discord ), etc.

## License

This project is licensed under the MIT License. See the LICENSE file for more details ( Noticed that we don't have a LICENSE file, this is a trash project, make whatever you want with it. x) ).

## Contributing

I thank my 2 friends and group members for helping me with this project: [`@ThomasBerthier`]( https://github.com/ThomasBerthier ), [`@BoucherL`]( https://github.com/BoucherL )

I'm not accepting pull requests, but feel free to fork this project and make your own version of it. x)
