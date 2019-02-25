#! /usr/bin/python3
import sys
import os
from PyQt5 import QtWidgets, QtGui, QtCore
from EdenPlayer.EMusicPlayer import EMusicPlayer
from time import sleep

class Window(QtWidgets.QMainWindow):

	def __init__(self):
		super(Window, self).__init__()
		self.screenShape = QtWidgets.QDesktopWidget().screenGeometry()
		self.setGeometry(0,10,self.screenShape.width()-10,self.screenShape.height()-20)
		self.setWindowTitle("Eden")

		self.cwd = os.path.dirname(os.path.realpath(__file__))

		self.setWindowIcon(QtGui.QIcon(self.cwd+'/imgs/logo.png'))

		with open(self.cwd+'/homepage_style.qss','r') as fh:
			self.setStyleSheet(fh.read())

		self.emp = EMusicPlayer()	

		openFile = QtWidgets.QAction('&Open File',self)
		openFile.setShortcut("Ctrl+N")
		openFile.triggered.connect(self.filePick)

		self.mainMenu = self.menuBar()
		fileMenu = self.mainMenu.addMenu('&File')
		fileMenu.addAction(openFile)
		self.addAction(openFile)

		exitApp = QtWidgets.QAction('&Exit',self)
		exitApp.setShortcut('Ctrl+Q')
		exitApp.triggered.connect(self.close_application)
		fileMenu.addAction(exitApp)
		self.addAction(exitApp)

		playAction = QtWidgets.QAction('&Play/Pause',self)
		playAction.setShortcut('Space')
		playAction.triggered.connect(self.playHandler)

		playbackMenu = self.mainMenu.addMenu('&Playback')
		playbackMenu.addAction(playAction)

		playAction = QtWidgets.QAction('&Stop',self)
		playAction.triggered.connect(self.stopHandler)

		playbackMenu.addAction(playAction)
		self.addAction(playAction)

		playbackMenu.addSeparator()

		self.deviceMenu = playbackMenu.addMenu('&Output Devices')
		self.refreshAction = QtWidgets.QAction('&Refresh Device List',self)
		self.defaultAction = QtWidgets.QAction('&Use Default Device', self)
		self.defaultAction.triggered.connect(self.set_default_device)
		self.refreshAction.triggered.connect(self.refreshList)
		self.deviceMenu.addAction(self.refreshAction)

		self.refreshList()

		fullscreenAction = QtWidgets.QAction('&Fullscreen',self)
		fullscreenAction.setShortcut('F11')
		fullscreenAction.triggered.connect(self.toggle_fullscreen)

		viewMenu = self.mainMenu.addMenu('&View')
		viewMenu.addAction(fullscreenAction)
		self.addAction(fullscreenAction)

		self.home()
		
	def close_application(self):
		choice = QtWidgets.QMessageBox.question(self, 'Quit',
			'Are you sure you want to exit?',
			QtWidgets.QMessageBox.Yes | QtWidgets.QMessageBox.No)
		if choice == QtWidgets.QMessageBox.Yes:
			self.emp.stop()
			sys.exit()
		else:
			pass

	def set_default_device(self):
		self.emp.use_default = True		

	def refreshList(self):
		self.deviceMenu.clear()
		self.deviceMenu.addAction(self.refreshAction)
		self.deviceMenu.addSeparator()
		for i in range(self.emp.dev_num):
			devName = self.emp.get_device_name(i)
			deviceDisp = QtWidgets.QAction('&'+devName,self)
			deviceDisp.triggered.connect(lambda state, x=devName: self.emp.set_device(x))
			self.deviceMenu.addAction(deviceDisp)
		self.deviceMenu.addAction(self.defaultAction)	

	def toggle_fullscreen(self):
		if self.isFullScreen():
			self.showMaximized()
			self.mainMenu.setVisible(True)
		else:
			self.showFullScreen()
			self.mainMenu.setVisible(False)	

	def home(self):
		oImage = QtGui.QImage(self.cwd+'/imgs/neon.jpg')
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
		playIcon = QtGui.QIcon(self.cwd+'/imgs/play.jpg')
		self.playBtn.setIcon(playIcon)
		self.playBtn.setIconSize(QtCore.QSize(24,24))
		stopIcon = QtGui.QIcon(self.cwd+'/imgs/stop.jpg')
		self.stopBtn.setIcon(stopIcon)
		self.stopBtn.setIconSize(QtCore.QSize(14,14))
		self.playBtn.move(int(self.screenShape.width()/2),int(self.screenShape.height()/2))
		self.stopBtn.move(int(self.screenShape.width()/2),int(self.screenShape.height()/2))
		self.playBtn.hide()
		self.stopBtn.hide()

		self.volumeSlider = QtWidgets.QSlider(QtCore.Qt.Horizontal, self)
		self.volumeSlider.setFocusPolicy(QtCore.Qt.NoFocus)
		self.volumeSlider.valueChanged[int].connect(self.changeVolume)
		self.volumeSlider.setValue(100)
		self.volumeSlider.hide()

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

		self.label.clicked.connect(lambda: self.unfade(self.selectNew, 1500))
		self.selectNew.clicked.connect(self.filePick)

		self.show()

	def changeVolume(self, value):
		self.emp.set_volume(0.01*value)

	def playHandler(self):
		if self.emp.stop_state:
			self.emp.play()
		else:	
			if self.emp.pause_state:
				self.emp.unpause()
			else:
				self.emp.pause()		

	def stopHandler(self):
		self.emp.stop()

	def filePick(self):
		fil = "mp3(*.mp3)"
		self.name = QtWidgets.QFileDialog.getOpenFileName(self,'Open File',filter=fil)
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
			self.volumeSlider.move(int(self.screenShape.width()/2)-5-self.playBtn.frameGeometry().width(), 75 + self.nowPlayingLabel.frameGeometry().height() + 15 + self.songName.frameGeometry().height() + 20 + self.playBtn.frameGeometry().height())
			self.volumeSlider.resize(10 + self.playBtn.frameGeometry().width() + self.stopBtn.frameGeometry().width(), 45)
			self.playBtn.show()
			self.stopBtn.show()
			self.volumeSlider.show()
			self.emp.load(self.name[0])
			self.emp.play()
		
	def fade(self, widget, time):
		effect = QtWidgets.QGraphicsOpacityEffect()
		widget.setGraphicsEffect(effect)

		self.animation = QtCore.QPropertyAnimation(effect, b"opacity")
		self.animation.setDuration(time)
		self.animation.setStartValue(1)
		self.animation.setEndValue(0)
		self.animation.start()

	def unfade(self, widget, time):
		effect = QtWidgets.QGraphicsOpacityEffect()
		widget.setGraphicsEffect(effect)
		widget.show()

		self.animation = QtCore.QPropertyAnimation(effect, b"opacity")
		self.animation.setDuration(time)
		self.animation.setStartValue(0)
		self.animation.setEndValue(1)
		self.animation.start()	


if __name__ == "__main__":
	app = QtWidgets.QApplication(sys.argv)
	GUI = Window()
	app.aboutToQuit.connect(GUI.emp.stop)
	sys.exit(app.exec_())