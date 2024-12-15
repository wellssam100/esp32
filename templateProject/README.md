
templateProject/
├── CMakeLists.txt
├── main/
│   ├── CMakeLists.txt
│   ├── main.c
├── components/ (optional)
├── sdkconfig


CMakeLists.txt defines how to build the project as a whole
Doesn't do anything specific, that is saved for the sub-Cmake files. It provides access to the libraries needed by child Cmake files.

ESP-IDF expects a directory named main. It is implicitly referenced through the esp-idf build system. 
main contains the application logic
main/main.c is the entry point
main/CMakeLists.txt is the file that specifies how to compile the source files

sdkconfig is the file generated by running idf.py menuconfig

components can hold reusable modules
components/
├── templateComponent/
│   ├── CMakeLists.txt
│   ├── include/
│   │   └── my_component.h
│   └── my_component.c
