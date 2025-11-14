# Hygienie Project

This is the refactored code for  the Hygienie sensor.
Application Behavior:
The Hygienie sensor operates in an ultra-low-power mode using ESP32 deep sleep. On first boot, the system initializes all modules (WiFi, MQTT, NTP time sync, LCD graphics) and displays a startup animation. After initialization, the device enters deep sleep and wakes under two conditions:

Timer-based wake-up (ESP_SLEEP_WAKEUP_TIMER):

Wakes at user-configured intervals
Displays LCD animation
If offline readings are buffered, connects to WiFi, sends all accumulated data via MQTT, syncs time with NTP server, then disconnects
Returns to deep sleep


External GPIO wake-up (ESP_SLEEP_WAKEUP_EXT0):

Triggered by reed switch on GPIO 27 (detects hygiene event like door opening or dispenser activation)
Records timestamp of event using AppMqttAddTime()
Displays LCD animation as visual feedback
If buffer reaches MAX_OFFLINE_READINGS threshold, immediately connects to WiFi, uploads all data via MQTT, syncs time, then disconnects
Returns to deep sleep


The device maintains event timestamps in RTC memory (survives deep sleep) and only activates WiFi/MQTT when needed, minimizing power consumption. All GPIOs are configured for low-power state before each sleep cycle. This architecture allows the sensor to run on battery power for extended periods while reliably capturing and transmitting hygiene compliance data.This means that source code has been refactored into modules, unit-tests have been written for each module and can be run on the esp32 itself for regression testing, and that project values such as wifi network, password, device id, and sleep duration can be entered in a terminal menu.


## Project layout

```
hygienie_project                      — Application project directory
  - components                 — Components of the application project
    + appdata                  — Module that describes how the data is structured.
    + appgraphics              — Module that uses graphics libary and displays animations
    + appmqtt                  — Module that encapsulates mqtt client
    + appsleep                 — Module that controls esp32 sleep  mode
    + appwifi                  — Module that encapsulates wifi libraries
  + main                       - Main source files of the application project
  - test                       — Test project directory
    - main                       — test main directory for test runner
      + hygienie_unit_test.c   — hygienie test runner.
  Makefile / CMakeLists.txt    - Makefiles of the application project
```

When the main application project is compiled, tests are not included. Test project includes the tests by setting `TEST_COMPONENTS` variable in the project makefile.

## Requirements

### Hardware required

* FireBeetle DFR0478, but any esp32 board will work.
* USB cable - USBA/


### Software Requirements
Please look at https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/
for installing esp-idf and https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/linux-macos-setup.html#get-started-linux-macos-first-steps
for configuring and building project on command line.

It is recommended that you run:
```
. $HOME/esp/esp-idf/export.sh
```
in your terminal.

and create the alias:
```
alias get_idf='. $HOME/esp/esp-idf/export.sh'# Configure the project
```
For the menu_config:

For the test project, you can open the project configuration menu (`idf.py menuconfig`) and explore a few options related to Unity under Component Config, Unity unit testing library.

### Build and flash

As explained above, this example contains two projects: application project and test project.

    * Run `idf.py -p PORT flash monitor` in the current directory (`unit_test`).
    * Observe the output: a list of random numbers and their mean value.

2. Test project is responsible for running the tests.

	* Enter `test` directory (`unit_test/test`), and run `idf.py -p PORT flash monitor`.
	* Observe the output: results of test case execution.


See the Getting Started Guide for full steps to configure and use ESP-IDF to build projects.



## Unit Testing

Inside the `testable` component, unit tests are added into `test` directory. `test` directory contains source files of the tests and the component makefile (component.mk / CMakeLists.txt).

```
unit_test
  - components                              - Components of the application project
    - testable
      - include
      - test                                - Test directory of the component
        * component.mk / CMakeLists.txt     - Component makefile of tests
        * test_mean.c                       - Test source file
      * component.mk / CMakeLists.txt       - Component makefile
      * mean.c                              - Component source file
```

When the main application project is compiled, tests are not included. Test project includes the tests by setting `TEST_COMPONENTS` variable in the project makefile.
### Application project output

```
In main application. Collecting 32 random numbers from 1 to 100:
   5   66   86   98   62   66   40   42    7   62
  33   77   40   54    8   75   39   82   73    4
  22    5   72   78   53    1    6   48   23    1
  84   55
Mean: 45
```

### Test project output

```
#### Executing one test by its name #####

Running Mean of an empty array is zero...
/Users/ivan/e/esp-idf/examples/system/unit_test/components/testable/test/test_mean.c:16:Mean of an empty array is zero:PASS

-----------------------
1 Tests 0 Failures 0 Ignored
OK

Running Mean of an empty array is zero...
/home/user/esp/esp-idf/examples/system/unit_test/components/testable/test/test_mean.c:15:Mean of an empty array is zero:PASS
Test ran in 16ms

-----------------------
1 Tests 0 Failures 0 Ignored
OK
Enter next test, or 'enter' to see menu
```
