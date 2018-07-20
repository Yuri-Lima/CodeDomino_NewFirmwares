# firmware_robot
This is the firmware for the code_domino robot, called Turrai. Turrai is a 10 x 10 x 10 cm Arduino Uno compatible robot. 
The objective of the project is to deliver a complete educational platform to teach kids between 4 and 99 years to program a robot using domino pieces (virtual or tangible). We are developing four different interaction models, based on the abstraction capabilities of the users in different ages. They are:
* A basic pictographic domino for young toddlers who don't read, based on RFID. The pieces represent the moving capabilities of the robot;
* A basic android application where toddlers can draw paths, translate in virtual domino pieces, and upload to the robot.
* An advanced domino for kids, based on RFID, encompassing not only the movement pieces but complex program commands, as conditionals, functions, variables and parameters.
* An advanced android application, who replicates the tangible advanced domino and can be uploaded to the robot via Bluetooth. The app also permits to backup codes, share, and consult other codes. 

The robot is composed of: 
* Lasercut body in acrylic or MDF 2,5mm;
* 2 5V step motor 28byj48 with ULN2003 board driver;
* Hub shield with 74HC595 Shift Register for the motors;
* Ultrassonic sensor connector in shield;
* Gyroscope MPU-6050 GY-521 connector in shield;
* 6 operation buttons;
* Buzzer;
* Servo motor controlling pen;
* RFID RC-522 sensor connector in shield;
* Bluetooth HC-05 or HC-06 connector in shield;
* Extension bus for other modules (line follower, etc.).
Instructions to build your own Turrai Robot will be found in www.codedomino.org, and in this github project.
