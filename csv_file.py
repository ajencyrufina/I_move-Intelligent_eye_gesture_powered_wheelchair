import serial
import csv
from datetime import datetime

PORT = 'COM12'
BAUD = 115200

ser = serial.Serial(PORT, BAUD, timeout=1)

filename = "ecg_data.csv"

with open(filename, mode='w', newline='') as file:
    writer = csv.writer(file)
    
    # Header
    writer.writerow(["Timestamp", "ECG_Value"])
    
    print("Recording... Press Ctrl+C to stop.")
    
    try:
        while True:
            if ser.in_waiting:
                line = ser.readline().decode('utf-8', errors='ignore').strip()
                
                if line:
                    try:
                        value = float(line)

                        # Convert to readable datetime
                        now = datetime.now()
                        timestamp = now.strftime("%Y-%m-%d %H:%M:%S.%f")[:-3]

                        writer.writerow([timestamp, value])
                        print(timestamp, value)

                    except ValueError:
                        pass

    except KeyboardInterrupt:
        print("\nRecording stopped. Data saved to", filename)

ser.close()