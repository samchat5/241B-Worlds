#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    Dial,           sensorPotentiometer)
#pragma config(Sensor, in8,    G,              sensorGyro)
#pragma config(Sensor, dgtl1,	 LauncherSwitch,  sensorTouch)
#pragma config(Sensor, dgtl2,	 LauncherSwitch2,  sensorTouch)
#pragma config(Sensor, dgtl3,  WallSensor,     sensorSONAR_inch)
#pragma config(Sensor, dgtl6,  HighLED,        sensorLEDtoVCC)
#pragma config(Sensor, dgtl7,  MidLED,         sensorLEDtoVCC)
#pragma config(Sensor, I2C_1,  LMS,            sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_2,  RMS,            sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port1,           Angler,          tmotorVex393_HBridge, openLoop, reversed)
#pragma config(Motor,  port2,           RightFrontBack,tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           LeftFrontBack, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           RightMiddle,   tmotorVex393_MC29, openLoop, reversed, encoderPort, I2C_2)
#pragma config(Motor,  port5,           LeftMiddle,    tmotorVex393_MC29, openLoop, encoderPort, I2C_1)
#pragma config(Motor,  port6,           Intake,        tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port7,           Launcher,             tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           Flipper,             tmotorVex393_MC29, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//in8 is a gyro
#pragma CompetitionControl(Competition)
#pragma autonomousDuration(15)
#pragma UserControlDuration(120)
#include "Vex_Competition_Includes.c"

//variables
int mode=3;//To choose autonomus mode
int TB; //launch button was pressed
int lor;//direction motors spin for turn function
int TL=1;//launcher loadable setting 0 off 1 on
int x=0;//for timing autonomus
bool Tedge=false;//this keeps track of if the TL toggle
bool autocock=true;
/////////
/*
int oldHeading=0;
int curHeading=0;
float deltaHeading=0;
float angVel=0;
*/
/*
int
button has been pressed and toggles TL when the
button is unpressed,heres how
if(vexRT[Btn]==1){Tedge=true;}
if(Tedge==true&&vexRT[Btn]==0){Tedge=false; What the button is soposed to do goes here}
*/

//button values on lcd
const short leftButton = 1;
const short centerButton = 2;
const short rightButton = 4;
//end variables

//bLCDBacklight = true;// Turn on LCD Backlight


//functions

//getPressed
void getPressed(){//wait untill button on lcd is pressed
	while(nLCDButtons == 0){wait1Msec(5);}
}//end getPressed

