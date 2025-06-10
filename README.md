#  Crop Recommendation System on ESP32

A lightweight machine learning project that runs a TensorFlow Lite model on an ESP32 FireBeetle V2.0 to recommend suitable crops based on environmental inputs.


---
## Output Example

![Demo Output](Output%20Example/demo_output.gif)

---
## Dataset

The dataset used in this project was compiled by the **Indian Council of Agricultural Research (ICAR)** and made publicly available by the **Indian Chamber of Food and Agriculture (ICFA)**. It includes soil nutrient ratios (N, P, K, C, S, Mg, Fe, Mn), pH levels, temperature, humidity, and rainfall data.

- **Source:** Indian Chamber of Food and Agriculture (ICFA)  
- **Accessed from:** [Figshare - Crop Recommendation Dataset](https://figshare.com/articles/dataset/Crop_Recommendation_dataset/26308696)

---

##  Features
- Accepts **raw user input** via Serial Monitor:
  - Nitrogen, Phosphorus, Potassium (kg/ha)
  - Temperature (°C), Humidity (%), pH, Rainfall (mm)
- Internally **normalizes inputs**
- Runs `.tflite` model converted to C array (`crop_model_ESP.h`)
- **LED blinks** after each input and prediction
- Outputs **predicted crop** and **confidence scores**

---

##  Dependencies
- [EloquentTinyML](https://github.com/eloquentarduino/EloquentTinyML)
- [tflm_esp32](https://github.com/tensorflow/tflite-micro-arduino-examples)

Board: `FireBeetle-ESP32`  
Baud Rate: `115200`

---

##  Sample Input (via Serial Monitor)


- Enter Nitrogen (kg/ha): 80 Enter Phosphorus (kg/ha): 40 Enter Potassium (kg/ha): 90 Enter Temperature (°C): 26 Enter Humidity (%): 70 Enter pH: 6.2 Enter Rainfall (mm): 120

---

##  License
MIT License © Anant Sharma
