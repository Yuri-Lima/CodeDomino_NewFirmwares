# code_domino's Turrai Robot
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

## Getting Started

These instructions will get you a copy of the firmware up and running on your Turrai Robot.

### Prerequisites

You need the Arduino IDE to upload this firmware to your Arduino based Turrai robot. Also you will need the following libraries:
* SPI version 1.1.1
* MFRC522 version 1.3.6
* Ultrasonic version 2.1.0
* EEPROM version 2.0.0
* Servo version 1.1.1

Off course you need a Turrai complete robot. See www.codedomino.org and www.genioazul.com.br to build or buy your robot.

### Installing

A step by step series of examples that tell you how to get a development env running

Say what the step will be

```
Give the example
```

And repeat

```
until finished
```

End with an example of getting some data out of the system or using it for a little demo

## Running the tests

Explain how to run the automated tests for this system

### Break down into end to end tests

Explain what these tests test and why

```
Give an example
```

### And coding style tests

Explain what these tests test and why

```
Give an example
```

## Deployment

Add additional notes about how to deploy this on a live system

## Built With

* [Arduino IDE](http://www.arduino.cc) - The web framework used

## Contributing

Please read [CONTRIBUTING.md](https://gist.github.com/PurpleBooth/b24679402957c63ec426) for details on our code of conduct, and the process for submitting pull requests to us.

## Versioning

We use [SemVer](http://semver.org/) for versioning. 

## Authors

* **Daniel Chagas** - *Initial work* - [PurpleBooth](https://github.com/triwaca)
* ** Patrick Martins** - RFID reading
* ** Lucas de Lima** - Hardware and Hub Shield

See also the list of [contributors](https://github.com/CodeDomino/contributors) who participated in this project.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* This project received funds from Funcap - Fundação Cearense de Apoio ao Desenvolvimento Científico e Tecnológico - Edital Inovafir II 2017.
* This projetc received support from Fundação Edson Queiroz  - Diretoria de Pesquisa, Desenvolvimento e Inovação - Universidade de Fortaleza.