//getReleased
void getReleased(){
	while(nLCDButtons != 0){wait1Msec(5);}
}//end getReleased
//displaysensors
/*
void displayAngleStuff(){
	clearLCDLine(0);
	clearLCDLine(1);

	displayLCDPos(0,0);
	displayNextLCDString("Gyro: ");
	displayLCDPos(0,6);
	displayNextLCDNumber(SensorValue[G]);

	displayLCDPos(1,0);
	displayNextLCDString("AngVel: ");
	displayLCDPos(1,6);
	displayNextLCDNumber(angVel);
	*/
	/*
		switch(mode){
			//Increment/decrement mode when left/right lcd button press
			// for each mode if mode is 1 loop back around to 5
			case 1://Display first choice
				displayLCDCenteredString(0, "Display AngVel");
				displayLCDCenteredString(1, "<     Enter    >");
				getPressed();
				if(nLCDButtons == leftButton){
					getReleased();
					mode=2;
				}
				else if(nLCDButtons == rightButton){
					getReleased();
					mode++;
				}
				break;
			case 2://Display second choice
				displayLCDCenteredString(0, "Display Sensors");
				displayLCDCenteredString(1, "<     Enter    >");
				getPressed();
				if(nLCDButtons == leftButton){
					getReleased();
					mode--;
				}
				else if(nLCDButtons == rightButton){
				getReleased();
				mode=1;
				}
				break;
}
*/
void displaysensors(){
	clearLCDLine(0);
	clearLCDLine(1);

	displayLCDPos(0,0);
	displayNextLCDString("Gyro: ");
	displayLCDPos(0,6);
	displayNextLCDNumber(SensorValue[G]);

	displayLCDPos(1,0);
	displayNextLCDString("Wall: ");
	displayLCDPos(1,6);
	displayNextLCDNumber(SensorValue[WallSensor]);
}//end displaysensors
//turn
void turn(int angle,int re,int error){//angle is desired change in heading in 1/10ths of a degree
																			//re is number of times it checks and adjusts heading
																			//error is the range of ok error in desired heading in 1/10ths of a degree

	// BEFORE USE OF TURN WARNING
	//make sure before calling function setupgyro() has been run

	lor=1;//left or right will change the direction the wheels turn if turn is clockwise
	if(abs(angle)!=angle){lor=-1;}//if angle is negitive lor is negitive which reverses the direction of the motors

	angle=SensorValue[in8]+angle;//this changes the angle from wanted change in heading to desired resulting heading

	//While the gyro value is more than 10 degrees off from desired heading...
  while(SensorValue[in8] < angle - 100||SensorValue[in8] > angle + 100){//////////////quick inaccurate turn
  	displaysensors();
  	motor[RightFrontBack] = 127*lor;//r
		motor[LeftFrontBack] = -127*lor;//l
		motor[RightMiddle] = 127*lor;//r
		motor[LeftMiddle] = -127*lor;//l
  }//end while loop

  //Brief brake to eliminate some drift
 	displaysensors();
  motor[RightFrontBack] = -5*lor;//r
	motor[LeftFrontBack] = 5*lor;//l
	motor[RightMiddle] = -5*lor;//r
	motor[LeftMiddle] = 5*lor;//l
  wait1Msec(100);

  //Second while loop moves the robot goal at less power (this power should not be lowered to much or the robot will not be able to move)

  while(re>0){//each time it loops it should increase the acuracy of the turn however there is no gain after 5 beause it will just move back and forth
  	re=re-1;//decrese re by 1 to count the number of times recorrected
  	while(abs(SensorValue[in8]) > abs(angle)-error || abs(SensorValue[in8]) < abs(angle)+error){//if the sensore reading is more or less then wanted angle +or- error
  		displaysensors();
    	if(SensorValue[in8] > angle){//if heading is greator then desired heading decrease heading by turning right
  			motor[RightFrontBack] = -30;//r
  			motor[RightMiddle] = -30;
				motor[LeftFrontBack] = 30;//l
				motor[LeftMiddle] = 30;
    	}
    	if(SensorValue[in8] < angle){//if heading is less then desired heading increase heading by turning left
  			motor[RightFrontBack] = 30;//r
  			motor[RightMiddle] = 30;
				motor[LeftFrontBack] = -30;//l
				motor[LeftMiddle] = -30;
    	}
  	}
  	//Stop
		motor[RightFrontBack] = 0;//stop
		motor[RightMiddle] = 0;
		motor[LeftFrontBack] = 0;
		motor[LeftMiddle] = 0;
  	wait1Msec(150);
		}//end turn
	}//end turn function setup

//heading
void heading(int angle ,int re,int error){//angle is desired heading in 1/10ths of a degree, 0 = starting angle
																			//re is number of times it checks and adjusts heading, recomended 2
																			//error is the range of ok error in desired heading in 1/10ths of a degree, recomenderd 0
	// BEFORE USE OF HEADING WARNING
	//make sure before calling function setupgyro() has been run

	lor=1;//left or right will change the direction the wheels turn if turn is clockwise or not lor=1;cw lor=-1;ccw
	if(SensorValue[in8]>angle){lor=-1;}//if angle is negitive lor is negitive which reverses the direction of the motors

	//While the gyro value is more than 10 degrees off from desired heading...
	while(SensorValue[in8] < angle - 100||SensorValue[in8] > angle + 100){//////////////quick inaccurate turn
  	motor[RightFrontBack] = 127*lor;//r
		motor[LeftFrontBack] = -127*lor;//l
		motor[RightMiddle] = 127*lor;//r
		motor[LeftMiddle] = -127*lor;//l
  }//end while loop

  //Brief brake to eliminate some drift
  motor[RightFrontBack] = -5*lor;//r
	motor[LeftFrontBack] = 5*lor;//l
	motor[RightMiddle] = -5*lor;//r
	motor[LeftMiddle] = 5*lor;//l
  wait1Msec(100);

  //Second while loop moves the robot goal at less power (this power should not be lowered to much or the robot will not be able to move)

  while(re>0){//each time it loops it should increase the acuracy of the turn however there is no gain after 5 beause it will just move back and forth
  	re=re-1;//decrese re by 1 to count the number of times recorrected
  	while(abs(SensorValue[in8]) > abs(angle)-error || abs(SensorValue[in8]) < abs(angle)+error){//if the sensore reading is more or less then wanted angle +or- error
  	if(SensorValue[in8] > angle){//if heading is greator then desired heading decrease heading by turning right
  			motor[RightFrontBack] = -30;//r
  			motor[RightMiddle] = -30;
				motor[LeftFrontBack] = 30;//l
				motor[LeftMiddle] = 30;
    	}
    	if(SensorValue[in8] < angle){//if heading is less then desired heading increase heading by turning left
  			motor[RightFrontBack] = 30;//r
  			motor[RightMiddle] = 30;
				motor[LeftFrontBack] = -30;//l
				motor[LeftMiddle] = -30;
    	}
  	}
  	//Stop
		motor[RightFrontBack] = 0;//stop
		motor[RightMiddle] = 0;
		motor[LeftFrontBack] = 0;
		motor[LeftMiddle] = 0;
  	wait1Msec(150);
	}//end heading
}//end heading function setup
/*
void getDeltaHeading(){
		curHeading=SensorValue[in8];
		deltaHeading=curHeading-oldHeading;
		oldHeading=curHeading;
}
void getAngularVelocity(){
	getDeltaHeading();
	angVel=deltaHeading/time1[T2];
	clearTimer(T2);
}
*/
void gotowall(int dis,int inpower=0){// dis = distance from wall in inches,inpower=intake power
	while(SensorValue(WallSensor) > dis+18  || SensorValue(WallSensor)==-1){
		displaysensors();
	motor[RightFrontBack] = 127;//stop
	motor[RightMiddle] = 127;
	motor[LeftFrontBack] = 127;
	motor[LeftMiddle] = 127;
	motor[Intake] = inpower;
	}
	while(SensorValue(WallSensor) > dis  || SensorValue(WallSensor)==-1){		// Loop while robot's Ultrasonic sensor is further than d inches away from an object // || (or) it is '-1'.  (-1 is the value returned when nothing is in it's visable range)
		displaysensors();
	motor[RightFrontBack] = 30;//stop
	motor[RightMiddle] = 30;
	motor[LeftFrontBack] = 30;
	motor[LeftMiddle] = 30;
	motor[Intake] = inpower;
	}
	while(SensorValue(WallSensor) < dis && SensorValue(WallSensor)!=-1){//move backward to get the distance right after overshooting
		displaysensors();
	motor[RightFrontBack] = -25;//stop
	motor[RightMiddle] = -25;
	motor[LeftFrontBack] = -25;
	motor[LeftMiddle] = -25;
	motor[Intake] = inpower;
	}
	displaysensors();
	motor[RightFrontBack] = 0;//stop
	motor[RightMiddle] = 0;
	motor[LeftFrontBack] = 0;
	motor[LeftMiddle] = 0;
	motor[Intake] = 0;
}//end gotowall

