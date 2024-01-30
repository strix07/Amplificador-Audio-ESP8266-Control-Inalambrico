# Amplificador de audio con control inalámbrico mediante ESP8266

## Descripción
El proyecto consiste en el desarrollo de un prototipo de amplificador de audio de 3W con control inalámbrico a través de una aplicación móvil.

Se implementa una interfaz de usuario en la app que permite encender/apagar el amplificador, activar/desactivar el vúmetro y monitorear la temperatura.

Este proyecto es un excelente ejemplo de cómo el Internet de las Cosas (IoT) puede mejorar productos cotidianos agregando capacidades de control y monitoreo remoto.

Al incorporar conectividad WiFi y protocolos como MQTT, el amplificador puede ahora ser controlado desde un smartphone, sin necesidad de interacción física directa. Esto abre varias posibilidades:

- Control remoto desde cualquier lugar: puede encender o apagar el amplificador sin estar físicamente junto a él.
- Monitoreo remoto: la aplicación permite supervisar el estado y temperatura del dispositivo en tiempo real.
- Integración con otros sistemas y automatización: el amplificador podría, por ejemplo, apagarse automáticamente de noche.
  
 Así, el IoT está tomando dispositivos "tontos" y entregando control, conveniencia y capacidades mejoradas de manera simple y rentable. Esto se extrapola a infinitos escenarios, desde ciudades inteligentes hasta hogares conectados. Este proyecto ejemplifica perfectamente la promesa del IoT.

La comunicación se realiza mediante el protocolo MQTT y el microcontrolador ESP8266 con conexión WiFi.

## Diagrama de bloques
<p align="center">
  <img src="https://github.com/strix07/Amplificador-Audio-ESP8266-Control-Inalambrico/assets/142692042/fa6500d1-442b-4ee9-9fe5-91d5342dd40f" alt="Diagrama de bloques">
</p>

## Hardware
A nivel de hardware se implementaron las siguientes etapas:

- Fuente de alimentación lineal 12VDC
- Regulador 5V para módulo WiFi y amplificador
- Preamplificador con ecualizador a 3 bandas
- Amplificador de audio clase D PAM8403 de 3W
- Vúmetro de LEDs multiplexado con LM3914
- Circuito de medición de temperatura con termistor NTC
- Transistores para control de cargas
- ESP8266 NodeMCU con WiFi

## DIAGRAMA DEL CIRCUITO FINAL DEL PROYECTO
<p align="center">
  <img src="https://github.com/strix07/Amplificador-Audio-ESP8266-Control-Inalambrico/assets/142692042/d08258ff-8cac-444f-83eb-615382e4db8b" alt="Diagrama del circuito final del proyecto">
</p>

## Software
El firmware se desarrolló en la plataforma Arduino IDE, con las siguientes características:

- Conexión a red WiFi
- Cliente MQTT para comunicación con broker
- Recepción de comandos desde app móvil
- Control de salidas para activar/desactivar etapas
- Medición de temperatura con termistor
- Reproducción de audio de encendido desde memoria flash

## DIAGRAMA DE FLUJO DEL PROGRAMA FINAL

<p align="center">
  <img src="https://github.com/strix07/Amplificador-Audio-ESP8266-Control-Inalambrico/assets/142692042/60f8fccd-8a7f-4178-822a-60951a9a1aa5" alt="Diagrama de flujo del programa final">
</p>


## Resultados
El prototipo implementado cumple con los requerimientos funcionales planteados. Se demostró la factibilidad técnica de controlar de forma remota un sistema de audio empleando un microcontrolador WiFi programable.

El código fuente se encuentra documentado y disponible en este repositorio.

En el enlace de abajo se muestra el funcionamiento del prototipo final:
https://drive.google.com/file/d/16fFqcBq9rB1Fyi92lzwNm0zpvFEQhDhh/view?usp=drivesdk



