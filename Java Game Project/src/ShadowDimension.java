import bagel.*;

/**
 * Class for the game of SWEN20003 Project 2, Semester 2, 2022
 *
 * Please enter your name below
 * @author Ke Liao
 * @version 1.0
 */

public class ShadowDimension extends AbstractGame {
    private final static int WINDOW_WIDTH = 1024;
    private final static int WINDOW_HEIGHT = 768;
    private final static int TITLE_X = 260;
    private final static int TITLE_Y = 250;
    private final static int LV0_INS_X_OFFSET = 90;
    private final static int LV0_INS_Y_OFFSET = 190;
    private final static int LV1_INS_X = 350;
    private final static int LV1_INS_Y = 350;
    private final static int TITLE_FONT_SIZE = 75;
    private final static int INSTRUCTION_FONT_SIZE = 40;
    private final static String FONT_FILE_ADDRESS = "res/frostbite.ttf";
    private final static String GAME_TITLE = "SHADOW DIMENSION";
    private final static String LEVEL0_INSTRUCTION = "PRESS SPACE TO START\n" +
            "USE ARROW KEYS TO FIND GATE";
    private final static String LEVEL1_INSTRUCTION = "PRESS SPACE TO START\n" +
            "PRESS A TO ATTACK\n" +
            "DEFEAT NAVEC TO WIN";
    private final static String LOSE_MESSAGE = "GAME OVER!";
    private final static String WIN_MESSAGE = "CONGRATULATIONS!";
    private final static String LEVEL_WIN_MESSAGE = "LEVEL COMPLETE";
    private final static int MAX_TRANSITION_COUNTER= 180;
    private final Image LV0_BACKGROUND = new Image("res/background0.png");
    private final Image LV1_BACKGROUND = new Image("res/background1.png");
    private final Font TITLE_FONT;
    private final Font INSTRUCTION_FONT;
    private boolean level1Start, level0Start, level0Win, level1Win, gameLose;
    private int levelTransitionCounter;

    private double winMessageXLoc, winMessageYLoc, loseMessageXLoc, loseMessageYLoc;
    private Level level0, level1;


    /**
     * Constructor for the game
     */
    public ShadowDimension(){
        super(WINDOW_WIDTH, WINDOW_HEIGHT, GAME_TITLE);
        this.level0Start = this.level1Start = false;
        TITLE_FONT = new Font(FONT_FILE_ADDRESS, TITLE_FONT_SIZE);
        INSTRUCTION_FONT = new Font(FONT_FILE_ADDRESS, INSTRUCTION_FONT_SIZE);
        level1Start = level0Start = level0Win = level1Win = gameLose = false;
        winMessageXLoc = ((double) (Window.getWidth() - TITLE_FONT.getWidth(WIN_MESSAGE)))/2;
        winMessageYLoc = ((double) (Window.getHeight() + TITLE_FONT_SIZE))/2;
        loseMessageXLoc = ((double) (Window.getWidth()- TITLE_FONT.getWidth(LOSE_MESSAGE)))/2;
        loseMessageYLoc = ((double) (Window.getHeight() + TITLE_FONT_SIZE))/2;
        level0 = new Level0();
        level1 = new Level1();
        levelTransitionCounter = 0;
    }

    /**
     * The entry point for the program.
     */
    public static void main(String[] args) {
        ShadowDimension game = new ShadowDimension();
        game.run();

    }


    /**
     * Performs a state update for the game, allow the display of appropriate message
     * and levels depending on keys pressed.
     * Allows the game to exit when the escape key is pressed.
     * @param input keys
     */
    @Override
    protected void update(Input input) {


        if (input.wasPressed(Keys.ESCAPE)){
            Window.close();
        }
        if (input.wasPressed(Keys.W)){
            level0Start = true;
            level0Win = true;
        }
        if (!level0Start){
            if (input.wasPressed(Keys.SPACE)){
                if (!level0Start){
                    level0Start = true;
                }
            }
            this.drawStartScreen();

        }else if (gameLose){
            this.drawMessage(LOSE_MESSAGE);
        }else if (!level1Start && level0Win && levelTransitionCounter <= MAX_TRANSITION_COUNTER){
            this.drawMessage(LEVEL_WIN_MESSAGE);
            levelTransitionCounter+=1;
        }
        else if (!level1Start && level0Win){
            if (input.wasPressed(Keys.SPACE)){
                if (!level1Start){
                    level1Start = true;
                }
            }
            this.drawLv1StartMessage();
        }else if (level1Win && level0Win){
            this.drawMessage(WIN_MESSAGE);
        }else if (level0Win && level1Start){
            LV1_BACKGROUND.draw(Window.getWidth()/2.0, Window.getHeight()/2.0);
            level1.update(input);
        }else{
            LV0_BACKGROUND.draw(Window.getWidth()/2.0, Window.getHeight()/2.0);
            level0.update(input);
        }

        if (level0.hasLost() || level1.hasLost()){
            gameLose = true;
        }

        if (level1.hasWon()){
            level1Win = true;
        }
        if (level0.hasWon()){
            level0Win = true;
        }

    }



    /**
     * Draw the message of Start Screen
     */
    private void drawStartScreen(){
        TITLE_FONT.drawString(GAME_TITLE, TITLE_X, TITLE_Y);
        INSTRUCTION_FONT.drawString(LEVEL0_INSTRUCTION,TITLE_X + LV0_INS_X_OFFSET, TITLE_Y + LV0_INS_Y_OFFSET);
    }

    /**
     * Draw the instruction message for level 1
     */
    private void drawLv1StartMessage(){
        INSTRUCTION_FONT.drawString(LEVEL1_INSTRUCTION, LV1_INS_X, LV1_INS_Y);
    }

    /**
     * Draw the message inputted
     * @param message This is the message to be drawn
     */
    private void drawMessage(String message){
        TITLE_FONT.drawString(message, (Window.getWidth()/2.0 - (TITLE_FONT.getWidth(message)/2.0)),
                (Window.getHeight()/2.0 + (TITLE_FONT_SIZE/2.0)));
    }

}
