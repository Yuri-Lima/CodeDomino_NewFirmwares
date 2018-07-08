/*
 * CODE DOMINO
 * por Daniel Almeida Chagas
 * prof.daniel.chagas@gmail.com
 * 
 * Funcionamento:
 * O robô inicia parado, aguardando a leitura de uma peça RFID para ou iniciar a execução de um programa ou gravar um programa.
 * O loop dessa chamada está na função loop(), e é feita a cada passar da variável passo. Esta inicia alta (50) mas é diminuida 
 * até o valor 3 gradativamente, para implementar a aceleração e desaceleração do robô (mais precisão no movimento).
 * 
 * A cada 1 segundo a função loop() chama outras funções acessórias: leRfid() e medeDistancia().
 * 
 * Ao ler um RFID o robô chama as funções executaInstrução() ou gravaInstrução() dependendo da variável executando (marcada 
 * como FALSE ele irá gravar o comando). A função também chama a outra iniciaGravacaoPrograma(), caso a peça seja 1 (inicia gravação). 
 * 
 * A função iniciaGravacaoPrograma() faz o robô andar para frente buscando novas peças para gravar. 
 * 
 * A função gravaInstrucao() recebe o ID do comando e o valor do parâmetro. Ela incrementa a variável passosCaminhar afim de que 
 * a cada peça encontrada, o robô ande um pouco mais buscando novas peças (parando quando encontra uma peça fim). 
 * 
 * A função executaInstrução() contém o comportamento do robô para cada peça. 
 * - 1 e 2 teoricamente nunca são executadas aqui;
 * - 3 é a peça executar (play). 
 * - 4 é a peça frente (forward). Se vier sem parâmetro, usa uma variável com uma distância padrão. Multiplica o parâmetro por um 
 *   valor para valer os passos curtos do motor. 
 * - 5 e 6 são as de giro no próprio eixo. Se vierem sem parâmetro, gira 90 graus. A multiplicação é para transformar os graus em 
 *   passos do motor.
 * - 7 é espere (break). Espera 1 segundo ou o número de segundos dado como parâmetro. 
 * - 8 e 9 são as peças repetir e fim repetir. 
 * 
 * A função fim() reseta as variáveis, voltando o robô para o estado de repouso inicial. A leitura de RFID a cada segundo continua. 
 * 
 * A função caminhar() controla a variável passo para permitir aceleração e desaceleração do robô. Ela também chama as funções 
 * direita() e esquerda() igualmente, fazendo que o robô ande para frente. O padrão de passo gerado por essas funções é agrupado e 
 * enviado ao chip Shift Register. 
 * 
 * As funções direita() e esquerda() constroem o padrão de passos (nas variáveis binarioDir e binarioEsq). Os passos seguem o padrão
 * 8 passos. 
 * 
 * A função girar() recebe o parâmetro da direção a fazer (1 para horário, 0 para anti-horário), e também faz a aceleração e 
 * desaceleração do passo (porém com passo mínimo 6, para não ir tão rápido). 
 * 
 * A função erro() toca um tom negativo no buzzer e para o robô. 
 * 
 * 1.0 - Versão inicial do Code Domino.
 * 1.1 - Implementado movimento com 4 passos. 
 */
#include <SPI.h>
#include <MFRC522.h>

//pinos da shield
int sck = 13; //SPI
int miso = 12; //SPI
int mosi = 11; //SPI
#define RST_PIN         9
#define SS_PIN          10
int latchPin = 8; //Pin connected to ST_CP of 74HC595
int clockPin = 7; //Pin connected to SH_CP of 74HC595
int dataPin = 6; ////Pin connected to DS of 74HC595
int trig = 5; //ultrasom
int echo = 4; //ultrasom
int buzzer = 3; //som
int led = 2;
//A0 são botões
//Analógicos de A1 a A5 são sensores de linha

//Instanciação de Objetos
//---------RFID
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
MFRC522::StatusCode status; //variável para pegar status do cartão
byte buffer[18];  //data transfer buffer (16+2 bytes data+CRC)
byte size = sizeof(buffer);
uint8_t pageAddr = 0x06;

