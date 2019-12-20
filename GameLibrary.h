#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <fstream>
#include <strstream>

//1		2
//
//
//4		3

#define newSeed() srand(int(time(NULL) ) )

namespace Math {
	static float dist(sf::Vector2f a, sf::Vector2f b) {
		sf::Vector2f c = a - b;
		return sqrt((c.x * c.x) + (c.y * c.y));
	}

	static sf::Vector2f normDist(sf::Vector2f a, sf::Vector2f b) {
		sf::Vector2f c = a - b;
		return c / dist(a, b);
	}

	static float getAngle(sf::Vector2f start, sf::Vector2f nxt) {
		sf::Vector2f c = start - nxt;

		float ab = c.y;
		float ac = sqrt((c.x * c.x) + (c.y * c.y));
		float angle = float(asin(ab / ac) * (180 / 3.14159f)) - 90.f;

		if (c.x < 0) {
			angle = angle * -1.f;
		}
		return angle;
	}

	static float positiveAngle(float angle) {
		if (angle < 0) {
			angle = angle + 360.f;
		}
		return angle;
	}

	static sf::Vector2f diff(sf::Vector2f a, sf::Vector2f b) {
		return a - b;
	}

	static sf::Vector2f v2fAbs(sf::Vector2f a) {
		a.x = abs(a.x);
		a.y = abs(a.y);

		return a;
	}

	static int random(int a, int b) {
		return (rand() % (a - b) + a);
	}

	static sf::Vector2f randomVec(int xmin, int xmax, int ymin, int ymax) {
		return sf::Vector2f(float(random(xmin, xmax)), float(random(ymin, ymax)));
	}

	static sf::Vector2f v2fZero() {
		return sf::Vector2f(0.f, 0.f);
	}

	static sf::Vector2f avgV2f(sf::Vector2f first, sf::Vector2f second) {
		return sf::Vector2f((first.x + second.x) / 2.f, (first.y + second.y) / 2.f);
	}
	
	static sf::Color avgColor(sf::Color a, sf::Color b) {
		return sf::Color( (a.r + b.r)/2, (a.g + b.g) / 2, (a.b + b.b) / 2);
	}
	
	static sf::Color hexToRGB(std::string hex) {
		sf::Color ret = sf::Color::White;
		if (!(hex.length() < 6)) {
			ret.r = std::stoi(hex.substr(0, 2), 0, 16);
			ret.g = std::stoi(hex.substr(2, 4), 0, 16);
			ret.b = std::stoi(hex.substr(4, 6), 0, 16);
		}
		return ret;
	}

	class Clock {
	public:
		Clock() {
			this->clock = new sf::Clock();
			this->deltaTime = 0.f;
		}

		~Clock() {
			delete this->clock;
		}

		void Update() {
			this->deltaTime = clock->restart().asSeconds();
		}
		
		float deltaTime;
	private:
		sf::Clock* clock;
	};
}

namespace DrawFunctions {
	static void DrawText(sf::Vector2f pos, std::string napis,
					sf::Font* font, unsigned int fontSize, sf::RenderWindow* window) {
		sf::Text text(napis, *font, fontSize);
		text.setPosition(pos);
		text.setFillColor(sf::Color::White);
		text.setOutlineColor(sf::Color::Black);
		text.setOutlineThickness(1.f);
		window->draw(text);
	}

	static sf::Text getText(std::string napis, sf::Font* font,
		unsigned int fontSize, sf::Vector2f pos = sf::Vector2f(0.f,0.f)) {
		sf::Text text(napis, *font, fontSize);
		text.setPosition(pos);
		text.setFillColor(sf::Color::White);
		text.setOutlineColor(sf::Color::Black);
		text.setOutlineThickness(1.f);

		return text;
	}
}

namespace Draw {
	class SpriteAnimator {
	public:
		SpriteAnimator(sf::Sprite* sprite, float AnimInterval, sf::Vector2i frameSize, int anim) {
			this->sprite = sprite;
			this->frameSize = frameSize;

			this->AnimInterval = AnimInterval;
			
			this->animTime = 0.f;
			this->xanim = 0;
			this->anim = anim;
			this->maxAnim = int(floor(this->sprite->getTexture()->getSize().x / frameSize.x));
		}

