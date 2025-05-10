# Linux Service Manager

[![YouTube Presentation](https://img.shields.io/badge/Video_Presentation-red?style=social&logo=youtube)](https://youtu.be/uo0hi0SMrHg?si=FiZpykZZuh8WWxAj)

The Linux Service Manager is a Qt-based application designed to provide a user-friendly interface for managing system services on a Linux operating system. This project allows users to easily interact with systemd, providing capabilities to list, control, and inspect services.

A detailed report on the project is available in the [rapport_projet.pdf](rapport_projet.pdf) file within this repository, and also on [Google Drive](https://drive.google.com/file/d/1u9P7GXWG5byRqh-ZCVEvPPU4lOXXKphF/view?usp=sharing).

## Introduction

The Linux Service Manager simplifies the administration of Linux services by offering a graphical interface for common tasks.

### Objective

This application provides the following core functionalities:

* Listing system services and their current status.
* Controlling service states: starting, stopping, and restarting.
* Viewing service logs for troubleshooting and monitoring.

A presentation video of the project is available on [YouTube](https://youtu.be/uo0hi0SMrHg?si=FiZpykZZuh8WWxAj).

## Features

* **Service Listing:**
    * Displays a comprehensive list of all system services and their status (e.g., active, inactive, failed).
    * Provides detailed information about each service, including its name, description, and current state.
    * Supports automatic refreshing of the service list to reflect real-time changes.
* **Service Control:**
    * Enables users to start, stop, and restart services with simple button clicks.
    * Offers visual feedback to indicate the success or failure of service operations.
* **Log Visualization:**
    * Allows users to view service logs directly within the application.
    * Provides the option to specify the number of log lines to display, facilitating efficient log analysis.
* **User Interface:**
    * Developed with Qt to provide a cross-platform graphical user interface (GUI).
    * Employs a table widget for clear and organized display of services.
    * Includes intuitive controls (buttons, menus) for easy service management.

## Architecture

The application follows an object-oriented architecture, separating concerns for better maintainability and scalability.

* **ServiceManager Class:**
    * Encapsulates the logic for interacting with the Linux `systemd` service manager.
    * Uses the `QProcess` class to execute `systemctl` commands.
* **MainWindow Class:**
    * Manages the graphical user interface of the application.
    * Utilizes Qt widgets to display service information and handle user interactions.

## Installation

### Prerequisites

Before building the application, ensure that the following software is installed on your system:

* CMake (version 3.5 or later)
* Qt 5 (with the Widgets module)

### Build Instructions

Follow these steps to build the application from source:

1.  **Create a build directory:**

    ```bash
    mkdir build
    cd build
    ```

2.  **Configure the project with CMake:**

    ```bash
    cmake ..
    ```

3.  **Build the application:**

    ```bash
    make
    ```

4.  **Install the application (optional):**

    ```bash
    sudo make install
    ```

## Usage

The Linux Service Manager provides a user-friendly GUI for common service management tasks:

* **Displaying Service List:** The main window displays a table with all services and their status.
* **Controlling Services:** Buttons are provided to start, stop, and restart the selected service.
* **Viewing Logs:** The "View Logs" button opens a dialog to display the logs of the selected service.
* **Automatic Refresh:** The application can automatically refresh the service list at a specified interval.

## Tests

The application has undergone thorough testing, including:

* **Unit Tests:** To verify the functionality of individual classes (`ServiceManager`, `MainWindow`).
* **Functional Tests:** To ensure the application behaves as expected in various user scenarios.

All tests were successful, demonstrating the application's reliability in listing, starting, stopping, and restarting services. Log viewing and automatic refresh features also functioned correctly.

## Conclusion

This project successfully delivered a user-friendly and efficient application for managing Linux services. It empowers system administrators and developers with a valuable tool to control and monitor services effectively.

For more details, a presentation video is available on [YouTube](https://youtu.be/uo0hi0SMrHg?si=FiZpykZZuh8WWxAj).

