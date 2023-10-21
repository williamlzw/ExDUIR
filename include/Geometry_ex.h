#pragma once
#include "help_ex.h"

enum LineType
{
	LINE,
	BEZIER,
	QBEZIER
};

class D2DLine
{
public:
	LineType Type;
	D2D1_POINT_2F c1;
	D2D1_POINT_2F c2;
	D2D1_POINT_2F end;
	D2DLine();
	D2DLine(float x, float y);
	D2DLine(D2D1_POINT_2F p);
	D2DLine(D2D1_POINT_2F _c1, D2D1_POINT_2F _end);
	D2DLine(D2D1_POINT_2F _c1, D2D1_POINT_2F _c2, D2D1_POINT_2F _end);
};

class Geometry
{
	bool _is_defaule = true;
public:
	ID2D1PathGeometry* GraphicsCache = NULL;
	D2D1_POINT_2F Start;
	std::vector<D2DLine> Lines;
	bool Changed = true;
	Geometry();
	Geometry(float x, float y);
	Geometry(D2D1_POINT_2F _start);
	~Geometry();
	void Begin(float x, float y);
	void Begin(D2D1_POINT_2F _start);
	void Clear();
	void Insert(int index, D2DLine& l);
	D2DLine& operator[](int idx)
	{
		return this->Lines[idx];
	}
	void Add(float x, float y);
	void Add(D2D1_POINT_2F p);
	void Add(D2D1_POINT_2F c1, D2D1_POINT_2F c2, D2D1_POINT_2F end);
	void Add(D2D1_POINT_2F c1, D2D1_POINT_2F end);
	ID2D1PathGeometry* UpdateCache(bool force = false);
	void Move(float xf, float yf);
	void ZoomByLocatin(D2D1_POINT_2F cent, float xper, float yper);
	void Rotate(D2D1_POINT_2F cent, float angle);
	void Zoom(D2D1_POINT_2F cent, float xp, float yp);
	void Flip(D2D1_POINT_2F cent, bool isHorizontal);
	Geometry* Reversal();
	Geometry* Link(Geometry* target);

	//从该图形复制一个新对象
	Geometry* Clone();
	//判断点是否包含在该图形内部
	bool Contains(D2D1_POINT_2F p);
	//根据指定误差判断点是否命中该图形的线
	bool StrokeHit(D2D1_POINT_2F p, float px);
	//该图形是否完全包含目标图形
	bool Contains(Geometry* p);
	//该图形与目标图形是否存在任意一个完全包含另外一个的关系
	bool ArbitraryContains(Geometry* p);
	//该图形是否完全包含目标图形
	bool Contains(Geometry& p);
	//判断该图形与目标图形是否碰撞或重叠
	bool CollisionWhith(Geometry* p);
	//判断该图形与目标图形是否碰撞或重叠
	bool CollisionWhith(Geometry& p);
	Geometry* operator * (float v)
	{
		Geometry* geo = new Geometry(D2D1_POINT_2F{ this->Start.x * v,this->Start.y * v });
		geo->Lines.insert(geo->Lines.end(), this->Lines.begin(), this->Lines.end());
		for (int i = 0; i < geo->Lines.size(); i++)
		{
			geo->Lines[i].c1 = D2D1_POINT_2F{ geo->Lines[i].c1.x * v,geo->Lines[i].c1.y * v };
			geo->Lines[i].c2 = D2D1_POINT_2F{ geo->Lines[i].c2.x * v,geo->Lines[i].c2.y * v };
			geo->Lines[i].end = D2D1_POINT_2F{ geo->Lines[i].end.x * v,geo->Lines[i].end.y * v };
		}
		return geo;
	}
	Geometry* operator / (float v)
	{
		Geometry* geo = new Geometry(D2D1_POINT_2F{ this->Start.x * v,this->Start.y * v });
		geo->Lines.insert(geo->Lines.end(), this->Lines.begin(), this->Lines.end());
		for (int i = 0; i < geo->Lines.size(); i++)
		{
			geo->Lines[i].c1 = D2D1_POINT_2F{ geo->Lines[i].c1.x / v,geo->Lines[i].c1.y / v };
			geo->Lines[i].c2 = D2D1_POINT_2F{ geo->Lines[i].c2.x / v,geo->Lines[i].c2.y / v };
			geo->Lines[i].end = D2D1_POINT_2F{ geo->Lines[i].end.x / v,geo->Lines[i].end.y / v };
		}
		return geo;
	}
};


class SpecializedSink : public ID2D1SimplifiedGeometrySink
{
public:
	SpecializedSink();
	Geometry* skin;
	std::vector<Geometry*> Mulskin;

	STDMETHOD_(ULONG, AddRef)(THIS);
	STDMETHOD_(ULONG, Release)(THIS);
	STDMETHOD(QueryInterface)(THIS_ REFIID iid, void** ppvObject);
	STDMETHOD_(void, BeginFigure)(D2D1_POINT_2F startPoint, D2D1_FIGURE_BEGIN figureBegin);
	STDMETHOD_(void, AddBeziers)(const D2D1_BEZIER_SEGMENT* beziers, UINT beziersCount);
	STDMETHOD_(void, AddLines)(const D2D1_POINT_2F* points, UINT pointsCount);
	STDMETHOD_(void, AddLine)(D2D1_POINT_2F point);
	STDMETHOD_(void, AddBezier)(_In_ CONST D2D1_BEZIER_SEGMENT* bezier);
	STDMETHOD_(void, EndFigure)(D2D1_FIGURE_END figureEnd);
	STDMETHOD_(void, SetFillMode)(D2D1_FILL_MODE fillMode);
	STDMETHOD_(void, SetSegmentFlags)(D2D1_PATH_SEGMENT vertexFlags);
	STDMETHOD(Close)();
private:
	UINT m_cRef;
};