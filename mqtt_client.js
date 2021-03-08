// // npm install mqtt --save
// // npm install mqtt -g

var mqtt = require('mqtt');

options = {
    clientId:"mqttjs01",
    username:"steve",
    password:"password",
    clean:true
};

var client = mqtt.connect("mqtt://broker.emqx.io");
// var client = mqtt.connect("broker.emqx.io", options);


client.on('message', function(topic, message, packet) {
    console.log("message is " + message);
    console.log("topic is " + topic);
});


client.on("connect", function() {
    console.log("connected");
});


var topic_o = {"AmbInt/sensors/pot_led":1,"AmbInt/sensors/temp_led":1,"AmbInt/sensors/int_led":1};
client.subscribe(topico_o);



//handle errors
client.on("error",function(error){
    console.log("Can't connect" + error);
    process.exit(1)
});


client.end();
















// var mqtt    = require('mqtt');
// var count =0;
// var client  = mqtt.connect("mqtt://broker.emqx.io",{clientId:"mqttjs01"});
// console.log("connected flag  " + client.connected);

// //handle incoming messages
// client.on('message',function(topic, message, packet){
// 	console.log("message is "+ message);
// 	console.log("topic is "+ topic);
// });


// client.on("connect",function(){	
// console.log("connected  "+ client.connected);

// })
// //handle errors
// client.on("error",function(error){
// console.log("Can't connect" + error);
// process.exit(1)});
// //publish
// function publish(topic,msg,options){
// console.log("publishing",msg);

// if (client.connected == true){
	
// client.publish(topic,msg,options);

// }
// count+=1;
// if (count==2) //ens script
// 	clearTimeout(timer_id); //stop timer
// 	client.end();	
// }

// //////////////

// var options={
// retain:true,
// qos:1};
// var topic="testtopic";
// var message="test message";
// var topic_list=["topic2","topic3","topic4"];
// var topic_o={"topic22":0,"topic33":1,"topic44":1};
// console.log("subscribing to topics");
// client.subscribe(topic,{qos:1}); //single topic
// client.subscribe(topic_list,{qos:1}); //topic list
// client.subscribe(topic_o); //object
// var timer_id=setInterval(function(){publish(topic,message,options);},5000);
// //notice this is printed even before we connect
// console.log("end of script");