void move(int dis,int inpower=0,int indist=dis){//dis is desired movement in inches, inpower sets power of intake 127 in & -127 out (defualts to 0 if not specified)
	//SensorValue[LM]=0;
	//SensorValue[RM]=0;
	if(abs(dis)==dis){lor=1;}else{lor=-1;}//is distance is negitive reverse motor direction and change dis to positive
	// highspeed motors 392 ticks per revolution of the output shaft.
	// http://help.robotc.net/Sandbox/Zendesk-Output/Content/Resources/topics/VEX_Cortex/ROBOTC/Motor_and_Servo/nMotorEncoder.htm

	int dl=SensorValue[LMS]+(dis/(3.14159265359*4))*392;//dis converted to encoder units + initial left encoder value
	int dlindist=SensorValue[LMS]+(indist/(3.14159265359*4))*392;
	//int dr=SensorValue[RMS]+(dis/(3.14159265359*4))*392; //dis converted to encoder units + initial right encoder value
	if(lor==1){
		while(SensorValue[LMS]<dl-500){//more then half a wheel to dis
			displaysensors();
			//move motors forward
			motor[RightFrontBack] = 127;motor[RightMiddle] = 127;
			motor[LeftFrontBack] = 127;motor[LeftMiddle] = 127;
			if(SensorValue[LMS]<dlindist){motor[Intake] = inpower;}else{motor[Intake]=0;}
		}//end while values arnt within 2*PI of dis
		//break
			motor[LeftFrontBack] = -5;motor[LeftMiddle] = -5;
			motor[RightFrontBack] = -5;motor[RightMiddle] = -5;
			motor[Intake] = inpower;
			wait1Msec(100);

		//end break
		while(SensorValue[LMS]<dl){//almost at dis
			displaysensors();
			motor[LeftFrontBack] = 30;motor[LeftMiddle] = 30;
			motor[RightFrontBack] = 30;motor[RightMiddle] = 30;
			if(SensorValue[LMS]<dlindist){motor[Intake] = inpower;}else{motor[Intake]=0;}
		}//end while values are almost at dis
		while(SensorValue[LMS]>dl){//after dis
			displaysensors();
			//move motors back
			motor[LeftFrontBack] = -30;motor[LeftMiddle] = -30;
			motor[RightFrontBack] = -30;motor[RightMiddle] = -30;
			if(SensorValue[LMS]>dlindist){motor[Intake] = inpower;}else{motor[Intake]=0;}
		}//end while values are more then dis
	}//end lor==1
	if(lor==-1){
		while(SensorValue[LMS]>dl+500){//more then half a wheel to dis
			displaysensors();
			//move motors back
			motor[LeftFrontBack] = -127;motor[LeftMiddle] = -127;
			motor[RightFrontBack] = -127;motor[RightMiddle] = -127;
			if(SensorValue[LMS]>dlindist){motor[Intake] = inpower;}else{motor[Intake]=0;}
		}//end while values arnt within 2*PI of dis
			displaysensors();
		//break
			motor[LeftFrontBack] = 5;motor[LeftMiddle] = 5;
			motor[RightFrontBack] = 5;motor[RightMiddle] = 5;
			motor[Intake] = inpower;
			wait1Msec(100);
		//end break
		while(SensorValue[LMS]>dl){//almost at dis
			displaysensors();
			//move motors back
			motor[LeftFrontBack] = -30;motor[LeftMiddle] = -30;
			motor[RightFrontBack] = -30;motor[RightMiddle] = -30;
			if(SensorValue[LMS]>dlindist){motor[Intake] = inpower;}else{motor[Intake]=0;}
		}//end while values are almost at dis
		while(SensorValue[LMS]<dl){//after dis
			displaysensors();
			motor[LeftFrontBack] = 30;motor[LeftMiddle] = 30;
			motor[RightFrontBack] = 30;motor[RightMiddle] = 30;
			if(SensorValue[LMS]>dlindist){motor[Intake] = inpower;}else{motor[Intake]=0;}
		}//end while values are after dis
	}//end lor==-1
	motor[RightFrontBack] = 0;//stop
	motor[RightMiddle] = 0;
	motor[LeftFrontBack] = 0;
	motor[LeftMiddle] = 0;
	motor[Intake] = 0;
}//end move
//set up gyro
void setupGyro(){
	/*this can be done in pre_auton but will be done as soon as the robot is powered on
	//instead of when the autonomus is run
	//that means the robot would have to be turned on after setting it up for autonomus
  //Completely clear out any previous sensor readings by setting the port to "sensorNone"*/
  SensorType[in8] = sensorNone;
  wait1Msec(1000);
  //Reconfigure Analog Port 8 as a Gyro sensor and allow time for ROBOTC to calibrate it
  SensorType[in8] = sensorGyro;
  wait1Msec(1200);
  //Adjust SensorScale to correct the scaling for your gyro
  //SensorScale[in8] = 260;
  //Adjust SensorFullCount to set the "rollover" point. A value of 3600 sets the rollover point to +/-3600
  SensorFullCount[in8] = 360000;//rolls over at 100 rotations
  //Display the sensors on the LCD so that whenever tan autonmous is executed, the autonomous values are displayed.
  displaysensors();
}//end setupgyro function
void launch(int inpower=0){//for autonomus only becuase when function is used it stops other actions from happening
		displaysensors();
		////////////// 	Launching for autonomus
		while(SensorValue[LauncherSwitch]==0&&SensorValue[LauncherSwitch2]==0){motor[Launcher]=127;}//stage 1 of launching, push back rack until rack switch(RS) is pressed by slip gear teeth
		motor[Launcher]=127;
		wait1Msec(350);
		motor[Launcher]=0;//done launching
}//end launch function
/*
void launchprep(){
		while(SensorValue[RS]==0){motor[port7]=127; motor[port9]=127;}
		motor[port7]=0;
		motor[port9]=0;//done preping

}*///end launchprep function
void showlaunchrange(){
//highflag launch range
if(SensorValue[WallSensor]>36&&SensorValue[WallSensor]<50){SensorValue[dgtl6]=1;}//led on dgtl6 is on
else{SensorValue[dgtl6]=0;}//led on dgtl6 is off

//lowflag launch range
if(SensorValue[WallSensor]>11&&SensorValue[WallSensor]<18){SensorValue[dgtl7]=1;}//led on dgtl7 is on
else{SensorValue[dgtl7]=0;}//led on dgtl7 is off
//launch toggle
if(TL==1){SensorValue[dgtl8]=1;}
else{SensorValue[dgtl8]=0;}
}//end showlaunchrange function

