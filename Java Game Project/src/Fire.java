import bagel.DrawOptions;
import bagel.util.Point;


/**
 * Fire are attacks used by the enemies of our games.
 */
public class Fire extends GameEntity{
    private boolean isActive;
    private DrawOptions fireDirection;


    /**
     * Constructor for the fire attack of the enemy
     * @param imageLoc location of the fire image file
     * @param topLeftCorner top left corner location for the fire
     */
    public Fire(String imageLoc, Point topLeftCorner) {
        super(imageLoc, topLeftCorner);
        this.isActive = false;
        this.fireDirection = new DrawOptions();
    }

    /**
     * Check if the fire would damage the player
     * @param player This is the player in question
     */
    public boolean wouldDamagePlayer(Player player){
        if (isActive && this.isColliding(player)){
            if(!player.getImmuneState()) {
                return true;
            }
        }
        fireDirection = new DrawOptions();
        return false;
    }

    /**
     * Set the fire's current active state to the state specified(true or false)
     * @param state is the new state of activity for fire
     */
    public void setActiveState(boolean state) {
        isActive = state;
    }

    /**
     * Render fire with rotation specified by direction from the attackPoint
     * @param direction is which direction the enemy is attacking from
     * @param attackPoint is point of attack of the enemy
     */
    public void fireRender(int direction, Point attackPoint){
        double fireWidth = super.getEntityImage().getWidth();
        double fireHeight = super.getEntityImage().getHeight();
        double newFireX, newFireY;
        if (direction == Enemy.TOP_LEFT){
            newFireX = attackPoint.x - fireWidth;
            newFireY = attackPoint.y - fireHeight;
        }else if(direction == Enemy.TOP_RIGHT){
            newFireX = attackPoint.x;
            newFireY = attackPoint.y - fireHeight;
        }else if(direction == Enemy.BOT_LEFT){
            newFireX = attackPoint.x - fireWidth;
            newFireY = attackPoint.y;
        }else if(direction == Enemy.BOT_RIGHT){
            newFireX = attackPoint.x;
            newFireY = attackPoint.y;
        }else{

            // Notify user in case invalid directional input
            System.out.println("Invalid Direction");
            return;
        }

        Point newFireLoc = new Point(newFireX, newFireY);
        super.changeLocation(newFireLoc);
        drawFire(direction);

    }

    /**
     * Draw the fire
     * @param direction This is direction for which the fire is to be drawn
     */
    public void drawFire(int direction){
        if (isActive == false){
            return;
        }
        if (direction==Enemy.BOT_LEFT){
            fireDirection.setRotation(Math.PI*3/2);
        }else if (direction == Enemy.BOT_RIGHT){
            fireDirection.setRotation(Math.PI);
        }else if(direction == Enemy.TOP_RIGHT){
            fireDirection.setRotation(Math.PI/2);
        }else if(direction == Enemy.TOP_LEFT){
            fireDirection.setRotation(0);
        }
        double fireX = super.getTopLeftPoint().x;
        double fireY = super.getTopLeftPoint().y;
        super.getEntityImage().drawFromTopLeft(fireX, fireY, fireDirection);


    }


}
