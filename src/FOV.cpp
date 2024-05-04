#include <SFML\Graphics.hpp>
constexpr auto PI = 22 / 7.f;

void CreateArrow(sf::ConvexShape&);
float getMagnitude(sf::Vector2f);
void Normalize(sf::Vector2f&);
void getCircle(sf::ConvexShape&, float, sf::Vector2f, float);

int main()
{
	sf::RenderWindow window(sf::VideoMode(600, 400), "FOV !!");

	sf::CircleShape circle(25);
	circle.setFillColor(sf::Color::Blue);
	circle.setOrigin(circle.getRadius(), circle.getRadius());
	circle.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);

	sf::CircleShape circleEnemy(10);
	circleEnemy.setFillColor(sf::Color::Magenta);
	circleEnemy.setOrigin(circleEnemy.getRadius(), circleEnemy.getRadius());
	circleEnemy.setPosition(window.getSize().x / 3.f, window.getSize().y / 3.f);

	sf::CircleShape circleTest(5);
	circleTest.setFillColor(sf::Color::Cyan);
	circleTest.setOrigin(circleTest.getRadius(), circleTest.getRadius());

	sf::CircleShape circleTest2(5);
	circleTest2.setFillColor(sf::Color::Blue);
	circleTest2.setOrigin(circleTest2.getRadius(), circleTest2.getRadius());

	sf::ConvexShape cone(31);
	cone.setFillColor(sf::Color::Black);
	float radius = 200;
	float distanceBetween2Points = 0.0;
	float angleRange = PI / 3;
	getCircle(cone, radius, circle.getPosition(), angleRange);

	double horizontal = 0.0;
	double vertical = 0.0;
	float speed = 100;
	float angle = 0.0;
	float lookAtAngle = 0.0;
	float getPositionX = 0.0;
	float getPositionY = 0.0;
	float dotProductP0 = 0.0;
	float compareAngleP0 = 0.0;
	float dotProductP1 = 0.0;
	float compareAngleP1 = 0.0;

	sf::Vector2f direction;
	sf::Vector2f position = circle.getPosition();
	sf::Vector2f lookAtDirection;
	sf::Vector2f lookAtEnemyCircle;
	sf::Vector2f mousePosition;
	sf::Vector2i pixelPos;
	sf::Vector2f worldPos;
	sf::Vector2f P0;
	sf::Vector2f P1;

	sf::Clock clock;
	float time = 0.0;
	float currentTime = 0.0;
	float deltaTime = 0.0;

	while (window.isOpen())
	{
		currentTime = clock.getElapsedTime().asSeconds();
		deltaTime = currentTime - time;
		time = currentTime;

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			if (horizontal > -1)
				horizontal -= 10 * deltaTime;
			//horizontal = -1;
		}

		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			if (horizontal < 1)
				horizontal += 10 * deltaTime;
			//horizontal = 1;
		}

		else
			horizontal = 0;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			if (vertical > -1)
				vertical -= 10 * deltaTime;
			//vertical = -1;
		}

		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			if (vertical < 1)
				vertical += 10 * deltaTime;
			//vertical = 1;
		}

		else
			vertical = 0;

		if (horizontal != 0 || vertical != 0)
		{
			direction.x = horizontal;
			direction.y = vertical;

			Normalize(direction);

			angle = atan2(direction.y, direction.x);

			position += deltaTime * speed * direction;
		}

		pixelPos = sf::Mouse::getPosition(window);
		worldPos = window.mapPixelToCoords(pixelPos);

		circle.setPosition(position);
		sf::Vector2f coneCenter(circle.getPosition());

		lookAtDirection = worldPos - coneCenter; // L vector
		lookAtEnemyCircle = circleEnemy.getPosition() - circle.getPosition(); // P vector

		sf::Vector2f perpendicularPLeft(-lookAtEnemyCircle.y, -lookAtEnemyCircle.x);
		Normalize(perpendicularPLeft);

		sf::Vector2f perpendicularPRight(lookAtEnemyCircle.y, lookAtEnemyCircle.x);
		Normalize(perpendicularPRight);

		P0 = (circleEnemy.getPosition() + perpendicularPLeft * circleEnemy.getRadius());
		P1 = (circleEnemy.getPosition() + perpendicularPRight * circleEnemy.getRadius());

		circleTest.setPosition(P0);
		circleTest2.setPosition(P1);

		sf::Vector2f lookAtEnemyCircleP0 = P0 - coneCenter;
		sf::Vector2f lookAtEnemyCircleP1 = P1 - coneCenter;

		dotProductP0 = (lookAtDirection.x * lookAtEnemyCircleP0.x) + (lookAtDirection.y * lookAtEnemyCircleP0.y); // Dot Product
		dotProductP1 = (lookAtDirection.x * lookAtEnemyCircleP1.x) + (lookAtDirection.y * lookAtEnemyCircleP1.y); // Dot Product

		compareAngleP0 = dotProductP0 / (getMagnitude(lookAtDirection) * getMagnitude(lookAtEnemyCircleP0));
		compareAngleP1 = dotProductP1 / (getMagnitude(lookAtDirection) * getMagnitude(lookAtEnemyCircleP1));

		getPositionX = cone.getPosition().x - circleEnemy.getPosition().x;
		getPositionY = cone.getPosition().y - circleEnemy.getPosition().y;

		distanceBetween2Points = sqrt((getPositionX) * (getPositionX)+(getPositionY) * (getPositionY));

		if (distanceBetween2Points <= radius + circleEnemy.getRadius() &&
			(acos(compareAngleP0) <= (angleRange / 2) || acos(compareAngleP1) <= (angleRange / 2)))

			cone.setFillColor(sf::Color::Red);

		else
			cone.setFillColor(sf::Color::Black);

		lookAtAngle = atan2(lookAtDirection.y, lookAtDirection.x);

		window.clear(sf::Color::White);
		cone.setPosition(position);
		cone.setRotation(lookAtAngle * 180 / PI);
		window.draw(cone);
		window.draw(circleEnemy);
		window.draw(circle);
		window.draw(circleTest);
		window.draw(circleTest2);
		window.display();
	}

	return 0;
}

float getMagnitude(sf::Vector2f vector)
{
	return sqrt(vector.x * vector.x + vector.y * vector.y);
}

void Normalize(sf::Vector2f& vector)
{
	float mag = getMagnitude(vector);

	if (mag == 0)
		return;

	vector = (1 / mag) * vector;
}

void getCircle(sf::ConvexShape& circle, float radius, sf::Vector2f center, float angleRange)
{
	//angle = 0 -> 360
	int pointsCount = circle.getPointCount();
	float deltaTheta = angleRange / (pointsCount - 1);
	float theta = -angleRange / 2;
	sf::Vector2f point;

	for (int i = 0; i < pointsCount - 1; i++)
	{
		point.x = center.x + radius * cos(theta);
		point.y = center.y + radius * sin(theta);

		circle.setPoint(i, point);
		theta += deltaTheta;
	}

	circle.setPoint(pointsCount - 1, center);
	circle.setOrigin(center);
	circle.setPosition(center);
}