void automode(){//old autonomus dial
	/*
	if(SensorValue[in1] > (4095/5)*0 && SensorValue[in1] < (4095/5)*1 ){mode = 1;}//auto mode 1 far left (counter clockwise)
  if(SensorValue[in1] > (4095/5)*1 && SensorValue[in1] < (4095/5)*2 ){mode = 2;}//auto mode 2 left
	if(SensorValue[in1] > (4095/5)*2 && SensorValue[in1] < (4095/5)*3 ){mode = 3;}//auto mode 3 mid
	if(SensorValue[in1] > (4095/5)*3 && SensorValue[in1] < (4095/5)*4 ){mode = 4;}//auto mode 4 right
	if(SensorValue[in1] > (4095/5)*4 && SensorValue[in1] < (4095/5)*5 ){mode = 5;}//auto mode 5 far right (clockwise)
	could be done with this code in comment below but understanding is better then line efficency
int n=number of automodes, like 5 or 9
for(int k=0;k<n-1;k++){if(SensorValue[in1]>(4095/n)*k && SensorValue[in1]< (4095/n)*(k+1) ){mode = k+1}}*/
	mode=1;
}//end auto mode check function
void stopmotors(){//for stopping all motors
	displaysensors();
	motor[port1]=0; // lift
	motor[port2]=0;//right front & back wheels
	motor[port3]=0;// left front & back wheels
	motor[port4]=0;//right middle wheel
	motor[port5]=0;//left middle wheel
	motor[port6]=0;//Intake
	motor[port7]=0;//launcher
	motor[port8]=0;//launcher two
	motor[port9]=0;//flipper
	motor[port10]=0;
}// end stopmotors
//lcding
void automodelcd(){
	clearLCDLine(0);// Clear line 0 of the LCD
	clearLCDLine(1);// Clear line 1 of the LCD
	while(nLCDButtons != centerButton){//Switch case for 5 autonomi
		switch(mode){
			//Increment/decrement mode when left/right lcd button press
			// for each mode if mode is 1 loop back around to 5
			case 1://Display first choice
				displayLCDCenteredString(0, "#1 Blue 2 Ball");
				displayLCDCenteredString(1, "<     Enter    >");
				getPressed();
				if(nLCDButtons == leftButton){
					getReleased();
					mode=16;
				}
				else if(nLCDButtons == rightButton){
					getReleased();
					mode++;
				}
				break;
			case 2://Display second choice
				displayLCDCenteredString(0, "#2 Blue Platform");
				displayLCDCenteredString(1, "<     Enter    >");
				getPressed();
				if(nLCDButtons == leftButton){
					getReleased();
					mode--;
				}
				else if(nLCDButtons == rightButton){
				getReleased();
				mode++;
				}
				break;
			case 3://Display third choice
				displayLCDCenteredString(0, "#3 Blue Back");
				displayLCDCenteredString(1, "<     Enter    >");
				getPressed();
				if(nLCDButtons == leftButton){
					getReleased();
					mode--;
				}
				else if(nLCDButtons == rightButton){
					getReleased();
					mode++;
				}
				break;
			case 4://Display fourth choice
				displayLCDCenteredString(0, "#4 Straight Ahead");
				displayLCDCenteredString(1, "<     Enter    >");
				getPressed();
				if(nLCDButtons == leftButton){
					getReleased();
					mode--;
				}
				else if(nLCDButtons == rightButton){
					getReleased();
					mode++;
				}
				break;
			case 5://Display 5th choice
				displayLCDCenteredString(0, "#5 Neutral None");
				displayLCDCenteredString(1, "<     Enter    >");
				getPressed();
				if(nLCDButtons == leftButton){
					getReleased();
					mode--;
				}
				else if(nLCDButtons == rightButton){
					getReleased();
					mode++;
				}
				break;
			case 6://Display 6th choice
				displayLCDCenteredString(0, "#6 Ball Launch");
				displayLCDCenteredString(1, "<     Enter    >");
				getPressed();
				if(nLCDButtons == leftButton){
					getReleased();
					mode--;
				}
				else if(nLCDButtons == rightButton){
					getReleased();
					mode++;
				}
				break;
			case 7://Display 7th choice
				displayLCDCenteredString(0, "#7 Red Back");
				displayLCDCenteredString(1, "<     Enter    >");
				getPressed();
				if(nLCDButtons == leftButton){
					getReleased();
					mode--;
				}
				else if(nLCDButtons == rightButton){
					getReleased();
					mode++;
				}
				break;
			case 8://Display 8th choice
				displayLCDCenteredString(0, "#8 Red Platform");
				displayLCDCenteredString(1, "<     Enter    >");
				getPressed();
				if(nLCDButtons == leftButton){
					getReleased();
					mode--;
				}
				else if(nLCDButtons == rightButton){
					getReleased();
					mode++;
				}
				break;
			case 9://Display nth choice
				displayLCDCenteredString(0, "#9 Red Ball");
				displayLCDCenteredString(1, "<     Enter    >");
				getPressed();
				if(nLCDButtons == leftButton){
					getReleased();
					mode--;
				}
				else if(nLCDButtons == rightButton){
					getReleased();
					mode++;
				}
				break;
			case 10://Display nth choice
				displayLCDCenteredString(0, "#10 R far ball/Cap");
				displayLCDCenteredString(1, "<     Enter    >");
				getPressed();
				if(nLCDButtons == leftButton){
					getReleased();
					mode--;
				}
				else if(nLCDButtons == rightButton){
					getReleased();
					mode++;
				}
				break;
			case 11://Display nth choice
				displayLCDCenteredString(0, "#11 B far ball/Cap");
				displayLCDCenteredString(1, "<     Enter    >");
				getPressed();
				if(nLCDButtons == leftButton){
					getReleased();
					mode--;
				}
				else if(nLCDButtons == rightButton){
					getReleased();
					mode++;
				}
				break;
			case 12://Display nth choice
				displayLCDCenteredString(0, "#12 null");
				displayLCDCenteredString(1, "<     Enter    >");
				getPressed();
				if(nLCDButtons == leftButton){
					getReleased();
					mode--;
				}
				else if(nLCDButtons == rightButton){
					getReleased();
					mode++;
				}
				break;
			case 13://Display nth choice
				displayLCDCenteredString(0, "#13 Null");
				displayLCDCenteredString(1, "<     Enter    >");
				getPressed();
				if(nLCDButtons == leftButton){
					getReleased();
					mode--;
				}
				else if(nLCDButtons == rightButton){
					getReleased();
					mode++;
				}
				break;
			case 14://Display nth choice
				displayLCDCenteredString(0, "#14 Null");
				displayLCDCenteredString(1, "<     Enter    >");
				getPressed();
				if(nLCDButtons == leftButton){
					getReleased();
					mode--;
				}
				else if(nLCDButtons == rightButton){
					getReleased();
					mode++;
				}
				break;
			case 15://Display nth choice
				displayLCDCenteredString(0, "#15 Null");
				displayLCDCenteredString(1, "<     Enter    >");
				getPressed();
				if(nLCDButtons == leftButton){
					getReleased();
					mode--;
				}
				else if(nLCDButtons == rightButton){
					getReleased();
					mode++;
				}
				break;
			case 16://Display nth choice
				displayLCDCenteredString(0, "#16 Null");
				displayLCDCenteredString(1, "<     Enter    >");
				getPressed();
				if(nLCDButtons == leftButton){
					getReleased();
					mode--;
				}
				else if(nLCDButtons == rightButton){
					getReleased();
					mode=1;
				}
				break;
			default://incase the mode is set to
				mode = 5;//somthing not defined by
				break;// a previous case
		}
	}
}//end lcding
void displayautomode(){
	clearLCDLine(0);// Clear line 0 of the LCD
	clearLCDLine(1);// Clear line 1 of the LCD
	switch(mode){
		case 1://Display first choice
			displayLCDCenteredString(0, "#1 Blue 2 Ball");
			displayLCDCenteredString(1, "Selected");
			break;
		case 2://Display second choice
			displayLCDCenteredString(0, "#2 Blue Platform");
			displayLCDCenteredString(1, "Selected");
			break;
		case 3://Display third choice
			displayLCDCenteredString(0, "#3 Blue Back");
			displayLCDCenteredString(1, "Selected");
			break;
		case 4://Display fourth choice
			displayLCDCenteredString(0, "#4 Straight Ahead");
			displayLCDCenteredString(1, "Selected");
			break;
		case 5://Display 5th choice
			displayLCDCenteredString(0, "#5 Neutral None");
			displayLCDCenteredString(1, "Selected");
			break;
		case 6://Display 6th choice
			displayLCDCenteredString(0, "#6 Ball Launch");
			displayLCDCenteredString(1, "Selected");
			break;
		case 7://Display 7th choice
			displayLCDCenteredString(0, "#7 Red Back");
			displayLCDCenteredString(1, "Selected");
			break;
		case 8://Display 8th choice
			displayLCDCenteredString(0, "#8 Red Platform");
			displayLCDCenteredString(1, "Selected");
			break;
		case 9://Display 8th choice
			displayLCDCenteredString(0, "#9 Red 2 Ball");
			displayLCDCenteredString(1, "Selected");
			break;
		case 10://Display 8th choice
			displayLCDCenteredString(0, "#10 R far ball/Cap");
			displayLCDCenteredString(1, "Selected");
			break;
		case 11://Display 8th choice
			displayLCDCenteredString(0, "#11 B far ball/Cap");
			displayLCDCenteredString(1, "Selected");
			break;
		case 12://Display 8th choice
			displayLCDCenteredString(0, "#12 Null");
			displayLCDCenteredString(1, "Selected");
			break;
		case 13://Display 8th choice
			displayLCDCenteredString(0, "#13 Null");
			displayLCDCenteredString(1, "Selected");
			break;
		case 14://Display 8th choice
			displayLCDCenteredString(0, "#14 Null");
			displayLCDCenteredString(1, "Selected");
			break;
		case 15://Display 8th choice
			displayLCDCenteredString(0, "#15 Null");
			displayLCDCenteredString(1, "Selected");
			break;
		case 16:
			displayLCDCenteredString(0, "#16 Null");
			displayLCDCenteredString(1, "Selected");
			break;
		default://Error? becuase the set mode is
			//not defined by display switch statment
			displayLCDCenteredString(0, "Error?");
			displayLCDCenteredString(1, "Power Cycle");
			break;
	}
}//end displayautomode
//end functions

