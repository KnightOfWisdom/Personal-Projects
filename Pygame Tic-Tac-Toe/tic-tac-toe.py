import pygame, sys
from pygame.locals import *
import time, random

# Constants
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
FPS = 60
SCREEN_WIDTH = 800
SCREEN_HEIGHT = 800
GRIDS_IN_ROW = 3
GRIDS_IN_COL = 3
EQUAL_PIECES_TO_WIN = 3

# Classes
class Pieces(pygame.sprite.Sprite):
    def __init__(self, centerX, centerY, type):
        self.type = type
        super().__init__()
        if type == "X":
            self.image = pygame.image.load("Assets/Cross.png")
        elif type == "O":
            self.image = pygame.image.load("Assets/Circle.png")
        else:
            print("Type Does not Exist!")
            exit()
        self.rect = self.image.get_rect()
        self.rect.center = (centerX, centerY)
    def getType(self):
        return self.type
    def draw(self):
        displaySurf.blit(self.image, self.rect)


class Grid(pygame.sprite.Sprite):
    def __init__(self, left, top):
        super().__init__()
        self.piece = None
        self.rect = pygame.Rect(left, top, 200, 200)
    def click(self, turn, event):
        mouse_pos = event.pos
        centerX = self.rect.centerx
        centerY = self.rect.centery
        if event.type == pygame.MOUSEBUTTONDOWN:
            if self.rect.collidepoint(mouse_pos):
                if self.piece == None:
                    self.piece = Pieces(centerX, centerY, turn)
                    return True
        return False
    def getPiece(self):
        if self.piece is not None:
            return self.piece.getType()
        
    def drawPiece(self):
        if self.piece is not None:
            self.piece.draw()
    def clearPiece(self):
        self.piece = None
    
    

pygame.init()
turn = "X"  # x goes first
framePerSec = pygame.time.Clock()
titleFont = pygame.font.SysFont("Ariel", 100)
displaySurf= pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
font = pygame.font.SysFont("Ariel", 25)
board = [[None]*3,[None]*3, [None]*3]
gameWon = False
gameDrawn = False
gameStarted = False

def createBoard(currBoard):
    gridX=100
    gridY=100
    for row_num in range (GRIDS_IN_COL):
        for col_num in range (GRIDS_IN_ROW):
            currBoard[row_num][col_num] = Grid(gridX, gridY)
            gridX += 200
        gridX = 100
        gridY += 200

def drawGameScreen():
    displaySurf.fill(WHITE)
    turnMessage = turn+"\'s turn"
    turnMessageText = font.render(turnMessage, True, BLACK)
    turnMessageRect = turnMessageText.get_rect(center=(400,750))
    displaySurf.blit(turnMessageText, turnMessageRect)
    for row in range(GRIDS_IN_COL):
        for col in range(GRIDS_IN_ROW):
            board[row][col].drawPiece()
    pygame.draw.line(displaySurf, BLACK, (100, 300), (700, 300))
    pygame.draw.line(displaySurf, BLACK, (100, 500), (700, 500))
    pygame.draw.line(displaySurf, BLACK, (300,100), (300, 700))
    pygame.draw.line(displaySurf, BLACK, (500,100), (500, 700))
    pygame.display.update()

def changeTurn(currTurn):
    if currTurn == "X":
        return "O"
    else: 
        return "X"

def addPiece(currBoard, gridPerRow, gridPerCol,currTurn, event):
    for row_num in range(gridPerCol):
        for col_num in range(gridPerRow):
            if currBoard[row_num][col_num].click(currTurn, event):
                return True
    return False

def rowWinCheck(currBoard, gridPerRow, gridPerCol):
    for row in range (0, gridPerCol):
        firstOnRow = currBoard[row][0].getPiece() 
        numEqualPiece = 0
        if (firstOnRow is None):
            continue
        else:
            numEqualPiece += 1
        for i in range (1, gridPerRow):
            currPiece = currBoard[row][i].getPiece()
            if (currPiece == firstOnRow):
                numEqualPiece += 1
        if numEqualPiece == EQUAL_PIECES_TO_WIN:
            return True
    return False

