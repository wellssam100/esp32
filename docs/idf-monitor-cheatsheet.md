---
title: IDF Monitor
---

:link\_to\_translation:[zh\_CN:\[中文\]]{.title-ref}

IDF Monitor uses the [esp-idf-monitor]() package as a serial terminal
program which relays serial data to and from the target device\'s serial
port. It also provides some ESP-IDF-specific features.

IDF Monitor can be launched from an ESP-IDF project by running
`idf.py monitor`.

Keyboard Shortcuts
==================

For easy interaction with IDF Monitor, use the keyboard shortcuts given
in the table. These keyboard shortcuts can be customized, for more
details see [Configuration File]() section.
| Keyboard Shortcut | Action          | Description |
|---|---|---|
| Ctrl + \]              | Exit the program     ||
| Ctrl + T               | Menu escape key      | Press and follow it by one of the keys given below.       |
| - Ctrl + T             | Send the menu characteritself to remote                             ||
| - Ctrl + \]            | Send the exit  character itself to remote                           ||
| - Ctrl + P             | Reset target into bootloader to pause app via RTS and DTR lines     | Resets the target into the bootloader using the RTS and DTR lines (if connected). This stops the board from executing the application, making it useful when waiting for another device to start. For additional details, refer to `target-reset-into-bootloader`{.interpreted-text role="ref"}.|
| - Ctrl + R             | Reset target board via RTS                                          | Resets the target board and re-starts the application via the RTS line (if connected).|
| - Ctrl + F             | Build and flash the project                                         | Pauses idf\_monitor to run the project `flash` target, then resumes idf\_monitor. Any changed source files are recompiled and then re-flashed. Target `encrypted-flash` is run if idf\_monitor was started with argument `-E`.|
| - Ctrl + A (or A)      | Build and flash the app only                                        | Pauses idf\_monitor to run the `app-flash` target, then resumes idf\_monitor. Similar to the `flash` target, but only the main app is built and re-flashed. Target `encrypted-app-flash` is run if idf\_monitor was started with argument `-E`.|
| - Ctrl + Y             | Stop/resume log output printing on screen                           | Discards all incoming serial data  while activated. Allows to quickly pause and examine log output without quitting the monitor.  |
|- Ctrl + L |Stop/resume log output saved to file            |Creates a file in the project directory and the output is written to that file until this is disabled with the same keyboard shortcut (or IDF Monitor exits).                        |
|- Ctrl + I (or I)|Stop/resume printing timestamps  |IDF Monitor can print a timestamp in the beginning of each line. The timestamp format can be changed by the `--timestamp-format` command line argument.                              |
|- Ctrl + H (or H)|Display all keyboard shortcuts     | |
|- Ctrl + X (or X)| Exit the program | |
|- Ctrl + C | Interrupt running application |Pauses IDF Monitor and runs [GDB]() project debugger to debug the application at runtime. This requires `CONFIG_ESP_SYSTEM_GDBSTUB_RUNTIME`{.interpreted-text role="ref"} option to be enabled.|

Any keys pressed, other than `Ctrl-]` and `Ctrl-T`, will be sent through
the serial port.
