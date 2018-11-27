
//Programa : Code Dominó
//Autor : Daniel Chagas / Patrick / Yuri Lima
//=====================================================================================================
#include <SoundCod.h> //Lib CodeDomino
#include <ButtonCod.h> //Lib CodeDomino
#include <RecordFlash.h>
#include <SoundCod2.h> //--> Lincena de uso https://creativecommons.org/licenses/by-sa/4.0/  --> https://github.com/OttoDIY/DIY
#include <BatLevelCod.h>
#include <BlueDebug.h>
//=====================================================================================================
//Include de libs convencionais
#include <SPI.h>
#include <deprecated.h>
#include <MFRC522.h> //Version 1.3.6
#include <MFRC522Extended.h>
#include <require_cpp11.h>
#include <Ultrassonic.h>

//Etapa 1 - Motor
float GrausPassoDoMotor = 0.1757;
float m_erro_r = 1.05;
float r_360 = (360 / GrausPassoDoMotor) + m_erro_r;

//Etapa 2 - Roda 
float raioRoda =  3.5;//3.3;
float C = (2 * PI * raioRoda); //C"..

//Etapa 3 - Carro
float raioEixo = 6.25;//4.1; 
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
#define debug_loop 0
#define debug_rfid 0
#define debug_logicflow 0
#define debug_runflow 0
#define debug_shapes 0
#define debug_walk 0
#define debug_alocarMatriz 0
#define debug_desalocarMatriz 0
#define debug_disable_coil 0
#define debug_loop_batery 0
//=========================================================================================
//Função Som
const int buzzer_pin = 3;
sound buzzer(&buzzer_pin);//Aqui já é algo relacionado a lib SoundCod.h
SoudCod2 buzzer2(0);
//=========================================================================================
//Valor de cada leitura dos botões personalizados de acordo com o nome do modelo (DECANO)
const int Bot_Pin = 0;
const int Bot_D = 670;
const int Bot_E = 727;
const int Bot_C = 784;
const int Bot_A = 850;
const int Bot_N = 928;
const int Bot_O = 1020;
button optionPin(&Bot_Pin, &Bot_D, &Bot_E, &Bot_C, &Bot_A, &Bot_N, &Bot_O); //Aqui já é algo relacionado a lib ButtonCod.h
//=========================================================================================
//Valor das peças para orientação
/*
	Nesse projeto inicial, o objetivo é deixar funcionando para nossa primeita categoria (Little Kids), ou seja, 
	Comandos básicos devem ser implementados.
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
int amount_Parts = 0;
//=========================================================================================
//Função Loop
const int timer_B = 10;       //Timer Botão
const int timer_R = 500;  	 //Timer RFID
const int timer_F = 1000;   //Timer  shapes Geometricas
const int timer_S = 1500;  // Timer Ultrassonic
const int timer_R_F = 800;//Recording Flash
const int timer_Batery = 10000;//Recording Flash
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
record _record(0);
//Batery Level
BatLevelCod BatLevelCod(0);
//bluetooth_debug
BlueDebug BlueDebug(0);
char bufferDebug[20];

void setup()
{ 	
	SPI.begin();
	mfrc522.PCD_Init();
	//buzzer2.sing(S_superHappy);
	#if debug_setup
	Serial.begin(115200);
	memcpy(buffer,"X0000000",(size-2)/2);//X para sinalizar inicio de comandos
	Serial.write(char(buffer[0]));
	Serial.println();
	//===================================================================================================
	Serial.print(F("Passos para a RODA rodar (r_360): "));  Serial.println(r_360);
	Serial.print(F("Distancia pecorrida pela RODA (C): ")); Serial.println(C);
	Serial.print(F("Distancia pecorrida pelo CARRO no proprio EIXO (C_): "));    Serial.println(C_);
	Serial.print(F("Quantas voltas a RODA tem que dar para o CARRO rodar no EIXO (revol_): "));   Serial.println(revol_);
	Serial.print(F("Quantos passos para o CARRO rodar no EIXO (e_360): "));   Serial.println(e_360);
	Serial.print("Tensao: ");
	Serial.print(BatLevelCod.readVcc(4));//Passe como parametro quantas amostras de leitura deseja.
	Serial.println("mV");
	#endif
	if(BatLevelCod.readVcc(10) < 4600)
	{
		buzzer2.sing(S_disconnection);
		return;
	}	
	//===================================================================================================
	pinMode(latchPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	pinMode(dataPin, OUTPUT);
	digitalWrite(latchPin, LOW);
	shiftOut(dataPin, clockPin, MSBFIRST, B00000000); //envia resultado binÃ¡rio para o shift register
	digitalWrite(latchPin, HIGH);
	//BlueDebug.print_string_int("O numero", trig);
	buzzer2.sing(S_connection);
	//buzzer.soundHome();
}

void loop()
{
	short int sizeDebug;
	static short int option = 0, callback_end_walk = 0, option_runflow = 0;
	static float dist = (r_360 * 3) / C, U_sonic = 0.00;// 3 cm
	static bool callback_end_runflow = false, callback_read_rfid = false, callback_end_logicflow = true, 
	flag_button = true, callback_end_sonic = false, callback_begin_runflow = false;
	millisAtual = millis();
	//===================================================================================================
	if (millisAtual % timer_Batery == 0)
	{	
		#if debug_loop_batery
			BlueDebug.print_string("Batery OK");
		#endif
		if(BatLevelCod.readVcc(10) < 4600)
		{
			buzzer2.sing(S_disconnection);
			#if debug_loop_batery
				BlueDebug.print_string("Batery Low");
			#endif
			delay(4000);
			buzzer2.sing(S_disconnection);
		}	
	}				
	//===================================================================================================
	//Função ler Botão com return da opção
	//===================================================================================================
	if (millisAtual % timer_B == 0)
	{
			char LQ0 = optionPin.pressedtime(); 
			#if debug_loop
				//Serial.print("TQ - Quick / L - Long: ");Serial.print(LQ0);Serial.print(" - ");
			#endif
			if(LQ0 == 'Q' || LQ0 == 'L') option = optionPin.readbutton(LQ0);
			#if debug_loop
				BlueDebug.print_string("Option: ");BlueDebug.print_int(option);
			#endif
			callback_begin_runflow = true;
			delay(2);		
	}
	
	//===================================================================================================
	//Função de leitura RFID para a função logicflow //Monta os blocos logicos 
	//===================================================================================================
	if (millisAtual % timer_R == 0)
	{
		if(option == 1)
		{
			delay(5);
			if(callback_end_logicflow) callback_end_walk = walk(1, 1, dist , 0, 1,7);//procurando a primeira peça 
			
			if(callback_end_walk == 2) option = 0;

			callback_read_rfid = readRfid();

			if(callback_read_rfid)
			{
				callback_end_logicflow = logicflow(callback_read_rfid);
				if(!callback_end_logicflow)
				{
					option = 0;
					callback_end_logicflow = true;
					callback_begin_runflow = true;
				} 
			}
		}
	}
	
	//===================================================================================================
	//Para executação da função shapes ou runflow //Executa os blocos logicos
	//===================================================================================================
	
	if (millisAtual % timer_F == 0 )
	{
		if ((option_runflow  > 1) && (option_runflow < 7) && callback_begin_runflow)
		{ 	
			delay(2000);
			//callback_end_runflow = shapes(option_runflow);
			callback_end_runflow = runflow();
			if(callback_end_runflow)
			{
				buzzer2.sing(S_happy);
				callback_begin_runflow = false;
				option_runflow = 0;	
				option = 0;	
			}
		}
	}
	//===================================================================================================
	//Função de Gravação na Flash // Grava os blocos logicos
	//===================================================================================================
	if (option != 0 && option != 1)
	{

			#if debug_loop
				BlueDebug.print_string("Recording... ");
			#endif
			_record.Record(option, instructionBuff);
			BlueDebug.print_string(instructionBuff);
			option_runflow = option;
			option = 0;
			delay(2);		
	}
	//if (millisAtual % 50 == 0 )
}
/*
=====================================================================================================
*/
bool logicflow(bool callback_read_rfid)
{	
	static short int  linha = 0, coluna = 0;
	static char i = 0;			
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
			memset(buffer, 0, sizeof(buffer));
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
	byte p = 0;
	bool callback = false;
	float stepsAway, angledSteps;//stepsAway em centimetros -- angledSteps em graus
	while(instructionBuff[p] != 0)
	{
		#if debug_runflow
			Serial.print("Comando: ");
			Serial.println(p);
		#endif	
		switch(instructionBuff[p])
		{
			case Front:
				stepsAway = 11.00;
				callback = walk(1, 1,  int((r_360 * stepsAway) / C), 0, 1,3);
				#if debug_runflow	
					Serial.println("F");
				#endif
				delay(5);	
			break;
			case Left:
				angledSteps = 90.00;
				callback = walk(-1, 1, int((e_360 * angledSteps) / 360.00), 0, 1,3);
				#if debug_runflow
					Serial.println("L");
				#endif
				delay(5);		
			break;
			case Right:
				angledSteps = 90.00;//96foi preciso realizar esse incremento, por enquanto motivo nao encontrado. 
				callback = walk(1, -1,  int((e_360 * angledSteps) / 360.00), 0, 1,3);
				#if debug_runflow
					Serial.println("R");
				#endif
				delay(5);		
			break;
			case Back:
				stepsAway = 11.00;
				callback = walk(-1, -1,  int((r_360 * stepsAway) / C), 0, 1,3);
				#if debug_runflow
					Serial.println("B");	
				#endif
				delay(5);	
			break;
			case Angle:
				angledSteps = 60;
				callback = walk(1, -1,  int((e_360 * angledSteps) / 360.00), 0, 1,3);
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
	#if debug_runflow
			Serial.print("Quantos Blocos de Comando: ");
			Serial.println(p);
		#endif	
	return callback;
}
bool readRfid()
{
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
			callback =  walk(m[0][0], m[0][1],  m[0][2], 0, 1,4);
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
			callback =  walk(m[0][0], m[0][1],  m[0][2], 0, 0,4);
			delay(10);
			desalocarMatriz(m,1);
		break;
		case 3: //Rotação angular no proprio eixo ////==> era 3
			ang = 90; //escolha o angulo
			m = alocarMatriz(1,3);
			m[0][0] = 1; m[0][1] = -1; m[0][2] = (e_360 * ang) / 360; //linha 1Âª comando regra de
			#if debug_shapes
			Serial.print(m[0][0]);Serial.print(" - ");Serial.print (m[0][1]);Serial.print(" - ");Serial.println(m[0][2]);
			#endif
			callback =  walk(m[0][0], m[0][1],  m[0][2], 0, 0,4); 
			delay(10);
			desalocarMatriz(m,1);
		break;
		case 4://Triangulo retangulo 30
			ang = 30; //escolha o angulo
			cateto_O = 10;
			m = alocarMatriz(6,3);//6 linhas e 3 colunas
			m[0][0] = 1; m[0][1] = 1; m[0][2] = (r_360 * cateto_O) / C; //Cateto oposto
			m[1][0] = 1; m[1][1] = -1; m[1][2] = (e_360 * (90 - ang)) / 360; //complemento de angulo
			m[2][0] = -1; m[2][1] = -1; m[2][2] = (r_360 * (cateto_O / sin(ang * (PI/180)))) / C;//hipotenusa  SOH
			m[3][0] = 1; m[3][1] = -1; m[3][2] = (e_360 * ang) / 360; //angulo que desejamos
			m[4][0] = 1; m[4][1] = 1; m[4][2] = (r_360 * (cateto_O / tan(ang * (PI/180)))) / C; // cateto adjacente TOA
			m[5][0] = -1; m[5][1] = 1; m[5][2] = (e_360 * 90) / 360; //Volta na posição que iniciou
			for(byte i = 0;i < 6;i++)
			{	
				#if debug_shapes
				Serial.print(m[i][0]);Serial.print(" - ");Serial.print (m[i][1]);Serial.print(" - ");Serial.println(m[i][2]);
				#endif
				callback =  walk(m[i][0], m[i][1],  m[i][2], 0, 0,4);
				delay(10);
			}
				desalocarMatriz(m,6);
		break;  	
		case 5: //Quadrado 10x10 em 90°
			ang = 90; //escolha o angulo
			m = alocarMatriz(2,3);
			m[0][0] = 1; m[0][1] = 1; m[0][2] = (r_360 * 10) / C; //linha 1Âª comando
			m[1][0] = -1; m[1][1] = 1; m[1][2] = (e_360 * ang) / 360; //linha 2Âª comando
			for(byte i = 0;i < 4;i++)
			{  
				for(byte j = 0;j < 2;j++)
				{ 
					#if debug_shapes
					Serial.print(m[j][0]);Serial.print(" - ");Serial.print (m[j][1]);Serial.print(" - ");Serial.println(m[j][2]);
					#endif
					callback =  walk(m[j][0], m[j][1],  m[j][2], 0, 0,4);
					delay(10);
				}
			}
			desalocarMatriz(m,2);
		break;
		case 6: //Circulo
			//razao = 3;
			ang = 880; // 870 para razao de 6
			m = alocarMatriz(6,3);
			m[0][0] = 1; m[0][1] = 1; m[0][2] = (e_360 * ang) / 360; //linha 1Âª comando
			#if debug_shapes
			Serial.print(m[0][0]);Serial.print(" - ");Serial.print (m[0][1]);Serial.print(" - ");Serial.println(m[0][2]);
			#endif
			callback =  walk(m[0][0], m[0][1], m[0][2], 6, 1,4);
			desalocarMatriz(m,1);
		break;
		/*
		case 3://Posicionamentos angulare (Circulo)
			ang = 30; //escolha o angulo
			cateto_O = 45;
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
int  walk(int _Right, int _Left, int stepstowalk, int _freqRot, int _CW_CCW, byte speed)
{ 	 
	#if debug_walk
	Serial.print(_Left);Serial.print(" - ");Serial.println(_Right);
	#endif
	//==============================================================
	//variaveis de verificação de distancia --> PENDENTE
	//==============================================================
	int option = 0, callback_end_walk = 1;
	bool flag_Left = true, flag_Right=true, flag_button = true;//Aciona um Mecanismo para gerar curvas, com uma razão proporcional.
	int stepLeft = 3, stepRight = 3;//Faz o fluxo step binario 
	byte binLeft[4]= {B10010000, B11000000, B01100000, B00110000};//FullStep
	byte binRight[4]= {B00001100, B00000110, B00000011, B00001001};//FullStep
	if(_Right == 0) flag_Right = false;//Stop
	if(_Left == 0) flag_Left = false;//Stop
	while (stepstowalk > 0)
	{
		
		if (millisAtual % speed == 0)
		{
			char LQ0 = optionPin.pressedtime();
			if(LQ0 == 'Q' || LQ0 == 'L') option = optionPin.readbutton(LQ0);
			if(option == 1 || option == 7)
			{
				stepstowalk = 0; 
				callback_end_walk = 2;
				disable_coil();
				return callback_end_walk;
			}
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
			if(stepstowalk ==0)callback_end_walk = 1;
			else callback_end_walk = 0;
			flag_Left = true;
			flag_Right= true;
			delay(2);
		//}	
		}
		millisAtual = millis();
	}
	//Desabilitar bobina para economia de energia
	disable_coil();
	return callback_end_walk;
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
	int i;
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

