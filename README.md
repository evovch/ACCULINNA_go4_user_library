Go4 user library project for Mbs DAQ.


Environment
===========

ROOT and Go4 are required for this library.

First execute your go4login script in the corresponding directory:

> . ./go4login

You can tune the env.sh script to your system and use it instead:

> . ./env.sh

You can check the values of all involved environment variables by:

> ./showEnv.sh

Unfortunately there is no 'easy' way to reset the current terminal (no, 'reset' command does not do that!), so in case something goes wrong, just close the terminal or your ssh connection and start a new one.

Building
========

Build the user library using a simple shell script which in turn calls for cmake:

> ./compile.sh

CMake building
==============

Use cmake directly for custom building:

> mkdir build && cd build
> 
> cmake ../ -DCMAKE_INSTALL_PREFIX=install_path

CMake options
-------------

Here are some specific options affecting output binaries.

Use `-DREADER_MODE=ON` to build only for output files reading. This mode ignores _useranalysis_ compilation and does not require _go4_ installation. 

Building Doxygen documentation
==============================

In order to produce documentation from the comments in the code use:

> genDocs.sh

Result will be located in the 'docs' directory.

LMD file analysis
=================

Tune the run.sh script to your needs and run it:

> ./run.sh

Cleanup
=======

You can remove all generated files by:

> ./doclean.sh

Note that doclean.sh removes both build result and textual output generate during analysis.
Analysis results should be located outside of the repository by specifying corresponding paths in the run.sh script.

Analysis of output root files
=============================

A ROOT macro 'macros/analyse4.C' is provided as a template to start your simple analysis by means of native ROOT. Please, follow the instructions written at the beginning of the file.

> cd macros
> root -l "analyse4.C(\"filename.lmd.root\", \"config.xml\", 10)" > macroout.txt 2> macroerr.txt

Input data structure
====================

EVENT
-----

An event consists of a header and optionally subevents.
I have no idea if an event has a footer because the MbsAPI,
provided by the Go4 framework, is used and no raw data is accessed at this level.

	event header
	subevent
	subevent
	...
	subevent
	[event footer]

SUBEVENT
--------

Each subevent consists of a header and optionally subsubevents.
Again, I have no idea if a subevent has a footer because the MbsAPI,
provided by the Go4 framework, is used and no raw data is accessed at this level.
The subevent header contains the size of the subevent data payload which is used
to loop over the words in a for-style loop. The pointer to the first word of the
first subsubevent is provided.

	subevent header
	subsubevent
	subsubevent
	...
	subsubevent
	[subevent footer]

SUBSUBEVENT
-----------

One subsubevent is a group of words of the following structure:

	subsubevent header
	data word
	data word
	...
	data word
	subsubevent footer

Subsubevent header and footer each have the length of one word (32 bits).
A data word (32 bits) can be coming from a unit by one of the vendors - CAEN or MESYTEC.
The subsubevent header is used to identify the vendor.
The footer is used only to separate the subsubevents.
It can be (and currently is) used to identify the length of the subsubevent.

geo and module_id
=================

MESYTEC electronics blocks have module IDs. This ID is written only once in the header for the whole subsubevent. Nice!

CAEN electronics blocks have equivalent field which is called geo.

These two types of value have common name addr. For MESYTEC blocks geo=-1, addr=module_id. For CAEN blocks module_id=-1, addr=geo.

Unfortunately, CAEN blocks write geo in the header, in each data word and in the footer. This means that within each subsubevent this geo should be the same for all words. This seems to work fine in the analysis if the electronics does its job. However, in this particular unpacking/analysis code, only the geo from the header is taken, and all geo values from data words and the footer are ignored due to the following reason. CAEN V560 scalers do not have geo in the data words. Data from CAEN V560 looks like the following:

	header
	32-bit scaler[0] value
	32-bit scaler[1] value
	...
	32-bit scaler[N] value
	footer

