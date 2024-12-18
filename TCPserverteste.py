import socket  # Importa a biblioteca de sockets para comunicação de rede

ID_KIT = 8
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  # Cria o socket TCP/IP
host = "192.168.1.101" #ifconfig do meu pc
port = 8098  # Porta do servidor
server.bind((host, port))  # Associa o endereço IP e a porta ao socket
server.listen(1)  # Aguarda por conexões

print(f"Servidor a escutar no IP {host}, porta {port}")
while True:
    # Aceita a conexão do cliente
    clientsocket, addr = server.accept()
    print("---------------------------------------------------------")
    print("Servidor com endereço", clientsocket.getsockname())
    print("Cliente com endereço", clientsocket.getpeername())

    # Recebe a mensagem do cliente
    msg = clientsocket.recv(100).decode('ascii')
    print("Mensagem recebida:", msg)
    print("Recebido de", addr)

    # Inicializa variáveis para armazenar os valores de cada atributo
    ID = None
    VAR = None
    TEMP = None

    # Dividir a mensagem em pares de atributo/valor usando '&' como delimitador
    pares = msg.split('&')

    # Itera sobre os pares e divide-os em atributo e valor
    for par in pares:
        if '=' in par:
            atributo, valor = par.split('=', 1)  # Divide em atributo e valor

            # Atribui o valor às variáveis correspondentes
            if atributo == "ID":
                ID = valor
            elif atributo == "VAR":
                VAR = valor
            elif atributo == "TEMP":
                TEMP = valor

    # Imprime a interpretação dos frames (atributos e valores)
    if ID:
        print(f"ID recebido: {ID}")
    if VAR:
        print(f"VAR recebido: {VAR}")
    if TEMP:
        print(f"TEMP recebido: {TEMP}")

    print("---------------------------------------------------------")

    # Envia uma resposta para o cliente
    clientsocket.send(bytes("MSG:RX:OK", 'ascii'))

    # Fecha a conexão
    clientsocket.close()
