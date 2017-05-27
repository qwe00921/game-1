#ifndef _MapElem_H_
#define _MapElem_H_

class MapElem//地图障碍物信息
{
public:	
	MapElem(int x,int y);
	MapElem(int x, int y,int elem_id,int land_id,int iImgIndex,int &iIsFind);
	~MapElem();
	void paint(DGraphics* g);
public:
	int x;
	int y;
	int barrier_id;
	int land_id;
private:
	int frame;
	int index;
	int max_index;
	int change_frame;
	TRImagePtr img[4];
private:
	/*
	* 禁止对象拷贝，声明私有方法并且不实现
	*/
	MapElem(const MapElem&);
	MapElem& operator=(const MapElem&);
};
#endif