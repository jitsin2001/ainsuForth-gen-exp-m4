// Thu Aug 29 04:31:31 UTC 2019
// 4737-a3c-00g- // ainsuForth-gen-exp-m4

#ifdef HAS_EXP_MFOUR_QSPI_FLASH
#define CURRENT_FILESYSTEM fatfs

#define SPI_FlashROM_FILENAME "/forth/ascii_xfer_a001.txt"
#define SPI_FlashROM_TOPDIR   "/forth"
#define WORKING_DIR "/forth"

#include <Arduino.h>
#include "../../yaffa.h"

#ifdef EXT_KERN_GETLINE
#include "getline.h"
#ifdef HAS_QSPI_FLASHROM_LIB
#warning HAS_QSPI_FLASHROM_LIB includes extra code on line 16 __getline_m4_exp.cpp__
#define FLASH_DEVICE_GD25Q
#ifdef FLASH_DEVICE_GD25Q

#include <SPI.h>
#include <SdFat.h>
#include <Adafruit_SPIFlash.h>

#if defined(__SAMD51__) || defined(NRF52840_XXAA)
Adafruit_FlashTransport_QSPI flashTransport(PIN_QSPI_SCK,
    PIN_QSPI_CS, PIN_QSPI_IO0, PIN_QSPI_IO1, PIN_QSPI_IO2,
      PIN_QSPI_IO3);
#else
#if (SPI_INTERFACES_COUNT == 1)
Adafruit_FlashTransport_SPI flashTransport(SS, &SPI);
#else
Adafruit_FlashTransport_SPI flashTransport(SS1, &SPI1);
#endif
#endif

Adafruit_SPIFlash flash(&flashTransport);
FatFileSystem fatfs;
File myFile;

#endif

/******************************************************************************/
/** getDLKey                                                                 **/
/**   waits for the next valid key to be entered in download mode (DL)       **/
/**   and return its value                                                   **/
/**   Watches for code-escape key '\' (backslash) to detect the '\end.' word **/
/**   Valid characters are:  Backspace, Carriage Return (0x0d), Escape,      **/
/**   Tab, Newline (0x0a) and standard (printable) characters                **/
/******************************************************************************/

void setup_qspiFlashROM(void) {
	Serial.print("Hello from setup_qspi m4 getline stuff.   ");

	Serial.print("VALID m4 getline c9vj  ");

	if (!flash.begin()) {
		Serial.println("E: could not find flash on QSPI bus.");
		while(1);
	}
	Serial.println("Found QSPI Flash.");
	// Serial.println(flash.GetJEDECID(), HEX);

	if (!CURRENT_FILESYSTEM.begin(&flash)) {
		Serial.println("Failed to mount filesystem!");
		Serial.println("Was CircuitPython loaded on the board first to create the filesystem?");
		while(1);
	}
	Serial.println("NOV 2018: Mounted filesystem!");
	}

/******************************************************************************/
/** getLine                                                                  **/
/**   read in a line of text ended by a Carriage Return (ASCII 13)           **/
/**   Valid characters are:  Backspace, Carriage Return, Escape, Tab, and    **/
/**   standard printable characters. Passed the address to store the string, **/
/**   and Returns the length of the string stored                            **/
/******************************************************************************/