There is no reason to try to extract geo from data word.

Another reason is that RIO machine time, which is 32-bits wide, is artificially packed inside a fake CAEN subsubevent liek that:

	header
	32-bit machine time value
	footer

A special geo value is written inside the header. At the moment of writing of this README, this special value was 30. It does not matter as long as you use setup.xml file and specify correct geo there. There is no hardcoded value.

Output data structure
=====================

In order for the code to be independent of the hardware setup as much as possible, the setup.xml has been invented. This setup configuration file allows building repacking(monitoring)-step output event structure 'on the fly'. To make this possible the Go4 composite event functionality is employed.

Lower level entity - detector message (DetMessage class).
TClonesArray of DetMessage's is encompassed in the station-event (DetEventStation class).
A group of DetEventStation's makes up one detector-event (DetEventDetector class).
A group of DetEventDetector's plus one common-event (DetEventCommon) make up one full-event (DetEventFull).

Common information of the event is something which is usually called 'event header'. This habitual name is not used intentionally as it may introduce ambiguity - when processing raw data from the detector, data come in portions which are called events which have their headers (yes, event headers).

The following numbering scheme is implemented.

DetEventCommon has hardcoded detectorID=0.
Each next detector, found in the setup.xml, is given a new detectorID starting from 1.
Each detector has some number of stations. Each next found station for the current detector is given a new stationID. Internally stations are numbered from 0. However, in the end each station gets its unique stUID computed as

	stUID = detectorID * 100 + stationID

Even though "Ignore" detector and its "Ignore" station are the special entities with some specific processing, IDs are anyway assigned to them.

Consider the following example. Setup configuration XML file looks like the following:

	...
	<mapping addr="10"	elblock="mQDC"	startelectrch="0"	nelectrch="4"	detector="F3"	startdetch="0"	folder="Beam_detector_ToF"/>
	<mapping addr="10"	elblock="mQDC"	startelectrch="4"	nelectrch="4"	detector="F5"	startdetch="0"	folder="Beam_detector_ToF"/>
	<mapping addr="10"	elblock="mQDC"	startelectrch="8"	nelectrch="4"	detector="F6"	startdetch="0"	folder="Beam_detector_ToF"/>
	<mapping addr="10"	elblock="mQDC"	startelectrch="16"	nelectrch="16"	detector="Ignore"	startdetch="0"	folder="Ignore"/>
	<mapping addr="11"	elblock="mTDC"	startelectrch="0"	nelectrch="4"	detector="tF3"	startdetch="0"	folder="Beam_detector_ToF"/>
	<mapping addr="3"	elblock="V775"	startelectrch="0"	nelectrch="16"	detector="tSQY_L"	startdetch="0"	folder="Left_telescope"/>
	<mapping addr="3"	elblock="V775"	startelectrch="16"	nelectrch="16"	detector="tSQY_R"	startdetch="0"	folder="Right_telescope"/>
	<mapping addr="4"	elblock="V775"	startelectrch="0"	nelectrch="16"	detector="tCsI_L"	startdetch="0"	folder="Left_telescope"/>
	<mapping addr="4"	elblock="V775"	startelectrch="16"	nelectrch="16"	detector="tCsI_R"	startdetch="0"	folder="Right_telescope"/>
	...

Result:

	Detector "DetEventCommon", detectorID = 0
	Detector "Beam_detector_ToF", detectorID = 1
		Station "F3", stationID = 0, stUID = 100
		Station "F5", stationID = 1, stUID = 101
		Station "F6", stationID = 2, stUID = 102
		Station "tF3", stationID = 3, stUID = 103
	Detector "Ignore", detectorID = 2
		Station "Ignore", stationID = 0, stUID = 200
	Detector "Left_telescope", detectorID = 3
		Station "tSQY_L", stationID = 0, stUID = 300
		Station "tCsI_L", stationID = 1, stUID = 301
	Detector "Right_telescope", detectorID = 4
		Station "tSQY_R", stationID = 0, stUID = 400
		Station "tCsI_R", stationID = 1, stUID = 401

