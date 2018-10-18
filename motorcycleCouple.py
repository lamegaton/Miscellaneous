'''For this tutorial, i'm gonna make a simple moving object
using pygame. This is based from the chimp tutorial on 
pygame website'''

'''--- let get it started ---'''

#first we need to import module 

import os, sys
import pygame
from pygame.locals import *
from pygame.compat import geterror

# os, and sys allow us to use path object

#next we need to load an image of the moving object

def load_image(name, colokey=None):
		fullname = os.path.join('data',name)
	try:
		image = pygame.image.load(fullname)
	except pygame.error:
		print('cannot load image',fullname)
		raise SystemExit(str(geterror())) #this is for error handling
	image = image.convert()
	if colorkey is not None:
		if colorkey is -1:
			colokey = image.get_at((0,0))
		image.set_colorkey(colorkey, RLEACCEL)#RLEACCEL to provide better \
		#performance on non accelerated displays.
	return image, image.get_rect()
	
# after loading the image and process it
# we go to the next step define the main object
# Sprite is a moving object
class Bike(pygame.sprite.Sprite):
	"""let move the bike accorss the screen"""\
	#underscore is used to make a local fucntion
	#we also need to initialize module Sprite
	def __init__(self):
		pygame.sprite.Sprite.__init__(self) #call sprite initializer
		#load image into self
		self.image, self.rect = load_image('motorcycleCouple.png', -1)
		self.area  = screen.get_rect()
		self.rect.topleft = 10,10
		self.move = 9
	
	def _run(self):
		newpos = self.rect.move(self.move,0) #move the retangle x = 9,y = 0\
		#that means moving 9 pixel every frame
		if self.rect.left < self.area.left or \
			self.rect.right > self.area.right:
			newpos = self.rect.move((self.move,0))
			self.image = pygame.transform.flip(self.image,1,0)
		self.rect = newpos

def main():
	#this is main function, start initialize everything and 
	#running in a loop like what we have learned in C
# initialize
	pygame.init()
	screen = pygame.display.set_mode((500,500))#set the size of the window
	pygame.display.set_caption('Motorcycle Couple')
# create the background
	background = pygame.Surface(screen.get_size())
	background = background.convert()
	background = load_image('background.png',)
	
# display the background
	screen.blit(background,(0,0))
	pygame.display.flip()
# game object
	bike = Bike()
	sprite = pygame.sprite.RenderPlain((bike))
# main loop
	going = True
	
	while going:
		clock.tick(60)
		#handle input events
		for event in pygame.event.get():
			if event.type == QUIT:
				going = False
			elif event.type == KEYDOWN and event.key == K_ESCAPE:
				going = False
			
		sprite.update()
		#draw everything
		screen.blit(background,(0,0))
		sprite.draw(screen)
		pygame.display.flip()
	pygame.quit()

if __name__ == '__main__':
	main()
