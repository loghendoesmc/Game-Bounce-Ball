#include <iostream>
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;
int ResW=600, ResH=400; /*разрешение экрана*/
float offsetX = 0, offsetY = 0;
const int H = 7;
const int W = 40;

//карта
String TestMap[H] = {
"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
"B    *   *                            B",
"B                           *      *  B",
"B    B    0000000000000000            B",
"B                                  E  B",
"B  B  *   *    B  *  B          *     B",
"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
};
//класс игрока и функции
class PLAYER {
public:

	float dx, dy;
	FloatRect rect;
	bool onGround;
	bool gameover; //конец игры
	Sprite sprite;
	int hp = 60; //здоровье

	PLAYER(Texture& image)
	{
		sprite.setTexture(image);
		gameover = false;
		rect = FloatRect(1 * 64, 1 * 64, 64, 62); //начальные координаты, где появляется шар, его хитбокс
		dx = dy = 0;
	};

	/*функция обновления*/
	void update(float time) {
		rect.left += dx * time;
		Collision(0);

		if (hp <= 0) {
		gameover = true;
		sprite.setColor(Color::Black);
		}

	

		if (!onGround) 
		dy = dy + 0.0005 * time;
		rect.top += dy * time;
		onGround = false;
		Collision(1);

		sprite.setTextureRect(IntRect(3 * 64, 0, 64, 64)); //текстурка шара
		sprite.setPosition(rect.left - offsetX, rect.top - offsetY);
		dx = 0;
	};

	/*функция коллизии с картой*/
	void Collision(int dir)
	{
		for (int i = rect.top / 64; i < (rect.top + rect.height) / 64; i++)
			for (int j = rect.left / 64; j < (rect.left + rect.width) / 64; j++)
			{
				if (TestMap[i][j] == 'B')//коллизия с красными блоками
				{
					if ((dx > 0) && (dir == 0)) rect.left = j * 64 - rect.width;
					if ((dx < 0) && (dir == 0)) rect.left = j * 64 + 64;
					if ((dy > 0) && (dir == 1)) { rect.top = i * 64 - rect.height;  dy = 0;   onGround = true; } 
					if ((dy < 0) && (dir == 1)) { rect.top = i * 64 + 64;   dy = 0; }
				}

				if (TestMap[i][j] == '0')//монеты
				{
					TestMap[i][j] = ' '; //исчезновение монет
				}
				if (TestMap[i][j] == '*')//взаимодействие с шипами
				{
					if ((dx > 0) && (dir == 0)) rect.left = j * 64 - rect.width;
					if ((dx < 0) && (dir == 0)) rect.left = j * 64 + 64;
					if ((dy > 0) && (dir == 1)) { rect.top = i * 64 - rect.height;  dy = 0;   onGround = true; }
					if ((dy < 0) && (dir == 1)) { rect.top = i * 64 + 62;   dy = 0; }
					hp = hp - 1; //при взаимодействии с шипами хп будет уменьшаться
				}
				if (TestMap[i][j] == 'E')//взаимодействие с концом уровня
				{
					TestMap[i][j] = 'W';
					gameover = true;
				}
			}
	}
};

int main() 
{
	RenderWindow window(VideoMode(ResW, ResH), "Bounce_Ball"); //окно игры

	Texture tileset; //загрузка текстур
	tileset.loadFromFile("tile.png");

	PLAYER p(tileset);  // создание спрайтов
	Sprite Ground(tileset); //основной спрай игры

	Clock clock; //часы

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds(); //берём время
		clock.restart();
		time = time / 600;  //Скорость игры
		//теперь игра зависит от времени а не от производительности процессора

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}
		if (!p.gameover) { //если gameover=true то мы не можем двигаться
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				p.dx = -0.23; //скорость влево
			}
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				p.dx = 0.23; //скорость вправо
			}
			if (Keyboard::isKeyPressed(Keyboard::Up))
			{
				if (p.onGround) { p.dy = -0.36; p.onGround = false; } //прыжок
			}
		}
		p.update(time);

		if (p.rect.left > ResW / 2) offsetX = p.rect.left - ResW / 2; //перемещение камеры по Х
		if (p.rect.top > ResH / 2) offsetY = p.rect.top - ResH / 2; //перемещение камеры по Y

		window.clear(Color::Cyan); //задний фон

		for (int i = 0; i < H; i++) {  //Отрисовка карты
			for (int j = 0; j < W; j++) {
				if (TestMap[i][j] == 'B') Ground.setTextureRect(IntRect(0, 0, 64, 64));

				if (TestMap[i][j] == '0') Ground.setTextureRect(IntRect(5 * 64, 0, 64, 64));

				if (TestMap[i][j] == '*') Ground.setTextureRect(IntRect(4 * 64, 0, 64, 64));

				if (TestMap[i][j] == 'E') Ground.setTextureRect(IntRect(6 * 64, 0, 64, 64));;

				if (TestMap[i][j] == 'W') Ground.setTextureRect(IntRect(0, 64, 512, 128));;

				if (TestMap[i][j] == ' ') continue;

				Ground.setPosition(j * 64 - offsetX, i * 64 - offsetY);
				window.draw(Ground);
			}
		}
		window.draw(p.sprite);
		window.display();
		if (p.gameover == true) {  //закрытие игры, если она закончена
			sleep(seconds(2));  //задержка перед закрытием
			return 0;
		}
	}
}