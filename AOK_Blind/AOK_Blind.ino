/*
******************************************************************************************************************************************************************
*
* A-OK AC114-01B remote control (RF 433.92MHz) for roller shades and motorized projector screens
* Control code by Antti Kirjavainen (antti.kirjavainen [_at_] gmail.com)
* 
* https://www.a-okmotors.com/en/
* 
* This is an incomplete, but working implementation of the A-OK protocol.
* 
* 
* HOW TO USE
* 
* Capture your remote controls with RemoteCapture.ino and copy paste the 65 bit commands to A-OK.ino
* for sendAOKCommand(). More info about this provided in RemoteCapture.ino.
* 
* 
* HOW TO USE WITH EXAMPLE COMMANDS
* 
* 1. Set the motor or receiver into pairing mode with its PROGRAM button.
* 2. Check the instructions on whether you need to send the pairing command
* ("sendAOKCommand(AOK_PROGRAM_EXAMPLE);") or UP command ("sendAOKCommand(AOK_UP_EXAMPLE);").
* Typically you need to transmit within 10 seconds.
* 3. Now you can control the motor, e.g. "sendAOKCommand(AOK_DOWN_EXAMPLE);" (or AOK_UP_EXAMPLE, AOK_STOP_EXAMPLE etc.).
* 
* Setting limits for roller shade motors is quicker with the remotes, although you can use your Arduino
* for that as well.
*
*
* PROTOCOL DESCRIPTION
* 
* One remote control is not enough data to understand the protocol, but here's what I've
* figured out so far.
* 
* UP and DOWN buttons send two different commands for some reason, listed below
* as AOK_UP/DOWN_EXAMPLE and AOK_AFTER_UP_DOWN_EXAMPLE. However, the latter command
* would not seem to be required at all.
* 
* Tri-state bits are used.
* 
* AGC:
* Some remotes start the first command with a preamble of 8 times: HIGH of approx. 340 us + LOW of approx. 520 us
* But most remotes do not transmit that preamble.
* Every remote starts the command with an AGC HIGH of approx. 5200 us.
* Then go LOW for approx. 530 us and start the commands bits.
* 
* COMMAND BITS:
* 49 bits for a unique remote ID
* 7 bits for command (UP = 0001011, DOWN = 1000011, STOP = 0100011, PROGRAM = 1010011, AFTER UP/DOWN = 0100100)
* 5 bits are unknown (maybe some kind of checksum?)
* 4 bits at the end are always 0111, except for the AFTER UP/DOWN commands 1001
* 
* = 65 bits in total
* 
* RADIO SILENCE:
* Some remotes instantly repeat the commands, some transmit a radio silence of approx. 5030 us at the end
* of each command.
* 
* 
* TIMINGS:
* All sample counts below listed with a sample rate of 44100 Hz
* (sample count / 44100 = microseconds).
* 
* Pulse length:
* SHORT = 272 us (12 samples)
* LONG = 567 us (25 samples, approx. 2 * SHORT)
*
* Data bits:
* Data 0 = short HIGH, long LOW (wire 100)
* Data 1 = long HIGH, short LOW (wire 110)
* 
* This code transmits a radio silence of 5034 us (222 samples) after each command, although not all AC114-01B
* remotes do.
*
******************************************************************************************************************************************************************
*/



// Example commands:
int incomingByte;

