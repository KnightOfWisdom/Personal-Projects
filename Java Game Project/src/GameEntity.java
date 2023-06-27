import bagel.Image;
import bagel.util.Point;
import bagel.util.Rectangle;

/**
 * Game Entity are all the possible entities that
 * can be interacted with within the game
 */
public abstract class GameEntity {
    private Image entityImage;
    private Rectangle collisionBox;


    /**
     * The constructor for the gameEntity
     * @param imageLoc is the address of the image of the entity
     * @param topLeftCorner is the position of top left corner of the entity
     */
    public GameEntity(String imageLoc, Point topLeftCorner){
        this.entityImage = new Image(imageLoc);
        double width = this.entityImage.getWidth();
        double height = this.entityImage.getHeight();
        this.collisionBox = new Rectangle(topLeftCorner, width, height);
    }

    /**
     * Render the game entity in the game
     */
    public void drawEntity(){
        double entityX = this.getTopLeftPoint().x;
        double entityY = this.getTopLeftPoint().y;
        this.entityImage.drawFromTopLeft(entityX,entityY);
    }

    /**
     * Get the image of the entity
     * @return Image This is the image of entity
     */
    public Image getEntityImage() {
        return entityImage;
    }

    /**
     * Set the image of the entity as the new image
     * @param entityImage This is the new image of the entity
     */
    public void setEntityImage(Image entityImage) {
        this.entityImage = entityImage;
    }

    /**
     * Set the collision box of the entity to the new collision box
     * @param collisionBox This is the new collision box
     */
    public void setCollisionBox(Rectangle collisionBox) {
        this.collisionBox = collisionBox;
    }

    /**
     * Get the top left of the collision box for the entity
     * @return Point This is the top left point of collision box
     */
    public Point getTopLeftPoint(){
        return this.collisionBox.topLeft();
    }

    /**
     * Get the Centre of the collision box for the entity
     */
    public Point getCentre(){
        return this.collisionBox.centre();
    }

    /**
     * Get the top right of the collision box for the entity
     * @return Point the centre of the image
     */
    public Point getTopRightPoint(){
        return this.collisionBox.topRight();
    }

    /**
     * Get the bottom left of the collision box for the entity
     */
    public Point getBotLeftPoint(){
        return this.collisionBox.bottomLeft();
    }

    /**
     * Get the bot right of the collision box for the entity
     */
    public Point getBotRightPoint(){
        return this.collisionBox.bottomRight();
    }

    /**
     * Change the location of the entity to the new point
     * @param newPoint This is the new top left position of the entity
     */
    public void changeLocation(Point newPoint){
        this.collisionBox.moveTo(newPoint);
    }

    /**
     * Check if the entity is colliding with the inputted entity
     * @param entity This is the other game entity we are checking against
     * @return Whether the two entities are colliding or not
     */
    public boolean isColliding(GameEntity entity){
        return this.collisionBox.intersects(entity.collisionBox);
    }
}
