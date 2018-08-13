{
	TString myGO4SYS = gSystem->Getenv("GO4SYS");
	if (myGO4SYS == "") {
		cerr << "GO4SYS environment variable not defined. "
		     << "Please run go4login or define GO4SYS manually. " << endl
		     << "Only $GO4SYS/include path will be passed to the ROOT interpreter."
		     << endl;
		return;
	}
	gInterpreter->AddIncludePath(myGO4SYS + "/include/");
}
