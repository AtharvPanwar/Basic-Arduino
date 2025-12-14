// 1. Include the necessary Libraries
#include <Keypad.h>
#include <LiquidCrystal.h>

// 2. Define LCD Pin Connections (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// 3. Define Keypad Configuration (SAME AS BEFORE)
const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four columns

// Define the keys on the 4x4 keypad (operators A, B, C, D are now assigned)
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'}, // A = +
  {'4','5','6','B'}, // B = -
  {'7','8','9','C'}, // C = *
  {'*','0','#','D'}  // D = /
};

// Define the connections for rows and columns (MATCHING WIRING)
byte rowPins[ROWS] = {A0, A1, A2, A3}; 
byte colPins[COLS] = {6, 7, 8, 9};    

// Initialize the Keypad object
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// 4. Global Variables for calculation and state
float number1 = 0; // Changed to float to handle division results
float number2 = 0;
float result = 0;
char operation = ' ';
bool newNumber = true; 

void setup() {
  lcd.begin(16, 2);
  resetCalculator(); 
}

void loop() {
  char customKey = customKeypad.getKey();

  if (customKey) {
    
    // --- 1. HANDLE NUMBER INPUT (0-9) ---
    if (customKey >= '0' && customKey <= '9') {
      int num = customKey - '0'; 

      if (newNumber) {
        number1 = number1 * 10 + num;
        printInput(number1, 0, true); 
      } else {
        number2 = number2 * 10 + num;
        printInput(number2, 1, true); 
      }
    }
    
    // --- 2. HANDLE OPERATOR INPUT (A, B, C, D) ---
    else if (customKey == 'A' || customKey == 'B' || customKey == 'C' || customKey == 'D') {
      if (newNumber) {
        // Store the operator based on the key pressed
        if (customKey == 'A') operation = '+';
        else if (customKey == 'B') operation = '-';
        else if (customKey == 'C') operation = '*';
        else if (customKey == 'D') operation = '/';
        
        newNumber = false; // Switch to entering number2
        
        // Update display to show operator and prompt for N2
        lcd.setCursor(15, 0);
        lcd.print(operation);
        lcd.setCursor(0, 1);
        lcd.print("N2:");
      }
    }
    
    // --- 3. HANDLE EQUALS ('#') ---
    else if (customKey == '#') {
      performCalculation();
    }
    
    // --- 4. HANDLE CLEAR/RESET ('*') ---
    else if (customKey == '*') {
      resetCalculator();
    }
  }
}

// Function to display the input number on a specific row
void printInput(float num, int row, bool clearLine) {
  // Clear the line if requested
  if (clearLine) {
    lcd.setCursor(3, row); 
    lcd.print("          "); 
  }
  
  // Print the number
  lcd.setCursor(3, row);
  // Using 0 decimal places for cleaner display of integers
  lcd.print(num, 0); 
}

// Function to perform the calculation using a switch statement
void performCalculation() {
  // Use a switch statement to handle all four operations
  switch (operation) {
    case '+':
      result = number1 + number2;
      break;
    case '-':
      result = number1 - number2;
      break;
    case '*':
      result = number1 * number2;
      break;
    case '/':
      // Handle division by zero
      if (number2 != 0) {
        result = number1 / number2;
      } else {
        lcd.clear();
        lcd.print("ERROR: Div by Zero!");
        delay(2000);
        resetCalculator();
        return; 
      }
      break;
    default:
      // If no valid operation was selected
      return;
  }
  
  // Display the final result
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(number1, 0); // Print N1 (as integer for neatness)
  lcd.print(operation);
  lcd.print(number2, 0); // Print N2
  lcd.print("=");
  
  lcd.setCursor(0, 1);
  lcd.print("Result: ");
  // Display result with 2 decimal places if it's not an integer
  if (result == (long)result) {
      lcd.print(result, 0); 
  } else {
      lcd.print(result, 2); 
  }

  
  // Prepare for the next calculation
  number1 = result;
  number2 = 0;
  operation = ' ';
  newNumber = true; 
}

// Function to reset all variables and display
void resetCalculator() {
  number1 = 0;
  number2 = 0;
  result = 0;
  operation = ' ';
  newNumber = true;
  
  lcd.clear();
  lcd.print("4-Function Calc");
  lcd.setCursor(0, 1);
  lcd.print("N1:");
}
