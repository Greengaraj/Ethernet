#include <SPI.h>                                                // Библиотека для работы с SPI
#include <Ethernet2.h>                                          // Библиотека для работы с Ethernet Shield

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };            // MAC-адрес контроллера
char server[] = "192.168.x.x";                                  // Адрес запрашиваемого сервера

IPAddress ip(192, 168, x, x);                                   // Задаю статический IP-адрес на тот случай, если у DHCP выдать IP-адрес не получится
EthernetClient client;                                          // Создаю клиента, который будет подключаться к необходимому серверу и порту
 
void setup()                                                    // Функция, задающая настройки
{
  Serial.begin(9600);                                           // Открываю последовательный порт
  while (!Serial){}                                             // Жду открытия монитор COM-порта для того, чтобы отследить все события в программе
  Serial.print("Serial init OK\r\n");

  if (Ethernet.begin(mac) == 0)                                 // Запускаю Ethernet-соединение
  {
    Serial.println("Failed to configure Ethernet using DHCP");  // Если не удалось сконфигурировать Ethernet при помощи DHCP
    Ethernet.begin(mac, ip);                                    // Продолжать дальше смысла нет, поэтому вместо DHCP попытаю сделать это при помощи IP-адреса
  }
  Serial.print("My IP address: ");                              // Получаю и вывожу локальный IP адрес
  Serial.println(Ethernet.localIP());
  delay(1000);                                                  // Даю Ethernet 1 секунду на инициализацию
  Serial.println("connecting...");

  if (client.connect(server, 8080))                             // Если подключение установлено, сообщаю об этом в COM-порт
  {
    Serial.println("connected");
    // формируем HTTP-запрос
    client.println("GET /station/new_task/ HTTP/1.1");
    client.println("Host: 192.168.x.x:8080");
    client.println("Connection: close");
    client.println();
  }
  else
  {
    Serial.println("connection failed");                        // Если соединения с сервером нет, пишу об этом в COM-порт
  }
}
 
void loop()
{
  if (client.available())                                       // Если есть непрочитанные байты принятые клиентом от удаленного сервера, с которым установлено соединение
  {
    char c = client.read();                                     // Считываю данные и печатаю в Serial-порт
    Serial.print(c);
  }
  if (!client.connected())                                      // Если сервер отключился
  {
    Serial.println();
    Serial.println("disconnecting.");                           // Печатаю об этом в Serial-порт
    client.stop();                                              // Останавливаю работу клиента
    while(1){}                                                  // Останавливаю программу в бесконечном цикле
  }
}
