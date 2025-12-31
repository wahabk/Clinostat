/*
 * Test program to read path.txt from SD card and print it to Serial
 * This helps verify file format and SD card access before attempting actual path execution
 */

#include <SPI.h>
#include <SD.h>

File pathFile;
const int chipSelect = 10; // Change this to match your SD shield or module
const char* PATH_FILENAME = "path.txt"; // Define filename in one place for consistency

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  while (!Serial) {
    ; // Wait for serial port to connect (needed for native USB port only)
  }

  Serial.println("\n\nOpenClino Path Reader Test");
  Serial.println("-------------------------");

  // Initialize SD card
  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("Failed!");
    Serial.println("* Is the SD card inserted?");
    Serial.println("* Is the wiring correct?");
    Serial.println("* Is the chipSelect pin correct?");
    return;
  }
  Serial.println("Done.");

  // Open the path file
  pathFile = SD.open(PATH_FILENAME);
  if (pathFile) {
    Serial.println("Successfully opened path.txt");
    Serial.println("File contents:");
    Serial.println("-------------------------");
    
    // Print the raw file content
    while (pathFile.available()) {
      Serial.write(pathFile.read());
    }
    pathFile.close();
    
    Serial.println("\n-------------------------");
    Serial.println("Now parsing the file line by line:");
    
    // Reopen the file to read again
    pathFile = SD.open(PATH_FILENAME);
    if (pathFile) {
      int lineCount = 0;
      
      // Read and parse each line
      while (pathFile.available()) {
        String line = pathFile.readStringUntil('\n');
        line.trim(); // Remove any whitespace/newlines
        
        if (line.length() > 0) {
          lineCount++;
          
          // Parse the space-delimited values
          int firstSpace = line.indexOf(' ');
          
          if (firstSpace > 0) {
            float xPos = line.substring(0, firstSpace).toFloat();
            float yPos = line.substring(firstSpace + 1).toFloat();
            
            Serial.print("Line ");
            Serial.print(lineCount);
            Serial.print(": X=");
            Serial.print(xPos);
            Serial.print(", Y=");
            Serial.println(yPos);
          }
          else {
            Serial.print("Line ");
            Serial.print(lineCount);
            Serial.print(": Invalid format - ");
            Serial.println(line);
          }
        }
      }
      
      pathFile.close();
      Serial.println("-------------------------");
      Serial.print("Total positions: ");
      Serial.println(lineCount);
    }
    else {
      Serial.println("Error reopening file for parsing");
    }
  }
  else {
    Serial.println("Error opening path.txt");
    Serial.println("Make sure the file exists on the SD card");
    
    // List all files on the SD card to help troubleshoot
    Serial.println("\nFiles found on the SD card:");
    Serial.println("-------------------------");
    File root = SD.open("/");
    printDirectory(root, 0);
  }
}

void loop() {
  // Nothing to do in the loop
}

// Function to print directory contents, including subdirectories
void printDirectory(File dir, int numTabs) {
  while (true) {
    File entry = dir.openNextFile();
    if (!entry) {
      // No more files
      break;
    }
    
    // Print tabs for subdirectories
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    
    // Print the name
    Serial.print(entry.name());
    
    // Show if it's a directory or print the file size
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // Files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}
