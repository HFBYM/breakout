#include"player.h"
#include<iostream>
void Player::init()
{

}

void Player::log_all()
{
	log_renderer();
	log_keyboard();
}

void Player::detach_all()
{
	detach_renderer();
	detach_keyboard();
}

void Player::processInput(int key, int action)
{
	std::cout << key << "\t" << action << std::endl;
}

