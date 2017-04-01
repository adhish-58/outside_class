task main() {
	int straight = 50, turn = 30, steer = 40, end = 0;
	int distance_1, distance_2;
	int distance_from_wall = 20;
	int threshold = 30, difference;
	int hysteresis = 2;
	bool Wall = false;
	int i = 0;

	byte fileHandle;
	short BytesWritten;
	string write;
	long int total_start_time;
	long int begin_time;
	long int stop_time;
	int time_output, average_heading, distance;
	int counter = 0;
	int size = 10, total_size = (size/2)*(size+1); // total size is in integer division
	long int heading[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	long int total_heading;

	SetSensorLowspeed(IN_1); // Ultrasonic Sensor
	SetSensorLowspeed(IN_2); // Ultrasonic Sensor
	SetSensorLowspeed(IN_3); // HiTechnic Compass Sensor
	// Motor A drives right wheel and Motor B drives left wheel

	DeleteFile("output.txt");
	CreateFile("output.txt", 512, fileHandle);

	Wait(2000);

	total_start_time = FirstTick();
	begin_time= CurrentTick();

	while (!(Wall) || ((distance_1 < threshold) && (distance_2 < threshold))
	{
		for (i = 1; i < size; i++) {
			heading[i] = heading[i-1];
		}

		stop_time =	CurrentTick();
		//total_heading += SensorHTCompass(IN_1);
		//counter++;
		heading[0] = SensorHTCompass(IN_3);

		if ((stop_time - begin_time) >= 2000)
		{
			distance = MotorRotationCount(OUT_A);
			time_output = (stop_time - total_start_time);

			for (i = 0; i < size; i++) {
				total_heading += heading[i] * (size - i);
			}

			average_heading = (total_heading)/total_size;
			begin_time = CurrentTick();
			//counter = 0;
			total_heading = 0;
      		write = StrCat(NumToStr(time_output), ", ", NumToStr(average_heading), ", ", NumToStr(distance));
			WriteLnString(fileHandle, write, BytesWritten);
		}

		distance_1 = SensorUS(IN_1);
		distance_2 = SensorUS(IN_2);

		ClearScreen();
		NumOut(40, LCD_LINE1, distance_1);
		NumOut(40, LCD_LINE2, distance_2);

		difference = distance_1 - distance_2;

		if ((difference < hysteresis*(-1))) {
			OnFwd(OUT_A, turn);
			OnFwd(OUT_B, end);
		} else if ((difference > hysteresis)) {
			OnFwd(OUT_A, end);
			OnFwd(OUT_B, turn);
		} else if ((difference < hysteresis) && (distance_1 > distance_from_wall)) {
			OnFwd(OUT_A, steer);
			OnFwd(OUT_B, straight);
		} else if ((difference < hysteresis) && (distance_1 < distance_from_wall)) {
			OnFwd(OUT_A, straight);
			OnFwd(OUT_B, steer);
		} else {
			OnFwd(OUT_AB, straight);
			Wall = true;
		}

		// if (Wall && readings > (distance_from_wall + threshold))
		//	OnFwd(OUT_AB, end);
	}

	OnFwd(OUT_AB, end);
	CloseFile(fileHandle);
}


