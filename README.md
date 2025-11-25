# 🦾 SORTBOT – Robotic Arm Color Sorting System

SORTBOT is an automated color-sorting robotic system built using an ESP32, a 5-axis robotic arm, TCS3200 color sensor, conveyor belt, and an electromagnet gripper.  
It detects the color of incoming parcels and automatically places them in their respective bins.  
A touchscreen dashboard made with **LVGL** shows real-time sorting counts and system status.

---

## 📸 Project Images

*(Include the images inside an `images/` folder in your repository)*

images/20250328_233457.jpg
images/20250328_233511.jpg
images/20250328_234119.jpg
images/20250328_234612.jpg

yaml
Copy code

---

## 📖 Overview

SORTBOT is designed to simulate an industrial parcel-sorting system on a small scale.  
It uses:

- **TCS3200 Color Sensor** to detect object color  
- **ESP32** for computing + wireless features  
- **5 Servo Motors** for robotic arm control  
- **Electromagnet + Relay** for gripping objects  
- **Conveyor Belt** for object feeding  
- **LVGL UI** for real-time monitoring  

This project demonstrates automation, mechatronics, and embedded system concepts.

---

## 🚀 Features

### ✔ Automatic Color Detection  
Detects **Red, Green, Blue, White** using mapped RGB frequency values.

### ✔ 5-Axis Robotic Arm  
Uses calibrated servo sequences for smooth, collision-free motion.

### ✔ Electromagnetic Gripper  
Activates during pickup and deactivates during placement for precise gripping.

### ✔ Conveyor Belt System  
Moves cubes to the color sensor for detection.

### ✔ LVGL Touchscreen Dashboard  
Displays:
- Total parcel count  
- Individual color counts  
- ESP32 IP & Camera IP  
- System activity indicators  
---

## 🛠 Hardware Used

| Component | Purpose |
|----------|---------|
| ESP32 Dev Kit | Main controller |
| 5× Servo Motors | Robotic arm joints |
| Electromagnet + Relay | Pickup mechanism |
| TCS3200 Color Sensor | Detects cube color |
| IR Sensor | Object presence |
| Conveyor Belt System | Moves parcels |
| TFT Display (ILI9341) | Touch UI |
| Custom 3D-Printed Arm | Mechanical support |

---

## 🧠 Working Principle

1. Conveyor belt pushes the parcel under the TCS3200 sensor.  
2. RGB frequencies are read and converted to a color value.  
3. ESP32 selects the correct servo sequence based on color.  
4. Robotic arm moves to pickup point, activates electromagnet, lifts cube.  
5. Arm places cube into assigned bin and releases it.  
6. LVGL updates sorting status and counts.

---

## 🔧 Key Code Snippets

### 🎯 Color Detection
if (r > 225 && g < 110 && b < 150) return "Red";
if (g > 155 && r < 121 && b < 140) return "Green";
if (b > 195 && g < 142 && r < 120) return "Blue";
if (b > 230 && g < 250 && r < 250) return "White";

### Smooth Servo Motion
while (currentAngles[s] != targetAngle) {
    currentAngles[s] += step;
    servos[s].write(currentAngles[s]);
    delay(12);
}
### Electromagnet Control
cpp
Copy code
if (stepIndex == 6) digitalWrite(RELAY_PIN, HIGH);
if (stepIndex == 11) digitalWrite(RELAY_PIN, LOW);
### Calibration & Testing
Servo Calibration
Fine-tuned each joint for smooth, collision-free motion.

### Sensor Calibration
RGB values mapped under controlled LED lighting for accuracy.

### System Testing
Repeated pickup/drop cycles to check reliability.

### Final Output
~95% accurate color sorting

Smooth robotic arm movement

Stable electromagnet pickup

Fully automated workflow

Real-time UI monitoring
