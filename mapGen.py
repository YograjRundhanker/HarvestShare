import pygame
import sys
import random
import time
import math
import numpy as np
from PIL import Image

class Maze:
    def __init__(self, rows, cols, start, end):
        self.n = rows  # define the n attribute
        self.grid = self.create_grid(rows, cols)
        self.currentCell = self.grid[start[0]][start[1]]
        self.start = start
        self.end = end

    def cell_at(self, row, col):
        return self.grid[row][col]
    
    def create_grid(self, rows, cols):
        grid = [[Cell(row, col) for col in range(cols)] for row in range(rows)]
        return grid

    def find_valid_neighbours(self, cell):
        neighbours = []
        row, col = cell.row, cell.col
        directions = [(0, 1), (0, -1), (1, 0), (-1, 0)]
        for direction in directions:
            new_row, new_col = row + direction[0], col + direction[1]
            if new_row >= 0 and new_row < self.n and new_col >= 0 and new_col < self.n:
                neighbour = self.grid[new_row][new_col]
                if not neighbour.visited:
                    neighbours.append(neighbour)
        return neighbours

class Cell:
    def __init__(self, row, col):
        self.row = row
        self.col = col
        self.visited = False
        self.color = (255, 255, 255)
    
    def __repr__(self):
        return f"({self.row}, {self.col})"

n=10
# Initialize the matrix with zeros
matrix = [[0 for _ in range(n)] for _ in range(n)]

# Choose a random starting position
row = random.randint(0, n - 1)
col = random.randint(0, n - 1)

# Set the starting position to 1
matrix[row][col] = 1

# Choose a random direction and move one step in that direction
directions = [(0, 1), (0, -1), (1, 0), (-1, 0)]
while True:
    direction = random.choice(directions)
    new_row = row + direction[0]
    new_col = col + direction[1]
    if new_row < 0 or new_row >= n or new_col < 0 or new_col >= n:
        # We've hit the edge of the map, so stop
        break
    if matrix[new_row][new_col] != 0:
        # We've already been to this cell, so stop
        break
    # Set the new position to 1 and update the current position
    matrix[new_row][new_col] = 1
    row = new_row
    col = new_col

# create a maze object
maze = Maze(row, col, (0, 0), ((row-1), (col-1)))

# Set the starting cell to green
maze.currentCell.color = (0, 255, 0)

while True:
    neighbours = maze.find_valid_neighbours(maze.currentCell)
    if len(neighbours) == 0:
        break
    nextCell = random.choice(neighbours)
    maze.currentCell.color = (255, 255, 255)
    nextCell.visited = True
    nextCell.color = (0, 255, 0)
    maze.currentCell = nextCell
pygame.init()
SCREEN = pygame.display.set_mode((1600, 900))
CLOCK = pygame.time.Clock()

