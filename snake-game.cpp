#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>

const int TILE_SIZE = 20; // Size of each grid cell
const int WIDTH = 25;     // Number of cells in width
const int HEIGHT = 20;    // Number of cells in height
const float SPEED = 0.1f; // Speed of the snake (seconds per frame)

struct SnakeSegment {
    int x, y;
};

enum Direction { Up, Down, Left, Right };

class SnakeGame {
public:
    SnakeGame()
        : window(sf::VideoMode(WIDTH * TILE_SIZE, HEIGHT * TILE_SIZE), "Snake Game"), direction(Right), gameOver(false), timer(0.f) {
        snake.push_back({WIDTH / 2, HEIGHT / 2}); // Initial position of the snake
        spawnFood();
    }

    void run() {
        sf::Clock clock;
        while (window.isOpen()) {
            float deltaTime = clock.restart().asSeconds();
            timer += deltaTime;

            handleEvents();
            if (timer > SPEED && !gameOver) {
                timer = 0.f;
                moveSnake();
            }

            render();
        }
    }

private:
    sf::RenderWindow window;
    std::vector<SnakeSegment> snake;
    Direction direction;
    bool gameOver;
    float timer;

    sf::Vector2i food;

    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Handle keyboard input for snake direction
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && direction != Down)
            direction = Up;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && direction != Up)
            direction = Down;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && direction != Right)
            direction = Left;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && direction != Left)
            direction = Right;
    }

    void moveSnake() {
        // Move the snake's body
        for (int i = snake.size() - 1; i > 0; --i) {
            snake[i] = snake[i - 1];
        }

        // Move the head
        if (direction == Up) snake[0].y -= 1;
        if (direction == Down) snake[0].y += 1;
        if (direction == Left) snake[0].x -= 1;
        if (direction == Right) snake[0].x += 1;

        // Check collisions
        if (snake[0].x < 0 || snake[0].y < 0 || snake[0].x >= WIDTH || snake[0].y >= HEIGHT) {
            gameOver = true; // Collided with wall
        }
        for (int i = 1; i < snake.size(); ++i) {
            if (snake[i].x == snake[0].x && snake[i].y == snake[0].y) {
                gameOver = true; // Collided with itself
            }
        }

        // Check if snake eats the food
        if (snake[0].x == food.x && snake[0].y == food.y) {
            snake.push_back(snake.back()); // Add a new segment
            spawnFood();
        }
    }

    void spawnFood() {
        food.x = std::rand() % WIDTH;
        food.y = std::rand() % HEIGHT;
    }

    void render() {
        window.clear();

        // Draw snake
        sf::RectangleShape rectangle(sf::Vector2f(TILE_SIZE, TILE_SIZE));
        rectangle.setFillColor(sf::Color::Green);
        for (auto &segment : snake) {
            rectangle.setPosition(segment.x * TILE_SIZE, segment.y * TILE_SIZE);
            window.draw(rectangle);
        }

        // Draw food
        rectangle.setFillColor(sf::Color::Red);
        rectangle.setPosition(food.x * TILE_SIZE, food.y * TILE_SIZE);
        window.draw(rectangle);

        // Display game over message
        if (gameOver) {
            sf::Font font;
            font.loadFromFile("arial.ttf"); // Make sure you have a font file in the directory
            sf::Text text("Game Over!", font, 40);
            text.setFillColor(sf::Color::White);
            text.setPosition(WIDTH * TILE_SIZE / 4, HEIGHT * TILE_SIZE / 3);
            window.draw(text);
        }

        window.display();
    }
};

int main() {
    std::srand(std::time(nullptr));
    SnakeGame game;
    game.run();
    return 0;
}
