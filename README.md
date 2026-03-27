# AndeSports - Sistema Pub/Sub con Sockets en C

## 1. Compilación

gcc broker_tcp.c -o brokerTCP   
gcc publisher_tcp.c -o publisherTCP   
gcc subscriber_tcp.c -o subscriberTCP   

gcc broker_udp.c -o brokerUDP   
gcc publisher_udp.c -o publisherUDP   
gcc subscriber_udp.c -o subscriberUDP   

## 2. Ejecución

### 2.1 Ejecución del Broker
./brokerTCP o ./brokerUDP

### 2.2 Ejecución del Publisher
./publisherTCP \< partido \> o ./publisherUDP \< partido \>

### 2.3 Ejecución del Subscriber
./publisherTCP \< partido \> o ./publisherUDP \< partido \>

En ambos casos un ejemplo de partido es: "Partido A vs B", para que funcione correctamente el partido del subscriber debe coincidir con el partido de un publisher. En caso de querer varios subscribers o publishers debe hacerse desde terminales diferentes. 

## 3. Librerías Utilizadas

### 3.1 stdio.h

### 3.2 stdlib.h

### 3.3 string.h

### 3.4 unistd.h

### 3.5 arpa/inet.h

### 3.6 sys/select.h

