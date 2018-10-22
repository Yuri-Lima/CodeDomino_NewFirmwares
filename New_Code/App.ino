//Programa : Code Dominó
//Teste de commint
//Autor : Daniel Chagas / Patrick / Yuri Lima
/*
	Criar botÃ£o reset diretamente da placa arduino, sempre que houver necessidade de parar a execuÃ§Ã£o do codigo.
	Nova roda 3D https://www.thingiverse.com/thing:862438/files
	https://wakatime.com/projects
	https://youtu.be/B86nqDRskVU - Mecanismo de redução do motor
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
//=====================================================================================================
//Etapa 1 - Motor
const float GrausPassoDoMotor = 0.1757;
const float m_erro_r = 1.05;
float r_360 = (360 / GrausPassoDoMotor) + m_erro_r;

//Etapa 2 - Roda 
const float raioRoda =  3.30;
float C = (2 * PI * raioRoda); //C"

//Etapa 3 - Carro
const float raioEixo = 4.5; 
float C_ = (2 * PI * raioEixo); //C'

//Etapa 4 - Revoluções 
float revol_ = C_ / C;//C" / C'

//Etapa 5 - Passos
const float m_erro_e = 0.045;
int e_360 = r_360 * (revol_ + m_erro_e);//passo para rotação do proprio eixo
//=====================================================================================================
/*
#include <deprecated.h>
#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <require_cpp11.h>
#include <Ultrasonic.h>
*/
//=========================================================================================
//Função Loop
unsigned long millisAnterior = 0;
unsigned long millisAtual = 0;
unsigned long millisAnterior2 = 0;
unsigned long millisAtual2 = 0; 

//=========================================================================================
//Função Lerbotao
int valorBotao = 0;

//=========================================================================================
//Função Loop e Caminhar
int passo = 50; //milissegundos entre cada instruÃ§Ã£o de movimento.

//=========================================================================================
//Função Caminhar
int latchPin = 8; //Pin connected to ST_CP of 74HC595
int clockPin = 7; //Pin connected to SH_CP of 74HC595
int dataPin = 6; ////Pin connected to DS of 74HC595
int passosCaminhar = 0;
int frenagem = 20; //milissegundos entre cada instruÃ§Ã£o de movimento.

//=========================================================================================
//Função Caminhar e Esqueda e Direita
byte binarioDir = B00000000;
byte binarioEsq = B00000000;

//=========================================================================================
//Função Lerbotao e loop
bool botao = false;
#define botao_gravar_parar 709

//=========================================================================================
//Função alocarMatriz e desalocarMatriz
int **m;