//Variáveis
unsigned long millisAnterior = 0;
unsigned long millisAtual = 0;
boolean caminhando = false; //define se o robô está em movimento ou não
//boolean gravando = false; //define se o robô está gravando comandos ou não.
boolean executando = false; //define se o robô está executando comandos ou não.
//char programa[64] = "SFLE"; //vetor dos comandos
int programa[64] = {3, 8, 4, 5, 9, 5, 4, 6, 2};
int parametros[64] = {0, 4, 0, 0, 0, 45, 7, 50, 0}; //vetor dos parâmetros
int ponteirosRepetir[32]; //vetor que armazena os ponteiros de repetição
int ponteiro = 0; //ponteiro indicando em que passo está do programa
int passo = 50; //milissegundos entre cada instrução de movimento.
int setDistanciaBasica = 10; //distancia básica de movimento do robô em linha reta
int passosCaminhar = 0; //quantidade de passos que o robô deve percorrer até a próxima instrução.
int grausGirar = 0; //quantidade de passos que o robô deve percorrer até a próxima instrução.
int movimentacaoX = 0; //variáveis que gravam o deslocamento total do robô, para a instrução voltar ao início. 
int movimentacaoY = 0;
int passoDir = 1; //vai de 1 a 8
int passoEsq = 1;
byte binarioDir = B00000000;
byte binarioEsq = B00000000;

void setup() {
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(14, INPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  SPI.begin(); 
  mfrc522.PCD_Init();
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, B00000000); //envia resultado binário para o shift register
  digitalWrite(latchPin, HIGH);
  inicio();
}

void loop() {
  millisAtual = millis();
  //A cada tempo do passo, execute
  if(millisAtual - millisAnterior >= passo) {
    millisAnterior = millisAtual;
    if(caminhando) {
      caminhar();
    } else {
      if(executando){
        ponteiro++;
        bipe();
        executaPrograma(ponteiro);
      }
    }
  }
  if(millisAtual%1000==0){
    //De um em um segundo lê o RFID e mede a distância
    mensagemDebug("Lendo RFID...");
    leRfid();
    //medeDistancia();
  }
  if((millisAtual%3000==0)&&!executando&&caminhando){
    bipeFino();
  }
  if((millisAtual%50==0)&&digitalRead(14)){
    afirmativo();
    executando = true;
    executaPrograma(0);
  }
}

void leRfid() {
  //mensagemDebug("leRfid() lendo RFID...");
  //Códigos de leitura do RFID
  //Se não houver tag, não faz nada.
  if (!mfrc522.PICC_IsNewCardPresent()) return;
  //Chama o leitor de RFID e lê uma tag. 
  if (!mfrc522.PICC_ReadCardSerial()) return;
  bipe();
  status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(pageAddr, buffer, &size);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Read() falhou: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    erro();
    return;
  }
  mfrc522.PICC_HaltA();
  
  int instrucao = block(0);
  int parametro = block(4);
  //Se for a instrução 1, gravar programa, inicia a gravação imediata e não passa para as outras funções.
  if(instrucao==1){
    iniciaGravacaoPrograma();
    return;
  }
  if(instrucao==2){ //Se a instrução for 2, ele para a gravação. 
    fim();
    return;
  } 
  if(instrucao==3){
    executando = true;
    executaPrograma(0);
  } else {
    //Se estiver no modo executando, executa a tag. Senão grava a tag
    if((caminhando && executando)||(!caminhando && !executando)){
      executaInstrucao(instrucao,parametro);
    } else if(!executando){
      gravaInstrucao(instrucao,parametro);
    } 
  }
  String temp = "leRfid() ";
  temp.concat(instrucao);
  temp.concat(":");
  temp.concat(parametro);
  mensagemDebug(temp);
}

void iniciaGravacaoPrograma() {
  mensagemDebug("iniciaGravacaoPrograma() Gravando peças na memória...");
  passosCaminhar+=2000; //inicia a caminhada do robô buscando peças
  caminhando = true;
  executando = false;
  ponteiro = 0;
  for(int i = 0;i<64;i++){ //apaga programa anterior
    programa[i] = 2;
    parametros[i] = 0;
  }
}

void gravaInstrucao(int instrucao,int parametro){
  mensagemDebug("gravaInstrucao() +1 peça no programa...");
  //A cada nova instrução de gravação, aumenta a distância de movimento de busca do robô.
  passosCaminhar+=1500;
  //Grava a instrução recebida no vetor programa
  programa[ponteiro] = instrucao;
  parametros[ponteiro] = parametro;
  ponteiro++;
}

