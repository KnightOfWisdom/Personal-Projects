import bagel.util.Point;

/**
 * SinkHoles are subclass of Obstacles which damages player in the event the
 * player collides with SinkHole
 */
public class SinkHole extends Obstacle {

    private static final String SINKHOLE_IMG_ADDRESS="res/sinkhole.png";
    private static final int SINKHOLE_DMG = 30;

    /**
     * Constructor for the sinkhole
     * @param topLeftCorner The location of the top left corner of the sinkhole collision box
     */
    public SinkHole(Point topLeftCorner) {
        super(SINKHOLE_IMG_ADDRESS, topLeftCorner);
    }

    /**
     * Check for collision with player & inflicting damage if it does
     */
    @Override
    public boolean playerCollisionChecker(Player player){
        if (this.isColliding(player) && !player.getImmuneState()){
            player.takeDamage(SINKHOLE_DMG);
            int playerHp = player.getHealth();
            System.out.println("Sinkhole inflicts "+SINKHOLE_DMG+" damage Points on Fae.  " +
                    "Fae's current health:  "+playerHp+"/"+Player.getMaxHp());
            this.setActive(false);  // Sinkhole no longer active once hit player
            return true;
        }
        return false;
    }

}
