
byte buffer[]={48,49,50,51,52,53,54,55};

void setup() 
{
  Serial.begin(9600);
}

void loop() 
{
    int b =4;
    String c="";

    for (byte i = b; i < b + 4; i++) //
    {
        int  a= buffer[i];
        Serial.print(" ");
        Serial.print(a);
        if (a>47 && a<58) c.concat(char(buffer[i]));
    }
    Serial.println(" -> ");
    Serial.println(atoi(c.c_str()));
    delay(1000);
  //========================================================================
}

int potencia(int base, int expoente)
{
    if(expoente == 0)   return 1;
    
    else    return base * potencia(base, expoente - 1);
}