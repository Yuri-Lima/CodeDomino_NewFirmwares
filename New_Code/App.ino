
//Programa : Code Dominó
//Autor : Daniel Chagas / Patrick / Yuri Lima
//=====================================================================================================
/*
	Include de libs novas
	Ao final a ideia é termos modulos de libs, para posterior inclusão em uma lib_master
*/
#include <SoundCod.h> //Lib CodeDomino
#include <ButtonCod.h> //Lib CodeDomino
//Pedente criação das libs, da função bool readRfid(), bool walk(), bool shapes().
//=====================================================================================================
//Include de libs convencionais
#include <SPI.h>
#include <deprecated.h>
#include <MFRC522.h> //Version 1.3.6
#include <MFRC522Extended.h>
#include <require_cpp11.h>
#include <Ultrassonic.h>

/*
	Fontes de estudos
	O.B.S.:Criar botÃ£o reset diretamente da placa arduino, sempre que houver necessidade de parar a execuÃ§Ã£o de qualquer bug
	https://www.thingiverse.com/thing:862438/files ---> roda 3D usada
	https://wakatime.com/projects ---> Medição de tempo de programação, para futuros relatorios
	https://youtu.be/B86nqDRskVU ---> Mecanismo de redução do motor
	https://www.filipeflop.com/blog/como-gravar-dados-no-cartao-rfid/ ---> RFID
	https://youtu.be/g2Tco_v73Pc ---> AlocaÃ§Ã£o dinamica
	https://youtu.be/zzHcsJDV3_o --> Encoder incremental - animação
*/
//==================================================================================================================
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
//Etapa 1 - Motor
float GrausPassoDoMotor = 0.1757;
float m_erro_r = 1.05;
float r_360 = (360 / GrausPassoDoMotor) + m_erro_r;

//Etapa 2 - Roda 
float raioRoda =  3.3;
float C = (2 * PI * raioRoda); //C"..

//Etapa 3 - Carro
float raioEixo = 4.1; 
float C_ = (2 * PI * raioEixo); //C'

//Etapa 4 - Revoluções 
float revol_ = C_ / C;//C" / C'

//Etapa 5 - Passos
float m_erro_e = 0.050; 
float e_360 = r_360 * (revol_ + m_erro_e);//passo para rotação do proprio eixo
//=====================================================================================================
//Debug
/*Estás opções servem para diminuir o uso de memoria de armazenamento, habilitar apenas quando necessário, ou seja, para debug.
	O.B.S.:Caso venha a usar, habilite primeiro o debug_setup, para habilitrar o Serial.begin.
*/
#define debug_setup 1 
#define debug_rfid 0
#define debug_logicflow 0
#define debug_runflow 0
#define debug_shapes 0
#define debug_walk 0
#define debug_alocarMatriz 0
#define debug_desalocarMatriz 0
#define debug_disable_coil 0
//=========================================================================================
//Função Som
#define buzzer_pin 3
sound buzzer(buzzer_pin);//Aqui já é algo relacionado a lib SoundCod.h
//=========================================================================================
//Valor de cada leitura dos botões personalizados de acordo com o nome do modelo (DECANO)
#define Bot_Pin A0
#define Bot_D 709
#define Bot_E 761
#define Bot_C 822
#define Bot_A 894
#define Bot_N 977
#define Bot_O 1050
button optionPin(Bot_Pin, Bot_D, Bot_E, Bot_C, Bot_A, Bot_N, Bot_O); //Aqui já é algo relacionado a lib ButtonCod.h
//=========================================================================================
//Valor das peças para orientação
/*
	Nesse projeto inicial, o objetivo é deixar funcionando para nossa primeita categoria (Little Kids), ou seja, 
	Comandos básicos devem ser implementados. O define Angle, apenas não está em uso, porem ja temos aplicação que envolve 
	os movimento em curva, pois encontrei uma especie de razão, onde conseguimos mudar o tamanha do raio, assim conseguimos realizar
	curvas de acordo com angulos especificados, porem identifiquei, que com certeza termos algumas limitação quanto a angulos minimos.
*/	
#define Start 'S'
#define End   'E'
#define Front 'F'
#define Back  'B'
#define Left  'L'
#define Right 'R'
#define Angle 'A'
//=========================================================================================
//Função RFID
const int sck  =  13; 
const int miso =  12;
const int mosi =  11;
const int RST_PIN = 9;
const int SS_PIN  = 10;
short int amount_Parts = 0;
//=========================================================================================
//Função Loop
const int timer_B = 100;     //Timer Botão
const int timer_R = 500;  	//Timer RFID
const int timer_F = 1000;  //Timer  shapes Geometricas
const int timer_S = 1500; // Timer Ultrassonic
//=========================================================================================
//Função  walk
const int latchPin = 8;  //Pin connected to ST_CP of 74HC595
const int clockPin = 7; //Pin connected to SH_CP of 74HC595
const int dataPin = 6; //Pin connected to DS of 74HC595
//=========================================================================================
//Função  Ultrassonic
int trig = 5; //ultrasom
int echo = 4; //ultrasom
Ultrassonic sonic(trig, echo);
//=========================================================================================
/*
	Função alocarMatriz e desalocarMatriz --> Apenas para a função Shapes para aloção de fomatos geometricos predefinidos,
	como uma especie de padrões de fabrica.
*/
int **m;//Matriz bidimensional
//=========================================================================================
//Função Loop
unsigned long millisAnterior = 0;
unsigned long millisAtual = 0;
//=========================================================================================
/*
	Instanciação de Objetos RFID
	In this example we will write/read 16 bytes (page 6,7,8 and 9).
	Ultraligth mem = 16 pages. 4 bytes per page.  
	Pages 0 to 4 are for special functions. 
*/
MFRC522 mfrc522(SS_PIN, RST_PIN);//Create MFRC522 instance
MFRC522::StatusCode status;//variable to get card status 
byte buffer[18]; //data transfer buffer (16+2 bytes data+CRC)
byte size = sizeof(buffer);
uint8_t pageAddr = 0x06;
/*
	Bom por enquanto limitei a quantidade de comandos a realizar, pois para a categoria nao tem necessidade, até pq, podemos
	criar novos tipos de blocos. 
	Na etapa apos a leitura de comandos, apenas é possivel, realizar nova leitura, quando realizar um reset, em breve irei implementar 
	as demais utilidades. 
*/
//Função logicflow e runflow
char instructionBuff[20];
byte c = 0;