		void changeAnim(int anim) {
			this->xanim = 0;
			this->anim = anim;
		}

		void setMaxAnim(int maxAnim) {
			this->maxAnim = maxAnim;
		}

		void Update(float deltaTime) {
			this->animTime += deltaTime;

			if (this->animTime >= this->AnimInterval) {
				xanim++;

				if (xanim > maxAnim) {
					xanim = 0;
				}

				this->sprite->setTextureRect(sf::IntRect(xanim*frameSize.x, anim*frameSize.y, 
														 frameSize.x, frameSize.y));

				this->animTime = 0.f;
			}
		}

	private:
		sf::Sprite* sprite;
		sf::Vector2i frameSize;

		float AnimInterval;
		float animTime;
		int anim;
		int xanim;
		int maxAnim;
	};

	enum class GradientType {
		LeftToRight = 0,
		TopToBottom = 1,
		TopLeftToBottomRight = 2,
		TopRightToBottomLeft = 3,
		TopLeft = 4,
		TopRight = 5,
		BottomLeft = 6,
		BottomRight = 7
	};

	class GradientRect {
	public:
		GradientRect(sf::FloatRect pas, GradientType type, sf::Color colorA, sf::Color colorB){
			std::vector<sf::Color> colors;

			rect.setPrimitiveType(sf::Quads);

			if (type == GradientType::LeftToRight) {
				for (int i = 0; i < 2; i++) {
					colors.push_back(colorA);
					colors.push_back(colorB);
				}
			}
			else if (type == GradientType::TopToBottom) {
				colors.push_back(colorA);
				colors.push_back(colorA);

				colors.push_back(colorB);
				colors.push_back(colorB);
			}
			else if (type == GradientType::TopLeftToBottomRight) {
				colors.push_back(colorA);
				colors.push_back(Math::avgColor(colorA, colorB));
				colors.push_back(Math::avgColor(colorA, colorB));
				colors.push_back(colorB);
			}
			else if (type == GradientType::TopRightToBottomLeft) {
				colors.push_back(Math::avgColor(colorA, colorB));
				colors.push_back(colorA);
				colors.push_back(colorB);
				colors.push_back(Math::avgColor(colorA, colorB));
			}
			else if (type == GradientType::TopLeft) {
				colors.push_back(colorB);
				colors.push_back(colorA);
				colors.push_back(colorA);
				colors.push_back(colorA);
			}
			else if (type == GradientType::TopRight) {
				colors.push_back(colorA);
				colors.push_back(colorB);
				colors.push_back(colorA);
				colors.push_back(colorA);
			}
			else if (type == GradientType::BottomLeft) {
				colors.push_back(colorA);
				colors.push_back(colorA);
				colors.push_back(colorB);
				colors.push_back(colorA);
			}
			else if (type == GradientType::BottomRight) {
				colors.push_back(colorA);
				colors.push_back(colorA);
				colors.push_back(colorA);
				colors.push_back(colorB);
			}

			this->rect.append(sf::Vertex(sf::Vector2f(pas.left,             pas.top), colors[0]));
			this->rect.append(sf::Vertex(sf::Vector2f(pas.left + pas.width, pas.top), colors[1]));
			this->rect.append(sf::Vertex(sf::Vector2f(pas.left + pas.width, pas.top + pas.height), colors[3]));
			this->rect.append(sf::Vertex(sf::Vector2f(pas.left			  , pas.top + pas.height), colors[2]));
		}

		void Draw(sf::RenderWindow* window) {
			window->draw(this->rect);
		}
		sf::VertexArray* getArray() {
			return &this->rect;
		}
	private:
		sf::VertexArray rect;
	};

	class Line {
	public:
		Line(sf::Vector2f a, sf::Vector2f b, sf::Color color) {
			ar = sf::VertexArray(sf::Lines);

			ar.append(sf::Vertex(a, color));
			ar.append(sf::Vertex(b, color));
		}

		void Draw(sf::RenderWindow* window) {
			window->draw(this->ar);
		}

		sf::VertexArray* getArray() {
			return &this->ar;
		}
	private:
		sf::VertexArray ar;
	};

