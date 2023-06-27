import bagel.util.Point;


/**
 * This class encapsulate Navec, which are boss enemy of level 1
 */
public class Navec extends Enemy{
    private static final String NAVEC_LEFT_IMG_ADDRESS = "res/navec/navecLeft.png";
    private static final String NAVEC_RIGHT_IMG_ADDRESS = "res/navec/navecRight.png";
    private static final String NAVEC_LEFT_IMMUNE_IMG = "res/navec/navecInvincibleLeft.png";
    private static final String NAVEC_RIGHT_IMMUNE_IMG = "res/navec/navecInvincibleRight.png";
    private static final String NAVEC_FIRE = "res/navec/navecFire.png";
    private static final int MAX_HP=80;
    private static final int NAVEC_RANGE = 200;
    private static final int NAVEC_DMG = 20;
    private Fire navecFire;

    /**
     * Constructor for the Navec
     * @param topLeftCorner the position of the top left corner of the Navec
     */
    public Navec(Point topLeftCorner) {
        super(NAVEC_RIGHT_IMG_ADDRESS, topLeftCorner, MAX_HP);

        if (super.getDirection() == Character.LEFT){
            super.charImgChange(NAVEC_LEFT_IMG_ADDRESS);
        }

        // Set demonFire to appropriate location later when demon attacks
        navecFire = new Fire(NAVEC_FIRE, topLeftCorner);
    }

    @Override
    public double hpPercent() {
        return 100*((double)super.getHealth())/MAX_HP;
    }

    @Override
    public void playerDetect(Player player){
        if (this.isInRange(player,NAVEC_RANGE)){
            navecFire.setActiveState(true);
            attackPlayer(player);
        }else{
            navecFire.setActiveState(false);
        }
    }

    @Override
    public void attackPlayer(Player player) {

        Point playerCentre = player.getCentre();
        Point navecCentre = super.getCentre();
        Point attackPoint; // the point from where attack come from
        int attackDirection;
        if (playerCentre.x <= navecCentre.x){
            if (playerCentre.y<= navecCentre.y){
                attackDirection = Enemy.TOP_LEFT;
                attackPoint = super.getTopLeftPoint();
            }else{
                attackDirection = Enemy.BOT_LEFT;
                attackPoint = super.getBotLeftPoint();
            }
        }else{
            if (playerCentre.y <= navecCentre.y){
                attackDirection = Enemy.TOP_RIGHT;
                attackPoint = super.getTopRightPoint();
            }else{
                attackDirection = Enemy.BOT_RIGHT;
                attackPoint = super.getBotRightPoint();
            }
        }
        navecFire.fireRender(attackDirection, attackPoint);
        if (navecFire.wouldDamagePlayer(player)){
            this.damagePlayer(player);
        }
    }

    @Override
    public void damagePlayer(Player player) {
        player.takeDamage(NAVEC_DMG);
        player.setImmuneState(true);
        int playerHp = player.getHealth();
        System.out.println("Navec inflicts "+NAVEC_DMG+" damage Points on Fae.  " +
                "Fae's current health:  "+playerHp+"/"+Player.getMaxHp());
    }

    /**
     * Manage the immunity state of the Navec, making sure appropriate images are rendered
     */
    @Override
    public void stateManage(){
        if ((super.getImmuneStateCounter()+1) >= Character.getMaxImmuneCount()){
            this.toVulnerableImg();
        }
        super.stateManage();
    }

    /**
     * Have Navec take damage(if not immune) and changing their image as well as
     * their immunity status to be appropriate for after the damage was taken
     * @param amount This is amount of damage taken
     */
    @Override
    public void takeDamage(int amount){
        if (!super.getImmuneState()){
            super.takeDamage(amount);
            super.setImmuneState(true);
            this.toImmuneImg();
            System.out.println("Fae inflicts "+amount+" damage Points on Navec. " +
                    "Navec's current health:  "+this.getHealth()+"/"+MAX_HP);
        }

    }

    @Override
    public void toImmuneImg() {
        if (super.getDirection()==Character.LEFT){
            charImgChange(NAVEC_LEFT_IMMUNE_IMG);
        }else{
            charImgChange(NAVEC_RIGHT_IMMUNE_IMG);
        }
    }

    @Override
    public void toVulnerableImg() {
        if (super.getDirection()==Character.LEFT){
            charImgChange(NAVEC_LEFT_IMG_ADDRESS);
        }else{
            charImgChange(NAVEC_RIGHT_IMG_ADDRESS);
        }
    }

    @Override
    public void swapDirection() {
        int newDirection = super.setOppDirection();

        // We don't care which way demon is facing if they are moving up or down
        if (newDirection == Character.UP || newDirection == Character.DOWN){
            return;
        }else if (newDirection == Character.LEFT){
            if (super.getImmuneState()){
                super.charImgChange(NAVEC_LEFT_IMMUNE_IMG);
            }
            else{
                super.charImgChange(NAVEC_LEFT_IMG_ADDRESS);
            }
        }else{
            if (super.getImmuneState()){
                super.charImgChange(NAVEC_RIGHT_IMMUNE_IMG);
            }else{
                super.charImgChange(NAVEC_RIGHT_IMG_ADDRESS);
            }
        }
    }
}
