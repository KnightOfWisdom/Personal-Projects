import bagel.*;
import bagel.util.Colour;
import bagel.util.Point;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;

/**
 * Level 1 of the game
 */
public class Level1 extends Level{
    private final Font ENEMY_HP_FONT;
    private static final String LEVEL_FILE = "res/level1.csv";
    private static final String LEVEL_BACKGROUND = "res/background1.png";
    private static final String ObstacleImg = "res/tree.png";
    private final Image BACKGROUND;
    private DrawOptions enemyHpColor = new DrawOptions();
    private Player player;
    private ArrayList<Obstacle> gameObstacles = new ArrayList<Obstacle>();
    private ArrayList<Demon> demonList = new ArrayList<Demon>();
    private Navec navec;
    private final static int ENEMY_HP_Y_OFFSET = -6;
    private final static Colour GREEN = new Colour(0, 0.8, 0.2);
    private final static Colour ORANGE = new Colour(0.9, 0.6, 0);
    private final static Colour RED = new Colour(1, 0, 0);
    private int timeScale;


    /**
     * Constructor for level 1 of the game
     */
    public Level1(){
        super();
        this.ENEMY_HP_FONT = new Font(Level.getHpFontAddress(), 30);
        this.loadLevel(LEVEL_FILE);
        this.BACKGROUND = new Image(LEVEL_BACKGROUND);
        this.timeScale = 0;
    }

    @Override
    public void loadLevel(String fileLoc) {
        String data;
        try (BufferedReader levelDataFile = new BufferedReader(new FileReader(fileLoc))){
            while ((data = levelDataFile.readLine()) != null){

                // Extract info from the line read
                String[] entityData = data.split(",");
                double xCoordinate = Double.parseDouble(entityData[1]);
                double yCoordinate = Double.parseDouble(entityData[2]);
                Point topLeftCoordinate = new Point(xCoordinate, yCoordinate);

                // Assign info to appropriate game entity
                if (entityData[0].equals("Fae")){
                    this.player = new Player(topLeftCoordinate);
                }else if (entityData[0].equals("Tree")){
                    Obstacle newWall = new Obstacle(ObstacleImg, topLeftCoordinate);
                    this.gameObstacles.add(newWall);
                } else if (entityData[0].equals("Sinkhole")) {
                    Obstacle newSinkHole = new SinkHole(topLeftCoordinate);
                    this.gameObstacles.add(newSinkHole);
                }else if (entityData[0].equals("Demon")){
                    Demon demon = new Demon(topLeftCoordinate);
                    this.demonList.add(demon);
                }else if (entityData[0].equals("Navec")){
                    this.navec = new Navec(topLeftCoordinate);
                } else if (entityData[0].equals("TopLeft")){
                    this.setMinXBound(xCoordinate);
                    this.setMinYBound(yCoordinate);
                }else if (entityData[0].equals("BottomRight")){
                    this.setMaxXBound(xCoordinate);
                    this.setMaxYBound(yCoordinate);
                }
            }
        }catch (IOException e){
            e.printStackTrace();
        }
    }

    @Override
    public void drawHp() {
        super.drawPlayerHp(player);
        for (Demon demon:demonList){
            drawEnemyHp(demon);
        }
        drawEnemyHp(navec);
    }

    /**
     * Render the hp for the enemy at the correct location
     * @param enemy The enemy those hp is to be rendered
     */
    private void drawEnemyHp(Enemy enemy){
        double currPercent = enemy.hpPercent();
        if (currPercent <= RED_HP_BOUNDARY){
            enemyHpColor.setBlendColour(RED);
        }else if (currPercent<= ORANGE_HP_BOUNDARY){
            enemyHpColor.setBlendColour(ORANGE);
        }else{
            enemyHpColor.setBlendColour(GREEN);
        }
        Point currPoint = enemy.getTopLeftPoint();
        ENEMY_HP_FONT.drawString(Math.round(currPercent) + "%", currPoint.x,
                currPoint.y+ENEMY_HP_Y_OFFSET, enemyHpColor);
    }

    @Override
    public boolean hasWon() {
        if (navec.isDead()){
            return true;
        }
        return false;
    }

    /**
     * Handle the collision between the enemy entity and the Boundary
     * @param enemy The enemy in question
     */
    public void enemyBoundaryCollisionHandler(Enemy enemy){
        Point enemyLoc = enemy.getTopLeftPoint();
        double minXBound = getMinXBound(), minYBound = getMinYBound();
        double maxXBound = getMaxXBound(), maxYBound = getMaxYBound();
        if ((enemyLoc.x < minXBound) || (enemyLoc.x > maxXBound)){
            enemy.swapDirection();
        } else if ((enemyLoc.y<minYBound) || (enemyLoc.y>maxYBound)){
            enemy.swapDirection();
        }
    }

    /**
     * Handle collision of the specified Enemy to boundary and object
     * @param enemy The enemy in question
     */
    public void enemyObstacleCollisionHandler(Enemy enemy){
        for (Obstacle obstacle:gameObstacles){
            obstacle.enemyCollisionHandler(enemy);
        }
    }
    @Override
    public boolean hasLost() {
        if (player.isDead()){
            return true;
        }
        return false;
    }

    @Override
    public void update(Input input) {
        player.stateManage();

        // Deal with timescale control
        if (input.wasPressed(Keys.L)){
            if (this.timeScale < 3){
                this.timeScale += 1;
                System.out.println("Sped up, Speed:  "+timeScale);
            }

        }
        if (input.wasPressed(Keys.K)){
            if(this.timeScale>-3){
                this.timeScale -= 1;
                System.out.println("Slowed down, Speed:  "+timeScale);
            }
        }

        if (input.isDown(Keys.A)){
            player.attack();
        }

        // Move the characters and handle collisions
        this.playerControl(player, input, gameObstacles);

        // Perform state update for the demons in the level
        for (Demon demon:demonList){
            demon.stateManage();
            demon.moveEnemy(timeScale);
            player.playerEnemyCollision(demon);
            this.enemyBoundaryCollisionHandler(demon);
            this.enemyObstacleCollisionHandler(demon);
            demon.playerDetect(player);
        }

        // State update for Navec
        navec.stateManage();
        navec.moveEnemy(timeScale);
        player.playerEnemyCollision(navec);
        this.enemyBoundaryCollisionHandler(navec);
        this.enemyObstacleCollisionHandler(navec);
        navec.playerDetect(player);

        // Draw the Characters and their hp
        player.drawEntity();
        navec.drawEntity();
        for (int i=0; i<demonList.size(); i++){
            Demon demon = demonList.get(i);
            if (demon.isDead()) {
                demonList.remove(demon);
                i--;
            }else{
                demon.drawEntity();
            }

        }
        this.drawHp();

        // Draw the obstacles within the level and remove inactive ones
        for (int i=0; i<gameObstacles.size(); i++){
            Obstacle obstacle = gameObstacles.get(i);
            if (obstacle.isActive()) {
                obstacle.drawEntity();
            }else{
                gameObstacles.remove(obstacle);
                i--;
            }

        }
    }

}
