// Gemini config library
// Exposes a function `askForData(prompt)` which can be used to set parameters thru BLE Serial
// Can be used to tune Kp/Kd etc.

String askForData(String prompt) {
  Serial1.println(prompt); // print the display prompt

  while (Serial1.available() == 0) {}; // wait for incoming data
  String userInput = Serial1.readString(); // read the data
  
  return userInput;
}
