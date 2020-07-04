<!-- Headings --> <!-- Strong --> <!-- Italics --> <!-- Blockquote --> <!-- Links --> <!-- UL --> <!-- OL --> <!-- Images --> <!-- Code Blocks --> <!-- Tables --> 
<!-- Task Lists -->

> # Decabot New Version from Firmware

### For you knowledge, this project is a remix from the [original project](https://github.com/Yuri-Lima/firmware_robot). I had just built a new firmware and included some hardware to cooperate with it.


> ## The biography studied:

#### 3D Wheel
* https://www.thingiverse.com/thing:862438/files

#### Productivity metrics and automatic time tracking for programmers
* https://wakatime.com/projects 

#### Engine reduction mechanism
* https://youtu.be/B86nqDRskVU

#### Batery Level
* https://www.automalabs.com.br/como-medir-a-tensao-de-alimentacao-do-arduino/

#### RFID record and write
* https://www.filipeflop.com/blog/como-gravar-dados-no-cartao-rfid/

#### Dynamic allocation memory
* https://youtu.be/g2Tco_v73Pc

#### Incremental encoder - animation
* https://youtu.be/zzHcsJDV3_o

#### Right Angle Triangle Theorem SOH CAH TOA  
* https://youtu.be/TjC3F9sj-x0 
* https://www.youtube.com/watch?v=3iHUX_oOcX0 
* https://www.dobitaobyte.com.br/sirene-com-arduino/ 

#### Over in the Air com HC 05 
* https://youtu.be/xXUKfTNHkKE?list=PL4vok-JSscqD_f4F9JEucI_fXp49GaD4L
* https://sites.google.com/site/wayneholder/inexpensively-program-your-arduino-via-bluetooth

#### MPU6050
* https://github.com/tockn/MPU6050_tockn

> ## What was the changes

1. The main code;
1. External reset button when something went wrong, like a bug;
1. [MPU 6050](https://github.com/Yuri-Lima/CodeDomino_NewFirmwares/blob/master/Code_MPU_PID/App_MPU6050_PID.ino)
1. [Posicion Corret MPU6050](https://youtu.be/N3V8oYgs7Ro)
1. Examples code to simulate individual part of the code:
  - [Gravador_Tag, Passo_Caminhar, Read_Write, StringToInt, eeprom_get, eeprom_put, memset](https://github.com/Yuri-Lima/CodeDomino_NewFirmwares/tree/master/Simula%C3%A7%C3%B5es)
1. Libraries Created
  * [SoundCod.h](https://github.com/Yuri-Lima/CodeDomino_NewFirmwares/tree/master/libraries/SoundCod)
  * [ButtonCod.h](https://github.com/Yuri-Lima/CodeDomino_NewFirmwares/tree/master/libraries/Button)
  * [RecordFlash.h](https://github.com/Yuri-Lima/CodeDomino_NewFirmwares/tree/master/libraries/RecordFlash)
  * [SoundCod2.h](https://github.com/Yuri-Lima/CodeDomino_NewFirmwares/tree/master/libraries/SoundCod2)
      * License from: https://creativecommons.org/licenses/by-sa/4.0/  
      * Part of the code from: https://github.com/OttoDIY/DIY
  * [BatLevelCod.h](https://github.com/Yuri-Lima/CodeDomino_NewFirmwares/tree/master/libraries/BatLevelCod)
  * [BlueDebug.h](https://bitbucket.org/apiice/firmware_robot/commits/d7aabaa0ca106b523513a8d5e12650b8c50fb360)

> ## Analysis of the power consumption
 
CodeDomino Acording of the test made at **25/10/2018 at 23:07**. The power consumption was around ~95mA when it is stopped **waiting to receve the first command**, however, when the first command comes up, The power **minimun consumption** when it was working at around 495mA e **max was 700mA** at the end of the way. There is a pico of energy, when the bobine of the motors turns off. The **Lipo Batery 2S de 1300mAh** has a range aprox in 4 hours of work. 
O.B.S.: It's still necessary wacht his behavior, because all of the informatios is just piece of ideia, however, i considerated all experience that i had  at the long way doing the tests. 
- **Calculate Base when it is stopped**: Lipo 1000mAh--> 1 / 0,095 = 10.526 * 60min = 631 minutes ou 10 horas stopped. 
- **Calculate Base when it is works**: Lipo 1000mAh--> 1 / 0,5 = 2 * 60min = 120 minutes ou 2 hours.

> ## Calculates to show how the robot moves

* --> May you can ask <-- 
- How many steps are to turn the wheel self around into 360º? --> Etapa 2 
- How many steps are to turn the robot self around into 360º??  --> Etapa 4
```
Etapa 1 - Motor
1º Modelo do motor de passo, para saber quantos graus por passo. 
2º Vamos dividir 360° por graus para obter quantos passos são necessários para 360°.
O.B.S.: É importante adicionar uma variavel para incrementar(+) uma Margem de Erro, pois temos possiveis oscilações.
_____________________________________________________________________________________________
| http://robocraft.ru/files/datasheet/28BYJ-48.pdf 
| Exemplos: (FullStep = 11,25° / 64 => 0.1757°) Motor de Passo 28BYJ-48                        |
| Exemplos: 1Step = 5.625 --> FullStep = 2 * 5.625° => 11,25° ---> 360 / 0.1757° ---> p' = 2000 Passos |
---------------------------------------------------------------------------------------------
Etapa 2 - Roda                                  
1º Precisamos das informações do diametro da RODA para obter o RAIO.
2º Na formula C"=2*PI*r vamos obter a distancia percorrida em 360°.

Etapa 3 - Carro
1º Precisamos das informações do diametro entre as duas rodas, para obter o RAIO.
2º Na formula C'=2*PI*r vamos obter a distancia percorrida em 360°, ou seja, no seu proprio eixo.

Etapa 4 - Revoluções                   
1º voltas = C" / C' Quantas voltas a roda do carro terá que fazer para que o carro realizar 360° em seu proprio eixo.

Etapa 5 - Passos 
1º P" = p' * voltas Em P" vamos obter a quantidade de passos para 360°

O.B.S.: É importante adicionar uma variavel para incrementar(+) uma Margem de Erro, pois temos possiveis oscilações. Recomendado >= 0.01

Detalhes importantes, tudo que for relacionado a movimentos de curva usamos e_360 e em linha reta r_360.

```

> ## Hardware

1. The main board from the original project;
1. Bluetooth HC 05 plus General Components;
1. MPU6050;

> ## Software
1. VsCode;
1. Arduino IDE;

> ## Protoypes Images and Videos
* [Posicion Corret MPU6050](https://youtu.be/N3V8oYgs7Ro)
* [Simples Tests](https://youtu.be/KEpbW0obaI0)
* [First tests using the MPU6050 into the project CodeDomino](https://youtu.be/_bp6ZzDUpjY)
* [Great results with MPU6050](https://youtu.be/3xucGsFeTs0)

![](Images)
<img src="Images/Image.png" width="300" height="300">
<img src="Images/Robo-decabot-fonte-do-autor.jpg" width="500" height="500">

> ## Contact
**Atention** If you go email me, please put as subject the name of the project, in this case: **(CodeDominor Remixer) Requests**

|  Name |  Email | Mobile  |
|-------|--------|---------|
|  Yuri Lima | y.m.lima19@gmail.com  | +353 83 419.1605  |   
