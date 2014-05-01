

void SigAlarm(int num)
{
	printf("Test Timeout!\n");
	abort();
}

void TestSetup()
{
	signal(SIGALRM, SigAlarm);


	alarm(60);
}

