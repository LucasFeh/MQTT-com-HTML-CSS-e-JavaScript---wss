
function startConnect(){
    const clientId = document.getElementById("client_id").value;
    host = document.getElementById("host").value;
    port = document.getElementById("port").value;
    username = document.getElementById("username").value;
    password = document.getElementById("password").value;
    const messages = document.getElementById("messages");


    if (messages) {
        messages.innerHTML += "<span> Connecting to: " + host + ":" + port + "</span><br>";
        messages.innerHTML += "<span> Using the client Id: " + clientId + "</span><br>";
    } else {
        console.error('Elemento com ID "messages" não encontrado.');
    }

    var client = mqtt.connect('wss://broker.hivemq.com:8884/mqtt')

    client.on("message", function (topic, payload) {
        alert([topic, payload].join(": "))
        client.end()
    })
    client.subscribe('LED');
    client.publish( 'LED' , "1")
    
    // client.onConnectionLost = onConnectionLost;
    // client.onMessageArrived = onMessageArrived;

    // client.connect({onSuccess: onConnect});
}

// function onConnect(){
//     topic = document.getElementById("topic").value;
//     messages.innerHTML += "<span> Subscibing to Topic" + topic + "</span><br>";
// }

// function onConnectionLost(){
//     messages.innerHTML += "<span> Error: Conexão perdida </span><br>";
//     if(responseObj.errorCode !== 0){
//         messages.innerHTML += "<span> Error: " + responseObj.errorMessage + "</span><br>";
//         client.subscribe(topic);
//     }
// }

// function onMessageArrived(message){
//     console.log("onMessageArrived: " + message.payloadString);
//     messages.innerHTML += "<span> Topic: " + message.destinationName + "| Message: " + message.payloadString +"</span><br>";

// }
// function startDisconnect(){

// }

// function publishMessage(){

    
// }