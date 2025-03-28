#include "crop_model_ESP.h"      // your .h model from xxd
#include <tflm_esp32.h>          // board-specific runtime (FireBeetle = ESP32)
#include <eloquent_tinyml.h>     // eloquent wrapper

#define TF_NUM_OPS 2             // we only use Dense + Softmax
#define ARENA_SIZE 16000         // adjust if needed for larger models

Eloquent::TF::Sequential<TF_NUM_OPS, ARENA_SIZE> tf;

#define N_INPUTS 7
#define N_OUTPUTS 22

// Simulated normalized input
float input[N_INPUTS] = {
  0.45, // Nitrogen
  0.35, // Phosphorus
  0.60, // Potassium
  0.42, // Temperature
  0.65, // Humidity
  0.55, // pH
  0.70  // Rainfall
};

// Corresponding crop labels
const char* crop_names[N_OUTPUTS] = {
  "apple", "banana", "blackgram", "chickpea", "coconut", "coffee", "cotton",
  "grapes", "jute", "kidneybeans", "lentil", "maize", "mango", "mothbeans",
  "mungbean", "muskmelon", "orange", "papaya", "pigeonpeas", "pomegranate",
  "rice", "watermelon"
};

void setup() {
  Serial.begin(115200);
  delay(3000);
  Serial.println("__Crop Recommendation on ESP32__");

  tf.setNumInputs(N_INPUTS);
  tf.setNumOutputs(N_OUTPUTS);
  tf.resolver.AddFullyConnected();
  tf.resolver.AddSoftmax();

  while (!tf.begin(crop_model_ESP_tflite).isOk()) {
    Serial.println("Model loading failed:");
    Serial.println(tf.exception.toString());
    delay(500);
  }

  Serial.println("Model loaded successfully. Running prediction...");

  // Predict
  if (!tf.predict(input).isOk()) {
    Serial.println("Prediction failed:");
    Serial.println(tf.exception.toString());
    return;
  }

  // Get highest scoring class
  int maxIdx = 0;
  float maxVal = tf.output(0);
  for (int i = 1; i < N_OUTPUTS; i++) {
    if (tf.output(i) > maxVal) {
      maxVal = tf.output(i);
      maxIdx = i;
    }
  }

  // Show result
  Serial.print("Predicted crop: ");
  Serial.println(crop_names[maxIdx]);

  Serial.println("Class scores:");
  for (int i = 0; i < N_OUTPUTS; i++) {
    Serial.print(crop_names[i]);
    Serial.print(": ");
    Serial.println(tf.output(i), 4);
  }

  Serial.print("Prediction time: ");
  Serial.print(tf.benchmark.microseconds());
  Serial.println(" us");
}

void loop() {
  // Run only once
  delay(10000);
}