#ifdef HAS_QSPI_FLASH_DEMO
File thisFile;
#endif


	uint8_t getLine(char* ptr, uint8_t buffSize) {
		char inChar;
		uint8_t count = 0;

#ifdef HAS_QSPI_FLASH_DEMO
		if (spiFlashReading) {
			if (CURRENT_FILESYSTEM.exists(SPI_FlashROM_FILENAME)) {
				if (fileClosed) {
					File bootPy = CURRENT_FILESYSTEM.open(SPI_FlashROM_FILENAME, FILE_READ);
					thisFile = (File) bootPy;
					fileClosed = FALSE ;
				}
			}
		}
#endif

// another getLine() stanza:
		do {
#ifdef HAS_QSPI_FLASH_DEMO
			if (spiFlashReading) {
				if (thisFile) {
					if (thisFile.available()) {
						// this is where every character of job.fs is read from SPI flash:
						inChar = thisFile.read();

						spiFlashWaiting = FALSE;
						if (thisFile.available()) {
							spiFlashWaiting = TRUE;
						} else {
							fileClosed = TRUE; // want to close the file - set a flag
							spiFlashReading = FALSE; // shutdown the reading mechanism
						}
					}
				}

			} else {
#endif

				if (noInterpreter) {
					inChar = getKey();
					if (inChar == '\\') {
						// Serial.print("ESC \\ SEEN in getLine().\r\n");
					}
				} else {
					inChar = getKey(); 
				}
#ifdef HAS_QSPI_FLASH_DEMO
			}
#endif

			// inChar is now populated; either by keypress or by byte stored in SPI flash.

			if (inChar == ASCII_BS || inChar == ASCII_DEL) {  // new: was only ASCII_BS
				if (count) {
					*--ptr = 0;
					count--; // ainsuForth improvement -- backspace behavior


					if (silentReading) { } else {
						if (flags & ECHO_ON) Serial.print("\b \b");
					}
				}
			}
			else if (inChar == ASCII_TAB || inChar == ASCII_ESC) {
				if (silentReading) { } else {
					if (flags & ECHO_ON) Serial.print("\a");
				}
			}
			else if (inChar == ASCII_CR || inChar == ASCII_NL) { // ainsuForth improvement

				// iirc, this was for use during colon definition upload,
				// using copy and paste into the serial terminal.

				// Basis: lack of newline sensitivity; added some.

				// Test: revert, attempt a multi-line code paste.  If
				// it fails or behaves poorly, reenable this code.
				// Compare the two behaviors.

				// iirc, interactive typing to the interpreter masks
				// this behavior, whereas a paste-in unmasks it.

				if (silentReading) { // the 'load' word
				} else {
					if (flags & ECHO_ON) Serial.print(" "); // seems to want a space after 'dot' for example.
				}
				// -----------------------------------------------------
				// TRY TO REMEMBER:
				// 
				// The mechanism to end a file upload to the SPI Flash
				// is a parsed word '\end.'

				// Whereas .. the mechanism to end a 'load' (compile
				// into VM code, stored in RAM -- from a disk file forth
				// source program that's stored on SPI FlashROM)
				// 
				// That mechanism is simply to detect if flash is waiting
				// (if we have reached EOF or not while reading a file
				// from the SPI FlashROM chip).
				// -----------------------------------------------------

				if (spiFlashWaiting) {
					// Serial.println("Flash is WAITING .. more to read.");
				} else { 
					silentReading = FALSE; // we are interactive once more, when spiFlashWaiting changes state
				}
				break;
			} else {
				// always suppress echo when the load word is executed.
				if (silentReading && spiFlashReading) { // the 'load' word
					int fakeSPxT = 0;
				} else {
					if (flags & ECHO_ON) {
						if (silentReading) {
						} else {
							// main forth interpreter typing echo is right here:
							Serial.print(inChar); // do NOT suppress this ordinarily, if ever.
						}
					}
				}

				*ptr++ = inChar;
				*ptr = 0;
				count++;
			}
			} while (count < buffSize);
			return (count);
		}

#endif
#endif // #ifdef EXT_KERN_GETLINE


#ifdef EXT_KERN_GETKEY

// ###bookmark
/******************************************************************************/
/** getKey                                                                   **/
/**   waits for the next valid key to be entered and return its value        **/
/**   Valid characters are:  Backspace, Carriage Return (0x0d), Escape,      **/
/**   Tab, Newline (0x0a) and standard (printable) characters                **/
/******************************************************************************/
char getKey(void) {
			char inChar;
			// the load word provides this boolean:
			if (spiFlashReading) {
				if (inChar == ASCII_BS  ||
						inChar == ASCII_TAB ||
						inChar == ASCII_CR  ||  
						inChar == ASCII_NL  ||   // new
						inChar == ASCII_DEL ||   // new
						inChar == ASCII_ESC ||
						isprint(inChar)) {
					return inChar;
				}
			} else {
				while (1) {
					if (Serial.available()) {
						inChar = Serial.read();
						if (inChar == ASCII_BS  ||
								inChar == ASCII_TAB ||
								inChar == ASCII_CR  ||  
								inChar == ASCII_NL  ||   // new
								inChar == ASCII_DEL ||   // new
								inChar == ASCII_ESC ||
								isprint(inChar)) {
							return inChar;
						}
					}
				}
			}
		}

#endif






		//  ---------------------------    ascii  transfer   --------------------------

		const char eflmkdir_str[] = "eflmkdir"; // forth vocabulary external flash mkdir
		void _eflmkdir(void) {

#ifdef HAS_QSPI_FLASH_DEMO

			if (!CURRENT_FILESYSTEM.exists(SPI_FlashROM_TOPDIR)) {
				Serial.println("/forth directory not found, creating...");
				if (!CURRENT_FILESYSTEM.mkdir(SPI_FlashROM_TOPDIR)) {
					Serial.println("Error, failed to create test directory!");
					while(1);
				}
				Serial.println("Created /forth directory!");
			}
#endif




#ifdef NEVER_DEFINED_TEN_THREE // nonsense tag to prevent compile
#ifndef HAS_STANDARD_BUILD_HERE

#ifdef HAS_QSPI_FLASH_DEMO
			if (!CURRENT_FILESYSTEM.exists(SPI_FlashROM_TOPDIR)) {
				Serial.println("BAD ROBOT - fatfs.exists fails on line 473 June 17, 2018.");
			} else {
				Serial.println("local: assuming /forth directory already exists.");
			}
#endif

#endif
#endif
			}


