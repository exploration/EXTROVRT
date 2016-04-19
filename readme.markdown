# EXTROVRT
([EXplo](http://www.explo.org) Telepresence RObot Via the inteRneT)

![Glamour shot of the EXTROVRT robot](Images/EXTROVRT.JPG)

(more photos in the [Images](Images/) directory)


## What Is It?
EXTROVRT is a telepresence robot controlled via [Adafruit IO](http://adafruit.io)'s MQTT service, which uses [Adafruit Feather](https://www.adafruit.com/categories/777) stuff: the [Feather Huzzah ESP8266](https://www.adafruit.com/products/2821) and [Servo Featherwing](https://www.adafruit.com/products/2928) as the main control boards. We also have a [custom control interface](https://github.com/exploration/EXTROVRT-Control) written in Sinatra which lets us use the keyboard to drive instead of the mouse.

EXTROVRT came about because we have a [Double Robot](http://www.doublerobotics.com/) which was given to us, which we love and is a real hoot. However, it was going to require a brand-new iPad to upgrade. We figured that we could achieve 90% of the functionality of a Double for about 5% of the cost. We set the goal of a robot that could drive around using commodity servo hardware, and hold a phone, iPod touch, iPad, or the like. This would allow us to "turn our head" in virtual meetings, and even drive around a bit, probably mostly on tables.

We've achieved our goal! You should be able to make this robot for about $100, give or take.

This approach also has the benefit of allowing us to use whichever phone/tablet, and whichever video chat client (Skype, Google Hangouts, HipChat Video, etc) works best at the moment. We can also more easily test accessories like camera lenses or microphones.


## How Does It Work?
**Note:** If you want multiple EXTROVRTs, you might want to check out our [control software](https://github.com/exploration/EXTROVRT-Control) for more detailed setup instructions to make that work.

If you just want one robot, you can pretty easily set up the Adafruit IO interface. The way that works is this: You [set up a feed](https://learn.adafruit.com/adafruit-io-basics-feeds/overview) on [Adafruit IO](http://adafruit.io) as such:

![Adafruit IO Feed Setup - Buttons with "F","B","L","R", and "S" values](Images/Adafruit_IO_Feed_Setup.png)

Each button then gets mapped to a simple value: `F` for `Forward`, `B` for `Back`, `R` for `Right`, `L` for `Left`, and `S` for `Stop`. Each button is set up to send `S` when it is let go:

![Adafruit IO Button Setup](Images/Adafruit_IO_Button_Setup.png)

For your initial setup, you may want to tweak the motor parameters so that max/min speed correspond to the "threshold" points of your servo motors. In that case, you should put in a "text" block so you can send the `MINUP`,`MINDOWN`,`MAXUP`,`MAXDOWN`,`STOPUP`, and `STOPDOWN` commands to the device. Make sure to record the motor values that work for you and hard-code them into the sketch so you won't have to adjust them every time you reset the device!

The Huzzah ESP8266 board will connect to your wifi, then connect to your Adafruit IO feed using [MQTT](https://learn.adafruit.com/mqtt-adafruit-io-and-you/overview). It monitors the feed for changes in direction, and sends motor information to the servos through the servo board. In practice, this all happens very darned close to real-time: close enough to work great! Until you hit the Adafruit IO API limit :/ But for the price of free, who's complaining?


## How Do I Build It?
### Parts + Assembly
You can get all of the parts for EXTROVRT at this [wishlist](https://www.adafruit.com/wishlists?wid=391765) here.

Physically, the entire robot is built around [this anodyzed aluminum chassis](https://www.adafruit.com/products/2943), two continuous-rotation micro servos with matching wheels, and a swivel-wheel for the front. The parts that hold the phone/tablet are all 3d-printed and bolted on (see below for details).


### Power
You need a battery source that's 5V and capable of handling something in the range of 500mA of current at 5V - 2.5 watts of power. The easiest way to accomplish this is to use a simple [USB Battery Pack](https://www.adafruit.com/products/1959) such as you might use for charging a phone, plugged right into the Feather Huzzah! You'll need to connect the `USB` and `GND` pins to the `+` and `-` screw terminals from the Adafruit Servo Wing to forward the power from USB to the servos (see power draw notes below).

You could alternately use an RC LiPo battery, or a simple 4xAA battery holder - anything is fine, but for this to work you'll have to connect a [UBEC](https://www.adafruit.com/products/1385) (buck converter) to the `USB` and `GND` pins on the Adafruit Feather, as well as the `+` and `-` pins from the Adafruit Servo Wing (the screw terminals) to get a stable 5 volt power source for the devices. Then connect your battery configuration to the other end of the UBEC.

(As an aside about power draw, my testing showed that the Huzzah draws about a continuous 0.05A or 50mA while idle, and the servos both running (normal speed) draws about 0.35A or 350mA. I imagine even if you maxed out the speed of these mini-servos, you'd still be within the 500mA spec of USB, let alone the spec of what a 7.2V 2S RC LiPo battery could attain.)

On my particular robot, I opted for a 2S (~7.3V) LiPo battery instead of the 4xAA holder, but that's only because I have those batteries (and and UBEC, and chargers) lying about from other projects. If you have RC batteries, you may prefer to use those, but most people will probably be happiest with just using a USB battery pack, since those are the most re-usable for other purposes when you're not EXTROVRT'ing.


### Wiring
Electronics-wise, you plug the servo "wing" on top of the Feather Huzzah! As noted above, you want to connect the `USB` and `GND` pins on the Adafruit Feather to the `+` and `-` pins from the Adafruit Servo Wing (the screw terminals) (and also to the 5V output of the UBEC if you're using one). On my devices, I install a power switch between the UBEC and the electronics so that I don't have to unplug any cables to turn the thing on + off.


### Arduino
The Arduino files are in the `Arduino` folder of this repository. You simply have to modify them to match your WiFi network SSID/password, and your particular Adafruit IO feed information. If you don't know how to upload Arduino code to the board, [this tutorial](https://learn.adafruit.com/adafruit-feather-huzzah-esp8266/overview) is exceedingly helpful.


### 3D Printing
Everything that's orange in the pictures is a 3d-printed thing. You'll need to print your own copy of those. You want two of the "arms", one top plate, and one base.

The CAD sources for the 3D-printed files are publicly available at [OnShape](https://cad.onshape.com/documents/d73cca1ae031f4b278264cff/w/0abe2b13ef53c6019a28a62a), which is handy if you want to fiddle with the models for your own setup.

I've also included the STL models and the [Simplify3D](https://www.simplify3d.com/) print setup files for a [PrintrBot Simple Metal](http://printrbot.com/shop/assembled-simple-metal/) in the `Models` and `Print Setup` folders, if you've got the same setup as me or use Simplify3D and want a starting point. I was able to print the entire thing within one 150mm square build plate.

I also had some luck making my own 3d-printed [Hex Standoffs](https://cad.onshape.com/documents/a721d24ec3e00633c0a5e028/w/56f9fc784b3159de2bad2fdf/e/3c67c912763ba927c46253c3), which is one fewer part you have to purchase if you have your own printer. Yes, I used imperial bolts (4/40) for that particular one, because I ran out of nice metric ones. But they should work with M3 bolts as well. And once again, it's OnShape, so it's easy to go + change dimensions and make your own - the model is fully parametric. You should be able to print them sideways on your build plate - it worked fine for me with no supports.