#define AOK_DOWN_EXAMPLE              "10100011011000011011110110001111000000010000000001000011111100011"
#define AOK_UP_EXAMPLE                "10100011011000011011110110001111000000010000000000001011101110011"
#define AOK_AFTER_UP_DOWN_EXAMPLE     "10100011011000011011110110001111000000010000000000100100110100101"
#define AOK_STOP_EXAMPLE              "10100011011000011011110110001111000000010000000000100011110100011"
#define AOK_PROGRAM_EXAMPLE           "10100011010001100101000000010110000000010000000001010011000000001"
#define AOK_UP_LONGPRESS_EXAMPLE1     "10100011011000011011110110001111000000010000000010001011001110011"
#define AOK_UP_LONGPRESS_EXAMPLE2     "10100011011000011011110110001111000000010000000001010101000000111"
#define AOK_DOWN_LONGPRESS_EXAMPLE    "10100011011000011011110110001111000000010000000011000011011100011"
#define AOK_STOP_LONGPRESS_EXAMPLE1   "10100011011000011011110110001111000000010000000001011010000010001"
#define AOK_STOP_LONGPRESS_EXAMPLE2   "10100011011000011011110110001111000000010000000001010000111111101"

#define TRANSMIT_PIN             13      // We'll use digital 13 for transmitting
#define REPEAT_COMMAND           8       // How many times to repeat the same command: original remotes repeat 8 (multi) or 10 (single) times by default
#define DEBUG                    false   // Do note that if you add serial output during transmit, it will cause delay and commands may fail

// If you wish to use PORTB commands instead of digitalWrite, these are for Arduino Uno digital 13:
#define D13high | 0x20; 
#define D13low  & 0xDF; 

// Timings in microseconds (us). Get sample count by zooming all the way in to the waveform with Audacity.
// Calculate microseconds with: (samples / sample rate, usually 44100 or 48000) - ~15-20 to compensate for delayMicroseconds overhead.
// Sample counts listed below with a sample rate of 44100 Hz:
#define AOK_AGC1_PULSE                   5300  // 234 samples
#define AOK_AGC2_PULSE                   530   // 24 samples after the actual AGC bit
#define AOK_RADIO_SILENCE                5030  // 222 samples

#define AOK_PULSE_SHORT                  270   // 12 samples
#define AOK_PULSE_LONG                   565   // 25 samples, approx. 2 * AOK_PULSE_SHORT

#define AOK_COMMAND_BIT_ARRAY_SIZE       65    // Command bit count



// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
void setup() {

  Serial.begin(9600); // Used for error messages even with DEBUG set to false
      
  if (DEBUG) Serial.println("Starting up...");
}
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
void loop() {

  // Pair a motor/receiver (first set the receiver to pairing mode by holding down
  // its PROGRAM button), then send the PROGRAM or UP command (check the instructions
  // of your device on which one is required for pairing):
  
  //sendAOKCommand(AOK_PROGRAM_EXAMPLE); // AM25-1.2/30-MEL-P motor requires this for pairing
  if (Serial.available() > 0) {
    // read the oldest byte in the serial buffer:
    incomingByte = Serial.read();
    // if it's a capital H (ASCII 72), turn on the LED:
    if (incomingByte == 'U') {
      sendAOKCommand(AOK_UP_EXAMPLE); // AC202-02 receiver requires this for pairing
      sendAOKCommand(AOK_AFTER_UP_DOWN_EXAMPLE);
    }
    if (incomingByte == 'u') {
      sendAOKCommand(AOK_UP_EXAMPLE); 
      sendAOKCommand(AOK_UP_LONGPRESS_EXAMPLE1);
      sendAOKCommand(AOK_UP_LONGPRESS_EXAMPLE2);
    }
    if (incomingByte == 'D') {
      sendAOKCommand(AOK_DOWN_EXAMPLE); // AC202-02 receiver requires this for pairing
      sendAOKCommand(AOK_AFTER_UP_DOWN_EXAMPLE);
    }
    if (incomingByte == 'd') {
      sendAOKCommand(AOK_DOWN_EXAMPLE);
      sendAOKCommand(AOK_DOWN_EXAMPLE);
      sendAOKCommand(AOK_DOWN_LONGPRESS_EXAMPLE);
    }
    if (incomingByte == 'S') {
      sendAOKCommand(AOK_STOP_EXAMPLE); // AC202-02 receiver requires this for pairing
    }
    if (incomingByte == 's') {
      sendAOKCommand(AOK_STOP_EXAMPLE);
      sendAOKCommand(AOK_STOP_LONGPRESS_EXAMPLE1);
      sendAOKCommand(AOK_STOP_LONGPRESS_EXAMPLE2);// AC202-02 receiver requires this for pairing
    }
    if (incomingByte == 'N') {
      
    }
  }
  
  //sendAOKCommand(AOK_UP_EXAMPLE); // AC202-02 receiver requires this for pairing
  
  //while (true) {} // Stop after pairing, you can use UP/STOP/DOWN commands afterwards
  // ---

  // Send the command:
  //sendAOKCommand(AOK_DOWN_EXAMPLE);
  //sendAOKCommand(AOK_AFTER_UP_DOWN_EXAMPLE); // This doesn't seem to be required at all, so you can most likely skip it
  //delay(3000);
  //sendAOKCommand(AOK_UP_EXAMPLE);
  //sendAOKCommand(AOK_AFTER_UP_DOWN_EXAMPLE); // This doesn't seem to be required at all, so you can most likely skip it
  //delay(3000);
}
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
void sendAOKCommand(char* command) {
  
  if (command == NULL) {
    errorLog("sendAOKCommand(): Command array pointer was NULL, cannot continue.");
    return;
  }

  // Prepare for transmitting and check for validity
  pinMode(TRANSMIT_PIN, OUTPUT); // Prepare the digital pin for output
  
  if (strlen(command) < AOK_COMMAND_BIT_ARRAY_SIZE) {
    errorLog("sendAOKCommand(): Invalid command (too short), cannot continue.");
    return;
  }
  if (strlen(command) > AOK_COMMAND_BIT_ARRAY_SIZE) {
    errorLog("sendAOKCommand(): Invalid command (too long), cannot continue.");
    return;
  }
  
  // Repeat the command:
  for (int i = 0; i < REPEAT_COMMAND; i++) {
    doAOKTribitSend(command);
  }

  // Disable output to transmitter to prevent interference with
  // other devices. Otherwise the transmitter will keep on transmitting,
  // disrupting most appliances operating on the 433.92MHz band:
  digitalWrite(TRANSMIT_PIN, LOW);
}
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
void doAOKTribitSend(char* command) {

  // Starting (AGC) bits:
  transmitHigh(AOK_AGC1_PULSE);
  transmitLow(AOK_AGC2_PULSE);

  // Transmit command:
  for (int i = 0; i < AOK_COMMAND_BIT_ARRAY_SIZE; i++) {

      // If current bit is 0, transmit HIGH-LOW-LOW (100):
      if (command[i] == '0') {
        transmitHigh(AOK_PULSE_SHORT);
        transmitLow(AOK_PULSE_LONG);
      }

      // If current bit is 1, transmit HIGH-HIGH-LOW (110):
      if (command[i] == '1') {
        transmitHigh(AOK_PULSE_LONG);
        transmitLow(AOK_PULSE_SHORT);
      }   
   }

  // Radio silence at the end.
  // It's better to go a bit over than under minimum required length:
  transmitLow(AOK_RADIO_SILENCE);
  
  if (DEBUG) {
    Serial.println();
    Serial.print("Transmitted ");
    Serial.print(AOK_COMMAND_BIT_ARRAY_SIZE);
    Serial.println(" bits.");
    Serial.println();
  }
}
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
void transmitHigh(int delay_microseconds) {
  digitalWrite(TRANSMIT_PIN, HIGH);
  //PORTB = PORTB D13high; // If you wish to use faster PORTB calls instead
  delayMicroseconds(delay_microseconds);
}
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
void transmitLow(int delay_microseconds) {
  digitalWrite(TRANSMIT_PIN, LOW);
  //PORTB = PORTB D13low; // If you wish to use faster PORTB calls instead
  delayMicroseconds(delay_microseconds);
}
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
void errorLog(String message) {
  Serial.println(message);
}
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
