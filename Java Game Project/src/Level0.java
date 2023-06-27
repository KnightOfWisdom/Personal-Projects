import bagel.Image;
import bagel.Input;
import bagel.Keys;
import bagel.util.Point;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;


/**
 * The level0 of the game
 */
public class Level0 extends Level{

    private static final String LEVEL_FILE = "res/level0.csv";
    private static final String ObstacleImg = "res/wall.png";
    private static final String LEVEL_BACKGROUND = "res/background0.png";
    private final static double WIN_MIN_X_COORDINATE = 950;
    private final static double WIN_MIN_Y_COORDINATE = 670;
    private Player player;
    private ArrayList<Obstacle> gameObstacles = new ArrayList<Obstacle>();
    private final Image BACKGROUND;


    /**
     * Constructor for level 0
     */
    public Level0(){
        super();
        loadLevel(LEVEL_FILE);
        BACKGROUND = new Image(LEVEL_BACKGROUND);
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
                }else if (entityData[0].equals("Wall")){
                    Obstacle newWall = new Obstacle(ObstacleImg, topLeftCoordinate);
                    this.gameObstacles.add(newWall);
                } else if (entityData[0].equals("Sinkhole")) {
                    Obstacle newSinkHole = new SinkHole(topLeftCoordinate);
                    this.gameObstacles.add(newSinkHole);
                }else if (entityData[0].equals("TopLeft")){
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
    }

    @Override
    public boolean hasWon() {
        double playerX = player.getTopLeftPoint().x;
        double playerY = player.getTopLeftPoint().y;
        if (playerX >= WIN_MIN_X_COORDINATE && playerY >= WIN_MIN_Y_COORDINATE){
            return true;
        }
        return false;
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

        if (input.isDown(Keys.A)){
            player.attack();
        }

        this.playerControl(player, input, gameObstacles);
        player.drawEntity();
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
