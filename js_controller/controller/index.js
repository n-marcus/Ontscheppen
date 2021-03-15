
let teensys = [];
let connect;
let send;

numTeensys = 2;

function setup()  {
  colorMode(HSB);
  rectMode(CENTER);
  createCanvas(window.innerWidth, window.innerHeight);
  background(255);
  for (let i = 0; i < numTeensys; i ++) {
    teensys[i] = new TeensyController((width / (numTeensys + 1)) * (i + 1), height / 2, i);
  }


  connect = new Connect();
  connect.connectToServer(function() {
    server = new Server();
    server.startServer(9000);
    server.getMessage(function(address,msg) {
      oscReceiver(address,msg);
    });
  });

  //maak een connect-object aan dat zorgt voor de communicatie met oscServer.js
  send = new Connect();

  //maak verbinding met oscServer.js, en voer code tussen {} uit zodra deze verbinding tot stand is gekomen.
  send.connectToServer(function() {
    //maak een nieuwe client-object aan.
    client = new Client();

    //start de client en laat deze berichten sturen naar het ip-adres 127.0.0.1 en poort 9000
    client.startClient("169.254.143.3",8888);
  });

}

function draw() {
  background(240);
  for (let i = 0; i < numTeensys; i ++) {
    teensys[i].draw();
  }
}

function windowResized() {
  resizeCanvas(windowWidth, windowHeight);
  for (let i = 0; i < numTeensys; i ++) {
    teensys[i].x = (width / (numTeensys + 1)) * (i + 1);
    teensys[i].y = height / 2;
    teensys[i].resize();
  }
}

function mouseMoved() {
  for (let i = 0; i < numTeensys; i ++) {
    // teensys[i].checkClick();
    teensys[i].checkHover();
  }
}

function oscReceiver(addr, msg) {
  console.log("osc message " + addr + " " + msg);
  teensys[0].fire(msg[0] % teensys[0].numSols, false);
}

function keyPressed() {
  console.log("pressed " + key);
  if (key == "Enter") {
    console.log("Pressed enter");
    sendIP();
  }
}

function sendIP() {
  // client.send("/hostIP",
}
