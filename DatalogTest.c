
task main()
{
	// The first argument is a datalog index number.
	// The filename will be saved in the rc-data folder as
	// datalog-<INDEX>.txt
	// The format is a comma separated value file.
	// The 2nd argument is the number of columns you wish to have in the file
	// The 3rd argument specifies if you wish to append to the datalog, if it
	// exists.
	if (!datalogOpen(0, 4, false))
	{
		playSound(soundException);
		displayTextLine(4, "Unable to open datalog");
		writeDebugStreamLine("Unable to open datalog");
		sleep(5000);
		stopAllTasks();
	}

	// You can add an entry at the specified column number
	for (int i = 0;i < 2000; i++)
	{
		datalogAddChar(0, 1 + i);
		datalogAddShort(1, 9000 + i);
		datalogAddLong(2, 2388738 + i);
		datalogAddFloat(3, 678.98 / (i + 1));
	}

	// Make sure you close the file properly
	datalogClose();
}
