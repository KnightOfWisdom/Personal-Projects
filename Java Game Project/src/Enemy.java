import bagel.util.Point;

import java.util.Random;

/**
 * This class encapsulates the enemy Entities for the game
 */
public abstract class Enemy extends Character{
    private double speed;
    private int direction;

    /**
     * Constant representing bottom left corner of enemy
     */
    public static final int BOT_LEFT = 1;

    /**
     * Constant representing top left corner of enemy
     */
    public static final int TOP_LEFT = 2;

    /**
     * Constant representing top right corner of enemy
     */
    public static final int TOP_RIGHT = 3;

    /**
     * Constant representing bottom right corner of enemy
     */
    public static final int BOT_RIGHT = 4;
    private static final double MAX_SPEED = 0.7;
    private static final double MIN_SPEED = 0.2;
    private static final double POSITIVE_SPEED_MULTIPLIER = 1.5;
    private static final double NEGATIVE_SPEED_MULTIPLIER = 0.5;


    /**
     * Constructor for the enemy character
     * @param imageLoc location of the image for the enemy
     * @param topLeftCorner top left corner of the enemy
     * @param hp hp of the enemy
     */
    public Enemy(String imageLoc, Point topLeftCorner, int hp) {
        super(imageLoc, topLeftCorner, hp);
        Random rand = new Random();
        direction = rand.nextInt(4)+1;
        speed = MIN_SPEED + (rand.nextDouble()*(MAX_SPEED-MIN_SPEED));
    }

    /**
     * Check if player is within attack range and attack if so
     * @param player is the player character
     */
    public abstract void playerDetect(Player player);

    /**
     * Attack the player and also get the attack rendered
     * @param player is the player character
     */
    public abstract void attackPlayer(Player player);

    /**
     * Damage the player(for the cases where their attack lands)
     * @param player is the player character
     */
    public abstract void damagePlayer(Player player);

    /**
     * Change the image of enemy to that of immune state
     */
    public abstract void toImmuneImg();

    /**
     * Change the img of the enemy to that of non_immune state
     */
    public abstract void toVulnerableImg();

    /**
     * Get the direction the enemy is moving in
     * @return The constant representing direction the enemy is moving in
     */
    public int getDirection() {
        return direction;
    }


    /**
     * Set the direction of enemy to the opposite direction
     * @return The new direction of the character
     */
    public int setOppDirection(){
        if (direction == Character.UP){
            direction = Character.DOWN;
        }else if(direction == Character.DOWN){
            direction = Character.UP;
        }else if(direction == Character.LEFT){
            direction = Character.RIGHT;
        }else{
            direction = Character.LEFT;
        }
        return direction;
    }

    /**
     * Move the enemy based on the timeScale
     * @param timeScale is the timescale which affects the enemy speed
     */
    public void moveEnemy(int timeScale) {
        if (timeScale == 0){
            super.move(this.direction, this.speed);
        }else if (timeScale>0){
            double currSpeed = Math.pow(POSITIVE_SPEED_MULTIPLIER, timeScale) * speed;
            super.move(this.direction, currSpeed);
        }else{
            double currSpeed = Math.pow(NEGATIVE_SPEED_MULTIPLIER, -1*timeScale) * speed;
            super.move(this.direction, currSpeed);
        }

    }

    /**
     * Handling Swapping direction of enemy and associated change in image
     */
    public abstract void swapDirection();

    /**
     * Allow enemy detect character within specified range
     * @param player The player in question
     * @param range The radius of the detection range
     * @return Whether the player is within range of enemy or not
     */
    public boolean isInRange(Player player, int range){
        Point playerCentre = player.getCentre();
        Point demonCentre = this.getCentre();
        double xDiff = playerCentre.x - demonCentre.x;
        double yDiff = playerCentre.y - demonCentre.y;
        double entityDist = Math.sqrt(Math.pow(xDiff,2) + Math.pow(yDiff,2));
        if (entityDist <= range) {
            return true;
        }
        return  false;
    }

}