void executaPrograma(int ponteiro) {
  if(programa[ponteiro]!=0) { //não executa numericos
    int ponteiroTemp = ponteiro + 1;
    int quantidNumericas = 0;
    int novoParametro = 0;
    while(programa[ponteiroTemp]==0) { //procura as peças numéricas de parâmetro
      ponteiroTemp++;
      quantidNumericas++;
      //if(ponteiroTemp>9) break;
    }
    int j=0;
    for(int i=quantidNumericas;i>0;i--){
      novoParametro = novoParametro + (parametros[ponteiro+i] * pow(10,j)); //transforma as peças numéricas em um parâmetro novo
      j++;
    }
    String temp = "executaPrograma() Instrução ";
    temp.concat(ponteiro);
    temp.concat("-> ");
    temp.concat(programa[ponteiro]);
    temp.concat(":"); 
    if(quantidNumericas == 0) { //se houver peças numéricas, elas serão o parâmetro. Se não, será o parâmetro padrão. 
      temp.concat(parametros[ponteiro]);
      mensagemDebug(temp);
      executaInstrucao(programa[ponteiro],parametros[ponteiro]);
    } else {
      temp.concat(novoParametro);
      mensagemDebug(temp);
      executaInstrucao(programa[ponteiro],novoParametro);
    }
  }
}

void executaInstrucao(int instrucao,int parametro){
  switch (instrucao) {
    case 1:
      iniciaGravacaoPrograma(); //Se a tag for gravar programa, inicia a gravação.
      break;
    case 2:
      afirmativo();
      fim(); //Se a tag for parar, para o movimento e a gravação.
      break;
    case 3:
      afirmativo();
      executando = true; //se a tag for executar, inicia a execução do programa.
      break;
    case 4: //forward
      if(parametro==0) parametro=setDistanciaBasica;
      passosCaminhar = passosCaminhar + (parametro * 100);
      caminhando = true;
      break;
    case 5: //right
      if(parametro==0) parametro=90;
      grausGirar = round(parametro * 7.8);
      caminhando = true;
      break;
    case 6: //left
      if(parametro==0) parametro=90;
      grausGirar = round(parametro * 7.8) * (-1);
      caminhando = true;
      break;
    case 7: //break
      passo = parametro*1000;
      break;
    case 8: //repetir
      if(parametro==0) parametro=1; //se o usuário não colocar um número após a peça repetir, ele executa a rotina uma vez.
      ponteirosRepetir[0] = ponteiro; //array impar que guarda o ponteiro que marca o repetir, afim do código poder voltar à instrução.
      ponteirosRepetir[1] = parametro; //array par que guarda o contador do for.
      if(true){ 
        String temp = "Repetindo o bloco ";
        temp.concat(ponteirosRepetir[1]);
        temp.concat(" vezes...");
        mensagemDebug(temp);
      }
      break;
    case 9: //fim repetir
      ponteirosRepetir[1]= ponteirosRepetir[1] - 1;
      if(ponteirosRepetir[1]>0){ 
        ponteiro = ponteirosRepetir[0];
        String temp = "Repetição nº ";
        temp.concat(ponteirosRepetir[1]);
        mensagemDebug(temp);
      } else {
        mensagemDebug("Fim do repetir.");
      }
      break;
    default:
      mensagemDebug("executaInstrucao() peça desconhecida!");
      erro();
      break;
  }
}

void fim(){
  mensagemDebug("fim() Fim de execução.");
  passo = 50;
  passosCaminhar = 0;
  grausGirar = 0;
  caminhando = false;
  executando = false;
  ponteiro = 0;
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, B00000000); //envia resultado binário para o shift register
  digitalWrite(latchPin, HIGH);
}

void medeDistancia(){
  //Mede distância na frente do robô com o sensor ultrasom
  mensagemDebug("Sensor de distância não instalado!");
}

void caminhar(){
  digitalWrite(latchPin, LOW);
  if(passosCaminhar!=0){
    //debug
    if(passosCaminhar%100==0){
      String temp = "Caminhando ";
      temp.concat(passosCaminhar);
      mensagemDebug(temp); 
    }
    direita(1);
    esquerda(1);
    if(passosCaminhar>50){ //acelera e freia o passo
      passo = round(passo*0.90);
      if(passo<3) passo=3;
    } else {
      passo = passo + 1;
    }
    passosCaminhar--;
    if(passosCaminhar==0 && executando==false) erro(); //se o robô andou buscando peças até acabar os passos, significa que ele não achou a peça final, o que é um erro.
  } else if(grausGirar!=0){
    //debug
    if(grausGirar%100==0){
      String temp = "Girando ";
      temp.concat(grausGirar);
      mensagemDebug(temp); 
    }
    girar();
  } else {
    caminhando = false;
    passo = 50;
  }
  shiftOut(dataPin, clockPin, MSBFIRST, binarioEsq | binarioDir ); //envia resultado binário para o shift register
  digitalWrite(latchPin, HIGH);
}

