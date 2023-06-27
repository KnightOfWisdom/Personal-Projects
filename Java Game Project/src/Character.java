import bagel.Image;
import bagel.util.Point;
import bagel.util.Rectangle;


/**
 * This class is for abstracting characters of the game
 */
public abstract class Character extends GameEntity{
    private int health;
    private boolean immuneState = false;
    private int immuneStateCounter = 0;
    private static final int MAX_IMMUNE_COUNT = 180;

    /**
     * Constant for representing character moving up
     */
    public static final int UP = 1;

    /**
     * Constant for representing character moving down
     */
    public static final int DOWN = 2;

    /**
     * Constant for representing character moving left
     */
    public static final int LEFT = 3;

    /**
     * Constant for representing character moving right
     */
    public static final int RIGHT = 4;


    /**
     * Constructor for Character
     * @param imageLoc is the address to image of the character
     * @param  topLeftCorner is the top left point position for character
     * @param hp is hp value of the character
     */
    public Character(String imageLoc, Point topLeftCorner, int hp) {
        super(imageLoc, topLeftCorner);
        health = hp;
    }

    /**
     * Get hp of the character
     * @return the returns the current hp
     */
    public int getHealth() {
        return health;
    }

    /**
     * Set the state of immunity to the state specified
     * @param immuneState new immune state of the Character
     */
    public void setImmuneState(boolean immuneState) {
        this.immuneState = immuneState;
    }


    /**
     * Get the state of immunity of Character
     * @return the invulnerability status of character
     */
    public boolean getImmuneState(){
        return immuneState;
    }

    /**
     * Method for taking damage
     * @param amount is amount of damage taken
     */
    public void takeDamage(int amount){
        if (!immuneState) {
            health -= amount;
        }
        if (health<0){
            health = 0;
        }
    }

    /**
     * Get the Maximum Immune Counter a character can have
     * @return the max immunity counter
     */
    public static int getMaxImmuneCount(){
        return MAX_IMMUNE_COUNT;
    }

    /**
     * Get the current Immune counter of the character
     * @return the current immuneStateCounter value
     */
    public int getImmuneStateCounter() {
        return immuneStateCounter;
    }

    /**
     * Manage the state of the character.
     * All characters have the state of invincibility/vulnerability
     */
    public void stateManage(){

        // Track the progress of immunity ensuring it expires at right time
        if (immuneState){
            immuneStateCounter += 1;
            if (immuneStateCounter>=MAX_IMMUNE_COUNT){
                setImmuneState(false);
                this.immuneStateCounter = 0;
            }
        }
    }

    /**
     * Change character's Image & collision box to match img at address for character
     * @param imgAddress This is the address for the new image
     */
    public void charImgChange(String imgAddress){
        Image characterImg = new Image(imgAddress);
        Point topLeft = super.getTopLeftPoint();
        Rectangle newCollisionBox = new Rectangle(topLeft.x,topLeft.y,
                characterImg.getWidth(), characterImg.getHeight());
        super.setCollisionBox(newCollisionBox);
        super.setEntityImage(characterImg);
    }

    /**
     * Check if the character is dead
     * @return whether the character is dead or not
     */
    public boolean isDead(){
        if (health<=0){
            return true;
        }
        return false;
    }

    /**
     * Methods for moving character by step_size in direction
     * @param direction direction the Character is moving in
     * @param STEP_SIZE is how far they move
     */
    public void move(int direction, double STEP_SIZE) {
        Point currTopLeft = super.getTopLeftPoint();
        if (direction == Character.UP){
            double newLeft = currTopLeft.x;
            double newTop = (currTopLeft.y)-STEP_SIZE;
            Point newTopLeft = new Point(newLeft, newTop);
            super.changeLocation(newTopLeft);
        }else if (direction == Character.DOWN){
            double newLeft = currTopLeft.x;
            double newTop = (currTopLeft.y)+STEP_SIZE;
            Point newTopLeft = new Point(newLeft, newTop);
            super.changeLocation(newTopLeft);
        }else if (direction == Character.LEFT){
            double newLeft = (currTopLeft.x)-STEP_SIZE;
            double newTop = currTopLeft.y;
            Point newTopLeft = new Point(newLeft, newTop);
            super.changeLocation(newTopLeft);
        }else if (direction == Character.RIGHT){
            double newLeft = (currTopLeft.x)+STEP_SIZE;
            double newTop = currTopLeft.y;
            Point newTopLeft = new Point(newLeft, newTop);
            super.changeLocation(newTopLeft);
        }
    }

    /**
     * Find the percent of hp the character in question has left
     * @return The percentage of hp character has left
     */
    public abstract double hpPercent();

}
