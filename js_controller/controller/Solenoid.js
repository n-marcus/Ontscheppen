
class Solenoid {
  constructor(x,y,size,i) {
    this.x = x;
    this.y = y;
    this.originalColor = color(200,20,255);
    this.color = this.originalColor;
    this.originalColorFade = 1;
    this.index = i;
    this.size = size;
    this.hover = false;
  }

  draw() {
    if (this.originalColorFade < 1) {
      this.originalColorFade += 0.05;
      this.color = lerpColor(this.color, this.originalColor, this.originalColorFade);
    }

    if (!this.hover) {
       noStroke()
     } else {
       stroke(0);
       strokeWeight(2);
     }
    fill(this.color);

    ellipse(this.x, this.y, this.size)
    //
    fill(0);
    textSize(18);
    text(this.index, this.x, this.y);

    // this.drawDots();
  }

  checkClick() {
    if (dist(mouseX, mouseY, this.x, this.y) < this.size) {
      this.fire(true);
    }
  }

  checkHover() {
    if (dist(mouseX, mouseY, this.x, this.y) < this.size) {
      if (this.hover == false) {
        this.fire(true);
        this.hover = true;
      }
    } else {
      this.hover = false;
    }
  }

  drawDots() {
    this.circleSize =  this.size / 3;
    this.circleInterval = TWO_PI / this.index;

    let circleIndex = 0;
    for(let a = 0; a < TWO_PI; a += this.circleInterval){
      let x = this.x + this.circleSize * cos(a);
      let y = this.y + this.circleSize * sin(a);
      fill(0);
      ellipse(x,y, 3);
      circleIndex ++;
    }
  }

  fire(sendOsc) {
    this.color = color(200, 255,255);
    this.originalColorFade = 0;
    if (sendOsc) {
      client.sendMessage("/solenoid", this.index, 50);
      console.log("sending osc message /solenoid " + this.index + " 50" );
    }
  }
}
