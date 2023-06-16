import pygame
from button import Button

SCREEN_HEIGHT = 500
SCREEN_WIDTH = 800

screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
pygame.display.set_caption("Knight")

btn_left = pygame.image.load("UI/BTN_Left.png").convert_alpha()
btn_right = pygame.image.load("UI/BTN_Right.png").convert_alpha()

start_img = pygame.image.load("UI/start_btn.png").convert_alpha()
exit_img = pygame.image.load("UI/exit_btn.png").convert_alpha()

start_button = Button(btn_left, btn_right, 10, 10, start_img, 1)
exit_button = Button(btn_left, btn_right, 10, 145, exit_img, 1)

run = True
while run:
    
    screen.fill((202, 228,241))
    
    if start_button.draw(screen):
        from Levels import Overworld
    if exit_button.draw(screen):
        run = False
    
    #even handler
    for event in pygame.event.get():
        #quit game
        if event.type == pygame.QUIT:
            run = False
        
    pygame.display.update()
        
pygame.quit()