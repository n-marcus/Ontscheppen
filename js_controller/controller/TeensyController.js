class TeensyController {
  constructor(x, y, i) {
    this.numSols = 16;
    this.solSides = 2;
    this.x = x;
    this.y = y;
    this.circleSize = width / (numTeensys * 4) ;
    this.circleInterval = TWO_PI / this.numSols;
    this.teensyBoxW = this.circleSize * 0.75;
    this.teensyBoxH = this.circleSize * 0.5;

    this.solenoids = [];
    this.index = i;

    this.bgColorOrignal = color(120)
    this.bgColor = this.bgColorOrignal;
    this.bgColorFade = 1;

    let circleIndex = 0;
    for(let i = 0; i < this.numSols; i ++){
      this.solenoids[circleIndex] = new Solenoid(0 ,0 , this.circleSize * 0.2, circleIndex);
      circleIndex ++;
    }

    this.resize();
    console.log("I am a Teensy Controller " + this.index);
  }

  resize() {
    // console.log("resizing teensy " + this.index);
    this.circleSize = width / (numTeensys * 4) ;
    this.circleInterval = TWO_PI / this.numSols;
    this.teensyBoxW = this.circleSize * 0.75;
    this.teensyBoxH = this.circleSize * 0.5;

    let circleIndex = 0;
    for(let a = 0; a < TWO_PI; a += this.circleInterval){
      this.solenoids[circleIndex].x = this.x + this.circleSize * cos(a);
      this.solenoids[circleIndex].y = this.y + this.circleSize * sin(a);
      this.solenoids[circleIndex].size = this.circleSize * 0.2;
      circleIndex ++;
    }
  }

  draw() {
    this.drawSolenoids();

    if (this.bgColorFade < 1) {
      this.bgColorFade += 0.01;
      this.bgColor = lerpColor(this.bgColor, this.bgColorOrignal, this.bgColorFade);
    }
    fill(this.bgColor);
    stroke(50);
    strokeWeight(5);

    rect(this.x,this.y, this.teensyBoxW, this.teensyBoxH, 10, 10);

    fill(0);
    textAlign(CENTER,CENTER);
    textSize(10);
    noStroke();
    text("Teensy " + (this.index + 1), this.x, this.y);
  }

  fire(sol) {
    this.solenoids[sol].fire();
  }

  update() {

  }

  drawSolenoids() {
    for (let i = 0; i < this.numSols; i ++) {
      stroke(0);
      strokeWeight(2);
      line(this.x, this.y, this.solenoids[i].x, this.solenoids[i].y);
      this.solenoids[i].draw();
    }
  }

  checkClick() {
    if (mouseX > this.x - (this.teensyBoxW / 2) && mouseX < this.x + (this.teensyBoxW / 2) && mouseY > this.y - (this.teensyBoxH / 2) && mouseY < this.y + (this.teensyBoxH / 2)) {
      this.click();
      console.log("clicked!");
    }
    for (let i = 0; i < this.numSols; i ++) {
      this.solenoids[i].checkClick();
    }
  }

  checkHover() {
    for (let i = 0; i < this.numSols; i ++) {
      this.solenoids[i].checkHover();
    }
  }

  click() {
    this.bgColor = color(0,0,80)
    this.bgColorFade = 0;
    this.fireAll();
  }

  fireAll() {
    for (let i = 0; i < this.numSols; i ++) {
      this.solenoids[i].fire(true);
    }
  }
}
