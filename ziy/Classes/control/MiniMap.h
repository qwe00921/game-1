#ifndef _MiniMap_H_
#define _MiniMap_H_

enum
{
		//偏移消息ID
	 MSG_MINI_MAP_PRESSED = 0,       //按下
	 MSG_MINI_MAP_RELEASED = 1,      //释放
	 MSG_MINI_MAP_RPOINT_RELEASED = 2,      //释放
	 MSG_MINI_MAP_DRAG_IN = 3,
	 MSG_MINI_MAP_DRAG_OUT 
};
enum 
{
	MINI_MAP_STATE_CLOSE,
		MINI_MAP_STATE_OPENNING,
		MINI_MAP_STATE_CLOSING,
		MINI_MAP_STATE_OPEN
};
class MiniMap : public TRControl
{
public:
	TRImagePtr icon;
    TRImagePtr icon_day;
    //TRImagePtr icon_night;
	int state;
	int showWhichBigPoint;//某个阵营放大显示  除了0 1 2表示不放大
private:
	TRImagePtr img_sun;
	TRImagePtr img_rain;
	TRImagePtr img_big_rain;
	TRImagePtr img_snow;
	TRImagePtr img_cloud;

	int point_in;
	int point_x;
	int point_y;
    int old_BG_index;
public:
	MiniMap(int msgBaseId);
	~MiniMap();	
	void changeState();
	virtual	int pointerPressed(int x, int y);
	virtual	int RpointerPressed(int x, int y);
	virtual	int pointerDragged(int x, int y);
	virtual void onPaint(DGraphics* g);
	virtual void handleTimer();
private:
    void loadBG();
	void paintMiniMapPoint(DGraphics* g);
	void paintMiniMapPointOwnTop(DGraphics* g); //我军在最上层绘制
	void paintMiniMapPointFriendTop(DGraphics* g);//友军在最上层绘制
	void paintMiniMapPointEnemyTop(DGraphics* g);//敌军在最上层绘制
};
#endif