//VARIAVEIS GLOBAIS
String broker = "broker.emqx.io"
int port = 1883;
String clientIdentifier = "android"
String topic = "AmbInt/sensor/luz"

mqtt.MqttClient client;
mqtt.MqttConnectionState connectionState;

int valor_luz = 1;

StreamSubscription subscription;


//DENTRO DO initState()
void initState() {
  super.initState();
  WidgetsBinding.instance.addPostFrameCallback(  (_) => _connect()  );
}

//FUNCAO _connect()
void _connect() async {
  client = mqtt.MqttClient(broker, '');
  client.port = port;
  client.keepAlivePeriod = 30;
  client.onDisconnected = _onDisconnected;

  final mqtt.MqttConnectMessage connMess = mqtt.MqttConnectMessage()
    .withClientIdentifier(clientIdentifier)
    .startClean()
    .keepAliveFor(30)
    .withWillQos(mqtt.MqttQos.atMostOnce);
  print("Client connecting...");
  client.connectionMessage = connMess;

  try {
    await client.connect();
  } catch (e) {
    print(e);
    _disconnected();
  }

  if (client.connectionState == mqtt.MqttConnectionState.connected) {
    print("Client connected");
    setState((){
      connectionState = client.connectionState;
    });
  } else{
    print("connection failed");
    _disconnected();
  }

  subscription = client.updates.listen(_onMessage);
  _subscribeToTopic(topic);  //TODO adicionar mais quantos topics houver
}

  void _disconnect() {
    print("Disconnect");
    client.disconnect();
    _onDisconnected();
  }

  void _onDisconnected() {
    print("Disconnected");
    setState(() {
      connectionState = client.connectionState;
      client = null;
      subscription.cancel();
      subscription = null;
    });
  }

  void _onMessage(List<mqtt.MqttReceivedMessage> event) {
    print(event.length);
    final mqtt.MqttPublishMessage recMess = event[0].payload as mqtt.MqttPublishMessage;
    final String message = mqtt.MqttPublishPayload.bytesToStringAsString(recMess.payload.message);
    currentTopic = event[0].topic;
    setState(() {
      valor_luz = double.parse(message); //TODO verificar o q foi a mensagem e alterar o valor certo
    });
  }

  void _publish(String topic, String message) {
    // publishMessage(String topic, MqttQos qualityOfService, Uint8Buffer data, {bool retain = false}) → int 
    //
    message_data = //message to Uint8Buffer
    client.publishMessage(topic, mqtt.MqttQos.atMostOnce, message_data);
  }