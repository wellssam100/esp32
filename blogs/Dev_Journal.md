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