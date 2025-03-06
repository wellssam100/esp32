## High Level Design of Project

The goal of this project is to use the ESP-32 to read in data from an external sensor (HC-SR04-Ultrasonic-Sensor) and display it on an OLED screen attached to the board. 
The second goal is to have the information gathered from the sensor on one machine transmit through the ESP32's wifi capabilities to another ESP32 and display on a screen there. 
Finally the goal of the project will be to add an front end where this telemetry can be consumed and actions can be sent out (command post). Most likely this will require some front end and graphics work, perhaps along with some cloud/pdc management. This is a stretch goal and will most likely be a different project. 

### Stories
#### Getting Ultrasonic Sensor Setup
- [x] ~~Connect third party HC Component library to dev Environment~~
- [x] Write the functions to collect the distance data See [here](https://randomnerdtutorials.com/complete-guide-for-ultrasonic-sensor-hc-sr04/) for basic overview
    - [x] Find out how to read in High or Low Pulse on a pin on the ESP32
- [x] Create a temp project to test ultrascale functions  
- [x] Power and connect device through ESP32
- [x] Establish I2C communication with device and print data to console

#### Getting OLED Screen Setup
- [x] Power and Connect device through ESP32
- [x] Draw test program to screen
- [ ] Write Hello World! to screen 
    - [ ] Create fucntions to draw images to screen based on position
    - [ ] Create font, allowing each letter to be drawn to the screen
    - [ ] Create word/sentence constructor, to string letters and punctuation together to be drawn as a unit

#### Connecting the dots
- [ ] Create listener for Ultrasonic traffic
- [ ] Parse int data into font/sentence structures
- [ ] Display on screen  
    - [ ] Refresh screen reliably