def colWinCheck(currBoard, gridPerRow, gridPerCol):
    for col in range (0, gridPerRow):
        firstOnCol = currBoard[0][col].getPiece() 
        numEqualPiece = 0
        if (firstOnCol == None):
            continue
        else:
            numEqualPiece += 1
        for i in range (1, gridPerCol):
            currPiece = currBoard[i][col].getPiece()
            if (currPiece == firstOnCol):
                numEqualPiece += 1
        if numEqualPiece == EQUAL_PIECES_TO_WIN:
            return True  
    return False

def winCheck(currBoard, gridPerRow, gridPerCol):

     # Check rows then column 
    if rowWinCheck(currBoard, gridPerRow, gridPerCol):
        return True
    if colWinCheck(currBoard, gridPerRow, gridPerCol):
        return True

    # Now check diagonals
    centrePiece = currBoard[1][1].getPiece()
    firstPiece = currBoard[0][0].getPiece()
    lastPiece = currBoard[2][2].getPiece()
    thirdPiece = currBoard[0][2].getPiece()
    seventhPiece = currBoard[2][0].getPiece()
        
    if centrePiece == firstPiece and centrePiece == lastPiece:
        if (centrePiece is not None):
            return True
    if centrePiece == thirdPiece and centrePiece == seventhPiece:
        if (centrePiece is not None):
            return True
        
    return False

def drawChecker(currBoard, gridPerRow, gridPerCol):
    for i in range(0, gridPerCol):
        for j in range(0, gridPerRow):
            if (currBoard[i][j].getPiece() is None):
                return False
    return True

def drawStatus():
    drawGameScreen()


def endGameScreen(message):
    endgameText = titleFont.render(message, True, BLACK)
    endGameTextRect = endgameText.get_rect(center = (400,350))
    restartMessage = "Press Space to Restart"
    restartText = font.render(restartMessage, 1, BLACK)
    restartTextRect = restartText.get_rect(center=(400,400))
    displaySurf.blit(endgameText, endGameTextRect)
    displaySurf.blit(restartText, restartTextRect)

def titleScreen():
    displaySurf.fill(WHITE)
    titleMessage = "TIC TAC TOE"
    instructionMessage = "Press Space to Start"
    titleText = titleFont.render(titleMessage, True, BLACK)
    titleTextRect = titleText.get_rect(center = (400,200))
    instructionText = font.render(instructionMessage, True, BLACK)
    instructionTextRect = instructionText.get_rect(center = (400,700))
    displaySurf.blit(titleText, titleTextRect)
    displaySurf.blit(instructionText, instructionTextRect)



def winScreen(turn):
    displaySurf.fill(WHITE)
    turn = changeTurn(turn)  # Makes sure we display the correct side that wins
    message = turn + " Wins!"
    endGameScreen(message)
    

def drawnScreen():
    displaySurf.fill(WHITE)
    message = "Its a Tie"
    endGameScreen(message)

def clearBoard(board):
    for row in range(GRIDS_IN_COL):
        for col in range(GRIDS_IN_ROW):
            board[row][col].clearPiece()

createBoard(board)
titleScreen()

while True:
    for event in pygame.event.get():
        if event.type == QUIT:
            pygame.quit()
            sys.exit()
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_SPACE:
                if not gameStarted:
                    gameStarted = True
                elif gameWon or gameDrawn:
                    gameDrawn = gameWon = False
                    turn = "X"
                    clearBoard(board)
                    gameStarted = False
                    titleScreen()
        elif event.type == MOUSEBUTTONDOWN and not gameWon and not gameDrawn:
            if addPiece(board, GRIDS_IN_ROW, GRIDS_IN_COL, turn, event):
                turn = changeTurn(turn)
            gameWon = winCheck(board, GRIDS_IN_ROW, GRIDS_IN_COL)
            if not gameWon:
                gameDrawn = drawChecker(board, GRIDS_IN_ROW, GRIDS_IN_COL)
     
    if gameWon:
        winScreen(turn)
    elif gameDrawn:
        drawnScreen()
    elif gameStarted: 
        drawStatus()
    pygame.display.update()
    framePerSec.tick(FPS)