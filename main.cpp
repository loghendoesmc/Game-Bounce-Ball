#include <iostream>
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;
int ResW = 600, ResH = 400; /*���������� ������*/
float offsetX = 0, offsetY = 0;
const int H = 7;
const int W = 40;

//�����
String TestMap[H] = {
"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
"B    *   *                            B",
"B                           *      *  B",
"B    B    0000000000000000            B",
"B                                  E  B",
"B  B  *   *    B  *  B          *     B",
"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
};
//����� ������ � �������
class PLAYER {
public:

	float dx, dy;
	FloatRect rect;
	bool onGround;
	bool gameover; //����� ����
	Sprite sprite;
	int hp = 60; //��������

	PLAYER(Texture& image)
	{
		sprite.setTexture(image);
		gameover = false;
		rect = FloatRect(1 * 64, 1 * 64, 64, 62); //��������� ����������, ��� ���������� ���, ��� �������
		dx = dy = 0;
	};

	/*������� ����������*/
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

		sprite.setTextureRect(IntRect(3 * 64, 0, 64, 64)); //��������� ����
		sprite.setPosition(rect.left - offsetX, rect.top - offsetY);
		dx = 0;
	};

	/*������� �������� � ������*/
	void Collision(int dir)
	{
		for (int i = rect.top / 64; i < (rect.top + rect.height) / 64; i++)
			for (int j = rect.left / 64; j < (rect.left + rect.width) / 64; j++)
			{
				if (TestMap[i][j] == 'B')//�������� � �������� �������
				{
					if ((dx > 0) && (dir == 0)) rect.left = j * 64 - rect.width;
					if ((dx < 0) && (dir == 0)) rect.left = j * 64 + 64;
					if ((dy > 0) && (dir == 1)) { rect.top = i * 64 - rect.height;  dy = 0;   onGround = true; }
					if ((dy < 0) && (dir == 1)) { rect.top = i * 64 + 64;   dy = 0; }
				}

				if (TestMap[i][j] == '0')//������
				{
					TestMap[i][j] = ' '; //������������ �����
				}
				if (TestMap[i][j] == '*')//�������������� � ������
				{
					hp = hp - 1; //��� �������������� � ������ �� ����� �����������
				}
				if (TestMap[i][j] == 'E')//�������������� � ������ ������
				{
					TestMap[i][j] = 'W';
					gameover = true;
				}
			}
	}
};

int main()
{
	RenderWindow window(VideoMode(ResW, ResH), "Bounce_Ball"); //���� ����

	Texture tileset; //�������� �������
	tileset.loadFromFile("tile.png");

	PLAYER p(tileset);  // �������� ��������
	Sprite Ground(tileset); //�������� ����� ����

	Clock clock; //����

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds(); //���� �����
		clock.restart();
		time = time / 600;  //�������� ����
		//������ ���� ������� �� ������� � �� �� ������������������ ����������

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}
		if (!p.gameover) { //���� gameover=true �� �� �� ����� ���������
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				p.dx = -0.23; //�������� �����
			}
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				p.dx = 0.23; //�������� ������
			}
			if (Keyboard::isKeyPressed(Keyboard::Up))
			{
				if (p.onGround) { p.dy = -0.36; p.onGround = false; } //������
			}
		}
		p.update(time);

		if (p.rect.left > ResW / 2) offsetX = p.rect.left - ResW / 2; //����������� ������ �� �
		if (p.rect.top > ResH / 2) offsetY = p.rect.top - ResH / 2; //����������� ������ �� Y

		window.clear(Color::Cyan); //������ ���

		for (int i = 0; i < H; i++) {  //��������� �����
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
		if (p.gameover == true) {  //�������� ����, ���� ��� ���������
			sleep(seconds(2));  //�������� ����� ��������� 
			return 0;
		}
	}
}