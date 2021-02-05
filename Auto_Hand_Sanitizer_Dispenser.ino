#include <Stepper.h>
#define STEPS 2038 // the number of steps in one revolution of your motor (28BYJ-48)
Stepper stepper(STEPS, 8, 10, 9, 11);
int trig = 7;//digital pin for the trigger on the ultrasonic sensor, the one that sends out the signal
int echo = 6; //digital pin for echo on the ultrasonic sensor that receives the signal from the trigger pin of the ultra sonic sensor
float duration, distance; //duration is how long it takes for the trigger signal to bounce off object and return to the echo receiver of the ultrasonic sensor
//we will use duration to calculate the distance. these are float values beacause we want more precise values
int initFlag = 0;//When the ultrasonic sensor first starts up, it needs some callibration time -- as in the distance reading may be wrong the first few executions

void setup() {
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(trig, LOW);//making sure that the trigger pin is low (has not sent a signal)
  delayMicroseconds(2);//waiting 2 microseconds to give time for the ultrasonic sensore to transition to a low
  digitalWrite(trig, HIGH);//writing a HIGH causes the trigger pin to send an ultrasonic wave out to be bounced off of an object a certain distance away
  delayMicroseconds(10);//keep it HIGH for 10 microsenconds to make sure the signal properly propagates
  digitalWrite(trig, LOW);//turn off the ultrasonic emitter

  duration = pulseIn(echo, HIGH); //duration is measured in microseconds
  /* pulseIn listens in on the echo pin number, waiting for it to turn HIGH. when it turns high it activates a timer that goes until 
   * the echo pin becomes a LOW again. The echo pin goes HIGH when the trigger pin goes LOW, and the echo pin only goes LOW again until the trigger wave is received 
   * by the echo pin. Basically PulseIn measures the time it takes for the echo pin to receive the reflected wave of the trigger pin, and loads it into duration
   */
  distance = (duration*.0343)/2; //time * speed = distance. the speed of sound is 0.0343 cm/microsecond. We divide ny 2 because the signal bounces off and travels back

  if(distance < 10 && initFlag == 1){//the initFlag must be a 1 in order for this loop to be accessed. Line 38 sets that condition
    Serial.print("Dispense!");//these two lines are for debugging purposes only, it lets us know if we have entered this loop!
    Serial.print('\n');
    dispense();//this is a function we defined just outside of the loop, it activates the servos to dispense hand sanitizer
  }
  
  distanceLog();//this is a function we defined just outside of the loop. It's for debugging only, it gives us the ultrasonic sensor's distance readings
  
  while(initFlag == 0){//we start off with the flag being set to 0, so that the 1st time this code executes it won't falsely dispense
    delay(4000);//we will wait 4 seconds, which was chosen arbitrarily 
    initFlag = 1; //the flag is set! now this while loop will be ignored and the dispensor will trigger
  }
}
void dispense(){
  stepper.setSpeed(6); // 6 rpm
  stepper.step(6114); // do 6114 steps
  delay(1000); // wait for one second
  stepper.setSpeed(6); // 6 rpm
  stepper.step(-6114); // do 6114 steps in the other direction with faster speed 
}
void distanceLog(){//this function prints out the current distance of the ultrasonic sensor!
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm");
  Serial.print('\t');
  Serial.print('\n'); 
}
//Justin Francisco
/***********************Acknowledgements**************************************
 * Stepper motor code was borrowed from Helen at: https://www.seeedstudio.com/blog/2019/03/04/driving-a-28byj-48-stepper-motor-with-a-uln2003-driver-board-and-arduino/
 * Ultrasonic sensor code borrowed from Isaac at: https://create.arduino.cc/projecthub/Isaac100/getting-started-with-the-hc-sr04-ultrasonic-sensor-036380
 */
