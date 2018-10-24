
/*

   Criar botão reset diretamente da placa arduino, sempre que houver necessidade de parar a execução do codigo.

*/
int x = 0;
#define botao_gravar_parar 709

unsigned long millisAnterior = 0;
unsigned long millisAtual = 0;
unsigned long millisAnterior2 = 0;
unsigned long millisAtual2 = 0;
long timerBotao = 0;

int valorBotao = 0;
bool ultimoEstadoBotoes = LOW;
int ultimoValorBotoes = 0;
long timerLongoPressionar = 1000;
int latchPin = 8; //Pin connected to ST_CP of 74HC595
int clockPin = 7; //Pin connected to SH_CP of 74HC595
int dataPin = 6; ////Pin connected to DS of 74HC595
int frenagem = 20; //milissegundos entre cada instrução de movimento.
int passo = 50; //milissegundos entre cada instrução de movimento.
int setDistanciaBasica = 10; //distancia básica de movimento do robô em linha reta
int passosCaminhar = 0;
boolean caminhando = false; //define se o robô está em movimento ou não
byte binarioDir = B00000000;
byte binarioEsq = B00000000;
int passoDir = 1; //vai de 1 a 4
int passoEsq = 1;
bool botao = false;
float angle = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, B00000000); //envia resultado binário para o shift register
}

void loop()
{
  millisAtual = millis();
  //A cada tempo do passo, execute
  if (millisAtual - millisAnterior >= passo)
  {
    millisAnterior = millisAtual;
    if (botao || x)
    {
      int instrucao = 8;
      x = x + int(caminhar(0, 0, 90, 1000));
      Serial.println(x);
      x <= (instrucao - 1) ? x + int(caminhar(1, 0, 90, 700)) : Serial.println("Fim");
      Serial.println(x);
      x <= (instrucao - 1) ? x + int(caminhar(0, 0, 90, 1000)) : Serial.println("Fim");
      Serial.println(x);
      x <= (instrucao - 1) ? x + int(caminhar(1, 0, 90, 700)) : Serial.println("Fim");
      Serial.println(x);
      x <= (instrucao - 1) ? x + int(caminhar(0, 0, 90, 1000)) : Serial.println("Fim");
      Serial.println(x);
      x <= (instrucao - 1) ? x + int(caminhar(1, 0, 90, 700)) : Serial.println("Fim");
      Serial.println(x);
      x <= (instrucao - 1) ? x + int(caminhar(0, 0, 90, 1000)) : Serial.println("Fim");
      Serial.println(x);
      x <= (instrucao - 1) ? x + int(caminhar(1, 0, 90, 700)) : Serial.println("Fim");
      Serial.println(x);

      botao = !botao;
    }
  }
  millisAtual2 = millis();
  if (millisAtual2 - millisAnterior2 >= 300)
  {
    millisAnterior2 = millisAtual2;
    leBotao();
  }
}


int caminhar(int dir, int esq, int angulo, int passosCaminhar)
{ //0 - direto, 1 - esquerda, 2 - direita, -1 tras)

  while (passosCaminhar > 0)
  {
    digitalWrite(latchPin, LOW);
    direita(dir); esquerda(esq);
    if (passosCaminhar > frenagem) //acelera e freia o passo
    {
      passo = round(passo * 0.90); //milissegundos entre cada instrução de movimento.
      //if(passo<3) passo=3;
    }
    else
    {
      passo = passo + 1;
    }
    passosCaminhar--;
    Serial.println(passosCaminhar);
    shiftOut(dataPin, clockPin, MSBFIRST, binarioEsq | binarioDir ); //envia resultado binário para o shift register
    digitalWrite(latchPin, HIGH);
  }
  caminhando = false;
  passo = 50;
  return passosCaminhar == 0 ? 0 : 1;
}

void esquerda(int direcao)
{
  if (direcao == 1)
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

void direita(int direcao)
{
  if (direcao == 1)
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

void leBotao()
{
  valorBotao = analogRead(A0);
  //Serial.println(valorBotao);
  if ((valorBotao > 100) && (valorBotao < botao_gravar_parar))
  {
    botao = !botao;
    delay(1000);
  }


}