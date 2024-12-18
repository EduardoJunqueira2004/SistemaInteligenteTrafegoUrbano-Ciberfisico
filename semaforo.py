from flask import Flask, request, jsonify

app = Flask(__name__)

# Variável global para armazenar o comando do semáforo
traffic_light_command = "OFF"

# Rota para receber dados do ESP32 via POST
@app.route('/trafficlight', methods=['POST'])
def receive_data():
    global traffic_light_command

    # Obter os dados enviados pelo ESP32
    data = request.json
    if data and 'state' in data:
        state = data['state']
        print(f"Estado recebido do ESP32: {state}")

        # Resposta de sucesso
        return jsonify({"message": "Estado recebido com sucesso", "received_state": state}), 200
    else:
        return jsonify({"error": "Dados inválidos"}), 400

# Rota para enviar comandos ao ESP32 via GET
@app.route('/trafficlight', methods=['GET'])
def send_command():
    global traffic_light_command

    # Retornar o comando atual do semáforo
    print(f"Comando enviado ao ESP32: {traffic_light_command}")
    return jsonify({"command": traffic_light_command}), 200

# Rota para atualizar o comando do semáforo (opcional, útil para teste manual)
@app.route('/setcommand', methods=['POST'])
def set_command():
    global traffic_light_command

    data = request.json
    if data and 'command' in data:
        traffic_light_command = data['command']
        print(f"Novo comando definido: {traffic_light_command}")
        return jsonify({"message": "Comando atualizado com sucesso", "new_command": traffic_light_command}), 200
    else:
        return jsonify({"error": "Dados inválidos"}), 400

if __name__ == '__main__':
    # Iniciar o servidor Flask com o endereço IP especificado
    app.run(host='172.16.192.157', port=3000, debug=True)