void pre_auton (){//pre_auton
	clearTimer(T1);// starts timer T1 for launcher loading
	//clearTimer(T2);// starts timer T2 for calculating angular velocity
	//call Time1[T1] to get timer value in msec
	bLCDBacklight=true;// Turn on LCD Backlight
	TB=0;
	automode();
	displayautomode();
	setupGyro();
}//end pre_auton

task autonomous(){

	if(mode==1){//auto 1 BLUE, 2 ball
		move(-45,127);
		move(45,127);
		turn(920,1,0);
		launch();
		gotowall(4);
		stopmotors();
	}//end of auto 1

	if(mode==2){//auto 2 BLUE, platform
		move(-25);
		turn(900,1,0);
		motor[RightFrontBack] = 127;motor[RightMiddle] = 127;//Final Push to the Platform
		motor[LeftFrontBack] = 127;motor[LeftMiddle] = 127;//Final Push to the Platform
		wait1Msec(4000);//+3 platform
		stopmotors();
	}//end auto 2

	if(mode==3){//auto 3 BLUE, back capflip
		//setupGyro(); done in preauton
		move(-20,-127);
		stopmotors();
	}//end auto 3

	if(mode==4){//auto 4 Nuetral, Straight ahead
		//setupGyro(); done in preauton
		gotowall(4);// + 2 low flag
		stopmotors();
	}//end auto 5

	if(mode==5){//auto 5 Nuetral, none
		setupGyro();
		wait1Msec(5000);
		stopmotors();
	}//end auto 5

	if(mode==6){//auto 6 Neutral,launch
		//setupGyro(); done in preauton
		launch();// + 4 high flag
		stopmotors();
	}//end auto 6

	if(mode==7){//auto 7 Red, Back capflip
		//setupGyro(); done in preauton
		move(-20,-127);
		stopmotors();
	}//end auto 7

	if(mode==8){//auto 8 Red, Platform
		move(-25);
		turn(-900,1,0);
		motor[RightFrontBack] = 127;motor[RightMiddle] = 127;//Final Push to the Platform
		motor[LeftFrontBack] = 127;motor[LeftMiddle] = 127;//Final Push to the Platform
		wait1Msec(4000);//+3 platform
		stopmotors();
	}
	if(mode==9){//auto 9 Red, Ball
		//setupGyro(); done in preauton
		move(41,127);//distance between the starting positon and the easy cap			(Works?)
	  //intake();//+1 easy cap
		move(-41,127);//move back to starting position
		heading(-900,1,0);//to align with starting tile toward flag
		launch();
		gotowall(4);//+2 Low flag
		stopmotors();

	}
	if(mode==10){//Red Auto, back lauch capflip
		launch();//+4 high flag
		heading(-890,2,0);//make turn tiles			(Adjust for post on red side?)
		move(41.25,-127);//distance between the starting positon and the easy cap
  	move(-41.25,-127,-20);//move back to starting position
		stopmotors();
	}
	if(mode==11){//Back Tile Blue Auto
		//setupGyro(); done in preauton
		heading(45+0.25,1,0);//to aim left at high
		launch();//+4 high flag
		heading(890,2,0);//make turn tiles
		move((46.6-3)+(23.3/2)-14,127);//distance between the starting positon and the easy cap			(Works?)
 		move(-58.2+3+14,127,-20);//move back to starting position
 		stopmotors();
	}
	if(mode==12){//Back Tile Red Auto
		//setupGyro(); done in preauton
	}
	if(mode==13){}
	if(mode==14){}
	if(mode==15){}
	if(mode==16){}

}//end auto