#ifdef HAS_QSPI_FLASH_DEMO
			void remove_a_file(void) {
				Serial.print("file: Deleting ");
				Serial.print(SPI_FlashROM_FILENAME);
				Serial.println("...");

				if (!CURRENT_FILESYSTEM.remove(SPI_FlashROM_FILENAME)) {
					Serial.print("Error, file ");
					Serial.print(SPI_FlashROM_FILENAME);
					Serial.println(" was not removed!");
					while(1);
				}
				Serial.println("Deleted file!");
				File writeFile = CURRENT_FILESYSTEM.open(SPI_FlashROM_FILENAME, FILE_WRITE);

				if (!writeFile) {
					Serial.print("Error, failed to open ");
					Serial.print(SPI_FlashROM_FILENAME);
					Serial.println(" for writing!");
					while(1); // what does this do .. hold the program in a forever loop upon failure?
					Serial.println("Exiting forever loop of getline.cpp -- probably means a serious error occurred.");
				} else {
					Serial.println("An empty new file was created in its place.");
					Serial.println("This kludge will go away when multi-filename usage is more fully integrated.");
					writeFile.println(" ");
					writeFile.close(); // housekeeping.
				}
			}
#endif

#ifdef HAS_QSPI_FLASH_DEMO
			void write_a_capture_file(void) {
				// Create a file in the test directory and write data to it.
				// Note the FILE_WRITE parameter which tells the library you intend to
				// write to the file.  This will create the file if it doesn't exist,
				// otherwise it will open the file and start appending new data to the
				// end of it.  More advanced users can specify different file modes by
				// using the FatFs f_open modes directly (which can be specified instead
				// of FILE_WRITE), see documentation at:
				//   http://elm-chan.org/fsw/ff/en/open.html

				File writeFile =               CURRENT_FILESYSTEM.open(SPI_FlashROM_FILENAME, FILE_WRITE);
				if (!writeFile) {
					Serial.print("Error, failed to open ");
					Serial.print(SPI_FlashROM_FILENAME);
					Serial.println(" for writing!");
					while(1);
				}



				writeFile.println(cpSource);

				// Close the file when finished writing.
				writeFile.close();
			}
#endif


#ifndef HAS_STANDARD_BUILD_HERE

#ifdef HAS_QSPI_FLASH_DEMO
			void read_a_test_file(void) {
				// Now open the same file but for reading.
				File readFile = CURRENT_FILESYSTEM.open(SPI_FlashROM_FILENAME, FILE_READ);
				if (!readFile) {
					Serial.print("Error, failed to open ");
					Serial.print(SPI_FlashROM_FILENAME);
					Serial.println(" for reading!");
					while(1);
				}
				// Read data using the same read, find, readString, etc. functions as when using
				// the serial class.  See SD library File class for more documentation:
				//   https://www.arduino.cc/en/reference/SD
				// Read a line of data:
				String line = readFile.readStringUntil('\n');
				Serial.print("First line of ");
				Serial.print(SPI_FlashROM_FILENAME);
				Serial.println(line);
				// You can get the current position, remaining data, and total size of the file:
				Serial.print("Ignore job.fs and say ");
				Serial.print(SPI_FlashROM_FILENAME);
				Serial.print(" here - several lines.");
				Serial.print("Total size of job.fs (bytes): "); Serial.println(readFile.size(), DEC);
				Serial.print("Current position in job.fs: "); Serial.println(readFile.position(), DEC);
				Serial.print("Available data to read in job.fs: "); Serial.println(readFile.available(), DEC);

				// And a few other interesting attributes of a file:
				Serial.print("File name: "); Serial.println(readFile.name());
				Serial.print("Is file a directory? "); Serial.println(readFile.isDirectory() ? "Yes" : "No");

				// You can seek around inside the file relative to the start of the file.
				// For example to skip back to the start (position 0):
				if (!readFile.seek(0)) {
					Serial.println("Error, failed to seek back to start of file!");
					while(1);
				}

				// And finally to read all the data and print it out a character at a time
				// (stopping when end of file is reached):
				Serial.println("Entire contents of job.fs:");
				while (readFile.available()) {
					char c = readFile.read();
					Serial.print(c);
				}

				// Close the file when finished reading.
				readFile.close();
			}
#endif
#endif

#ifdef HAS_QSPI_FLASH_DEMO


