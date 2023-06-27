import bagel.util.Point;

/**
 * This Class represents instances of immovable obstacle within the
 * levels of the game.
 */
public class Obstacle extends GameEntity{
    private boolean isActive;
    public Obstacle(String imageLoc, Point topLeftCorner) {
        super(imageLoc, topLeftCorner);
        isActive = true;
    }

    /**
     * For checking and dealing with collision with Enemy entities
     */
    public void enemyCollisionHandler(Enemy enemy){
        if (this.isColliding(enemy)){
            enemy.swapDirection();
        }
    }

    /**
     * For checking collisions with player
     */
    public boolean playerCollisionChecker(Player player){
        if (this.isColliding(player)){
            return true;
        }
        return false;
    }

    /**
     * Check if the obstacle is active
     * @return whether the obstacle is active or not
     */
    public boolean isActive() {
        return isActive;
    }

    /**
     * Set the obstacle's active status to that of inputted state
     * @param active This is new state for if the obstacle is active or not
     */
    public void setActive(boolean active) {
        isActive = active;
    }
}