	class Point {
	public:
		Point(sf::Vector2f position, float size, sf::Color color) {
			this->shape = new sf::CircleShape(size);
			this->shape->setPosition(position);
			this->shape->setFillColor(color);
			this->shape->setOrigin(sf::Vector2f(size / 2.f, size / 2.f));
		}
		~Point() {
			delete this->shape;
			this->shape = NULL;
		}

		void Draw(sf::RenderWindow* window) {
			window->draw(*this->shape);
		}

		sf::CircleShape* getShape() { return this->shape;  }

	private:
		sf::CircleShape* shape;
	};
}

namespace Clases {
	class File {
	public:
		File(std::string filename) {
			std::fstream stream;
			stream.open(filename, std::ios::in);

			if (stream.good()) {
				std::string linia;
				while (std::getline(stream, linia)) {
					this->file.push_back(linia);
				}
			}

			stream.close();
		}

		std::vector<std::string> file;
	};
}

namespace Audio {
	class Sound {
	public:
		Sound(std::string filename, float volume, bool loop) {
			this->buffer = new sf::SoundBuffer();
			this->buffer->loadFromFile(filename);
			this->sound = new sf::Sound(*this->buffer);
			this->sound->setVolume(volume);
			this->sound->setLoop(loop);
		}
		~Sound() {
			delete this->sound;
			delete this->buffer;
			this->buffer = NULL;
			this->sound = NULL;
		}

		void setVolume(float volume) {	this->sound->setVolume(volume);	}

		void play() { this->sound->play(); }
		void stop() { this->sound->stop(); }

		sf::Sound* getSound() { return this->sound; }
		sf::SoundBuffer* getBuffer() { return this->buffer; }
	private:
		sf::Sound* sound;
		sf::SoundBuffer* buffer;
	};
}

namespace UI {
	static char keyToChar(sf::Keyboard::Key key) {
		// letters return lower
		if (int(key) < 26) {
			return key + 0x61;
		}
		// numers
		else if (int(key) < 36) {
			return key + 0x16;
		}
		// numpad
		else if (int(key) > 74 && int(key) < 85) {
			return key - 0x1B;
		}
		// specific keys , - \ ' / .
		else if (key == sf::Keyboard::Key::Comma) {
			return ',';
		}
		else if (key == sf::Keyboard::Key::Hyphen) {
			return '-';
		}
		else if (key == sf::Keyboard::Key::Backslash) {
			return 92;
		}
		else if (key == sf::Keyboard::Key::Quote) {
			return 39;
		}
		else if (key == sf::Keyboard::Key::Divide) {
			return '/';
		}
		else if (key == sf::Keyboard::Key::Period) {
			return '.';
		}
		else if (key == sf::Keyboard::Key::Equal) {
			return '=';
		}

		return NULL;
	}

	static char toUpperCase(char a) {
		// letters
		if (a >= 97 && a <= 122) {
			a -= 0x20;
		}
		// numbers
		else if (a >= 48 && a <= 57) {
			switch (a - 48) {
				// 1 2 3 4 5  7 8 9 0
			case 1:
			case 3:
			case 4:
			case 5:
				a = a - 16;
				break;

			case 9:
				a = a - 17;
				break;

			case 2:
				a = 64;
				break;

			case 7:
				a = 38;
				break;

			case 8:
				a = 42;
				break;

			case 0:
				a = 41;
				break;

			case 6:
				a = 94;
				break;
			}
		}
		// special keys
		else if (a == ']' || a == '[') {
			a += 32;
		}
		else if (a == ';') {
			a = ':';
		}
		// \ //
		else if (a == 92) {
			a = '|';
		}

		// '
		else if (a == 39) {
			a = '"';
		}
		else if (a == ',') {
			a = '<';
		}
		else if (a == '.') {
			a = '>';
		}
		else if (a == '/') {
			a = '?';
		}
		else if (a == '-') {
			a = '_';
		}
		else if (a == '=') {
			a = '+';
		}
		return a;
	}

	static char getChar() {
		for (int i = 0; i < sf::Keyboard::Key::KeyCount; i++) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(i))) {
				char a = keyToChar(sf::Keyboard::Key(i));
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ||
					sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)) {
					a = toUpperCase(a);
				}
				return a;
			}
		}
		return NULL;
	}
}
