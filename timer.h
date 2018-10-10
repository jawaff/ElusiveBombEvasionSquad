class timer
{
private:
	//Holds the amount of ticks since the timer started.
	int startTicks[26];

	//Contains the time that the timer is paused for.
	int pausedTicks[26];

	//Timer status
	bool paused[26];
	bool started[26];

public:
	//How many timers
	int timers;
	//Initializes timer
	timer ();

	//Functions for the timer
	void start (int i);
	void stop (int i);
	void pause (int i);
	void unpause (int i);

	//Returns the time of the timer
	int get_Ticks (int i);

	//Returns the status of the timer
	bool is_Started (int i);
	bool is_Paused (int i);

};