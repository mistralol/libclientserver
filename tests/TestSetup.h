

static void SigAlarm(int num)
{
	printf("Test Timeout!\n");
	abort();
}

static void TestSetup()
{
	signal(SIGALRM, SigAlarm);


	alarm(300);
}

