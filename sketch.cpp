#include <Arduino.h>
#include <variant>
#include <iostream>
#include <concepts>
#include <type_traits>

// Define a C++20 concept
template <typename T>
concept Numeric = std::is_arithmetic_v<T>;

// A function template that uses the concept
template <Numeric T>
T addOne(T value) {
  return value + 1;
}

// A simple test function that uses std::variant
void testVariant() {
  std::variant<int, float, std::string> v;
  v = 42;
  Serial.print("Variant contains int: ");
  Serial.println(std::get<int>(v));
  
  v = 3.14f;
  Serial.print("Variant contains float: ");
  Serial.println(std::get<float>(v));
  
  v = "hello";
  Serial.print("Variant contains string: ");
  Serial.println(std::get<std::string>(v).c_str());
}

// Function to test C++20 concepts
void testConcept() {
  int i = 41;
  float f = 2.14f;
  
  Serial.print("Using concept with int: ");
  Serial.println(addOne(i));
  
  Serial.print("Using concept with float: ");
  Serial.println(addOne(f));
  
  // This would fail to compile if uncommented, as String is not Numeric
  // String s = "hello";
  // addOne(s);
}

void setup() {
    pinMode(22, OUTPUT);
    Serial.begin(9600);
    while(!Serial); // Wait for Serial to be ready
    Serial.println("Testing std::variant (C++20 feature)");
    testVariant();
    
    Serial.println("\nTesting C++20 concepts");
    testConcept();
}

void loop() {
    digitalWrite(22, HIGH);
    delay(50);
    digitalWrite(22, LOW);
    delay(100);
}
