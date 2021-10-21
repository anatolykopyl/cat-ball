# Cat Ball

The code I wrote to make a cat toy that will come to life when touched or 
can be controlled remotely via Wi-Fi.

I found that my cat loses interest in a toy as soon as he sees it not 
reacting to him catching it.

The idea is that a relatively strong motor with some mass attached to it's 
axis off center is glued to a spherical housing along with all the needed 
electronics. The violent vibration made by turning that motor on when 
something touches the ball makes this an interesting toy for a cat.


### Components:
- NodeMCU
- SW-18010P vibration sensor
- DC motor
- MX1508 DC motor driver (depends on what motor you'll use)
- battery

## Assembly
I glued the biggest coin (a nut would be fine too) I could find off axis to 
the motor and hooked the motor to the nodeMCU through the motor driver (pin 1).
And the vibration sensor to pin 3. The same battery powers both the motor and 
the nodeMCU.

Flash the .ino and now you can connect to the toy via Wi-Fi.

```
SSID: Cat Ball
KEY: churchill
```
Navigate to `192.168.4.1` when connected.
