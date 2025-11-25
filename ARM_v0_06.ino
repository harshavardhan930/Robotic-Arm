        #include <ESP32Servo.h>
        #define S0 19
        #define S1 18
        #define S2 2
        #define S3 4
        #define sensorOut 15
        #define IR_SENSOR_PIN 34  // IR sensor output pin
        #define LED_PIN 22         // LED pin
        #define RELAY_PIN 33        // Relay pin for electromagnet

        #define TXD2 17
        #define RXD2 16

        unsigned long redFrequency, greenFrequency, blueFrequency;

        Servo servos[5];  // Updated to 5 servos (removed hand servo)
        int servoPins[5] = {32, 14, 27, 26, 25};  // Pins for 5 servos
        int currentAngles[5] = {90, 160, 148, 0, 20};  // Track current angles

        String lastDetectedColor = "Unknown";

        // Sequences for different colors (last column removed for electromagnet handling)
        int redSequence[][5] = {
            {90, 168, 148, 0, 20},
            {4, 168, 148, 0, 20},
            {4, 168, 148, 0, 111},
            {4, 120, 148, 0, 111},
            {4, 120, 110, 0, 111},
            {4, 65, 110, 0, 111},
            {0, 65, 119, 0, 125},  // Pick
            {0, 65, 119, 0, 125},  // Pick (Activate magnet)
            {4, 120, 110, 0, 80},
            {93, 120, 110, 0, 123},
            {93, 120, 153, 0, 155},
            {93, 73, 153, 0, 155},  // Place
            {93, 73, 153, 0, 155},  // Place (Deactivate magnet)
            {90, 168, 148, 0, 20}
        };
        int blueSequence[][5] = {
        {90, 168, 148, 0, 20},
            {4, 168, 148, 0, 20},
            {4, 168, 148, 0, 111},
            {4, 120, 148, 0, 111},
            {4, 120, 110, 0, 111},
            {4, 65, 110, 0, 111},
            {0, 65, 119, 0, 125},  // Pick
            {0, 65, 119, 0, 125},  // Pick (Activate magnet)
            {4, 120, 110, 0, 80},
            {93, 120, 110, 0, 123},
            {98, 120, 47, 0, 69},  // Place
            {98, 48, 47, 0, 69},  // Place (Deactivate magnet)
            {98, 88, 102, 0, 140},
            {90, 160, 148, 0, 20}
        };
        int greenSequence[][5] = {
        {90, 168, 148, 0, 20},
            {4, 168, 148, 0, 20},
            {4, 168, 148, 0, 111},
            {4, 120, 148, 0, 111},
            {4, 120, 110, 0, 111},
            {4, 65, 110, 0, 111},
            {0, 65, 119, 0, 125},  // Pick
            {0, 65, 119, 0, 125},  // Pick (Activate magnet)
            {4, 120, 110, 0, 80},
            {70, 120, 121, 0, 180},
            {70, 73, 121, 0, 180},
            {70, 73, 121, 0, 128},  // Place
            {70, 88, 132, 0, 128},  // Place (Deactivate magnet)
            {90, 168, 148, 0, 20}
        };
        int blackSequence[][5] = {
            {90, 168, 148, 0, 20},
            {4, 168, 148, 0, 20},
            {4, 168, 148, 0, 111},
            {4, 120, 148, 0, 111},
            {4, 120, 110, 0, 111},
            {4, 65, 110, 0, 111},
            {0, 65, 119, 0, 125},  // Pick
            {0, 65, 119, 0, 125},  // Pick (Activate magnet)
            {4, 120, 110, 0, 80},
            {68, 120, 110, 0, 113},
            {80, 120, 24, 0, 21},
            {80, 37, 24, 0, 65},  // Place
            {80, 80, 24, 0, 65},  // Place (Deactivate magnet)
            {80, 80, 80, 0, 65},
            {90, 168, 148, 0, 20}
        };
        // Copy the same changes to blueSequence, greenSequence, and blackSequence

        void setup() {
            Serial.begin(115200);
            Serial2.begin(9600, SERIAL_8N1, TXD2, RXD2);
            pinMode(S0, OUTPUT);
            pinMode(S1, OUTPUT);
            pinMode(S2, OUTPUT);
            pinMode(S3, OUTPUT);
            pinMode(sensorOut, INPUT);
            pinMode(IR_SENSOR_PIN, INPUT);
            pinMode(LED_PIN, OUTPUT);
            pinMode(RELAY_PIN, OUTPUT);  // Initialize relay pin

            digitalWrite(S0, HIGH);
            digitalWrite(S1, LOW);
            digitalWrite(RELAY_PIN, LOW);  // Start with electromagnet OFF

            ESP32PWM::allocateTimer(0);
            ESP32PWM::allocateTimer(1);
            ESP32PWM::allocateTimer(2);
            ESP32PWM::allocateTimer(3);
            
            for (int i = 0; i < 5; i++) {
                servos[i].setPeriodHertz(50);
                servos[i].attach(servoPins[i], 500, 3500);
                servos[i].write(currentAngles[i]);
            }
        }

        void loop() {
        int irValue = digitalRead(IR_SENSOR_PIN);
        digitalWrite(LED_PIN, irValue == 0 ? LOW : HIGH);
            
            digitalWrite(S2, LOW);
            digitalWrite(S3, LOW);
            redFrequency = pulseIn(sensorOut, LOW);
            
            digitalWrite(S2, HIGH);
            digitalWrite(S3, HIGH);
            greenFrequency = pulseIn(sensorOut, LOW);
            
            digitalWrite(S2, LOW);
            digitalWrite(S3, HIGH);
            blueFrequency = pulseIn(sensorOut, LOW);

            int red = map(redFrequency, 30, 1000, 255, 0);
            int green = map(greenFrequency, 30, 1000, 255, 0);
            int blue = map(blueFrequency, 30, 1000, 255, 0);

            String detectedColor = detectColor(red, green, blue);



            if (detectedColor != lastDetectedColor) {
                lastDetectedColor = detectedColor;
                Serial2.println(detectedColor);
                executeMovement(detectedColor);
            }

        // Serial.print("  R: "); Serial.print(red);
        //  Serial.print("  G: "); Serial.print(green);
        //  Serial.print("  B: "); Serial.print(blue);
        // Serial.print("  Detected Color: "); 
        Serial.println(detectedColor);


        }

        String detectColor(int r, int g, int b) {
            if (r > 225 && g < 110 && b < 150) return "Red";
            if (g > 155 && r < 121 && b < 140) return "Green";
            if (b > 195 && g < 142 && r < 120) return "Blue";
            if (b > 230 && g < 250 && r < 250) return "White";
            return "";
        }

        void executeMovement(String color) {
            delay(2000);
            digitalWrite(LED_PIN, LOW);
            if (color == "Red") moveArm(redSequence, 14);
            else if (color == "Blue") moveArm(blueSequence, 14);
            else if (color == "Green") moveArm(greenSequence, 14);
            else if (color == "White") moveArm(blackSequence, 15);
        }

        void moveArm(int sequence[][5], int steps) {
            for (int i = 0; i < steps; i++) {
                for (int j = 0; j < 5; j++) {
                    moveServoSmoothly(j, sequence[i][j]);
                }

                if (i == 6) { // Pick-up step
                    digitalWrite(RELAY_PIN, HIGH);  // Activate electromagnet
                }
                if (i == 11) { // Place-down step
                    digitalWrite(RELAY_PIN, LOW);  // Deactivate electromagnet
                }
            }
        }

        void moveServoSmoothly(int servoIndex, int targetAngle) {
            int step = (targetAngle > currentAngles[servoIndex]) ? 1 : -1;
            while (currentAngles[servoIndex] != targetAngle) {
                currentAngles[servoIndex] += step;
                servos[servoIndex].write(currentAngles[servoIndex]);
                delay(12);
            }
        }
