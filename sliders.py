#!/usr/bin/python3
import serial
import pygame

from time import sleep

class slidersTest:
	def __init__(self):
		pygame.init()
		self.width=1000
		self.height=400
		self.size = [self.width, self.height]
		self.screen = pygame.display.set_mode(self.size)
		self.centreX = self.width/2
		self.centreY = self.height/2
		
		self.sliderWidth = self.width / 8
		self.sliderHeight = 300
		
		self.sliderbg = (100,255,100)
		self.labelColour=(255,255,255)
		self.t = 0
		pygame.display.set_caption("Sliders Test")
		done = False
		self.serialCom = serial.Serial('/dev/ttyACM0',9600)
		self.clock = pygame.time.Clock()
		while not done:
			for event in pygame.event.get():
				if event.type == pygame.QUIT: 
				    done=True 
				elif event.type == pygame.KEYDOWN:
				    if (event.key == pygame.K_q):
				        done=True
			self.clock.tick()
			self.screen.fill((0,0,0))
			self.doStuff()
			pygame.display.flip()

		pygame.quit()
	def doStuff(self):
		tmp = self.readValues()
		self.drawSliders(tmp)
	def readValues(self):
		s_bytes = self.serialCom.readline()
		decoded_bytes = s_bytes.decode("utf-8").strip('\n')
		values = [float(x) for x in decoded_bytes.split()]
		return values
	def drawSliders (self,values):
		stop = len(values)
		for i in range(0, stop, 1):
			self.drawSlider(i,values[i])
	def drawSlider (self,sliderNo,value):
		tmp = value / 1023 * self.sliderHeight
		pygame.draw.rect(self.screen, self.sliderbg, [self.sliderWidth*sliderNo+5, self.sliderHeight-tmp+5, self.sliderWidth-10,tmp+1 ])
		buf = "%d" % (value)
		self.drawLabel([self.sliderWidth*sliderNo+5,310],buf,30)
	def drawLabel (self,cords,message,fontsize):
		font = pygame.font.SysFont(None, fontsize)
		text = font.render(message, True, self.labelColour)
		self.screen.blit(text,(cords[0], cords[1]))
    
if __name__ == "__main__":
    tmp =  slidersTest()
