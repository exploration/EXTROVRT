# EXTROVRT
(EXplo Telepresence RObot Via the inteRneT)

![Glamour shot of the EXTROVRT robot](Images/EXTROVRT.JPG)

(more photos in the [Images](Images/) directory)


## What Is It?
EXTROVRT is a telepresence robot controlled via [Adafruit IO](http://adafruit.io)'s MQTT service, which uses [Adafruit Feather](https://www.adafruit.com/categories/777) stuff: the [Feather Huzzah ESP8266](https://www.adafruit.com/products/2821) and [Servo Featherwing](https://www.adafruit.com/products/2928) as the main control boards. We also have a [custom control interface](https://bitbucket.org/explo/extrovrt) written in Sinatra which lets us use the keyboard to drive instead of the mouse.

EXTROVRT came about because we have a [Double Robot](http://www.doublerobotics.com/) which was given to us, which we love and is a real hoot. However, it was going to require a brand-new iPad to upgrade. We figured that we could achieve 90% of the functionality of a Double for about 5% of the cost. We set the goal of a robot that could drive around using commodity servo hardware, and hold a phone, iPod touch, iPad, or the like. This would allow us to "turn our head" in virtual meetings, and even drive around a bit, probably mostly on tables.

We've achieved our goal! You should be able to make this robot for about $100, give or take.

This approach also has the benefit of allowing us to use whichever phone/tablet, and whichever video chat client (Skype, Google Hangouts, HipChat Video, etc) works best at the moment. We can also more easily test accessories like camera lenses or microphones.


## How Does It Work?
The way it works is this: You set up a "feed" on [Adafruit IO](http://adafruit.io) as such:

![Adafruit IO Feed Setup - Buttons with "F","B","L","R", and "S" values](Images/Adafruit_IO_Feed_Setup.png)

Each button is mapped to a simple value: `F` for `Forward`, `B` for `Back`, `R` for `Right`, `L` for `Left`, and `S` for `Stop`. Each button is set up to send `S` when it is let go:

![Adafruit IO Button Setup](Images/Adafruit_IO_Button_Setup.png)

The Huzzah ESP8266 board will connect to your wifi, then connect to your Adafruit IO feed using [MQTT](https://learn.adafruit.com/mqtt-adafruit-io-and-you/overview). It monitors the feed for changes in direction, and sends motor information to the servos through the servo board. In practice, this all happens very darned close to real-time: close enough to work great! Until you hit the Adafruit IO API limit :/ But for the price of free, who's complaining?


## How Do I Build It?
### Parts
You can get all of the parts for EXTROVRT at this [wishlist](https://www.adafruit.com/wishlists?wid=391765) here. I've included M4 nuts + bolts, although you may already have those or wish to use some sort of imperially-measured malarky.

You need two batteries/battery systems: a 3.3V LiPo to power the Huzzah! board, and something >5V to power the servo board. On the wishlist above, you'll see a 4xAA battery holder, and a 5V step-down UBEC to convert power to an even 5V / 3A. On my particular robot, I opted for a 2S (~7.3V) LiPo battery instead of the 4xAA holder, but that's only because I have those batteries (and chargers) lying about from other projects. If you have RC batteries, you may prefer to use those, but if you don't it's generally easier to stick with AA, because everybody has those.

The entire robot is built around [this anodyzed aluminum chassis](https://www.adafruit.com/products/2943), two continuous-rotation micro servos with matching wheels, and a swivel-wheel for the front. The parts that hold the phone/tablet are all 3d-printed and bolted on.

### 3D Printing
The CAD sources for the 3D-printed files are publicly available at [OnShape](https://cad.onshape.com/documents/d73cca1ae031f4b278264cff/w/0abe2b13ef53c6019a28a62a), which is handy if you want to fiddle with the models for your own setup.

I've also included the STL models and the [Simplify3D](https://www.simplify3d.com/) print setup files for a [PrintrBot Simple Metal](http://printrbot.com/shop/assembled-simple-metal/) in the `Models` and `Print Setup` folders, if you've got the same setup as me or use Simplify3D and want a starting point. I was able to print the entire thing within one 150mm square build plate.

I also had some luck making my own 3d-printed [Hex Standoffs](https://cad.onshape.com/documents/a721d24ec3e00633c0a5e028/w/56f9fc784b3159de2bad2fdf/e/3c67c912763ba927c46253c3), which is one fewer part you have to purchase if you have your own printer. Yes, I used imperial bolts (4/40) for that particular one, because I ran out of nice metric ones. But they should work with M3 bolts as well. And once again, it's OnShape, so it's easy to go + change dimensions and make your own - the model is fully parametric. You should be able to print them sideways on your build plate - it worked fine for me with no supports.

### Arduino
The Arduino files are in the `Arduino` folder. You just have to modify them to match your WiFi network SSID/password, and your particular Adafruit IO feed information. If you don't know how to upload Arduino code to the board, [this tutorial](https://learn.adafruit.com/adafruit-feather-huzzah-esp8266/overview) is exceedingly helpful.
