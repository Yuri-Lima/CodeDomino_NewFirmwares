//==================================================================================================================
//Fonte de estudos usadas até o momento
/*
	O.B.S.:Criar botÃ£o reset diretamente da placa arduino, sempre que houver necessidade de parar a execuÃ§Ã£o de qualquer bug
	https://www.thingiverse.com/thing:862438/files ---> roda 3D usada
	https://wakatime.com/projects ---> Medição de tempo de programação, para futuros relatorios
	https://youtu.be/B86nqDRskVU ---> Mecanismo de redução do motor
	https://www.filipeflop.com/blog/como-gravar-dados-no-cartao-rfid/ ---> RFID
	https://youtu.be/g2Tco_v73Pc ---> AlocaÃ§Ã£o dinamica
	https://youtu.be/zzHcsJDV3_o --> Encoder incremental - animação
	https://www.automalabs.com.br/como-medir-a-tensao-de-alimentacao-do-arduino/ --> Nivel de bateria
	Conteudo: Trigonometria Ângulos notávei SOH CAH TOA
	Lista estudada:
	https://youtu.be/TjC3F9sj-x0
	https://www.youtube.com/watch?v=3iHUX_oOcX0
	https://www.dobitaobyte.com.br/sirene-com-arduino/
	https://www.youtube.com/watch?v=xXUKfTNHkKE&list=PL4vok-JSscqD_f4F9JEucI_fXp49GaD4L&t=3s&index=8 --> Over in the Air com HC 05
	https://sites.google.com/site/wayneholder/inexpensively-program-your-arduino-via-bluetooth --> Over in the Air com HC 05
	https://github.com/tockn/MPU6050_tockn --> MPU6050
	
*/
//==================================================================================================================
//Duração de bateria
/*
	Consumo do CodeDomino
		De acordo com os testes realizados até o dia 25/10/2018 as 23:07, o consumo apresentado é de ~95mA parado,
		ou seja, esperando receber alguma comando.
		Após receber comandos, seu consumo varia a depender a executação das bobinas, no geral, podemos dizer que
		o consumo minimo em funcionamento é de 495mA e maximo de 700mA que ocorre ao final de percusso, uma especie
		de pico, mais posterior é realizar o desligamento das bobinas, voltando ao seu consumo base de ~95mA.
		Portando uma bateria Lipo 2S de 1300mAh tem uma autonomia de acordo com teste realizados de aproximadamente
		4 horas de atividades. 
		O.B.S.: Esse periodo de consumo ainda terá que sermais bem avaliado, pois essa informações é apenas uma ideia,
		levando em consideração as experiencias que tivier no dercorrer atual dos testes.
		Tampo de consumo parado: Lipo 1000mAh--> 1 / 0,095 = 10.526 * 60min = 631 minutos ou 10 horas parado.
		Tampo de consumo em funcionamento: Lipo 1000mAh--> 1 / 0,5 = 2 * 60min = 120 minutos ou 2 horas.
*/
//==================================================================================================================
//Funcionamento de calculos para movimentação do robo de acordo com os Motores
/*
	--> Possiveis questionamentos <--
	(Quantos passos para a roda fazer 360º?) --> Etapa 2
	((Quantos passos para o CARRO fazer 360º?)) --> Etapa 4

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
*/
//==================================================================================================================
/*
	Não deixa salva a logica de blocos em outro botão.
*/
//==================================================================================================================
//Valor das peças para orientação
/*
	Nesse projeto inicial, o objetivo é deixar funcionando para nossa primeita categoria (Little Kids), ou seja, 
	Comandos básicos devem ser implementados. O define Angle, apenas não está em uso, porem ja temos aplicação que envolve 
	os movimento em curva, pois encontrei uma especie de razão, onde conseguimos mudar o tamanha do raio, assim conseguimos realizar
	curvas de acordo com angulos especificados, porem identifiquei, que com certeza termos algumas limitação quanto a angulos minimos.
*/
//==================================================================================================================
//Instanciação de Objetos RFID
/*
	In this example we will write/read 16 bytes (page 6,7,8 and 9).
	Ultraligth mem = 16 pages. 4 bytes per page.  
	Pages 0 to 4 are for special functions. 
*/
//==================================================================================================================
//Função Walk
/*
		0 --> Parado, 1 --> frente , -1 --> tras
		Essa função faz todo controle da execução de passos, seja qualquer for a direção, até mesmo curvas. 
		Como as orientações basicas já nao são um misterio, vou tentar explicar um pouco sobre como tentei recolver
		o problema da curva, da seguinte forma: 
		Notei que é preciso uma das rodas dar menos passos que a outra, logico de acordo com a direção, então encontrei uma razão 
		dada pelo parametro _freqRot, ele é o responsavel, por inibir determidados passos com uma razão entre os passos a caminhar e
		o resto comparavel a 1, nesse ritimo, ao variar essa divisão, podemos obter acionamentos que variam de 1 passo a cada 2, como
		1 passo a cada 5 e assim por diante, sendo assim, quanto maior _freqRot, maior sera o tempo de acionamento para 1 passo e o raio 
		da circunfencia/ curva dimunui, ou seja, é inversamente proporcional. Todos os calculos para realizalçao de movimento circulares, 
		ou seja, curvas, tem que ser usado como paramentro a variavel, e_360, pois é nela que temos os passos, levando-se em consideração
		o eixo do carro e todas para seguir em linha reta, seja para tras ou para frente, tem como parametro/variavel r_360.
		Demais calculos é apenas simples regras 3. Foi usado, na função Shapes case 4, Seno e Tagente, pois para realizar é preciso para
		criar um Triangulo Retangulo apenas especificando o tipo do triangulo notavel e a medida do cateto opostos, para obter as demais
		medidas. 
		Enfim, vi que temos muito coisa a fazer, tanto para uma versão open, como para uma versão comercial mais bem elaborada no sentido (precisão). 
*/
//==================================================================================================================
//Over in the Air com HC 05
/*
	Toda a implementação seguiu as orientações dos links que coloquei a disposição, tem detalhes que é preciso estudar mais. 
	Exemplo: tempo de permanencia no pareamento. 
		Parear automaticamente, com uma lista predefinida de dispositivos. 
*/

