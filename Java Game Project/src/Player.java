import bagel.util.Point;

/**
 * Player Character for the game
 */
public class Player extends Character{
    private final static int PLAYER_DMG=20;
    private final static String FAE_LEFT_IMG_ADDRESS = "res/fae/faeLeft.png";
    private final static String FAE_RIGHT_IMG_ADDRESS = "res/fae/faeRight.png";
    private final static String ATTACK_LEFT_IMG_ADDRESS = "res/fae/faeAttackLeft.png";
    private final static String ATTACK_RIGHT_IMG_ADDRESS = "res/fae/faeAttackRight.png";
    private final static int MAX_HP = 100;
    private final static int MAX_ATTACK_COUNTER = 60;
    private final static int MAX_COOLDOWN_COUNTER = 120;
    private final static double STEP_SIZE = 2;
    private boolean facingRight;
    private int attackStateCounter;
    private int coolDownCounter;
    private boolean attackState;
    private boolean coolDownState;

    /**
     * Get the max hp of player character
     * @return The MAX_HP of the player
     */
    public static int getMaxHp(){
        return MAX_HP;
    }

    /**
     * Constructor for the Player Class
     * @param topLeftCorner is the topLeftCorner the player entity is to be located
     */
    public Player(Point topLeftCorner) {
        super(FAE_RIGHT_IMG_ADDRESS, topLeftCorner, MAX_HP);
        facingRight = true;
        coolDownState = false;
        attackState = false;
        coolDownCounter = 0;
        attackStateCounter = 0;
    }

    /**
     * Move the player, and not change image if requested.
     * @param direction This is which way the player is moving in
     * @param sameImg This represents if image are to be kept same regardless direction
     */
    public void movePlayer(int direction, boolean sameImg) {
        super.move(direction, STEP_SIZE);

        if(sameImg){
            return;
        }

        // Change image as required
        if (direction == Character.LEFT){
            if(facingRight){
                playerImgChange(true);
            }
        }else if (direction == Character.RIGHT){
            if (!facingRight){
                playerImgChange(true);
            }
        }

    }

    /**
     * Handles potential collision of player and Enemy
     * @param enemy is enemy entity player may be potentially colliding with
     */
    public void playerEnemyCollision(Enemy enemy){
        if (attackState && this.isColliding(enemy)){
            enemy.takeDamage(PLAYER_DMG);
        }
    }


    @Override
    public double hpPercent() {
        return 100*((double)super.getHealth())/MAX_HP;
    }

    /**
     * Manage the states of Player character.
     * This includes the additional state of attack and cooldown for player
     */
    public void stateManage(){
        super.stateManage();
        if (coolDownState){
            coolDownCounter +=1;
            if (coolDownCounter >= MAX_COOLDOWN_COUNTER){
                coolDownState = false;
                coolDownCounter = 0;
            }
        }

        if (attackState){
            attackStateCounter += 1;
            if (attackStateCounter >= MAX_ATTACK_COUNTER){
                coolDownState = true;
                attackState = false;
                attackStateCounter = 0;
                playerImgChange(false);
            }
        }
    }

    /**
     * Change the player Image and the associated collision box to the current player state.
     * If faceDirectionChange is requested, also switch the way player is facing
     * @param faceDirectionChange This asks for if the way player is facing is to be switched
     */

    public void playerImgChange(boolean faceDirectionChange){
        if (faceDirectionChange){
            facingRight = !facingRight;
        }

        if (facingRight){
            if (attackState){
                this.charImgChange(ATTACK_RIGHT_IMG_ADDRESS);
            }else{
                this.charImgChange(FAE_RIGHT_IMG_ADDRESS);
            }
        }else{
            if (attackState){
                this.charImgChange(ATTACK_LEFT_IMG_ADDRESS);
            }else{
                this.charImgChange(FAE_LEFT_IMG_ADDRESS);
            }
        }


    }

    /**
     * Deal with Player's attack
     */
    public void attack(){
        if (!attackState && !coolDownState){
            attackState = true;
            playerImgChange(false);
        }
    }
}
