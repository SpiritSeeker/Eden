import sys
from PyQt5 import QtWidgets

class Window(QtWidgets.QMainWindow):
	"""docstring for Window"""
	def __init__(self):
		super(Window, self).__init__()
		self.setGeometry(50,50,500,300)
		self.setWindowTitle("Dayummmm. Music!!!")

		extractAction = QtWidgets.QAction('&New Shit',self)
		extractAction.setShortcut("Ctrl+N")
		extractAction.setStatusTip('Open new music')

		self.statusBar()

		mainMenu = self.menuBar()
		stuffMenu = mainMenu.addMenu('&Good Stuff')
		stuffMenu.addAction(extractAction)

		extractAction = QtWidgets.QAction('&Eh. Quit',self)
		extractAction.setShortcut('Ctrl+Q')
		extractAction.setStatusTip('I\'mma fool')
		extractAction.triggered.connect(sys.exit)
		stuffMenu.addAction(extractAction)


		self.home()
		
	def home(self):
		self.showMaximized()	


def main():
	app = QtWidgets.QApplication(sys.argv)
	GUI = Window()
	sys.exit(app.exec_())


if __name__ == "__main__":
	main()	