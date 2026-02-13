import socket
import os
import sys
s = socket.socket(socket.AF_UNIX,socket.SOCK_STREAM)

class ProcessMonitor: 
    def __init__(self,socket_path):
        self.path = socket_path
        self.client = None
        self.accumulator = ""
    
    def connect(self):
        try:
            self.client = socket.socket(socket.AF_UNIX,socket.SOCK_STREAM)
            self.client.connect(self.path)
            print("connected to server!!")
            return True
        except Exception as e: 
            print(f"ERROR: {e}")    
            sys.exit(1)
        
    def listen(self):
        if not self.client: 
            print("Client is not connected. call connect() first")
            return
        try: 
            while True: 
                chunk = self.client.recv(4096).decode('utf-8')
                if not chunk: 
                    print("C engine disconnected")
                    break
                self.accumulator += chunk
                
                # bumper logic
                if '\n' in self.accumulator:
                    message,self.accumulator = self.accumulator.split('\n',1)
                    parsed_datas = self.parse_data(message)
                    self.display_data(parsed_datas)
                    print(f"capture snapshot (length: {len(message)})")
        except KeyboardInterrupt: 
            print(f"\nStopping...")
        except socket.error as e: 
            print(f"socket error during reception: {e}")
        
        finally:
            self.client.close()
            print("socket closed")

        
        pass    
    def parse_data(self,message): 
        raw_list = message.split(';')
        parsed_result = []

        for row in raw_list: 
            if not row:
                continue #skip empty string at the very end.
            cols = row.split(',')
            if len(cols) == 5:
                proc_dict = {
                    "pid": cols[0],
                    "name": cols[1],
                    "state": cols[2],
                    "cpu": cols[3],
                    "mem": cols[4]
                }        
                parsed_result.append(proc_dict)
        return parsed_result
    
    def display_data(self,results):
        os.system('clear')
        print(f"{'PID':<8} | {'NAME':<20} |{'ST':<5} | {'CPU%':<8} | {'MEM':<10}")
        print("-" * 50)  

        for proc in results[:10]: 
            print(f"{proc['pid']:<8} | {proc['name']:<20} | {proc['status']:<5} | {proc['cpu']:<8} | {proc['mem']:<10}")

if __name__== "__main__":
    SOCKET_PATH = "/tmp/proc_mon.sock"
    monitor = ProcessMonitor(SOCKET_PATH)
    if monitor.connect():
        monitor.listen()