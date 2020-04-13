#include <iostream>
#include <SFML/Graphics.hpp>

using namespace sf;

int ground = 360;
float offsetX = 0, offsetY = 0;
const int H = 12;
const int W = 40;


String TestMap [H] = {

"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
"B                                B     B",  
"B                                B     B",
"B                                B     B",
"B                                B     B",
"B         0000                BBBB     B",
"B                                B     B",
"BBB                              B     B",
"B              BB                BB    B",
"B              BB                      B",
"B    B         BB         BB           B",
"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB ",

};


class PLAYER {

public:

	float dx, dy;
	FloatRect rect;
	bool onGround;
	Sprite sprite;


	PLAYER(Texture& image)
	{
		sprite.setTexture(image);
		rect = FloatRect(6 * 64, 8 * 64, 40, 40);
		dx = dy = 0;
	};

	void update(float time) {


		rect.left += dx * time;
		Collision(0);
		if (!onGround) dy = dy + 0.0005 * time;
		rect.top += dy * time;
		onGround = false;
		Collision(1);


		sprite.setPosition(rect.left - offsetX, rect.top - offsetY);
		dx = 0;
	};

	void Collision(int dir) {
		for (int i = rect.top / 64; i < (rect.top + rect.height) / 64; i++)
			for (int j = rect.left / 64; j < (rect.left + rect.width) / 64; j++)
				if (TestMap[i][j] == 'B')
				{
					if ((dx > 0) && (dir == 0)) rect.left = j * 64 - rect.width;	
					if ((dx < 0) && (dir == 0)) rect.left = j * 64 + 64;
					if ((dy > 0) && (dir == 1)) { rect.top = i * 64 - rect.height;  dy = 0;   onGround = true; }
					if ((dy < 0) && (dir == 1)) { rect.top = i * 64 + 64;   dy = 0; }
				}



	};

};

int main()
{
	RenderWindow window(VideoMode(1920, 1080) , "Bounce_Ball"); //окно игры
	
	Texture t; //загрузка текстур
	Texture g;
	g.loadFromFile("images/ground_block.png");
	t.loadFromFile ("images/ball_small.png");

	PLAYER p (t);  // создание спрайтов
	Sprite Ground(g);
	
	Clock clock;


	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 500;  //Скорость игры

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}
		
		if (Keyboard::isKeyPressed(Keyboard::Left))
		{ 
			p.dx = -0.2; //скорость влево
		}
		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			p.dx = 0.2; //скорость вправо
		}
		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			if (p.onGround) { p.dy = -0.55; p.onGround = false;} //прыжок
		}

		p.update(time);

		if(p.rect.left>1920/2) offsetX = p.rect.left  - 1920 / 2; //камера по Х
		if(p.rect.top>1080) offsetY = p.rect.top - 1080 ; //камера по Y

		window.clear(Color::White);

		for (int i = 0; i < H; i++){  //Отрисовка текстур
			for (int j = 0; j < W; j++){
				if (TestMap[i][j] == 'B') Ground.setTexture(g);

				if (TestMap[i][j] == '0') Ground.setTexture(g);
					
				if (TestMap[i][j] == ' ') continue;

				Ground.setPosition(j * 64-offsetX, i * 64-offsetY);
				window.draw(Ground);
			}
		}
		window.draw(p.sprite);
		window.display();
	}

	return 0;
}