import sys
import socket
from PyQt6.QtWidgets import QApplication, QWidget, QPushButton, QVBoxLayout

HOST = '143.215.189.122'  # Raspberry Pi IP address
PORT = 12345              # Port number used by the server

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

class Example(QWidget):
    
    def __init__(self):
        super().__init__()
        self.initUI()

    def initUI(self):
        self.setWindowTitle('TCP Client')
        self.setGeometry(100, 100, 300, 200)
        
        self.button = QPushButton('UNLOCK', self)
        self.button.clicked.connect(self.send_message)
        
        layout = QVBoxLayout()
        layout.addWidget(self.button)
        self.setLayout(layout)
        s.connect((HOST, PORT))
        print('Connected to Raspberry Pi server')

    def send_message(self):
        message = 'hello'
        s.sendall(message.encode())
        #data = s.recv(1024)
        #print(f'Received from server: {data.decode()}')

if __name__ == '__main__':
    app = QApplication(sys.argv)
    ex = Example()
    ex.show()
    sys.exit(app.exec())
