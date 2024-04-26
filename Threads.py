import threading
import time

shared_data = None
lock = threading.Lock()
condition = threading.Condition(lock)

def worker_thread():
    global shared_data
    
    with condition:
        condition.wait_for(lambda: shared_data is not None)
    print("Worker thread: Data processed:", shared_data)

def main_thread():
    global shared_data

    worker = threading.Thread(target=worker_thread)
    worker.start()

    with lock:
        print("Main thread: Processing data.")
        for i in range(1, 6, 1):
            time.sleep(2)
            print("Data", i)# Simular un trabajo de larga duración
        shared_data = 42  # Establecer shared_data

    with condition:
        condition.notify_all()

    worker.join()

if __name__ == "__main__":
    main_thread()