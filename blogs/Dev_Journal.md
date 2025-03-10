## 03/06/2025
---
Today working on creating a new oled library to read in stuff
also thinking about how to create and add components to an esp idf project

### Thoughts
#### Creating ESP-IDF Components
Had a hard time adding the new ultrasonic library to the ESP project and having all the files be included in the compilation. 
The issue was with the requirements for the library and the CMakeLists file associated with it. <br>
```
|- project_top
    |- components
        |- testComponent
            |- include
                |- testComponent.h
            |- testComponent.c
            |- CMakeLists.txt
    |- main
        |- main.c
        |- CMakeLists.txt
    |- CMakeLists.txt

```
There are 3 `CMakeLists.txt` file, the top-level, main and component-level.<br>
In the top level, we need to include the following line to add a new dir to the component pool
```
    set(EXTRA_COMPONENT_DIRS "<abs or relative path to component>")
```
This will add the directory to the `COMPONENT_DIRS` search, which the folder `components` in an ESP project is already a part of by default<br>
Then we need to add some information to the component level CMake file.<br>
When building the component source files, we may need to include certain libraries (from ESP-IDF or others). These libraries become requirements of the component and need to be included when registering the component, that way during build CMake will add the right source. <br>
One tricky thing is the name of the components is not entirely clear, I have seen blog posts suggest that the name of the component is equal to the component directory name( `testComponent` in our case).<br>
```
set(req driver freertos esp_timer)
idf_component_register(SRCS "testComponent.c" INCLUDE_DIRS "include" REQUIRES ${req})
```
I believe `req` is always required. These requirements could change based on the build target, these make files could have some logic built in to pull variables from the `sdkconfig` file. 

#### OLED Planning/Dev Logs


## 03/02/2025
---
Working on the ultrasonic library, here are the goals for the day
- Understand the example library from [here](https://github.com/ESP32Tutorials/HC-SR04-Ultrasonic-Sensor-with-ESP32-ESP-IDF)
    - Understand how to manipulate the Ultrasonic sensor (with gpio)
    - Understand how to record the return signal
    - Understand how to get a distance measurement from those two signals ()

### Thoughts
Learning about doxygen grouping in the comments...
I don't think I want to fully setup a doxygen output, but it will be good to start off by commenting code in that way.
Looks like doxygen is derived from javadocs....

What are the fail conditions for the ping?
    - The ping can never returns and we end up waiting forever
    - The previous ping hasn't ended
    - The time of the ping is too large or too small and we can't process it (this is needed to deal with the fact that it is a critical section).
Is this process blocking? When should I consider that a process is blocking?<br>
The process is time sensitive, no other processes should run while this one is trying to receive the ping<br>
This guy has his own errors written, where are these being defined?</br>
Normally, errors are defined in the `esp_err.h` file from `0x101` to `0x10D` and some other errs in the 3000 range<br>
<br><br>
It seems like every single funciton is returning an esp_err_t, I bet that has something to do with the RTOS functionality
Instead of returning a value, pass the value into the funciton as a parameter and change it in the function. 