void esquerda(int direcao) {
  if(direcao==1){
    passoEsq++;
  } else {
    passoEsq--;
  }
  if(passoEsq>4) passoEsq = 1;
  if(passoEsq<1) passoEsq = 4;
  switch (passoEsq) {
    case 1:
      binarioEsq = B10010000;
      break;
    case 2:
      binarioEsq = B11000000;
      break;
    case 3:
      binarioEsq = B01100000;
      break;
    case 4:
      binarioEsq = B00110000;
      break;
    /*
    case 5:
      binarioEsq = B00100000;
      break;
    case 6:
      binarioEsq = B00110000;
      break;
    case 7:
      binarioEsq = B00010000;
      break;
    case 8:
      binarioEsq = B10010000;
      break;
      */
  }
}

void direita(int direcao) {
  if(direcao==1){
    passoDir++;
  } else {
    passoDir--;
  }
  if(passoDir>4) passoDir = 1;
  if(passoDir<1) passoDir = 4;
  switch (passoDir) {
    /*
    case 8:
      binarioDir = B00001000;
      break;
    case 7:
      binarioDir = B00001100;
      break;
    case 6:
      binarioDir = B00000100;
      break;
    case 5:
      binarioDir = B00000110;
      break;
      */
    case 4:
      binarioDir = B00001100;
      break;
    case 3:
      binarioDir = B00000110;
      break;
    case 2:
      binarioDir = B00000011;
      break;
    case 1:
      binarioDir = B00001001;
      break;
  }
}

void girar(){
  if(grausGirar > 0){
    direita(1);
    esquerda(0);
    grausGirar--;
  } else {
    direita(0);
    esquerda(1); 
    grausGirar++;
  }
  if((grausGirar>50)or(grausGirar<50)){ //acelera e freia o passo
      passo = round(passo*0.90);
      if(passo<6) passo=6;
  } else {
      passo = passo + 1;
  }
  if(grausGirar == 0) caminhando = false;
}

void inicio() {
  digitalWrite(led, HIGH);
  tone(buzzer, 523, 800);
  delay(150);
  tone(buzzer, 587, 800);
  delay(150);
  tone(buzzer, 659, 1500);
  delay(400);
  noTone(buzzer);
  digitalWrite(led, LOW);
  mensagemDebug("inicio() Code_Domino robot iniciado!");
}

void erro(){
  mensagemDebug("erro() ERRO!");
  tone(buzzer, 391, 800);
  delay(150);
  noTone(buzzer);
  delay(30);
  tone(buzzer, 261, 1500);
  delay(400);
  noTone(buzzer);
  fim();
}

void bipe() {
  digitalWrite(led, HIGH);
  tone(buzzer, 391, 100);
  delay(100);
  noTone(buzzer);
  digitalWrite(led, LOW);
}

void bipeFino() {
  tone(buzzer, 1047, 30);
  delay(30);
  noTone(buzzer);
}

void afirmativo() {
  tone(buzzer, 440, 200);
  delay(200);
  noTone(buzzer);
  delay(100);
  tone(buzzer, 494, 200);
  delay(200);
  noTone(buzzer);
  delay(100);
  tone(buzzer, 523, 400);
  delay(400);
  noTone(buzzer);
}

int block(int b){ 
  Serial.print("Lendo RFID bloco ");
  Serial.print(b);
  Serial.print(":");
  String c="";
  for (byte i = b; i < b+4; i++) {
    int a = buffer[i];
    Serial.print(" ");
    Serial.print(a);
    if (a>47 && a<58)
    c.concat(char(buffer[i]));
  }
  Serial.print(" -> ");
  return stringToInt(c);
}

int stringToInt(String minhaString) { //recebe uma string e transforma em inteiro
  int  x;
  int tam = minhaString.length() - 1;
  int numero = 0;
  for(int i = tam; i >= 0; i--) {
    x = minhaString.charAt(i) - 48;
    numero += x * potencia(10, tam - i);
  }
  Serial.println(numero);
  return numero;
}

void mensagemDebug(String mensagem){
  Serial.print(millisAtual);
  Serial.print(" - ");
  if(caminhando) Serial.print(" <C> ");
  if(executando) {
    Serial.print(" |E| ");
  }  else {
    Serial.print(" |G| ");
  }
  Serial.println(mensagem);
}

int potencia(int base, int expoente){
  if(expoente == 0)
  return 1;
  else   
  return base * potencia(base, expoente - 1);
}
