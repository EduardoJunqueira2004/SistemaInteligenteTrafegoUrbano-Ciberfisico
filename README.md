# SistemaInteligenteTrafegoUrbano-Ciberfisico
Sistema Inteligente de Monitorização e Análise de Tráfego Urbano.

Descrição: 

 

O Sistema de Monitorização de Tráfego Urbano é uma solução inovadora projetada para monitorizar e analisar o fluxo de veículos em tempo real, contribuindo para a gestão e otimização do tráfego nas cidades.  

Este sistema no nosso miniprojecto está conectado a uma plataforma de comunicação (sistema ciber físico), onde os dados são armazenados e processados. Este sistema vai estar implementado no MQTT Broker onde temos o servidor que vai recolher dados do ESP32 e aplicação mobile (que vai se criada) que é o cliente, posteriormente vai receber os dados do servidor (ESP32) e mandar dados para o servidor. 

 Para além da monitorização e análise, o sistema possui um componente físico, um atuador semáforo de três luzes (vermelho, amarelo e verde), que simula a gestão de tráfego em cruzamentos ou estradas movimentadas. Este semáforo é controlado com base nas informações recolhidas pelos sensores e nos padrões de tráfego analisados. Neste caso temos um só sensor que é o sensor de touch. 

Adicionalmente, a simulação de tráfego de veículos utiliza uma ferramenta digital ou software no PC, e a deteção de peões é realizada através um componente físico um sensor de touch que atua como um botão de chamada para passagem de pessoas. (este botão vai ser também implementado numa aplicação móvel. 

Desta forma, com base nas informações analisadas, o sistema pode emitir alertas automáticos para os controladores de tráfego sobre possíveis congestionamentos, acidentes ou outras anomalias.  

 

Requisitos funcionais da aplicação: 

Integração de sensor e atuador: Sensor de touch e semáforo(físicos). 

Comunicação Cliente/Servidor: Criação de um botão na aplicação móvel para simular o acionamento remoto do sensor de toque, garantindo a comunicação via Bluetooth. 

Visualização de dados: Visualização de dados na IDE do Arduíno ESP32. -->Servidor. 
