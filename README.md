# WordGame

**Project 1: Crosswords game**

The objective of this project was to develop a program that creates a board to play crosswords and save the resulting game session in a text file. A list of possible words that can be used to create the board is specified in a .txt file that is selected and imported when running the program in console mode. Restrictions against invalid inputs were implemented and the program robustness was ensured.

**Project 2: Multiplayer Crosswords game**

In this project, the saved game boards are used in a variation of the crossword game where the words must be completed letter by letter in order. The game can be played by 2 to 4 players and each player receives an equal-sized bag of letters from the words present in the specific board imported. 

Each player randomly receives by default 50% of the total number of letters on the board divided by the number of players in the game session. Alternatively, it can specified at the beginning of the game session the number of letters each player hand receives with a maximum of 80% of the number of letters on the board also divided by the number of players. The remaining letters stay in an invisible global "bag", from which a letter is drawn randomly to the player's hand that correctly inserted a letter.

The words must be completed in order and the instant a player completes a word, he receives one point, whereas if two words are completed with one play, 2 points are received. Once all the words are completed, the player with the most points wins. 

Each player has two consecutive turns in which he has 3 play options:

1 - A letter can be inserted by specifying their position on the board

2 - A letter can be swapped between the hand and the global "bag".

3 - A play can be passed

Finally, each player also has the option to QUIT the game and the gaming loop ends if fewer than 2 players are playing with the one with the greatest score being attributed the win.

UI:

1 - Defining the game parameters (imported board, nº  of players, nº of letters in the hand per player):

<img width="311" alt="1" src="https://github.com/user-attachments/assets/ba0e0d60-b61f-490d-8c67-ff3ce93c5c9d">

2 - Initiating the game:

<img width="330" alt="2" src="https://github.com/user-attachments/assets/ea387cb7-2abd-4a2e-8ad7-a467f8cfaf27">

3 - Completing a word:

<img width="314" alt="3" src="https://github.com/user-attachments/assets/542819a7-9fb4-4139-8901-edc00ab4a37b">









