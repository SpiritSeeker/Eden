import sys
from PyQt5 import QtWidgets, QtGui, QtCore

class Window(QtWidgets.QMainWindow):

	def __init__(self):
		super(Window, self).__init__()
		self.setGeometry(50,50,500,300)
		self.setWindowTitle("Eden")
		self.screenShape = QtWidgets.QDesktopWidget().screenGeometry()

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
		oImage = QtGui.QImage('imgs/neon.jpg')
		palette = QtGui.QPalette()
		palette.setBrush(10, QtGui.QBrush(oImage))
		self.setPalette(palette)

		self.label = QtWidgets.QLabel(self)
		self.label.setText("<span style='font-size:100pt;text-align:center;color:white;'>EDEN</span>")
		f = self.label.font()
		f.setStyleStrategy(QtGui.QFont.PreferAntialias)
		self.label.setFont(f)
		self.label.resize(self.label.minimumSizeHint())
		self.label.move(int((self.screenShape.width()-self.label.frameGeometry().width())/2), int((self.screenShape.height()-self.label.frameGeometry().height())/3))
		self.unfade(self.label)
		self.showMaximized()

	def fade(self, widget):
		self.effect = QtWidgets.QGraphicsOpacityEffect()
		widget.setGraphicsEffect(self.effect)

		self.animation = QtCore.QPropertyAnimation(self.effect, b"opacity")
		self.animation.setDuration(1000)
		self.animation.setStartValue(1)
		self.animation.setEndValue(0)
		self.animation.start()

	def unfade(self, widget):
		self.effect = QtWidgets.QGraphicsOpacityEffect()
		widget.setGraphicsEffect(self.effect)

		self.animation = QtCore.QPropertyAnimation(self.effect, b"opacity")
		self.animation.setDuration(1000)
		self.animation.setStartValue(0)
		self.animation.setEndValue(1)
		self.animation.start()	


if __name__ == "__main__":
	app = QtWidgets.QApplication(sys.argv)
	GUI = Window()
	sys.exit(app.exec_())