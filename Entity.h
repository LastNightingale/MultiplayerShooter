#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <vector>
#include <iostream>
#include <vector>

using namespace sf;

struct RenderList
{
	std::vector<RectangleShape> Rects;
};

class Entity
{
protected:
	RectangleShape m_Body;
public:
	Entity() = default;
	Entity(sf::RectangleShape Body);
	void Move(Vector2f MoveVector);

	virtual void Update(float dt);

	void AddToRenderList(RenderList& list);

	Vector2f GetPosition() const;

	void SetPosition(Vector2f pos);

	RectangleShape GetBody();

	FloatRect GetGlobalBounds() const;

	virtual bool Collided(Entity* other);
};

namespace sf
{
	//inline Packet& operator <<(Packet& packet, const Color& color) //color write
	//{
	//	return packet << color.r << color.g << color.b << color.a;
	//}
	//inline Packet& operator >>(Packet& packet, Color& color) //color recieve
	//{
	//	return packet >> color.r >> color.g >> color.b >> color.a;
	//}

	inline Packet& operator <<(Packet& packet, const Vector2f& vector) //vector write
	{
		return packet << vector.x << vector.y;
	}
	inline Packet& operator >>(Packet& packet, Vector2f& vector) //vector recieve
	{
		return packet >> vector.x >> vector.y;
	}
	//inline Packet& operator <<(Packet& packet, const RectangleShape& rect) //vector write
	//{

	//	return packet << vector.x << vector.y;
	//}
	//inline Packet& operator >>(Packet& packet, RectangleShape& rect) //vector recieve
	//{
	//	return packet >> vector.x >> vector.y;
	//}
	//inline Packet& operator <<(Packet& packet, const Entity& entity) //Entity write
	//{
	//	return packet << entity.GetPosition();
	//}
	//inline Packet& operator >>(Packet& packet, Entity& rect) //Entity recieve
	//{
	//	Vector2f position;
	//	packet >> position;
	//	rect.SetPosition(position);
	//	return packet;
	//}	
	template <typename T>
	inline Packet& operator <<(Packet& packet, const std::vector<T>& values)
	{
		//static_assert(!std::is_pointer<T>::value, "wrong data type");
		int size = values.size();
		packet << size;
		for (int i = 0; i < values.size(); ++i)
			packet << values[i];
		return packet;
	}

	template <typename T>
	inline Packet& operator >>(Packet& packet, std::vector<T>& values)
	{
		int size;	
		T value;
		packet >> size;
		//std::cout << size << std::endl;
		for (int i = 0; i < size; ++i)
		{
			packet >> value;
			values.push_back(value);
		}
		return packet;
	}
}