#ifndef HAS_STANDARD_BUILD_HERE
			void read_from_code_py_file(void) {
				File readCodeFile = CURRENT_FILESYSTEM.open("/main.py", FILE_READ);
				if (!readCodeFile) {
					Serial.println("Error, failed to open code.py for reading!");
					while(1);
				}

				String line = readCodeFile.readStringUntil('\n');
				Serial.print("First line of code.py: "); Serial.println(line);

				Serial.print("Total size of code.py (bytes): "); Serial.println(readCodeFile.size(), DEC);
				Serial.print("Current position in code.py: "); Serial.println(readCodeFile.position(), DEC);
				Serial.print("Available data to read in code.py: "); Serial.println(readCodeFile.available(), DEC);

				Serial.print("File name: "); Serial.println(readCodeFile.name());
				Serial.print("Is file a directory? "); Serial.println(readCodeFile.isDirectory() ? "Yes" : "No");

				if (!readCodeFile.seek(0)) {
					Serial.println("Error, failed to seek back to start of file!");
					while(1);
				}

				Serial.println("Entire contents of code.py:");
				while (readCodeFile.available()) {
					char c = readCodeFile.read();
					Serial.print(c);
				}

				readCodeFile.close();
			}
#endif
#endif


























#ifdef HAS_QSPI_FLASH_DEMO
			void tail_code_bb(void) {
#ifndef HAS_STANDARD_BUILD_HERE
				File testDirRoot = CURRENT_FILESYSTEM.open("/");
				if (!testDirRoot) {
					Serial.println("Error, failed to open root directory!");
					while(1);
				} else {
					Serial.println("Made it past the opening of the root directory.");
				}
#endif

#ifdef HAS_STANDARD_BUILD_HERE
				File testDir = CURRENT_FILESYSTEM.open("/lib");
				if (!testDir) {
					Serial.println("Error, failed to open test directory!");
					while(1);
				} else {
					Serial.println("Made it past the opening of the /lib directory.");
				}
#endif

#ifndef HAS_STANDARD_BUILD_HERE
				if (!testDirRoot.isDirectory()) {
					Serial.println("Error, expected root to be a directory!");
					while(1);
				} else {
					Serial.println("Good - root is a directory - not a file.  Continue.");
				}
#endif

#ifdef HAS_STANDARD_BUILD_HERE
				if (!testDir.isDirectory()) {
					Serial.println("Error, expected /lib to be a directory!");
					while(1);
				} else {
					Serial.println("Good - /lib is a directory - not a file.  Continue.");
				}
#endif

#ifndef HAS_STANDARD_BUILD_HERE
				Serial.println("Listing children of root directory:");
				File child = testDirRoot.openNextFile();
				while (child) {
					Serial.print("- "); Serial.print(child.name());
					if (child.isDirectory()) {
						Serial.print(" (directory)");
					}
					Serial.println();
					child = testDirRoot.openNextFile();
				}

				testDirRoot.rewindDirectory();
#endif


				while(1); Serial.println("WILL ROBINSON.");



#ifdef HAS_STANDARD_BUILD_HERE
				Serial.println("Listing children of directory /lib:");
				File child = testDir.openNextFile();
				while (child) {
					Serial.print("- "); Serial.print(child.name());
					if (child.isDirectory()) {
						Serial.print(" (directory)");
					}
					Serial.println();
					child = testDir.openNextFile();
				}


				testDir.rewindDirectory();
#endif



#ifdef HAS_EXTRA_STANDARD_BUILD_HERE

				File test2File = CURRENT_FILESYSTEM.open("/forth/foo/test2.txt", FILE_WRITE);
				test2File.close();
				Serial.println("Deleting /forth/foo/test2.txt...");
				if (!CURRENT_FILESYSTEM.remove("/forth/foo/test2.txt")) {
					Serial.println("Error, couldn't delete test.txt file!");
					while(1);
				}
				Serial.println("Deleted file!");

				Serial.println("Deleting /test directory and everything inside it...");
				if (!CURRENT_FILESYSTEM.rmdir("/test")) {
					Serial.println("Error, couldn't delete test directory!");
					while(1);
				}
				if (CURRENT_FILESYSTEM.exists("/test")) {
					Serial.println("Error, test directory was not deleted!");
					while(1);
				}
				Serial.println("Test directory was deleted!");

				Serial.println("Finished!");
#endif

#ifndef HAS_STANDARD_BUILD_HERE
#endif

			}
#endif


































































			void ascii_xfer_spi_flash_main(void) {
				read_from_code_py_file(); 
				tail_code_bb();

			}


#endif // #ifdef HAS_EXP_MFOUR_QSPI_FLASH - entire file omitted when this flag is set

			// END.
