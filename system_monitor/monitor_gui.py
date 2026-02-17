import tkinter as tk
from tkinter import ttk
import socket

class ProcessMonitorGUI:
    def __init__(self, root, socket_path):
        self.root = root
        self.root.title("Linux Process Monitor")
        self.root.geometry("800x500")

        # 1. UI Setup (The Table)
        cols = ("PID", "Name", "ST", "CPU%", "MEM (KB)")
        self.tree = ttk.Treeview(root, columns=cols, show='headings')
        for col in cols:
            self.tree.heading(col, text=col)
            self.tree.column(col, width=100, anchor='center')
        self.tree.pack(fill='both', expand=True)

        # 2. Socket Setup
        self.path = socket_path
        self.accumulator = ""
        self.client = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
        
        try:
            self.client.connect(self.path)
            self.client.setblocking(False) # Prevents the window from freezing
            self.update_loop() # Start the heartbeat
        except Exception as e:
            print(f"Connection Error: {e}")

    def update_loop(self):
        # This part replaces your 'while True'
        try:
            while True: 
                chunk = self.client.recv(4096).decode('utf-8')
                if not chunk: break
                self.accumulator += chunk
        except (BlockingIOError, socket.error):
            pass 

        if '\n' in self.accumulator:
            message, self.accumulator = self.accumulator.split('\n', 1)
            self.refresh_ui(message)

        # Run this function again in 1 second
        self.root.after(1000, self.update_loop)

    def refresh_ui(self, message):
        # Parse the string into rows
        rows = message.split(';')
        
        # Clear the table
        for item in self.tree.get_children():
            self.tree.delete(item)
            
        # Insert new rows
        for row in rows:
            if not row: continue
            cols = row.split(',')
            if len(cols) == 5:
                # This matches your C engine: PID, Name, State, CPU, Mem
                self.tree.insert('', 'end', values=cols)

if __name__ == "__main__":
    root = tk.Tk()
    app = ProcessMonitorGUI(root, "/tmp/proc_mon.sock")
    root.mainloop()