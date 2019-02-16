import sys
from PyQt5 import QtWidgets

class Window(QtWidgets.QMainWindow):

	def __init__(self):
		super(Window, self).__init__()
		self.setGeometry(50,50,500,300)
		self.setWindowTitle("Eden")

		extractAction = QtWidgets.QAction('&Open File',self)
		extractAction.setShortcut("Ctrl+N")

		mainMenu = self.menuBar()
		fileMenu = mainMenu.addMenu('&File')
		fileMenu.addAction(extractAction)

		extractAction = QtWidgets.QAction('&Exit',self)
		extractAction.setShortcut('Ctrl+Q')
		extractAction.triggered.connect(self.close_application)
		fileMenu.addAction(extractAction)

		self.home()
		
	def close_application(self):
		choice = QtWidgets.QMessageBox.question(self, 'Quit',
			'Are you sure you want to exit?',
			QtWidgets.QMessageBox.Yes | QtWidgets.QMessageBox.No)
		if choice == QtWidgets.QMessageBox.Yes:
			sys.exit()
		else:
			pass	


	def home(self):
		self.showMaximized()


def main():
	app = QtWidgets.QApplication(sys.argv)
	GUI = Window()
	sys.exit(app.exec_())


if __name__ == "__main__":
	main()	