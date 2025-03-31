----------------------------------------------------------------------------
🌱 Crop-Recommendation-Model-On-ESP32
----------------------------------------------------------------------------
- This is a fully embedded crop recommendation system that runs on an ESP32 board using a trained machine learning model. The model is deployed in TensorFlow Lite format and uses simulated environmental input to predict the most suitable crop for cultivation.
----------------------------------------------------------------------------
🔧 How It Works
----------------------------------------------------------------------------
-Trained a lightweight neural network using features:
- Nitrogen (N), Phosphorus (P), Potassium (K)
- Temperature, Humidity, pH, Rainfall
- Normalized data and trained in Python using TensorFlow
- Exported model to .tflite, converted to .h file using xxd
-  Model runs on ESP32 with EloquentTinyML and tflm_esp32
- Input is simulated via script (no sensors)
- Output shows predicted crop and confidence scores in Serial Monitor
----------------------------------------------------------------------------
📁 Folder Structure
----------------------------------------------------------------------------
crop_model_ESP/
├── crop_model_ESP.ino       # Arduino sketch
├── crop_model_ESP.h         # TFLite model as C array

---------------------------------------------------------------------------
📊 Example Output
---------------------------------------------------------------------------
- __Crop Recommendation on ESP32__
- Model loaded successfully. Running prediction...
- Predicted crop: papaya
- Class scores:
- apple: 0.0000
- ...
- papaya: 0.9797
- Prediction time: 309 us
--------------------------------------------------------------------------
🚀 Getting Started
----------------------------------------------------------------------------
- Train model in Python using crop dataset
- Export .tflite, convert to .h using:
-   xxd -i crop_model_ESP.tflite > crop_model_ESP.h
- Place .h in Arduino project folder
- Upload crop_model_ESP.ino to FireBeetle ESP32
- Open Serial Monitor at 115200 baud
- View predicted crop and class confidence
----------------------------------------------------------------------------
🙌 Author
----------------------------------------------------------------------------
Anant Sharma:
- For demo/test purposes, this project uses simulated input. You can extend this to integrate with real sensors like DHT22, pH probe, and NPK sensor for real-time farming solutions. 
- The dataset used is mixed datset from kaggle,ICAR-indian agricultural research institute, and Rajasthan Agricultural Research Institute (RARI).
