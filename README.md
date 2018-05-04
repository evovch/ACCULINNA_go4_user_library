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

A ROOT macro 'macros/analyse.C' is provided as a template to start your simple analysis by means of native ROOT. Please, follow the instructions written at the beginning of the file.

> cd macros
> root -l "analyse.C(\"filename.lmd.root\")" > macroout.txt 2> macroerr.txt

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

Project structure
=================

UserAnalysis
------------

### Unpacking

The first step of the analysis. Only reads raw data and extracts raw messages without mapping them to any detector. This step is able to produce a ROOT tree with unpacked raw messages (together with raw words) to be processed by any software.

### Monitoring

The second step of the analysis. Looks into the setup configuration and produces a ROOT tree ...

### Step2

Obsolete. A very primitive analysis step for MWPC decoding. Already implemented in the 'monitoring' step together with the rest of the setup.

### Learn

A subsystem which allows to perform analysis of a rather small portion of the input data (TODO implement nEvents limit) and generate a summaryLearn.txt file which contains information about which procID's (crates), addressed (geo/module) and electronics channels produce data. This information can be then compared to the setup file and create warnings abount unmapped channels or mapped channels with no information.

### Mesh

A small subsystem which is required by Go4 and used by UserAnalysis class to create a mesh-like structure of the analysis. This allows, in particular, to split the unpacked, but not mapped data into two or more streams to be processed by separate processors (i.e. monitoring and learn).

SetupConfig
-----------

A pure "C" sybsystem which performs import of the setup configuration XML. This code conforms c89 standard which makes it compilable by the RIO3 (RIO4?) VME controllers. It is planned that this subsystem will be also used to configure the DAQ hardware.

SetupConfigCppWrapper
---------------------

"C++" wrapper around SetupConfig for easier usage in this project.

usr
---

Directory for XML files, including setup configuration XML files.

textoutput
----------

Different text output files produced by the analysis. This directory must exist in order for the usr library to word. Please, use provided script to compile/clean/run anlysis. They take care.

macros
------

Directory for user macros.


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

Предлагается ввести новый тип subsubevent'ов в дополнение к имеющимся CAEN и MESYTEC. Назвать его можно, например, DAQSTAT. Особенностью data-слов (между header и footer) в таких subsubevent'ах будет то, что все их 32 бита будут передаваться дальше на обработку, без попытки выделить какие-лио значения, типа номер канала, значение ADC/QDC/TDC. В эту категорию попадают сообщения от счётчиков (CAEN V560) и машинное время от RIO. Желательно, чтобы header содержал длину subsubevent'а. Для машинного времени - 1, для CAEN V560 - количество работающих счётчиков. Эти числа должны задаваться в setup.xml как кол-во каналов.

TODO
====

0) Причесать вывод так, чтобы он был читаем для нового пользователя. Особое внимание требуется уделить сообщениям о каких-либо несоответствиях, ошибкам и предупреждениям.

1) Реализовать какую-то осмысленную обработку пустых событий, которые иногда выдаются MbsAPI. Такое, по-видимому, бывает на стыке файлов. В частности, я наблюдал их в начале файла.

2) Реализовать проверки входных данных с примененим информации, импортированной из usr/electronics.xml. В частности, можно проверять правильность данных в setup.xml и сами поступающие данные. Например, элементарно, можно проверять что номер канала, вычитанного из сообщения, допустим для данного типа блока электроники. Также можно проверять, что диапазон каналов, заданный для какого-либо блока в setup.xml, является допустимым для такого типа блоков.

3) Сделать красивые подписи на двумерке fHistoAddrVsProcid в классе UserProcLearn.

4) Реализовать ProcessUnmappedChannels для отфильтровки и вывода каналов с мэппингом, но без данных.

5) Придумать настроечный макрос для включения/выключения всяких опций типа вывода summary, включения/выключения этапов (learn).

