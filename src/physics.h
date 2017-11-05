#pragma once

#define CAR_ACCELERATION 0.04
#define BRAKE_ACCELERATION -0.05
#define MAX_SPEED 2.1
#define MIN_SPEED 0.0

bool carForward, carBrake;
float carSpeed, centrifugalPower, deltaTimeCentr;

void initPhysics() {
	carForward = carBrake = false;
	carSpeed = centrifugalPower = deltaTimeCentr = 0.0f;
}

void calcSpeed() {

	//	std::cout << carSpeed << std::endl;

	if (carForward) {
		carSpeed += CAR_ACCELERATION;
	}
	else {
		carSpeed -= CAR_ACCELERATION;
	}

	if (carBrake) {
		carSpeed += BRAKE_ACCELERATION;
	}

	if (carSpeed > MAX_SPEED) {
		carSpeed = MAX_SPEED;
	}

	if (carSpeed < MIN_SPEED) {
		carSpeed = MIN_SPEED;
	}

	carForward = carBrake = false;

}

void calcCentrifugal(bool toRight, bool toLeft, float deltaTime) {

	float dir = 0.0;
	deltaTimeCentr += deltaTime;

	if (toLeft)
		dir = -1.0;

	else if (toRight)
		dir = 1.0;

	else
		deltaTimeCentr = 0.0;

	centrifugalPower = dir * carSpeed * deltaTimeCentr * 0.008;
}