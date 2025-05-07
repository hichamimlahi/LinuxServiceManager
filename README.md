The 'Linux Service Manager' application allows managing services on Linux (listing, starting, stopping, restarting, viewing logs) through a Qt interface.

## Introduction

The Linux Service Manager is a Qt-based application designed to provide a user-friendly interface for managing system services on a Linux operating system.

### Objective
The application allows listing services, starting, stopping, restarting them, and displaying their logs.

### Target Audience
This application is intended for system administrators and developers.

## Features

* **List of services**
    * Displays all system services and their status.
    * Provides detailed information about each service.
    * Allows automatic refreshing of the list.
* **Service control**
    * Allows starting, stopping, and restarting services.
    * Provides visual feedback on operations.
* **Log visualization**
    * Displays service logs.
    * Allows specifying the number of lines to display.
* **User interface**
    * Graphical interface (GUI) built with Qt.
    * Table for displaying services.
    * Controls for managing services.

## Architecture

The application is structured in an object-oriented architecture, with the main classes `ServiceManager` and `MainWindow`. The `ServiceManager` class encapsulates the logic for managing Linux services, while the `MainWindow` class manages the user interface.

## Installation

### Prerequisites

* CMake (version 3.5 or higher)
* Qt 5

### Build instructions

1.  Create a build directory:

    ```bash
    mkdir build
    cd build
    ```
2.  Configure the project with CMake:

    ```bash
    cmake ..
    ```
3.  Build the application:

    ```bash
    make
    ```
4.  Install the application (optional):

    ```bash
    sudo make install
    ```

## Usage

The application provides a graphical interface to manage Linux services. The main features are:

* Displaying the list of services with their status.
* Starting, stopping, and restarting selected services.
* Viewing service logs.
* Automatic refreshing of the service list.

## Tests

All unit and functional tests were successful. The application demonstrated its ability to list, start, stop, and restart services reliably.

## Conclusion

This project resulted in the creation of an application offering a user-friendly and efficient interface for managing services on Linux.