void setup()
{ 	
	SPI.begin();
	mfrc522.PCD_Init();

	#if debug_setup
	Serial.begin(9600);
	memcpy(buffer,"X0000000",(size-2)/2);//X para sinalizar inicio de comandos
	Serial.write(char(buffer[0]));
	Serial.println();
	//===================================================================================================
	Serial.print(F("Passos para a RODA rodar (r_360): "));  Serial.println(r_360);
	Serial.print(F("Distancia pecorrida pela RODA (C): ")); Serial.println(C);
	Serial.print(F("Distancia pecorrida pelo CARRO no proprio EIXO (C_): "));    Serial.println(C_);
	Serial.print(F("Quantas voltas a RODA tem que dar para o CARRO rodar no EIXO (revol_): "));   Serial.println(revol_);
	Serial.print(F("Quantos passos para o CARRO rodar no EIXO (e_360): "));   Serial.println(e_360);
	#endif
	//===================================================================================================
	pinMode(latchPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	pinMode(dataPin, OUTPUT);
	digitalWrite(latchPin, LOW);
	shiftOut(dataPin, clockPin, MSBFIRST, B00000000); //envia resultado binÃ¡rio para o shift register
	digitalWrite(latchPin, HIGH);
	//buzzer.soundHome();
}

void loop()
{	 
	static short int option = 0, linha = 0, coluna = 0;
	float dist = (r_360 * 3) / C, U_sonic = 0.00;// 3 cm
	static bool callback_end_runflow = false, callback_read_rfid = false, callback_end_logicflow = true, 
				flag_button = true,callback_end_walk = false, callback_end_sonic = false;	
	//===================================================================================================
	//Função ler Botão com return da opção
	//===================================================================================================
	if (millisAtual % timer_B == 0)
	{
		char LQ0 = optionPin.pressedtime(); 
		if(LQ0 != '0') optionPin.readbutton(LQ0);
		if(flag_button)  //long pressed e quick pressed
		if(option != 0)	
		{
			flag_button = false;
		}
		delay(2);		
	}	
	//===================================================================================================
	//Função de leitura RFID para a função logicflow  
	//===================================================================================================
	if (millisAtual % timer_R == 0)
	{
		if((option != 0) && (option == 1))
		{
			delay(50);
			if((callback_end_logicflow) && (!callback_end_sonic))callback_end_walk = walk(1, 1, dist , 0, 1);//procurando a primeira peça

			callback_read_rfid = readRfid();

			if(callback_read_rfid && (!callback_end_sonic))
			{
				callback_end_logicflow = logicflow(callback_read_rfid);
				if(!callback_end_logicflow)
				{
					flag_button = true;
					option = 0;
				} 
			}
		}
		callback_read_rfid = 0;//Para controle real de retorno da função readRfid. 
	}
		//===================================================================================================
	//Para executação da função shapes ou runflow
	//===================================================================================================
	millisAtual = millis();
	if (millisAtual % timer_F == 0 )
	{
		if ((option != 0) && (option != 1))
		{ 	
			delay(2000);
			shapes(option);
			if(option == 3) 
			callback_end_runflow = runflow();
			if(callback_end_runflow) buzzer.soundOk();
			flag_button = true;	
			option = 0;	
		}
	}
	/*	
	if (millisAtual % timer_S == 0)
	{
		U_sonic = sonic.getDistancia(CENTIMETRO);
		if(U_sonic <= 10) 
		{
			callback_end_sonic = true;
			buzzer.Beep();
		}
		else callback_end_sonic = false;	
	}*/
}
/*
=====================================================================================================
*/
bool logicflow(bool callback_read_rfid)
{	
	static short int  linha = 0, coluna = 0;
	static char i = 0;
	c = 0;				
	if(callback_read_rfid)	
	{
		if((char(buffer[0]) == Start) && (amount_Parts == 0))
		{
			#if debug_logicflow
			Serial.println(char(buffer[0]));
			#endif
		}					
		if((amount_Parts >= 1) && (char(buffer[0]) != End) && (char(buffer[0]) != Start))//ou seja, achou a primeira peça, agora vai incluir na matriz de passos
		{	
			instructionBuff[i] = char(buffer[0]);
			#if debug_logicflow
			Serial.print(instructionBuff[i]);
			#endif
			i++;		
		}
		else if( ( char(buffer[0]) == End ))
		{	
			#if debug_logicflow
			Serial.println();
			for(byte x=0; x<i;x++)Serial.print(instructionBuff[x]);
			Serial.println();
			#endif
			//memset(instructionBuff, 0, sizeof(instructionBuff));
			memset(buffer, 0, sizeof(buffer));
			c = i;
			i = 0;
			amount_Parts=0;
			return false;
		}
		amount_Parts++;			
	}
	return true;		
}
bool runflow()
{	
	#if debug_runflow
		Serial.print("Quantos passos: ");
		Serial.println(c);
	#endif
	byte p = 0;
	bool callback = false;
	float stepsAway, angledSteps;//stepsAway em centimetros -- angledSteps em graus
	while(p < c)
	{
		#if debug_runflow
			Serial.print("Passo: ");
			Serial.print(p);
			Serial.print(" - ");
			Serial.print("Tag: ");
		#endif	
		switch(instructionBuff[p])
		{
			case Front:
				stepsAway = 11.00;
				callback = walk(1, 1,  int((r_360 * stepsAway) / C), 0, 1);
				#if debug_runflow	
					Serial.println("F");
				#endif
				delay(5);	
			break;
			case Left:
				angledSteps = 95.00;
				callback = walk(-1, 1, int((e_360 * angledSteps) / 360.00), 0, 1);
				#if debug_runflow
					Serial.println("L");
				#endif
				delay(5);		
			break;
			case Right:
				angledSteps = 95.00;//96foi preciso realizar esse incremento, por enquanto motivo nao encontrado. 
				callback = walk(1, -1,  int((e_360 * angledSteps) / 360.00), 0, 1);
				#if debug_runflow
					Serial.println("R");
				#endif
				delay(5);		
			break;
			case Back:
				stepsAway = 11.00;
				callback = walk(-1, -1,  int((r_360 * stepsAway) / C), 0, 1);
				#if debug_runflow
					Serial.println("B");	
				#endif
				delay(5);	
			break;
			case Angle:
				angledSteps = 60;
				callback = walk(1, -1,  int((e_360 * angledSteps) / 360.00), 0, 1);
				#if debug_runflow
					Serial.println("A");
				#endif
				delay(5);		
			break;
			default:
				buzzer.error();
				#if debug_runflow
					Serial.println("Error RunFlow");
				#endif		
			break;
		}	
		if(callback)p++;
		delay(2);
	}	
	return callback;
}
bool readRfid()
{
	//short int tam=((sizeof(buffer)-2)/2);
	//Serial.print(tam);
	buzzer.Beep();
	if ( ! mfrc522.PICC_IsNewCardPresent())	return false;
	if ( ! mfrc522.PICC_ReadCardSerial()) return false;
	#if debug_rfid
	Serial.println(F("Reading data ... "));
	#endif
    //data in 4 block is readed at once.
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(pageAddr, buffer, &size);
    if (status != MFRC522::STATUS_OK)return false;
	mfrc522.PICC_HaltA();
	buzzer.fineBeep();
	return true;
}
bool  shapes(int edro)
{   
	// int p = convertAngulo(45);
	float ang, dist, cateto_O, razao;
	bool callback = false;
	switch(edro)
	{
		case 9: //Rotação no eixo ////==> era 1
			ang = e_360; //escolha o angulo	
			m = alocarMatriz(1,3);
			m[0][0] = 1; m[0][1] = -1; m[0][2] = ang; //linha 1Âª comando
			callback =  walk(m[0][0], m[0][1],  m[0][2], 0, 1);
			#if debug_shapes
			Serial.print(m[0][0]);Serial.print(" - ");Serial.print (m[0][1]);Serial.print(" - ");Serial.println(m[0][2]);
			#endif
			delay(10);
			desalocarMatriz(m,1);
		break;
		case 2: //Reta em 
			dist = 10;// coloque aqui a distancia em cm
			m = alocarMatriz(1,3);
			m[0][0] = 1; m[0][1] = 1; m[0][2] = (r_360 * dist) / C; //linha 1Âª comando
			#if debug_shapes
			Serial.print(m[0][0]);Serial.print(" - ");Serial.print (m[0][1]);Serial.print(" - ");Serial.println(m[0][2]);
			#endif
			callback =  walk(m[0][0], m[0][1],  m[0][2], 0, 0);
			delay(10);
			desalocarMatriz(m,1);
		break;
		case 45: //Rotação angular no proprio eixo ////==> era 3
			ang = 90; //escolha o angulo
			m = alocarMatriz(1,3);
			m[0][0] = 1; m[0][1] = -1; m[0][2] = (e_360 * ang) / 360; //linha 1Âª comando regra de
			#if debug_shapes
			Serial.print(m[0][0]);Serial.print(" - ");Serial.print (m[0][1]);Serial.print(" - ");Serial.println(m[0][2]);
			#endif
			callback =  walk(m[0][0], m[0][1],  m[0][2], 0, 0); 
			delay(10);
			desalocarMatriz(m,1);
		break;
		case 4://Triangulo retangulo 30
			ang = 30; //escolha o angulo
			cateto_O = 10;
			/*
				Conteudo: Trigonometria Ângulos notávei SOH CAH TOA
				Lista estudada:
					https://youtu.be/TjC3F9sj-x0
					https://www.youtube.com/watch?v=3iHUX_oOcX0
					https://www.dobitaobyte.com.br/sirene-com-arduino/
			*/
			m = alocarMatriz(6,3);//6 linhas e 3 colunas
			m[0][0] = 1; m[0][1] = 1; m[0][2] = (r_360 * cateto_O) / C; //Cateto oposto
			m[1][0] = 1; m[1][1] = -1; m[1][2] = (e_360 * (90 - ang)) / 360; //complemento de angulo
			m[2][0] = -1; m[2][1] = -1; m[2][2] = (r_360 * (cateto_O / sin(ang * (PI/180)))) / C;//hipotenusa  SOH
			m[3][0] = 1; m[3][1] = -1; m[3][2] = (e_360 * ang) / 360; //angulo que desejamos
			m[4][0] = 1; m[4][1] = 1; m[4][2] = (r_360 * (cateto_O / tan(ang * (PI/180)))) / C; // cateto adjacente TOA
			m[5][0] = -1; m[5][1] = 1; m[5][2] = (e_360 * 90) / 360; //Volta na posição que iniciou
			for(int i = 0;i < 6;i++)
			{	
				#if debug_shapes
				Serial.print(m[i][0]);Serial.print(" - ");Serial.print (m[i][1]);Serial.print(" - ");Serial.println(m[i][2]);
				#endif
				callback =  walk(m[i][0], m[i][1],  m[i][2], 0, 0);
				delay(10);
			}
				desalocarMatriz(m,6);
		break;  	
		case 5: //Quadrado 10x10 em 90°
			ang = 90; //escolha o angulo
			m = alocarMatriz(2,3);
			m[0][0] = 1; m[0][1] = 1; m[0][2] = (r_360 * 10) / C; //linha 1Âª comando
			m[1][0] = -1; m[1][1] = 1; m[1][2] = (e_360 * ang) / 360; //linha 2Âª comando
			for(int i = 0;i < 4;i++)
			{  
				for(int j = 0;j < 2;j++)
				{ 
					#if debug_shapes
					Serial.print(m[j][0]);Serial.print(" - ");Serial.print (m[j][1]);Serial.print(" - ");Serial.println(m[j][2]);
					#endif
					callback =  walk(m[j][0], m[j][1],  m[j][2], 0, 0);
					delay(10);
				}
			}
			desalocarMatriz(m,2);
		break;
		case 6: //Circulo
			//razao = 3;
			ang = 440; // 870 para razao de 6
			m = alocarMatriz(6,3);
			m[0][0] = 1; m[0][1] = 1; m[0][2] = (e_360 * ang) / 360; //linha 1Âª comando
			#if debug_shapes
			Serial.print(m[0][0]);Serial.print(" - ");Serial.print (m[0][1]);Serial.print(" - ");Serial.println(m[0][2]);
			#endif
			callback =  walk(m[0][0], m[0][1], m[0][2], 6, 1);
			desalocarMatriz(m,1);
		break;
		/*
		case 3://Posicionamentos angulare (Circulo)
			ang = 30; //escolha o angulo
			cateto_O = 45;
			/
				Conteudo: Trigonometria Ângulos notávei SOH CAH TOA
				Lista estudada:
					https://youtu.be/TjC3F9sj-x0
					https://www.youtube.com/watch?v=3iHUX_oOcX0
					https://www.dobitaobyte.com.br/sirene-com-arduino/
			/
			m = alocarMatriz(1,3);//6 linhas e 3 colunas
			m[2][0] = 1; m[2][1] = 1; m[2][2] = (r_360 * (cateto_O / sin(ang * (PI/180)))) / C;//hipotenusa  SOH
			for(int i = 0;i < edro*2;i++)
			{	
				#if debug_shapes
				Serial.print(m[i][0]);Serial.print(" - ");Serial.print (m[i][1]);Serial.print(" - ");Serial.println(m[i][2]);
				#endif
				 walk(m[i][0], m[i][1],  m[i][2], 4, 0);
				delay(50);
			}
				desalocarMatriz(m,6);
		break;
		*/
		default :
		cateto_O = 0;
		#if debug_shapes
		Serial.println("Formato invalido."); 
		#endif 
	}
	return callback;    
}
bool  walk(int _Right, int _Left, int stepstowalk, int _freqRot, int _CW_CCW)
{ 	 /*
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
	#if debug_walk
	Serial.print(_Left);Serial.print(" - ");Serial.println(_Right);
	#endif
	//==============================================================
	//variaveis de verificação de distancia
	/*unsigned long millisAtual;
	int timer_S = 100;
	bool callback_end_sonic = false, flag_button = true;
	float U_sonic =0.00, detected_min = 4.00,detected_max = 15.00;*/
	//==============================================================
	bool flag_Left = true, flag_Right=true, flag_button = true;//Aciona um Mecanismo para gerar curvas, com uma razão proporcional.
	int stepLeft = 3, stepRight = 3;//Faz o fluxo step binario 
	byte binLeft[4]= {B10010000, B11000000, B01100000, B00110000};//FullStep
	byte binRight[4]= {B00001100, B00000110, B00000011, B00001001};//FullStep
	if(_Right == 0) flag_Right = false;//Stop
	if(_Left == 0) flag_Left = false;//Stop
	while (stepstowalk > 0)
	{
		//==============================================================
		//Bloco de verificação de distancia, ainda em testes
		/*
		millisAtual = millis();
		if (millisAtual % timer_S == 0)
		{
			U_sonic = sonic.getDistancia(CENTIMETRO);
			if((U_sonic <= detected_max) && (U_sonic >= detected_min))//range
			{
				callback_end_sonic = true;
				buzzer.Beep();
			}
			else callback_end_sonic = false;	
		}*/
		//==============================================================
		//Bloco de verificação de um botão qualquer
		//if(optionPin.readbutton()) 
		//{
			//flag_button = false;
			//stepstowalk = 0;
		//}	

		//if(flag_button)
		//{
			//================================================================================
			//Aqui temos como realizar uma curva ou circunfencia
			if(_freqRot > 0)
			{
				if(_CW_CCW == 1) 
				{	
					#if debug_walk
					Serial.println(stepstowalk % _freqRot);
					#endif
					(stepstowalk % _freqRot) != 1 ? flag_Left = false : flag_Left = true;
				}
				else if(_CW_CCW == -1)
				{
					(stepstowalk % _freqRot) != 1 ? flag_Right = false : flag_Right = true;
				}
			}
			//================================================================================
			//Aqui temos como realizar retas e curva em 90°
			if(flag_Left)
			{
				if (_Left == 1)
				{
					if(stepLeft == 0) stepLeft = 4;
					stepLeft--;				
				}
				else if (_Left == -1)
				{
					if(stepLeft == 3) stepLeft = -1;
					stepLeft++;
				}
				#if debug_walk
				Serial.print("Ligou Esquerda: ");Serial.print(stepLeft);Serial.print(" - ");Serial.println("0");
				#endif
			}
			if(flag_Right)
			{
				if (_Right == 1 )
				{
					if(stepRight == 0) stepRight = 4; 
					stepRight--;
				}
				else if (_Right == -1 )
				{	
					if(stepRight == 3) stepRight = -1;
					stepRight++;				
				}
				#if debug_walk
				Serial.print("Ligou Direita: ");Serial.print("0");Serial.print(" - ");Serial.println(stepRight);
				#endif
			}
			digitalWrite(latchPin, LOW);
			shiftOut(dataPin, clockPin, MSBFIRST, binRight[stepRight] | binLeft[stepLeft]); //envia resultado binÃ¡rio para o shift register
			digitalWrite(latchPin, HIGH);
			stepstowalk--;
			flag_Left = true;
			flag_Right= true;
			delay(2);
		//}	
	}
	//Desabilitar bobina para economia de energia
	disable_coil();
	return stepstowalk == 0 ? 1 : 0;
}
void disable_coil()
{
	digitalWrite(latchPin, LOW);
	shiftOut(dataPin, clockPin, MSBFIRST, B00000000); //envia resultado binÃ¡rio para o shift register
	digitalWrite(latchPin, HIGH);
	#if debug_disable_coil
		Serial.println("Motores desligados.");
	#endif
}
/*
=====================================================================================================
*/

int** alocarMatriz(int Linhas,int Colunas)//Recebe a quantidade de Linhas e Colunas como ParÃ¢metro
{  
	#if debug_alocarMatriz
	Serial.println("Done allocation");
	#endif
	int i,j; //VariÃ¡veis Auxiliares
	int **m = (int**)malloc(Linhas * sizeof(int*)); //Aloca um Vetor de Ponteiros
	if(m==NULL)
	{
		#if debug_alocarMatriz
		Serial.println("allocation error on line");
		#endif
		buzzer.error();
		return 0;
	}
	for (i = 0; i < Linhas; i++)//Percorre as linhas do Vetor de Ponteiros
	{  
		m[i] = (int*) malloc(Colunas * sizeof(int)); //Aloca um Vetor de Inteiros para cada posiÃ§Ã£o do Vetor de Ponteiros.
		if(m[i]==NULL)
		{
			#if debug_alocarMatriz
			Serial.println("allocation error in column");
			#endif
			buzzer.error();
			return 0;
		}
		for (j = 0; j < Colunas; j++)//Percorre o Vetor de Inteiros atual.
		{ 
			m[i][j] = 0; //Inicializa com 0.
		}
	}
	return m; //Retorna o Ponteiro para a Matriz Alocada	
}
void desalocarMatriz(int **m, int Linhas)
{
	 short int i;
	if(m[0][0]!=NULL)
	{
		for (i = 0; i < Linhas; i++)
		{
			free(m[i]);
		}
		free(m);
		#if debug_desalocarMatriz
		Serial.println("Undone allocation");//alocação desfeita
		#endif
	}
	else
	{ 	
		i = 0;
		#if debug_desalocarMatriz
		Serial.println("Non-undone allocation");//alocação não desfeita
		#endif
	}
}

int convertAngulo(float _angulo)
{  
	return int((e_360 * _angulo) / 360);
}

