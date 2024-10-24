import mmap
import time

shared_memory_name = "Local\\SharedMemoryHidBuzzReader"
buffer_size = 256
shm = mmap.mmap(-1, buffer_size, tagname=shared_memory_name)

try:
    while True:
        shm.seek(0)  # Move to the start of the memory
        data = (
            shm.read(buffer_size).rstrip(b"\x00").decode()
        )  # Read and decode the data
        print(f"Data: {data}")
        time.sleep(2)  # Simulate real-time polling with delay
finally:
    shm.close()  # Close the shared memory when done
