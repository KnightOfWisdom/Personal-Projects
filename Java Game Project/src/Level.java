import bagel.DrawOptions;
import bagel.Font;
import bagel.Input;
import bagel.Keys;
import bagel.util.Colour;
import bagel.util.Point;

import java.util.ArrayList;


/**
 * The parent class for all possible levels in the game.
 * Contains necessary attributes and methods applicable to all child levels
 */
public abstract class Level {
    private final static int PLAYER_HP_X=20;
    private final static int PLAYER_HP_Y=25;
    private final static String HP_FONT_ADDRESS = "res/frostbite.ttf";
    public final static int ORANGE_HP_BOUNDARY = 65;
    public final static int RED_HP_BOUNDARY = 35;
    private final Font PLAYER_HP_FONT;
    private DrawOptions playerHpColor = new DrawOptions();
    private final static Colour GREEN = new Colour(0, 0.8, 0.2);
    private final static Colour ORANGE = new Colour(0.9, 0.6, 0);
    private final static Colour RED = new Colour(1, 0, 0);
    private double minXBound, minYBound, maxXBound, maxYBound;


    /**
     * Constructor for the level
     */
    public Level(){
        PLAYER_HP_FONT = new Font(HP_FONT_ADDRESS, 30);
        playerHpColor.setBlendColour(GREEN);
    }

    /**
     * Get the address of the font file for writing Hp to the game
     * @return Address of the font file
     */
    public static String getHpFontAddress(){
        return HP_FONT_ADDRESS;
    }

    /**
     * Draw the Hp of player at the right location
     */
    public void drawPlayerHp(Player player){
        double currPercent = player.hpPercent();
        if (currPercent <= RED_HP_BOUNDARY){
            playerHpColor.setBlendColour(RED);
        }else if (currPercent<= ORANGE_HP_BOUNDARY){
            playerHpColor.setBlendColour(ORANGE);
        }
        PLAYER_HP_FONT.drawString(Math.round(currPercent) + "%", PLAYER_HP_X,
                PLAYER_HP_Y, playerHpColor);
    }

    /**
     * Load the level given the details from csv file
     * @param fileLoc The address of file storing details about the level
     */
    public abstract void loadLevel(String fileLoc);

    /**
     * Render hp of Characters within the game
     */
    public abstract void drawHp();

    /**
     * Check if the player has beat the level
     * @return Whether the win condition has been met or not
     */
    public abstract boolean hasWon();

    /**
     * Check if the player has lost the game
     * @return Whether the loss condition has been met or not
     */
    public abstract boolean hasLost();

    /**
     * Update the game state every frame
     * @param input user key input
     */
    public abstract void update(Input input);

    /**
     * Set the x-value of the left boundary
     * @param minXBound x-value of the left boundary
     */
    public void setMinXBound(double minXBound) {
        this.minXBound = minXBound;
    }

    /**
     * Set the y-value of the top boundary
     * @param minYBound y-value of the top boundary
     */
    public void setMinYBound(double minYBound) {
        this.minYBound = minYBound;
    }

    /**
     * Set the x-value of the right boundary
     * @param maxXBound x-val of the right boundary
     */
    public void setMaxXBound(double maxXBound) {
        this.maxXBound = maxXBound;
    }

    /**
     * Set the y-value of the bottom boundary
     * @param maxYBound y-val of the bottom boundary
     */
    public void setMaxYBound(double maxYBound) {
        this.maxYBound = maxYBound;
    }

    /**
     * Get the x-value of the left boundary
     * @return x-val of left boundary
     */
    public double getMinXBound() {
        return minXBound;
    }

    /**
     * Get the y-value of the top boundary
     * @return y-val of top boundary
     */
    public double getMinYBound() {
        return minYBound;
    }

    /**
     * Get the x-value of the right boundary
     * @return x-val of the right boundary
     */
    public double getMaxXBound() {
        return maxXBound;
    }

    /**
     * Get the y-value of the right boundary
     * @return y-val of the right boundary
     */
    public double getMaxYBound() {
        return maxYBound;
    }

    /**
     * Check for collision between player and boundary
     * @param player This is the player in question
     * @return Whether there has been collision or not
     */
    public boolean playerBoundaryCollisionChecker(Player player){
        boolean isOutOfBound = false;
        Point playerLoc = player.getTopLeftPoint();
        if ((playerLoc.x < minXBound) || (playerLoc.x > maxXBound)){
            isOutOfBound = true;
        }
        if ((playerLoc.y<minYBound) || (playerLoc.y>maxYBound)){
            isOutOfBound = true;
        }

       return isOutOfBound;
    }

    /**
     * Move the player according to supplied input
     * @param player is the instance of player
     * @param input is the input keys
     * */
    public void playerControl(Player player, Input input, ArrayList<Obstacle> gameObstacles){
        if (input.isDown(Keys.DOWN)){
            player.movePlayer(Character.DOWN, false);
            if (playerObstacleCollisionChecker(player, gameObstacles)){
                player.movePlayer(Character.UP, true);
            }
            if (playerBoundaryCollisionChecker(player)){
                player.movePlayer(Character.UP, true);
            }
        }
        if (input.isDown(Keys.UP)){
            player.movePlayer(Character.UP, false);
            if (playerObstacleCollisionChecker(player, gameObstacles)){
                player.movePlayer(Character.DOWN, true);
            }
            if (playerBoundaryCollisionChecker(player)){
                player.movePlayer(Character.DOWN, true);
            }
        }
        if (input.isDown(Keys.LEFT)){
            player.movePlayer(Character.LEFT, false);
            if (playerObstacleCollisionChecker(player, gameObstacles)){
                player.movePlayer(Character.RIGHT, true);
            }
            if (playerBoundaryCollisionChecker(player)){
                player.movePlayer(Character.RIGHT, true);
            }
        }if (input.isDown(Keys.RIGHT)){
            player.movePlayer(Character.RIGHT, false);
            if (playerObstacleCollisionChecker(player, gameObstacles)){
                player.movePlayer(Character.LEFT, true);
            }
            if (playerBoundaryCollisionChecker(player)){
                player.movePlayer(Character.LEFT, true);
            }
        }
    }

    /**
     * Check for player collision with obstacles in the game
     * @param player This is the player in question
     * @param gameObstacles This is the Arraylist of Obstacle entities
     * @return whether the player has collided with an obstacle or not
     */
    private boolean playerObstacleCollisionChecker(Player player, ArrayList<Obstacle> gameObstacles){
        for (int i=0; i<gameObstacles.size(); i++){
            Obstacle obstacle = gameObstacles.get(i);
            boolean isColliding = obstacle.playerCollisionChecker(player);
            if (isColliding){
                return true;
            }
        }
        return false;
    }


}
