/**
 * ここでそれぞれの画像のUV座標を指定する
 */
#pragma once

#include "stdafx.h"
#include "Common3DType.h"

//プレイヤーが死ぬときのエフェクトのUV座標
const std::vector <QuadrangleTexel>		kTexelPlayerDeath = {
	//		左下			左上			右下			右上
	{{0.0 / 5.0,1.0},{0.0 / 5.0,0.0},{1.0 / 5.0,1.0},{1.0 / 5.0,0.0}},//1/5
	{{1.0 / 5.0,1.0},{1.0 / 5.0,0.0},{2.0 / 5.0,1.0},{2.0 / 5.0,0.0}},//2/5
	{{2.0 / 5.0,1.0},{2.0 / 5.0,0.0},{3.0 / 5.0,1.0},{3.0 / 5.0,0.0}},//3/5
	{{3.0 / 5.0,1.0},{3.0 / 5.0,0.0},{4.0 / 5.0,1.0},{4.0 / 5.0,0.0}},//4/5
	{{4.0 / 5.0,1.0},{4.0 / 5.0,0.0},{5.0 / 5.0,1.0},{5.0 / 5.0,0.0}},//5/5
};

//敵が死ぬときのエフェクトのUV座標
const std::vector <QuadrangleTexel>		kTexelEnemyDeath = {
	//		左下			左上			右下			右上
	{{0.0 / 5.0,1.0},{0.0 / 5.0,0.0},{1.0 / 5.0,1.0},{1.0 / 5.0,0.0}},//1/5
	{{1.0 / 5.0,1.0},{1.0 / 5.0,0.0},{2.0 / 5.0,1.0},{2.0 / 5.0,0.0}},//2/5
	{{2.0 / 5.0,1.0},{2.0 / 5.0,0.0},{3.0 / 5.0,1.0},{3.0 / 5.0,0.0}},//3/5
	{{3.0 / 5.0,1.0},{3.0 / 5.0,0.0},{4.0 / 5.0,1.0},{4.0 / 5.0,0.0}},//4/5
	{{4.0 / 5.0,1.0},{4.0 / 5.0,0.0},{5.0 / 5.0,1.0},{5.0 / 5.0,0.0}},//5/5
};

//プレイヤーのUV座標
const std::vector <QuadrangleTexel>		kTexelPlayer = {
	//		左下			左上			右下			右上
	{{0.0 / 4.0,1.0},{0.0 / 4.0,0.0},{1.0 / 4.0,1.0},{1.0 / 4.0,0.0}},//1/4
	{{1.0 / 4.0,1.0},{1.0 / 4.0,0.0},{2.0 / 4.0,1.0},{2.0 / 4.0,0.0}},//2/4
	{{2.0 / 4.0,1.0},{2.0 / 4.0,0.0},{3.0 / 4.0,1.0},{3.0 / 4.0,0.0}},//3/4
	{{3.0 / 4.0,1.0},{3.0 / 4.0,0.0},{4.0 / 4.0,1.0},{4.0 / 4.0,0.0}},//4/4
	
};