void setup()
{ 
	//DEBUGPRINTLN0("Started SETUP");
	Serial.begin(9600);
	//=========================================================================================
	Serial.print(F("Passos para a RODA rodar (r_360): "));  Serial.println(r_360);
	Serial.print(F("Distancia pecorrida pela RODA (C): ")); Serial.println(C);
	Serial.print(F("Distancia pecorrida pelo CARRO no proprio EIXO (C_): "));    Serial.println(C_);
	Serial.print(F("Quantas voltas a RODA tem que dar para o CARRO rodar no EIXO (revol_): "));   Serial.println(revol_);
	Serial.print(F("Quantos passos para o CARRO rodar no EIXO (e_360): "));   Serial.println(e_360);
	//Serial.println(convertAngulo(45)); //45° é quantos passos?
	//=========================================================================================
	pinMode(latchPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	pinMode(dataPin, OUTPUT);
	digitalWrite(latchPin, LOW);
	shiftOut(dataPin, clockPin, MSBFIRST, B00000000); //envia resultado binÃ¡rio para o shift register
}

void loop()
{
	static int option;
	millisAtual = millis();
	//A cada tempo do passo, execute
	if (millisAtual - millisAnterior >= passo)
	{
		millisAnterior = millisAtual;
		if (botao)
		{ 
			delay(1000);
			formas(option);
			botao = !botao;
			option = 0;
			//Serial.println(revol_);
		}
	}
	millisAtual2 = millis();
	if (millisAtual2 - millisAnterior2 >= 250)
	{
		millisAnterior2 = millisAtual2;
		option = leBotao();
	}
}
int formas(int edro)
{   
	// int p = convertAngulo(45);
	float ang, dist, cateto_O;
	switch(edro)
	{
		case 7: //Rotação no eixo
			ang = e_360; //escolha o angulo	
			m = alocarMatriz(1,1);
			m[0][0] = 0; m[0][1] = 1; m[0][2] = ang; //linha 1Âª comando
			caminhar(m[0][0], m[0][1],  m[0][2], 0, 1);
			Serial.print(m[0][0]);Serial.print(" - ");Serial.print (m[0][1]);Serial.print(" - ");Serial.println(m[0][2]);
			delay(10);
			desalocarMatriz(m,1);
		break;
		case 2: //Reta em 
			dist = 10;// coloque aqui a distancia em cm
			m = alocarMatriz(1,1);
			m[0][0] = 0; m[0][1] = 0; m[0][2] = (r_360 * dist) / C; //linha 1Âª comando
			caminhar(m[0][0], m[0][1],  m[0][2], 0, 0);
			Serial.print(m[0][0]);Serial.print(" - ");Serial.print (m[1][1]);Serial.print(" - ");Serial.println(m[2][2]);
			delay(10);
			desalocarMatriz(m,1);
		break;
		case 1: //Rotação angular no proprio eixo
			ang = 90; //escolha o angulo
			m = alocarMatriz(1,1);
			m[0][0] = 0; m[0][1] = 1; m[0][2] = (e_360 * ang) / 360; //linha 1Âª comando regra de

			Serial.print(m[0][0]);Serial.print(" - ");Serial.print (m[0][1]);Serial.print(" - ");Serial.println(m[0][2]);
			caminhar(m[0][0], m[0][1],  m[0][2], 0, 0);
			
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
			m[0][0] = 0; m[0][1] = 0; m[0][2] = (r_360 * cateto_O) / C; //Cateto oposto
			m[1][0] = 0; m[1][1] = 1; m[1][2] = (e_360 * (90 - ang)) / 360; //complemento de angulo
			m[2][0] = 1; m[2][1] = 1; m[2][2] = (r_360 * (cateto_O / sin(ang * (PI/180)))) / C;//hipotenusa  SOH
			m[3][0] = 0; m[3][1] = 1; m[3][2] = (e_360 * ang) / 360; //angulo que desejamos
			m[4][0] = 0; m[4][1] = 0; m[4][2] = (r_360 * (cateto_O / tan(ang * (PI/180)))) / C; // cateto adjacente TOA
			m[5][0] = 1; m[5][1] = 0; m[5][2] = (e_360 * 90) / 360; //Volta na posição que iniciou
			for(int i = 0;i < 6;i++)
			{	
				Serial.print(m[i][0]);Serial.print(" - ");Serial.print (m[i][1]);Serial.print(" - ");Serial.println(m[i][2]);
				caminhar(m[i][0], m[i][1],  m[i][2], 0, 0);
				delay(50);
			}
				desalocarMatriz(m,6);
		break;  	
		case 5: //Quadrado 10x10 em 90°
			ang = 90; //escolha o angulo
			m = alocarMatriz(2,3);
			m[0][0] = 0; m[0][1] = 0; m[0][2] = (r_360 * 10) / C; //linha 1Âª comando
			m[1][0] = 1; m[1][1] = 0; m[1][2] = (e_360 * ang) / 360;; //linha 2Âª comando
			for(int i = 0;i < 4;i++)
			{  
				for(int j = 0;j < 2;j++)
				{ 
					Serial.print(m[j][0]);Serial.print(" - ");Serial.print (m[j][1]);Serial.print(" - ");Serial.println(m[j][2]);
					caminhar(m[j][0], m[j][1],  m[j][2], 0, 0);
					delay(50);
				}
			}
			desalocarMatriz(m,2);
		break;
		case 6: //Teste 1
			ang = 90;
			m = alocarMatriz(6,3);
			m[0][0] = 0; m[0][1] = 0; m[0][2] = (r_360 * 10) / C; //linha 1Âª comando
			m[1][0] = 1; m[1][1] = 0; m[1][2] = (e_360 * ang) / 360;; //linha 2Âª comando
			m[2][0] = 0; m[2][1] = 0; m[2][2] = (r_360 * 10) / C; //linha 1Âª comando
			m[3][0] = 0; m[3][1] = 1; m[3][2] = (e_360 * ang) / 360; //linha 1Âª comando
			m[4][0] = 0; m[4][1] = 0; m[4][2] = (r_360 * 10) / C; //linha 1Âª comando
			m[5][0] = 1; m[5][1] = 0; m[5][2] = (e_360 * (ang * 2)) / 360; //linha 1Âª comando
			for(int j = 0;j < 6;j++)
			{  
				Serial.print(m[j][0]);Serial.print(" - ");Serial.print (m[j][1]);Serial.print(" - ");Serial.println(m[j][2]);
				caminhar(m[j][0], m[j][1],  m[j][2], 0, 0);
				delay(50);
			}
			desalocarMatriz(m,2);
		break;
		case 3://Posicionamentos angulare (Circulo)
			ang = 30; //escolha o angulo
			cateto_O = 45;
			/*
				Conteudo: Trigonometria Ângulos notávei SOH CAH TOA
				Lista estudada:
					https://youtu.be/TjC3F9sj-x0
					https://www.youtube.com/watch?v=3iHUX_oOcX0
					https://www.dobitaobyte.com.br/sirene-com-arduino/
			*/
			m = alocarMatriz(1,3);//6 linhas e 3 colunas
			m[2][0] = 0; m[2][1] = 0; m[2][2] = (r_360 * (cateto_O / sin(ang * (PI/180)))) / C;//hipotenusa  SOH
			for(int i = 0;i < edro*2;i++)
			{	
				Serial.print(m[i][0]);Serial.print(" - ");Serial.print (m[i][1]);Serial.print(" - ");Serial.println(m[i][2]);
				caminhar(m[i][0], m[i][1],  m[i][2], 4, 0);
				delay(50);
			}
				desalocarMatriz(m,6);
		break;
		default :
		Serial.println("Formato invalido.");  
	}
	return 1;    
}
int leBotao()
{
	int option;
	valorBotao = analogRead(A0);
	//Serial.println(valorBotao);
	if ((valorBotao > 100) && (valorBotao < botao_gravar_parar))
	{
		botao = !botao;
		option =1;
		Serial.println("1");
		//delay(1000);
	}
	if ((valorBotao > botao_gravar_parar +10) && (valorBotao < 724 + 5))
	{
		botao = !botao;
		option =2;
		Serial.println("2");
		//delay(1000);
	}
	if ((valorBotao > 724 + 10) && (valorBotao < 780 +5))
	{
		botao = !botao;
		option =3;
		Serial.println("3");
		//delay(1000);
	}
	if ((valorBotao > 780 + 10) && (valorBotao < 847 +5))
	{
		botao = !botao;
		option =4;
		Serial.println("4");
		//delay(1000);
	}
	if ((valorBotao > 847 + 10) && (valorBotao < 924 +5))
	{
		botao = !botao;
		option =5;
		Serial.println("5");
		//delay(1000);
	}
	if ((valorBotao > 924 + 10) && (valorBotao < 1018 +5))
	{
		botao = !botao;
		option =6;
		Serial.println("6");
		//delay(1000);
	}
	delay(5);
	return option;
}
int caminhar(int dir, int esq, int passosCaminhar, int _freqRot, int _CW_CCW)
{ //0 - direto, 1 - esquerda, 2 - direita, -1 tras)

	while (passosCaminhar > 0)
	{
		digitalWrite(latchPin, LOW);
		if(_freqRot > 1)
		{
			if(_CW_CCW == 1) 
			{
				if((passosCaminhar % _freqRot) == 1) direita(dir);
				esquerda(dir); 
			}
			else if(_CW_CCW == 0)
			{
				if((passosCaminhar % _freqRot) == 1) esquerda(dir);
				direita(dir);
			}
		}
		else
		{
			direita(dir); 
			esquerda(esq);
		}
		shiftOut(dataPin, clockPin, MSBFIRST, binarioEsq | binarioDir ); //envia resultado binÃ¡rio para o shift register
		digitalWrite(latchPin, HIGH);
		if (passosCaminhar > frenagem) //acelera e freia o passo
		{
			passo = round(passo * 0.90); //milissegundos entre cada instruÃ§Ã£o de movimento.
		}
		else
		{
			passo = passo + 1;
		}
		passosCaminhar--;
		delay(5);
		//Serial.println(passosCaminhar);
	}
	passo = 50;
	return passosCaminhar == 0 ? 0 : 1;
}
int convertAngulo(float _angulo)
{  
	return int((e_360 * _angulo) / 360);
}
bool esquerda(int _direcao)
{
		static int passoEsq = 1;
	if (_direcao == 1)
	{
		passoEsq++;
	}
	else
	{
		passoEsq--;
	}

	if (passoEsq > 4) passoEsq = 1;
	if (passoEsq < 1) passoEsq = 4;

	switch (passoEsq)
	{
		case 1:
			binarioEsq = B10010000;//144
		break;

		case 2:
			binarioEsq = B11000000;//192
		break;

		case 3:
			binarioEsq = B01100000;//96
		break;

		case 4:
			binarioEsq = B00110000;//48
		break;
	}
}

bool direita(int _direcao)
{
	static int passoDir = 1; 
	if (_direcao == 1)
	{
		passoDir++;
	}
	else
	{
		passoDir--;
	}

	if (passoDir > 4) passoDir = 1;
	if (passoDir < 1) passoDir = 4;

	switch (passoDir)
	{
		case 4:
			binarioDir = B00001100;//12
		break;

		case 3:
			binarioDir = B00000110;//6
		break;

		case 2:
			binarioDir = B00000011;//3
		break;

		case 1:
			binarioDir = B00001001; //9
		break;
	}
}

//Ref.: https://youtu.be/g2Tco_v73Pc ---> AlocaÃ§Ã£o dinamica
int** alocarMatriz(int Linhas,int Colunas)//Recebe a quantidade de Linhas e Colunas como ParÃ¢metro
{  
	int i,j; //VariÃ¡veis Auxiliares
	int **m = (int**)malloc(Linhas * sizeof(int*)); //Aloca um Vetor de Ponteiros
	if(m==NULL)
	{
		Serial.println("Erro de alocacao em linha");
		return 0;
	}
	for (i = 0; i < Linhas; i++)//Percorre as linhas do Vetor de Ponteiros
	{  
		m[i] = (int*) malloc(Colunas * sizeof(int)); //Aloca um Vetor de Inteiros para cada posiÃ§Ã£o do Vetor de Ponteiros.
		if(m[i]==NULL)
		{
			Serial.println("Erro de alicacao em coluna");
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
	for (i = 0; i < Linhas; i++)
	{
		free(m[i]);
	}
	free(m);
}