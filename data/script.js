
  clientId = document.getElementById("client_id").value;
  host = document.getElementById("host").value;
  port = document.getElementById("port").value;
  username = document.getElementById("username").value;
  password = document.getElementById("password").value;
  topic = document.getElementById("topic_s").value;
  btnpublish = document.getElementById("btnpublish");
  const messages = document.getElementById("messages");
  const randomClientId = 'client_ID - ' + Math.random().toString(36).substring(7);
  let client;


function startConnect(){

    host = host ? host : "broker.hivemq.com";
    port = port ? port : 8884;
    clientId = clientId ? clientId : randomClientId;

    if (messages) {
        messages.innerHTML += "<span> Conectando em: " + host + ":" + port + "</span><br>";
        messages.innerHTML += "<span> Cliente ID: " + clientId + "</span><br>";
    } else {
        console.error('Elemento com ID "messages" não encontrado.');
    }

    //client = mqtt.connect(`wss:${host}:${port}/mqtt`)
    client = new Paho.MQTT.Client(host, Number(port), clientId); // parte que faz a conexão com o broker, importante se ater ao sufixo: wss: e prefixo: /mqtt 
    // wss se refere a conexão do tipo websocket seguro, e /mqtt é o caminho para a conexão com o broker
    
    var options = {
        onSuccess: onConnect,
        onFailure: ()=>{
            messages.innerHTML += "<span style='color: red;'> Tentando se conectar... </span><br>";
        }
    };
    
    client.connect(options);
    
    function onConnect() {
        console.log("Connected to broker");
        messages.innerHTML += "<span style='color: green;'> Conectado com Sucesso!! </span><br>";
        client.subscribe("Mensagens");
        message = new Paho.MQTT.Message("Conectadinho");
        message.destinationName = "Mensagens";
        client.send(message);
        
        if (topic !== "") {
            client.subscribe(topic);
            messages.innerHTML += "<span style = 'color: green;'> Inscrito no Topic " + topic + "</span><br>";
        } else {
            messages.innerHTML += "<span style ='color: red'; > Topic vazio!! </span><br>";
        }
    }

    
    function onMessageArrived(message){
        // console.log([topic, payload].join(": "));
        messages.innerHTML += "<span> Topic: " +  message.destinationName + " | Message: " + message.payloadString + "</span><br>";
        
    }
    
    client.onMessageArrived = onMessageArrived;
    
}

function publishMessage() {

    topicp = document.getElementById("topic_p").value;
    sendMessage = document.getElementById("message").value;

   message = new Paho.MQTT.Message(sendMessage);
   message.destinationName = topicp;
   client.send(message);
}