def generateRandomMap():
        
    BLACK = (0, 0, 0)
    WHITE = (255, 255, 255)
    GREEN = (0, 255, 128)
    
    WINDOW_HEIGHT = 730
    WINDOW_WIDTH = 1460 #These are for the maze/grid, not for the pygame window size

    blockSize = 146 #Set the size of the grid block
    rows, cols = (int(WINDOW_WIDTH/blockSize), int(WINDOW_HEIGHT/blockSize))
    maze = Maze(row, col, (0, 0), ((row-1), (col-1)))    
    trackLenght = 1
    movex = 70
    movey = 85

    startx, starty = 0, 3
    currentCell = maze.cell_at(startx, starty)
    
    straight1 = pygame.image.load('Images\TracksMapGen\Straight1.png')
    straight1Rect = straight1.get_rect()

    straight2 = pygame.image.load('Images\TracksMapGen\Straight2.png')
    straight2Rect = straight2.get_rect()

    curve1 = pygame.image.load('Images\TracksMapGen\Curve1.png')
    curve1Rect = curve1.get_rect()

    curve2 = pygame.image.load('Images\TracksMapGen\Curve2.png')
    curve2Rect = curve2.get_rect()

    curve3 = pygame.image.load('Images\TracksMapGen\Curve3.png')
    curve3Rect = curve3.get_rect()

    curve4 = pygame.image.load('Images\TracksMapGen\Curve4.png')
    curve4Rect = curve4.get_rect()

    straight1Top = pygame.image.load('Images\TracksMapGen\Straight1Top.png')
    straight1RectTop = straight1Top.get_rect()

    straight2Top = pygame.image.load('Images\TracksMapGen\Straight2Top.png')
    straight2RectTop = straight2Top.get_rect()

    curve1Top = pygame.image.load('Images\TracksMapGen\Curve1Top.png')
    curve1RectTop = curve1Top.get_rect()

    curve2Top = pygame.image.load('Images\TracksMapGen\Curve2Top.png')
    curve2RectTop = curve2Top.get_rect()

    curve3Top = pygame.image.load('Images\TracksMapGen\Curve3Top.png')
    curve3RectTop = curve3Top.get_rect()

    curve4Top = pygame.image.load('Images\TracksMapGen\Curve4Top.png')
    curve4RectTop = curve4Top.get_rect()

    bg = pygame.image.load('Images\TracksMapGen\Background.png')

    while True:

        #CurrentCell is the one at (0,3) position, im gonna look if there are unvisited cells from there
        if len(maze.find_valid_neighbours(currentCell)) > 0:
            if currentCell.x == 0 and currentCell.y == 3: #Second cell is always the one on top of first cell bc first cell is always a straight vertical track
                oldCell = currentCell
                currentCell = maze.cell_at(oldCell.x,oldCell.y-1)
                currentCell.color = GREEN
                oldCell.knock_down_wall(currentCell, "N")
                trackLenght += 1 #Keep track of length so to discard very short generated tracks
            else:
                random_unvisited_direction = random.choice(maze.find_valid_neighbours(currentCell))[0] #Pick a random direction to move
                oldCell = currentCell
                if random_unvisited_direction == "N": #Move according to the direction picked
                    currentCell = maze.cell_at(oldCell.x,oldCell.y-1)
                elif random_unvisited_direction == "S":
                    currentCell = maze.cell_at(oldCell.x,oldCell.y+1)
                elif random_unvisited_direction == "E":
                    currentCell = maze.cell_at(oldCell.x+1,oldCell.y)
                elif random_unvisited_direction == "W":
                    currentCell = maze.cell_at(oldCell.x-1,oldCell.y)
                
                oldCell.knock_down_wall(currentCell, random_unvisited_direction)
                trackLenght += 1
            
        else:
            #Track is ready (back in initial position)! lets check if its long enough
            if currentCell.x == 0 and currentCell.y == 4 and trackLenght > 40:
                currentCell.knock_down_wall(maze.cell_at(0,3), "N")
                
                for x in range(0, WINDOW_WIDTH, blockSize):
                    for y in range(0, WINDOW_HEIGHT, blockSize):
                        currentCell = maze.cell_at(int(x/blockSize),int(y/blockSize))
                        currentCell.color = (0,0,1,255)
                
                for x in range(0, WINDOW_WIDTH, blockSize):
                    for y in range(0, WINDOW_HEIGHT, blockSize):
                        currentCell = maze.cell_at(int(x/blockSize),int(y/blockSize))
                        
                        if currentCell.walls["N"] == False and currentCell.walls["S"] == False:
                            SCREEN.blit(straight2, straight2Rect.move(x+movex,y+movey))  
                        elif currentCell.walls["E"] == False and currentCell.walls["W"] == False:
                            SCREEN.blit(straight1, straight1Rect.move(x+movex,y+movey))  
                        elif currentCell.walls["N"] == False and currentCell.walls["W"] == False:
                            SCREEN.blit(curve3, curve3Rect.move(x+movex,y+movey)) 
                        elif currentCell.walls["W"] == False and currentCell.walls["S"] == False:
                            SCREEN.blit(curve2, curve2Rect.move(x+movex,y+movey))     
                        elif currentCell.walls["S"] == False and currentCell.walls["E"] == False:
                            SCREEN.blit(curve1, curve1Rect.move(x+movex,y+movey)) 
                        elif currentCell.walls["E"] == False and currentCell.walls["N"] == False:
                            SCREEN.blit(curve4, curve4Rect.move(x+movex,y+movey)) 
                
                #Save track and change background to transparent because that is how the main program needs the track image to be
                #You can leave the black background if you change the collision condition on the main program
                pygame.image.save(SCREEN, "randomGeneratedTrackBack.png")
                img = Image.open("randomGeneratedTrackBack.png")
                img = img.convert("RGBA")
                pixdata = img.load()
                for y in range(img.size[1]):
                    for x in range(img.size[0]):
                        if pixdata[x, y] == (0, 0, 0, 255) or pixdata[x, y] == (0, 0, 1, 255):
                            pixdata[x, y] = (0, 0, 0, 0)
                img.save("randomGeneratedTrackBack.png")

                SCREEN.blit(bg, (0,0))  
                for x in range(0, WINDOW_WIDTH, blockSize):
                    for y in range(0, WINDOW_HEIGHT, blockSize):
                        currentCell = maze.cell_at(int(x/blockSize),int(y/blockSize))
                        if currentCell.walls["N"] == False and currentCell.walls["S"] == False:
                            SCREEN.blit(straight2Top, straight2RectTop.move(x-20+movex,y+movey))  
                        elif currentCell.walls["E"] == False and currentCell.walls["W"] == False:
                            SCREEN.blit(straight1Top, straight1RectTop.move(x+movex,y-20+movey))  
                        elif currentCell.walls["N"] == False and currentCell.walls["W"] == False:
                            SCREEN.blit(curve3Top, curve3RectTop.move(x-15+movex,y-15+movey)) 
                        elif currentCell.walls["W"] == False and currentCell.walls["S"] == False:
                            SCREEN.blit(curve2Top, curve2RectTop.move(x-15+movex,y-15+movey))     
                        elif currentCell.walls["E"] == False and currentCell.walls["N"] == False:
                            SCREEN.blit(curve4Top, curve4RectTop.move(x-15+movex,y-15+movey))
                        elif currentCell.walls["S"] == False and currentCell.walls["E"] == False:
                            SCREEN.blit(curve1Top, curve1RectTop.move(x-15+movex,y-15+movey)) 
                          
                pygame.image.save(SCREEN, "randomGeneratedTrackFront.png")

                
                break

                
            else:
                #It wasnt long enough so we start again
                trackLenght = 0
                for x in range(0, WINDOW_WIDTH, blockSize):
                    for y in range(0, WINDOW_HEIGHT, blockSize):
                        maze.cell_at(int(x/blockSize),int(y/blockSize)).walls["N"] = True
                        maze.cell_at(int(x/blockSize),int(y/blockSize)).walls["S"] = True
                        maze.cell_at(int(x/blockSize),int(y/blockSize)).walls["E"] = True
                        maze.cell_at(int(x/blockSize),int(y/blockSize)).walls["W"] = True
                        maze.cell_at(int(x/blockSize),int(y/blockSize)).color = 0, 0, 0
                
                #Force occupied cells
                maze.cell_at(3,3).walls["N"] = False
                maze.cell_at(4,3).walls["N"] = False
                maze.cell_at(5,3).walls["N"] = False
                maze.cell_at(6,3).walls["N"] = False

                currentCell = maze.cell_at(startx, starty)
    return
 
generateRandomMap()
 
pygame.quit()
sys.exit()            
            
