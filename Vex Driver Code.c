#pragma config(Sensor, port10, Velocity,       sensorVexIQ_LED)
#pragma config(Motor,  motor2,          SPIN2,         tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor4,          Scoop,         tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,  motor5,          Spin1,         tmotorVexIQ, PIDControl, reversed, encoder)
#pragma config(Motor,  motor7,          LDrive,        tmotorVexIQ, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motor11,         RDrive,        tmotorVexIQ, PIDControl, reversed, driveRight, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//
int F1Speed = 100;
int F2Speed= 100;
int getmotorvelocity(int M)
{
	//A way for use to find a number based on how fast a motor is going.
	int F;
	int S;
	float velocity;
	resetMotorEncoder(M);
	// The motor encoder shows us how far the motor travel since the last time we checked it by checking it everty 100ms.
	F=getMotorEncoder(M);
	sleep(100);
	S=getMotorEncoder(M);
	velocity=F-S;
	return(velocity);
}
// This is jusr a copy and paste of the tank control defention while adding a speed varbiles to change it speed.
void custontankControl(TVexJoysticks rightJoystick = ChD, TVexJoysticks leftJoystick = ChA, short threshold = 10,float speed=1.00)// here is the varbiable
{
	short nRightSideSpeed;
	short nLeftSideSpeed;
	//getMotorSpeed
	updateMotorDriveTrain();

	nRightSideSpeed = getJoystickValue(rightJoystick);
	nLeftSideSpeed = getJoystickValue(leftJoystick);

	if(nGlobalJoyScaledValue != nMaxJoyScaleValue)
	{
		nRightSideSpeed = nRightSideSpeed * (nGlobalJoyScaledValue / 100.0);
		nLeftSideSpeed 	= nLeftSideSpeed * (nGlobalJoyScaledValue / 100.0);
	}

	if (abs(getJoystickValue(rightJoystick)) <= abs(threshold))
		nRightSideSpeed = 0;

	if (abs(getJoystickValue(leftJoystick)) <= abs(threshold))
		nLeftSideSpeed = 0;
	//here is how we change the speed of the motors while driving.
	setMotorSpeeds(nRightSideSpeed*speed, nLeftSideSpeed*speed);
}
task auto()
{
	//This whole task job is the change one of the led to green or red base on the speed of the flywheel from the velocity stement above
	repeat(forever)
	{
		displayVariableValues(1,getmotorvelocity(1));
		displayVariableValues(2,getmotorvelocity(4));
		if(getmotorvelocity(1) & getmotorvelocity(4) >22)
		{
			setTouchLEDRGB(9, 0, 255, 0);

		}

		if(getmotorvelocity(1) & getmotorvelocity(4) <22)
		{
			setTouchLEDRGB(9, 255, 0, 0);
		}

	}
	//This is why we change the speed of the flywheel based on if it get to the speed we need it at
	if(getmotorvelocity(1)>22)
	{
		F1Speed=50;
	}
	if(getmotorvelocity(1)<22)
	{
		F1Speed=100;
	}
	if(getmotorvelocity(4)>22)
	{
		F2Speed=50;
	}
	if(getmotorvelocity(4)<22)
	{
		F2Speed=100;
	}
}
task drive(){
	while(true)
	{
		// This task is deciable for moving the robot in general.
		//+ the 2 if staments below would change a led and the speed making the robot move backword by pressing Fup.
		int MSpeed = 1;

		if (BtnFUp & MSpeed==1)
		{
			MSpeed=-1;
			setTouchLEDRGB(6, 255, 0, 0);

		}
		if (BtnFUp & MSpeed==-1)
		{
			MSpeed=1;
			setTouchLEDRGB(6, 0, 255, 0);
		}
		//This is just the flywheel the spinner and the tank control with canging the flywheel mortor so the motor won't
		//stop the motors letting them spin freely
		custontankControl(ChD,ChA,10,MSpeed);
		//		tankControl(ChD,ChA,10);
		armControl(1 ,BtnRUp, BtnRDown, 100);
		armControl(4 ,BtnRUp, BtnRDown, 100);
		armControl(3 ,BtnLUp, BtnLDown, 100);
		setMotorBrakeMode(1,motorCoast);
		setMotorBrakeMode(4,motorCoast);

	}
}
task main()
{
	// This is to just start the two tasks
	startTask(drive);
	startTask(auto);
	// This is to stop the program shutting off since it would be done.
	while(true)
	{
		sleep(1);

	}
}
