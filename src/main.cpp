#include <Arduino.h>
#include <SD.h>

//SD
#define SD_CS 5
File dataFile;
char file_name[20] = "file_name.csv";
unsigned long set_pointer = 0;
bool scape = false;


void readFile();

void setup(){
    Serial.begin(115200);

    if (!SD.begin(SD_CS)){
        Serial.println("Falha ao inicializar o cartão SD.");
        return;
    }

    Serial.println("Cartão SD inicializado com sucesso.");

    Serial.printf("Reading file: %s\n", file_name);

    while(1){
        if(scape){
            dataFile.close();
            break;
        }
        readFile();
        }
    Serial.print("Acabou a leitura.");
}

void loop(){
    // Aqui você pode adicionar outras tarefas ou deixar o loop vazio, pois a leitura do arquivo já foi realizada no setup().
}


void sdRead(){
    int i = 0;

    dataFile = SD.open(file_name, FILE_READ);
    if (dataFile){
        if (i > 0){
            dataFile.seek(set_pointer); // Para setar a posição (ponteiro) de leitura do arquivo
        }
        String linha;

        if(dataFile.available()){  // Verifica se tem dado para ler
            linha = dataFile.readStringUntil('\n');

            set_pointer = dataFile.position(); // Guardar a posição (ponteiro) de leitura do arquivo

            // Separar os valores usando a vírgula como delimitador
            int posVirgula1 = linha.indexOf(',');
            int posVirgula2 = linha.indexOf(',', posVirgula1 + 1);

            // Extrair os valores de cada sensor
            String rpm = linha.substring(0, posVirgula1);
            String speed = linha.substring(posVirgula1 + 1, posVirgula2);
            String timestamp = linha.substring(posVirgula2 + 1);
        }
        else{
            scape = true;
        }
        
        if (i == 0){
            i += 1;
        }
    }
    else
    {
        Serial.println("Failed to open file for reading");
        return;
    }
}