import bagel.util.Point;

import java.util.Random;


/**
 * This class represents the basic enemy found in the game
 */
public class Demon extends Enemy{

    private static final String DEMON_LEFT_IMG_ADDRESS = "res/demon/demonLeft.png";
    private static final String DEMON_RIGHT_IMG_ADDRESS = "res/demon/demonRight.png";
    private static final String DEMON_IMMUNE_LEFT_ADDRESS = "res/demon/demonInvincibleLeft.png";
    private static final String DEMON_IMMUNE_RIGHT_ADDRESS = "res/demon/demonInvincibleRight.png";
    private static final String DEMON_FIRE_ADDRESS = "res/demon/demonFire.png";
    private static final int DEMON_RANGE = 150;
    public static final int DEMON_DMG = 10;
    private Fire demonFire;
    private static final int MAX_HP=40;

    private boolean stationary;


    /**
     * Constructor for the demon entities in the game
     * @param topLeftCorner is the position of top left corner of demon
     */
    public Demon(Point topLeftCorner) {
        super(DEMON_RIGHT_IMG_ADDRESS, topLeftCorner, MAX_HP);

        // Assign random state to demon
        Random rand = new Random();
        int isStationary = rand.nextInt(2);
        if (isStationary==1){
            stationary = true;
        }else{
            stationary = false;
        }

        if (this.getDirection() == Character.LEFT && !stationary){
            this.charImgChange(DEMON_LEFT_IMG_ADDRESS);
        }

        // Set demonFire to appropriate location later when demon attacks
        demonFire = new Fire(DEMON_FIRE_ADDRESS, topLeftCorner);

    }

    /**
     * Move the demon if not stationary
     */
    @Override
    public void moveEnemy(int timeScale) {
        if (!stationary){
            super.moveEnemy(timeScale);
        }

    }

    @Override
    public void swapDirection(){
        int newDirection = super.setOppDirection();

        // We don't care which way demon is facing if they are moving up or down
        if (newDirection == Character.UP || newDirection == Character.DOWN){
            return;
        }else if (newDirection == Character.LEFT){
            if (super.getImmuneState()){
                super.charImgChange(DEMON_IMMUNE_LEFT_ADDRESS);
            }
            else{
                super.charImgChange(DEMON_LEFT_IMG_ADDRESS);
            }
        }else{
            if (super.getImmuneState()){
                super.charImgChange(DEMON_IMMUNE_RIGHT_ADDRESS);
            }else{
                super.charImgChange(DEMON_RIGHT_IMG_ADDRESS);
            }
        }
    }

    @Override
    public double hpPercent() {
        return 100*(super.getHealth())/MAX_HP;
    }

    @Override
    public void playerDetect(Player player){
        if (this.isInRange(player,DEMON_RANGE)){
            demonFire.setActiveState(true);
            attackPlayer(player);
        }else{
            demonFire.setActiveState(false);
        }
    }

    @Override
    public void attackPlayer(Player player) {
        Point playerCentre = player.getCentre();
        Point demonCentre = super.getCentre();
        Point attackPoint; // the point from where attack come from
        int attackDirection;
        if (playerCentre.x <= demonCentre.x){
            if (playerCentre.y<=demonCentre.y){
                attackDirection = Enemy.TOP_LEFT;
                attackPoint = super.getTopLeftPoint();
            }else{
                attackDirection = Enemy.BOT_LEFT;
                attackPoint = super.getBotLeftPoint();
            }
        }else{
            if (playerCentre.y <= demonCentre.y){
                attackDirection = Enemy.TOP_RIGHT;
                attackPoint = super.getTopRightPoint();
            }else{
                attackDirection = Enemy.BOT_RIGHT;
                attackPoint = super.getBotRightPoint();
            }
        }
        demonFire.fireRender(attackDirection, attackPoint);
        if (demonFire.wouldDamagePlayer(player)){
            damagePlayer(player);
        }
    }

    /**
     * Manage the immunity state of the Demon, making sure appropriate images are rendered
     */
    @Override
    public void stateManage(){
        if ((super.getImmuneStateCounter()+1) >= Character.getMaxImmuneCount()){
            this.toVulnerableImg();
        }
        super.stateManage();
    }

    /**
     * Have demon take damage(if not immune) and changing their image as well as
     * their immunity status to be appropriate for after the damage was taken
     * @param amount This is the amount of damage taken
     */
    @Override
    public void takeDamage(int amount){
        if (!super.getImmuneState()){
            super.takeDamage(amount);
            super.setImmuneState(true);
            this.toImmuneImg();
            System.out.println("Fae inflicts "+amount+" damage Points on Demon.  " +
                    "Demon's current health:  "+this.getHealth()+"/"+MAX_HP);
        }

    }

    @Override
    public void damagePlayer(Player player) {
        player.takeDamage(DEMON_DMG);
        player.setImmuneState(true);
        int playerHp = player.getHealth();
        System.out.println("Demon inflicts "+DEMON_DMG+" damage Points on Fae.  " +
                "Fae's current health:  "+playerHp+"/"+Player.getMaxHp());
    }

    @Override
    public void toImmuneImg() {
        if (super.getDirection()==Character.LEFT){
            charImgChange(DEMON_IMMUNE_LEFT_ADDRESS);
        }else{
            charImgChange(DEMON_IMMUNE_RIGHT_ADDRESS);
        }
    }

    @Override
    public void toVulnerableImg() {
        if (super.getDirection()==Character.LEFT){
            charImgChange(DEMON_LEFT_IMG_ADDRESS);
        }else{
            charImgChange(DEMON_RIGHT_IMG_ADDRESS);
        }
    }


}