Electronics characteristics
===========================

A separate XML file should be provided for the advanced monitoring step. Along with user-defined histograms, this step produces a bunch of automatically generated histograms. These are distributions of the values coming from individual electronics channels. In order to have the control over the histograms parameters (nbins, xlow, xhigh) without hardcoding them, these numbers are stored in a separate XML file. This is also possible due to the fact the histograms' ranges should correspond to the hardware characteristics. For example, if an ADC/QDC/TDC works in a X-bit mode, a range of [0; 2^X] should be used. The type of the electronics block is chosen using the 'elblock' attribute of the setup configuration XML file. So, only the blocks specified in the electronics characteristics XML file can be used in the setup configuration XML file.

Project structure
=================

autoload
--------

Under development.

auxiliary
---------

Auxiliary files for the developer.

base
----

Some common general functionality which may be used by any part of the project.

build
-----

Build directory. If you use provided compile.sh script, this directory is generated automatically right before building the project.

data
----

Mostly repacking step output data structures.

docs
----

Output of the doxygen generator. By now these files should be produced by the user on the local machine. See [Building Doxygen documentation].

docu
----

Static documentation files.

macros
------

Directory for user macros.

setupconfig
-----------

A pure "C" sybsystem which performs import of the setup configuration XML. This code conforms c89 standard which makes it compilable by the RIO3 (RIO4?) VME controllers. It is planned that this subsystem will be also used to configure the DAQ hardware.

setupconfigcppwrapper
---------------------

"C++" wrapper around SetupConfig for easier usage in this project.

textoutput
----------

Different text output files produced by the analysis. This directory must exist in order for the user library to work. Please, use provided script to compile/clean/run anlysis. They take care.

useranalysis
------------

### Unpacking

The first step of the analysis. Only reads raw data and extracts raw messages without mapping them to any detector. This step is able to produce a ROOT tree with unpacked raw messages (together with raw words) to be processed by any software.

### Repacking

The second step of the analysis. Looks into the setup configuration and produces a ROOT tree ...

### Learn

A subsystem which allows to perform analysis of a rather small portion of the input data (TODO implement nEvents limit) and generate a summaryLearn.txt file which contains information about which procID's (crates), addressed (geo/module) and electronics channels produce data. This information can be then compared to the setup file and create warnings abount unmapped channels or mapped channels with no information.

### Mesh

A small subsystem which is required by Go4 and used by UserAnalysis class to create a mesh-like structure of the analysis. This allows, in particular, to split the unpacked, but not mapped data into two or more streams to be processed by separate processors (i.e. monitoring and learn).

### Raw monitoring



### Advanced monitoring



### Beam detector monitoring

usr
---

Directory for universal shared resources files, including setup configuration XML files and electronics characteristics XML files.

Developer reminder
==================

When trying to change something, do 'grep TODO' over the full source code and read all the messages left there. This may help to reduce debugging time.

Coding convention
=================

Rather official ROOT coding convention
--------------------------------------

Variables starting with 'f' are the class data members.
They are known everywhere inside the class methods.
Do not create local variables starting with 'f'.
This will make your code so hard to understand and create
potential naming clash...

Unofficial coding convention
----------------------------

+ variables starting with 'f' or 'm' are the class data members;

+ variables starting with 'p_' are the parameters of the the method;
in case you want to specify explicitly:

   - starting with 'i_' are the input parameters (usually byval);

   - starting with 'o_' are the output parameters (usually byref);

+ variables starting with v_ are the local variables;

+ variables without any identifiable prefix are usually the local ones -
this is usually the case for simple one-letter names: i, j, k, x, y, z, n, etc...

Short GCC reminder
==================

For more information refer to:

