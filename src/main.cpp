#include <Arduino.h>
#include <SD.h>

//SD
#define SD_CS 5
File dataFile;
char file_name[20] = "/data1.csv";
unsigned long set_pointer = 0;
bool ler = false;
int i = 0;
bool available = true;

void readFile()
{
    dataFile = SD.open(file_name, FILE_READ);

    if (dataFile) 
    {
        if(ler) 
        {
            dataFile.seek(set_pointer); // Para setar a posição (ponteiro) de leitura do arquivo
        }
        String linha;

        if (dataFile.available()) 
        {
            linha = dataFile.readStringUntil('\n');

            set_pointer = dataFile.position(); // Guardar a posição (ponteiro) de leitura do arquivo

            // Separar os valores usando a vírgula como delimitador
            int posVirgula1 = linha.indexOf(',');
            int posVirgula2 = linha.indexOf(',', posVirgula1 + 1);
            int posVirgula3 = linha.lastIndexOf(',');

            // Extrair os valores de cada sensor
            String rpm = linha.substring(0, posVirgula1);
            String speed = linha.substring(posVirgula1 + 1, posVirgula2);
            String timestamp = linha.substring(posVirgula2 + 1, posVirgula3);

            uint8_t rpm_value = String(rpm).toInt();
            uint8_t speed_value = String(speed).toInt();
            uint64_t timestamp_value = String(timestamp).toInt();

            Serial.printf("rpm=%d, speed=%d, timestamp=%d\n", rpm_value, speed_value, timestamp_value);

            ler = true;
        }
        else {
            available=false;
        }
    } else {
        Serial.println("Failed to open file for reading or the file not exist");
        return;
    }
}

void setup(){
    Serial.begin(115200);

    if (!SD.begin()){
        Serial.println("Falha ao inicializar o cartão SD.");
        return;
    }

    Serial.println("Cartão SD inicializado com sucesso.");

    Serial.printf("Reading file: %s\n", file_name);

    while(1){
        if(!available){
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