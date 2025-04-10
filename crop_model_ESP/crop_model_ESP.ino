#include "crop_model_ESP.h"
#include <tflm_esp32.h>
#include <eloquent_tinyml.h>

#define TF_NUM_OPS 2
#define ARENA_SIZE 16000
#define N_INPUTS 7
#define N_OUTPUTS 22
#define LED_PIN 2

Eloquent::TF::Sequential<TF_NUM_OPS, ARENA_SIZE> tf;

float input[N_INPUTS];
int currentFeature = 0;
bool readyForPrediction = false;
String inputBuffer = "";

const char* feature_names[N_INPUTS] = {
  "Nitrogen (kg/ha)", "Phosphorus (kg/ha)", "Potassium (kg/ha)",
  "Temperature (°C)", "Humidity (%)", "pH", "Rainfall (mm)"
};

// Min and max values for normalization
float feature_min[N_INPUTS] = {0, 5, 5, 8, 14, 3.5, 20};
float feature_max[N_INPUTS] = {140, 145, 205, 43, 100, 9.5, 300};

const char* crop_names[N_OUTPUTS] = {
  "apple", "banana", "blackgram", "chickpea", "coconut", "coffee", "cotton",
  "grapes", "jute", "kidneybeans", "lentil", "maize", "mango", "mothbeans",
  "mungbean", "muskmelon", "orange", "papaya", "pigeonpeas", "pomegranate",
  "rice", "watermelon"
};

void blink(int times, int delayTime = 200) {
  for (int i = 0; i < times; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(delayTime);
    digitalWrite(LED_PIN, LOW);
    delay(delayTime);
  }
}

void setup() {
  Serial.begin(115200);
  delay(3000);
  Serial.println("__Crop Recommendation on ESP32__");

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  tf.setNumInputs(N_INPUTS);
  tf.setNumOutputs(N_OUTPUTS);
  tf.resolver.AddFullyConnected();
  tf.resolver.AddSoftmax();

  while (!tf.begin(crop_model_ESP_tflite).isOk()) {
    Serial.println("Model loading failed:");
    Serial.println(tf.exception.toString());
    delay(500);
  }

  Serial.println("Model loaded.");
  askForNextInput();
}

void loop() {
  while (Serial.available()) {
    char c = Serial.read();

    if (c == '\n' || c == '\r') {
      if (inputBuffer.length() > 0) {
        float raw = inputBuffer.toFloat();
        float norm = normalize(raw, currentFeature);
        input[currentFeature] = norm;

        Serial.print("You entered: ");
        Serial.println(raw);

        inputBuffer = "";

        blink(1);  // Blink LED after each input
        currentFeature++;

        if (currentFeature < N_INPUTS) {
          askForNextInput();
        } else {
          readyForPrediction = true;
          currentFeature = 0;
        }
      }
    } else {
      inputBuffer += c;
    }
  }

  if (readyForPrediction) {
    runPrediction();
    readyForPrediction = false;
    askForNextInput();
  }
}

void askForNextInput() {
  Serial.println();  // Ensure input appears on a new line
  Serial.print("Enter ");
  Serial.print(feature_names[currentFeature]);
  Serial.println(":");
}

float normalize(float value, int index) {
  float minVal = feature_min[index];
  float maxVal = feature_max[index];
  return (value - minVal) / (maxVal - minVal);
}

void runPrediction() {
  Serial.println("\nRunning prediction...");

  if (!tf.predict(input).isOk()) {
    Serial.println("Prediction failed:");
    Serial.println(tf.exception.toString());
    return;
  }

  int maxIdx = 0;
  float maxVal = tf.output(0);
  for (int i = 1; i < N_OUTPUTS; i++) {
    if (tf.output(i) > maxVal) {
      maxVal = tf.output(i);
      maxIdx = i;
    }
  }

  blink(3, 150);  // Blink 3 times after prediction

  Serial.print("✅ Predicted crop: ");
  Serial.println(crop_names[maxIdx]);

  Serial.println("Class confidence scores:");
  for (int i = 0; i < N_OUTPUTS; i++) {
    Serial.print(crop_names[i]);
    Serial.print(": ");
    Serial.println(tf.output(i), 4);
  }

  Serial.print("Prediction time: ");
  Serial.print(tf.benchmark.microseconds());
  Serial.println(" us\n");
}
