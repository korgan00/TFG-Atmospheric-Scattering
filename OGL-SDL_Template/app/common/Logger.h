#ifndef LOGGER_H_
#define LOGGER_H_
#include <iostream>
#include <string>
#include <fstream>

#ifdef _WIN32
#include <windows.h>
#endif

#include <stdlib.h>
using namespace std;

class Log {
private:
	static void changeColor(int i) {
#ifdef _WIN32
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), i);
#endif
	}
	static void restoreColor() {
#ifdef _WIN32
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
#endif
	}

public:
	enum logLevel { TRACE = 0, DEBUG = 1, INFO = 2, WARN = 3, ERR = 4 };
	static logLevel currentLogLevel;

	static void error(string msg) {
		if (currentLogLevel <= ERR) {
			changeColor(0x0C);
			cerr << "!! ERROR: " << msg << endl;
			restoreColor();
		}
	}
	static void warning(string msg) {
		if (currentLogLevel <= WARN) {
			changeColor(0x0E);
			cerr << "!  WARN : " << msg << endl;
			restoreColor();
		}
	}
	static void info(string msg) {
		if (currentLogLevel <= INFO) {
			changeColor(0x0A);
			cout << "+  Info: " << msg << endl;
			restoreColor();
		}
	}
	static void debug(string msg) {
		if (currentLogLevel <= DEBUG) {
			changeColor(0x0B);
			cout << "-  debug: " << msg << endl;
			restoreColor();
		}
	}
	static void trace(string msg) {
		if (currentLogLevel <= TRACE) {
			changeColor(0x0F);
			cout << ".  trace: " << msg << endl;
			restoreColor();
		}
	}
};

#endif /* LOGGER_H_ */