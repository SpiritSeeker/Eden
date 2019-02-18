import sys
import os
from PyQt5 import QtWidgets, QtGui, QtCore
import pygame
pygame.init()

class Window(QtWidgets.QMainWindow):

	def __init__(self):
		super(Window, self).__init__()
		self.setGeometry(50,50,500,300)
		self.setWindowTitle("Eden")
		self.screenShape = QtWidgets.QDesktopWidget().screenGeometry()

		with open('homepage_style.qss','r') as fh:
			self.setStyleSheet(fh.read())

		openFile = QtWidgets.QAction('&Open File',self)
		openFile.setShortcut("Ctrl+N")
		openFile.triggered.connect(self.filePick)

		mainMenu = self.menuBar()
		fileMenu = mainMenu.addMenu('&File')
		fileMenu.addAction(openFile)

		exitApp = QtWidgets.QAction('&Exit',self)
		exitApp.setShortcut('Ctrl+Q')
		exitApp.triggered.connect(self.close_application)
		fileMenu.addAction(exitApp)

		playAction = QtWidgets.QAction('&Play/Pause',self)
		playAction.setShortcut('Space')
		playAction.triggered.connect(self.playHandler)

		stopAction = QtWidgets.QAction('&Stop',self)
		stopAction.triggered.connect(self.stopHandler)

		playbackMenu = mainMenu.addMenu('&Playback')
		playbackMenu.addAction(playAction)
		playbackMenu.addAction(stopAction)

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

		self.nowPlayingLabel = QtWidgets.QLabel(self)
		self.nowPlayingLabel.setText('Now Playing')
		self.nowPlayingLabel.setObjectName('nowPlayingLabel')
		self.nowPlayingLabel.resize(self.nowPlayingLabel.minimumSizeHint())
		self.nowPlayingLabel.move(int((self.screenShape.width()-self.nowPlayingLabel.frameGeometry().width())/2), 75)
		self.nowPlayingLabel.hide()

		self.songName = QtWidgets.QLabel(self)
		self.songName.setObjectName('songName')
		self.songName.move(int(self.screenShape.width()/2),int(self.screenShape.height()/2))

		self.playBtn = QtWidgets.QPushButton(self)
		self.playBtn.setObjectName('playBtn')
		self.stopBtn = QtWidgets.QPushButton(self)
		self.stopBtn.setObjectName('stopBtn')
		playIcon = QtGui.QIcon('imgs/play.jpg')
		self.playBtn.setIcon(playIcon)
		self.playBtn.setIconSize(QtCore.QSize(24,24))
		stopIcon = QtGui.QIcon('imgs/stop.jpg')
		self.stopBtn.setIcon(stopIcon)
		self.stopBtn.setIconSize(QtCore.QSize(14,14))
		self.playBtn.move(int(self.screenShape.width()/2),int(self.screenShape.height()/2))
		self.stopBtn.move(int(self.screenShape.width()/2),int(self.screenShape.height()/2))
		self.playBtn.hide()
		self.stopBtn.hide()

		self.playBtn.clicked.connect(self.playHandler)
		self.stopBtn.clicked.connect(self.stopHandler)

		self.label = QtWidgets.QPushButton(self)
		self.label.setText("EDEN")
		self.label.setObjectName('edenName')
		f = self.label.font()
		f.setStyleStrategy(QtGui.QFont.PreferAntialias)
		self.label.setFont(f)
		self.label.resize(self.label.minimumSizeHint())
		self.label.move(int((self.screenShape.width()-self.label.frameGeometry().width())/2), int((self.screenShape.height()-self.label.frameGeometry().height())*2/3))
		self.unfade(self.label,1000)

		self.selectNew = QtWidgets.QPushButton('Select a song',self)
		self.selectNew.setObjectName('selectNew')
		self.selectNew.resize(self.selectNew.minimumSizeHint())
		self.selectNew.move(int((self.screenShape.width()-self.selectNew.frameGeometry().width())/2), int((self.screenShape.height()-self.label.frameGeometry().height())*2/3)+self.label.frameGeometry().height())
		self.selectNew.hide()

		self.label.clicked.connect(self.selectNew.show)
		self.selectNew.clicked.connect(self.filePick)

		self.showMaximized()

	def playHandler(self):
		if self.stopStatus:
			pygame.mixer.music.play()
			self.stopStatus = False
		else:	
			if self.playStatus:
				pygame.mixer.music.pause()
				self.playStatus = False
			else:
				pygame.mixer.music.unpause()
				self.playStatus = True		

	def stopHandler(self):
		pygame.mixer.music.stop()
		self.stopStatus = True

	def filePick(self):
		self.name = QtWidgets.QFileDialog.getOpenFileName(self,'Open File')
		if self.name[0] is not '':
			base = os.path.basename(self.name[0])
			self.nowPlayingLabel.show()
			self.songName.setText(os.path.splitext(base)[0])
			self.songName.resize(self.songName.minimumSizeHint())
			self.songName.move(int((self.screenShape.width()-self.songName.frameGeometry().width())/2), 75 + self.nowPlayingLabel.frameGeometry().height() + 10)
			self.playBtn.resize(self.playBtn.minimumSizeHint())
			self.stopBtn.resize(self.stopBtn.minimumSizeHint())
			self.playBtn.move(int(self.screenShape.width()/2)-5-self.playBtn.frameGeometry().width(), 75 + self.nowPlayingLabel.frameGeometry().height() + 15 + self.songName.frameGeometry().height())
			self.stopBtn.move(int(self.screenShape.width()/2)+10, 5 + 75 + self.nowPlayingLabel.frameGeometry().height() + 15 + self.songName.frameGeometry().height())
			self.playBtn.show()
			self.stopBtn.show()
			self.playStatus = True
			self.stopStatus = False
			pygame.mixer.init()
			pygame.mixer.music.load(self.name[0])
			pygame.mixer.music.play()
		
	def fade(self, widget, time):
		self.effect = QtWidgets.QGraphicsOpacityEffect()
		widget.setGraphicsEffect(self.effect)

		self.animation = QtCore.QPropertyAnimation(self.effect, b"opacity")
		self.animation.setDuration(time)
		self.animation.setStartValue(1)
		self.animation.setEndValue(0)
		self.animation.start()

	def unfade(self, widget, time):
		self.effect = QtWidgets.QGraphicsOpacityEffect()
		widget.setGraphicsEffect(self.effect)

		self.animation = QtCore.QPropertyAnimation(self.effect, b"opacity")
		self.animation.setDuration(time)
		self.animation.setStartValue(0)
		self.animation.setEndValue(1)
		self.animation.start()	


if __name__ == "__main__":
	app = QtWidgets.QApplication(sys.argv)
	GUI = Window()
	sys.exit(app.exec_())