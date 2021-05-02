#ifndef YPLANTS_H
#define YPLANTS_H

#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include "gamelib.h"
#include "YSun.h"
#include "YZombies.h"

namespace game_framework {
	
	class YSunFlower
	{
	public:
		YSunFlower(int x, int y) {
			this->x = x + 8;
			this->y = y + 13;
			blood = 200;
			sun_make_time = 50;
			is_alive = true;
		}
		~YSunFlower() {
		}
		void LoadBitmap() {
			char *filename[8] = { ".\\bitmaps\\SunFlower\\SunFlower_0.bmp",".\\bitmaps\\SunFlower\\SunFlower_1.bmp"
				, ".\\bitmaps\\SunFlower\\SunFlower_2.bmp",".\\bitmaps\\SunFlower\\SunFlower_3.bmp"
				, ".\\bitmaps\\SunFlower\\SunFlower_4.bmp",".\\bitmaps\\SunFlower\\SunFlower_5.bmp"
				, ".\\bitmaps\\SunFlower\\SunFlower_6.bmp",".\\bitmaps\\SunFlower\\SunFlower_7.bmp"
			};
			for (int i = 0; i < 8; i++)
				sun_flower_animation.AddBitmap(filename[i], RGB(255, 255, 255));
		}
		void OnMove() {
			sun_flower_animation.OnMove();
			if (sun_make_time == 0) {
				sunanimation.OnMove();
				
			}
		}
		void OnShow() {
			if (IsAlive()) {
				sun_flower_animation.SetTopLeft(x, y);
				sun_flower_animation.OnShow();
			}
		}
		bool checkPlantCollideWithZombie(int zx, int zy) {
			if (zy == y - 13) {
				if(x > zx - 72 && x < zx - 52){
					return true;
				}
			}
			return false;
		}
		bool IsAlive() {
			return is_alive;
		}
		void SetIsAlive(bool alive) {
			is_alive = alive;
		}
		int  GetX() {
			return x;
		}
		int  GetY() {
			return y;
		}
		void LostBlood(int attack_blood) {
			blood = blood - attack_blood;
			if (blood == 0) {
				is_alive = false;
			}
		}
		int  GetBlood() {
			return blood;
		}

	private:
		int x, y;
		bool is_alive;
		int blood;
		int sun_make_time;
		CAnimation sun_flower_animation;
		CAnimation sunanimation;
	};

	class YPeaShooterBullet {
	public:
		YPeaShooterBullet(int x, int y) {
			this->x = x+30;
			this->y = y;
			is_alive = true;
		}

		void LoadBitmap()
		{
			peashooter_bullet.LoadBitmap(".\\bitmaps\\PeaShooter\\PeaNormal_0.bmp", RGB(255, 255, 255));
		}
		void OnMove()
		{
			x += 3;
			if (x > 800) {
				is_alive = false;
			};
		}

		void OnShow()
		{
			if (is_alive)
			{
				peashooter_bullet.SetTopLeft(x, y);
				peashooter_bullet.ShowBitmap();
			}
		}
		bool IsAlive()
		{
			return is_alive;
		}
		void SetIsAlive(bool alive)
		{
			is_alive = alive;
		}

		int GetX()
		{
			return x;
		}
		int GetY()
		{
			return y;
		}

	private:
		int x, y;
		bool is_alive;
		CMovingBitmap peashooter_bullet;
	};


	class YPeaShooter
	{
	public:
		YPeaShooter(int x, int y)
		{
			delay = 30;
			this->x = x + 20;
			this->y = y + 20;
			blood = 200;
			is_alive = true;
		}
		~YPeaShooter()
		{
		}
		void LoadBitmap()
		{
			char *filename[6] = { ".\\bitmaps\\PeaShooter\\PeaShooter_0.bmp", ".\\bitmaps\\PeaShooter\\PeaShooter_1.bmp", ".\\bitmaps\\PeaShooter\\PeaShooter_2.bmp", ".\\bitmaps\\PeaShooter\\PeaShooter_3.bmp", ".\\bitmaps\\PeaShooter\\PeaShooter_4.bmp", ".\\bitmaps\\PeaShooter\\PeaShooter_5.bmp" };
			for (int i = 0; i < 6; i++)
				peashooter_animation.AddBitmap(filename[i], RGB(255, 255, 255));
		}
		void OnMove()
		{
			peashooter_animation.OnMove();
			
			if (delay == 0) {
				fireBullet();
				delay = 80;
			}
			delay--;

			for (size_t i = 0; i < bullets_vector.size(); i++) {
				if (bullets_vector.at(i)->IsAlive())
					bullets_vector.at(i)->OnMove();
				else {
					bullets_vector.erase(bullets_vector.begin()+i);
				}
			}

		}
		void OnShow()
		{
			if (is_alive)
			{
				peashooter_animation.SetTopLeft(x, y);
				peashooter_animation.OnShow();
			}
			for (size_t i = 0; i < bullets_vector.size(); i++) {
				if (bullets_vector.at(i)->IsAlive())
					bullets_vector.at(i)->OnShow();
			}
		}
		bool IsAlive()
		{
			return is_alive;
		}
		void SetIsAlive(bool alive)
		{
			is_alive = alive;
		}
		int GetX()
		{
			return x;
		}
		int GetY()
		{
			return y;
		}
		void SetBlood(int attack_blood)
		{
			blood = blood - attack_blood;
			if (blood == 0) {
				is_alive = false;
			}
		}
		int GetBlood()
		{
			return blood;
		}
		void fireBullet() {
			auto sp = make_shared<YPeaShooterBullet>(x, y);
			sp->LoadBitmap();
			bullets_vector.push_back(sp);
		}
		void LostBlood(int attack_blood) {
			blood = blood - attack_blood;
			if (blood == 0) {
				is_alive = false;
			}
		}
		bool checkBulletCollideWithZombie(int zx, int mapy) {
			if (!bullets_vector.empty() && bullets_vector.at(0)->GetY() - 20 == mapy) {
				int t = bullets_vector.at(0)->GetX();
				if ( bullets_vector.at(0)->GetX() > zx + 20 && bullets_vector.at(0)->GetX() < zx + 80) {
					bullets_vector.at(0)->SetIsAlive(false);
					return true;
				}
			}
			return false;
		}
		bool checkPlantCollideWithZombie(int zx, int zy) {
			if (zy == y - 20) {
				if (x > zx - 60 && x < zx - 40) {
					return true;
				}
			}
			return false;
		}

	private:
		int x, y;
		bool is_alive;
		int blood;
		std::vector<shared_ptr<YPeaShooterBullet>> bullets_vector;
		CAnimation peashooter_animation;
		int delay;
	};
}

#endif