/////usercontrol/////

task usercontrol(){
	while(true){//loops always during driver control to constantly check buttons
		displaysensors();//this is so we can adjust our autos
		//////////////  Autonomous Mode lcd selection, press left or right lcd button to change mode
		if(nLCDButtons!=centerButton&&nLCDButtons!=0){automodelcd();displayautomode();}
		//////////////	Autonomous Activation
		x = 0;
		if(vexRT[Btn7U] == 1){
			startTask(autonomous);
			while(x <= 15000){wait1Msec(1);x++;}
		}
		///////////show launch range
		showlaunchrange();
		//////////////	Drive
		if(vexRT[Btn7R]==vexRT[Btn8L]){//normal driving
			motor[RightFrontBack]=vexRT[Ch2];motor[RightMiddle]=vexRT[Ch2];//right drive
			motor[LeftFrontBack]=vexRT[Ch3];motor[LeftMiddle]=vexRT[Ch3];//left drive

		}else if(vexRT[Btn7R]==1){//straight drive assist, both sides controled with right joystick
			motor[RightFrontBack]=vexRT[Ch2];motor[RightMiddle]=vexRT[Ch2];//right drive
			motor[LeftFrontBack]=vexRT[Ch2];motor[LeftMiddle]=vexRT[Ch2];//left drive(by right joystick)
		}else if(vexRT[Btn8L]==1){//point turn assist, both sides controlled with left joysick
			//up>clockwise down>CCW
			motor[RightFrontBack]=-1*vexRT[Ch3]*abs(vexRT[Ch3])/127;//right drive(by left joystick reversed)
			motor[RightMiddle]=-1*vexRT[Ch3]*abs(vexRT[Ch3])/127;//uses down scaler function y=x*|x|/127
			motor[LeftFrontBack]=vexRT[Ch3]*abs(vexRT[Ch3])/127;//left drive
			motor[LeftMiddle]=vexRT[Ch3]*abs(vexRT[Ch3])/127;
		}else{
			motor[RightFrontBack]=vexRT[Ch2];motor[RightMiddle]=vexRT[Ch2];//right drive
		}
		//////////////	Angler
		if (vexRT[Btn5U] == vexRT[Btn5D]) {motor[Angler] = 0;}//stop
		if (vexRT[Btn5U] == 1) 						{motor[Angler] = 127;}//up
		if (vexRT[Btn5D] == 1) 						{motor[Angler] = -127;}//down

		//////////////	Flipper
		if (vexRT[Btn5U] == vexRT[Btn5D]) {motor[Flipper] = 0;}//stop
		if (vexRT[Btn5D] == 1) 						{motor[Flipper] = -127;}//Down
		if (vexRT[Btn5U] == 1) 						{motor[Flipper] = 127;}//Up

		//////////////	Intake
		if (vexRT[Btn6U] == vexRT[Btn6D]) {motor[Intake] = 0;}//stop
		if (vexRT[Btn6U] == 1) 						{motor[Intake] = 127;}//in
		if (vexRT[Btn6D] == 1) 						{motor[Intake] = -127;}//out

		////////////// 	Launcher
		if(vexRT[Btn8U]==1){Tedge=true;}
		if(vexRT[Btn8U]==0&&Tedge==true){autocock= !autocock;Tedge=false;}
		if(SensorValue[LauncherSwitch]==0&&SensorValue[LauncherSwitch2]==0&&autocock==true){motor[Launcher]=127;}
		else{motor[Launcher]=0;}
		if(vexRT[Btn8R] == 1){motor[Launcher]=127;}

		/*
		if(TL==1&&SensorValue[SS]==1){motor[port7]=0;}//this is part of
		//making sure that the ball will be loadable
		if (vexRT[Btn8R] == 1){TB=1;}
		if(TB==0){//launcher button not pressed / done launching
			motor[Launcher]=0;
		}//end TB 0
		if(TB==1){//stage 1 of launching,
			//push back rack until rack switch(RS) is pressed by slip gear teeth
			motor[Launcher]=127;
			if(SensorValue[RS]==1){TB=2;}
		}//end TB 1
		if(TB==2){//stage 2 of launching,
			//keep pushing back rack until slip switch(SS) is unpressed
			//by being on slide part of gear
			motor[Launcher]=127;
			if(SensorValue[SS]==0){TB=3;}
		}//end TB 2
		if(TB==3){//stage 3 of launching,
			//push back rack until RS is pressed by rack gear teeth
			motor[Launcher]=127;
			if(SensorValue[RS]==1){TB=0;}
		}//end TB 3
		if(vexRT[Btn8D]==1){motor[Launcher]=127;}//manual launch
		//////////////// Launcher loadable settings
		if(TL==1&&time1[T1]%2000<100&&SensorValue[RS]!=1){motor[Launcher]=127;}
		if(TL==1&&SensorValue[SS]==0){motor[Launcher]=127;}
		//above makes sure that a ball will be loadable
		//when toggled on with Btn7L
		//below toggles TL at the trialing
		//edge of the button being pressed
		if(vexRT[Btn8U]==1){Tedge=true;}
		if(Tedge==true&&vexRT[Btn8U]==0){Tedge=false;TL=-1*TL;}
		*/

	}//end infinite driver contol loop
}//end usercontrol
