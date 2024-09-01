import socket
import keyboard
import time
import struct

PI_PICO_IP = "192.168.137.12"  
PI_PICO_PORT = 4444

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

def send_frame_to_pico(status, direction, velocity):
    frame = struct.pack('Bbh', status, direction, velocity)
    sock.sendto(frame, (PI_PICO_IP, PI_PICO_PORT))


def main():
    velocity = 250
    status = 0 
    last_send_time = time.time()

    while True:
        current_time = time.time()
        if current_time - last_send_time >= 0.115: 
            last_send_time = current_time

            direction = 5 

            if keyboard.is_pressed('w'):
                direction = 1
            elif keyboard.is_pressed('s'):
                direction = 2
            elif keyboard.is_pressed('a'):
                direction = 3
            elif keyboard.is_pressed('d'):
                direction = 4

            send_frame_to_pico(status, direction, velocity)
            print(f"Sent: status={status}, direction={direction}, velocity={velocity}")

            time.sleep(0.01)


if __name__ == "__main__":
    main()