> man gcc

In a normal situation building a project is done in several steps which include per-se compilation and, after that, linkage using **ld**.

File types
----------

**.h** - header, does not neet to be compiled directly;

**.c, .cpp, cxx** - source, to be compiled directly;
    good practice is to first compile each source into a separate object file and then
    build the desired target (exe or so) from the objects files;

**.o** - object file;

**.exe** or no suffix - executable;

**.so** - dynamic library (basically, same as executable but with no entry point - no main());

**.a** - static library.

Most used flags
---------------

**-o [output_file_name]** - specify explicitly the output file name;

**-c** - only compile up to the object file (do not run the linker **ld**);

**-I[path]** - (capital I) specify additional include directory where the compiler searches for the files (usually headers) included in your sources;

**-L[path]** - (capital L) specify additional directory where the **linker** searches for the libraries;

**-l[lib_name]** - (small l) library name atached during linkage. The 'lib' prefix and the '.so' suffix of the lib_name should be both omitted;

**-shared** - create a dynamic library, not executable;

**-fPIC** - the 'position-independent-code' flag; when creating dynamic libraries the sources have to be compiled with this flag;

**-std=c++11** - enable C++11 support;

**-Wall** - enables all the warnings about constructions that some users consider questionable, and that are easy to avoid;

**-Wextra** - enables some extra warning flags that are not enabled by **-Wall**;

**-Wpedantic** - issue all the warnings demanded by strict ISO C and ISO C++;

A command wirtten inside the ` quotes is executed and its result is put in the place.

rootcint/rootcling
==================

For rootcling (or older rootcint) see official site, there's not so much to read:
https://root.cern.ch/cling

Short Makefile structure reminder
=================================

	.PHONY: list of fake targets (not files)

	target: prerequisites
	(tab character) command to obtain the target, possibly, but not necessarily, using the prerequisites
	(tab character) command2
	...
	(tab character) commandN

Automatic variables:

**$@** - the file name of the target of the rule.

**$^** - the names of all the prerequisites, with spaces between them.

See also:
https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html

Proposals
=========

Предлагается ввести новый тип subsubevent'ов в дополнение к имеющимся CAEN и MESYTEC. Назвать его можно, например, DAQSTAT. Особенностью data-слов (между header и footer) в таких subsubevent'ах будет то, что все их 32 бита будут передаваться дальше на обработку, без попытки выделить какие-либо значения, типа номер канала, значение ADC/QDC/TDC. В эту категорию попадают сообщения от счётчиков (CAEN V560) и машинное время от RIO. Желательно, чтобы header содержал длину subsubevent'а. Для машинного времени - 1, для CAEN V560 - количество работающих счётчиков. Эти числа должны задаваться в setup.xml как кол-во каналов.

TODO
====

0) Причесать вывод так, чтобы он был читаем для нового пользователя. Особое внимание требуется уделить сообщениям о каких-либо несоответствиях, ошибкам и предупреждениям.

1) Реализовать какую-то осмысленную обработку пустых событий, которые иногда выдаются MbsAPI. Такое, по-видимому, бывает на стыке файлов. В частности, я наблюдал их в начале файла.

2) Реализовать проверки входных данных с примененим информации, импортированной из usr/electronics.xml. В частности, можно проверять правильность данных в setup.xml и сами поступающие данные. Например, элементарно, можно проверять что номер канала, вычитанного из сообщения, допустим для данного типа блока электроники. Также можно проверять, что диапазон каналов, заданный для какого-либо блока в setup.xml, является допустимым для такого типа блоков.

3) Сделать красивые подписи на двумерке fHistoAddrVsProcid в классе UserProcLearn.

4) Реализовать ProcessUnmappedChannels для отфильтровки и вывода каналов с мэппингом, но без данных.

5) Придумать настроечный макрос для включения/выключения всяких опций типа вывода summary, включения/выключения этапов (learn).
