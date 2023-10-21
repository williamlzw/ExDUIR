#include "Geometry_ex.h"

D2DLine::D2DLine() :
	end({ 0.0f,0.0f }),
	c1({ 0.0f,0.0f }),
	c2({ 0.0f,0.0f })
{
	Type = LINE;
}

D2DLine::D2DLine(float x, float y) :
	c1({ 0.0f,0.0f }),
	c2({ 0.0f,0.0f })
{
	Type = LINE;
	end = { x,y };
}

D2DLine::D2DLine(D2D1_POINT_2F p) :
	c1({ 0.0f,0.0f }),
	c2({ 0.0f,0.0f })
{
	Type = LINE;
	end = p;
}

D2DLine::D2DLine(D2D1_POINT_2F _c1, D2D1_POINT_2F _end) :
	c2({ 0.0f,0.0f })
{
	Type = QBEZIER;
	c1 = _c1;
	end = _end;
}

D2DLine::D2DLine(D2D1_POINT_2F _c1, D2D1_POINT_2F _c2, D2D1_POINT_2F _end)
{
	Type = BEZIER;
	c1 = _c1;
	c2 = _c2;
	end = _end;
}


Geometry::Geometry() :_is_defaule(true)
{
}

Geometry::Geometry(float x, float y) : _is_defaule(false)
{
	this->Start = { x,y };
}

Geometry::Geometry(D2D1_POINT_2F _start) :_is_defaule(false)
{
	this->Start = _start;
}

Geometry::~Geometry()
{
	Lines.resize(0);
	if (GraphicsCache)
	{
		GraphicsCache->Release();
	}
}

void Geometry::Begin(float x, float y)
{
	_is_defaule = false;
	this->Start = { x,y };
	this->Changed = true;
}

void Geometry::Begin(D2D1_POINT_2F _start)
{
	_is_defaule = false;
	this->Start = _start;
	this->Changed = true;
}

void Geometry::Clear()
{
	this->Start = { 0.0f,0.0f };
	this->Lines.clear();
	this->Changed = true;
}

void Geometry::Insert(int index, D2DLine& l)
{
	this->Lines.insert(Lines.begin() + index, l);
	this->Changed = true;
}

void Geometry::Add(float x, float y)
{
	if (_is_defaule) {
		_is_defaule = false;
		this->Start = { x,y };
	}
	else {
		this->Lines.push_back(D2DLine(x, y));
	}
	this->Changed = true;
}

void Geometry::Add(D2D1_POINT_2F p)
{
	if (_is_defaule) {
		_is_defaule = false;
		this->Start = p;
	}
	else {
		this->Lines.push_back(D2DLine(p));
	}
	this->Changed = true;
}

void Geometry::Add(D2D1_POINT_2F c1, D2D1_POINT_2F c2, D2D1_POINT_2F end)
{
	if (!_is_defaule)
		this->Lines.push_back(D2DLine(c1, c2, end));
	else
		throw "未指定起始坐标,无法添加BESIER子对象!";
	this->Changed = true;
}

void Geometry::Add(D2D1_POINT_2F c1, D2D1_POINT_2F end)
{
	if (!_is_defaule)
		this->Lines.push_back(D2DLine(c1, end));
	else
		throw "未指定起始坐标,无法添加QBESIER子对象!";
	this->Changed = true;
}

ID2D1PathGeometry* Geometry::UpdateCache(bool force)
{
	if (this->Lines.size() > 0)
	{
		if (Changed || force)
		{
			ID2D1PathGeometry* new_cache = NULL;
			g_Ri.pD2Dfactory->CreatePathGeometry(&new_cache);
			if (new_cache)
			{
				ID2D1GeometrySink* skin = NULL;
				new_cache->Open(&skin);
				skin->BeginFigure(Start, D2D1_FIGURE_BEGIN_FILLED);
				for (int i = 0; i < this->Lines.size(); i++)
				{
					auto line = this->Lines[i];
					switch (line.Type)
					{
					case LineType::LINE:
					{
						skin->AddLine(line.end);
						break;
					}
					case LineType::BEZIER:
					{
						skin->AddBezier({ line.c1, line.c2,line.end });
						break;
					}
					case LineType::QBEZIER:
					{
						skin->AddQuadraticBezier({ line.c1,line.end });
						break;
					}
					default:
						break;
					}
				}
				skin->EndFigure(D2D1_FIGURE_END::D2D1_FIGURE_END_OPEN);
				skin->Close();
				skin->Release();
			}
			if (this->GraphicsCache != NULL)
			{
				this->GraphicsCache->Release();
			}
			this->GraphicsCache = new_cache;
			Changed = false;
		}
	}
	return this->GraphicsCache;
}

void Geometry::Move(float xf, float yf)
{
	if (this->Lines.size() > 0)
	{
		Start = { Start.x + xf,Start.y + yf };
		for (int i = 0; i < this->Lines.size(); i++)
		{
			auto& line = this->Lines[i];
			switch (line.Type)
			{
			case LineType::LINE:
			{
				line.end = { line.end.x + xf,line.end.y + yf };
				break;
			}
			case LineType::BEZIER:
			{
				line.c1 = { line.c1.x + xf,line.c1.y + yf };
				line.c2 = { line.c2.x + xf,line.c2.y + yf };
				line.end = { line.end.x + xf,line.end.y + yf };
				break;
			}
			case LineType::QBEZIER:
			{
				line.c1 = { line.c1.x + xf,line.c1.y + yf };
				line.end = { line.end.x + xf,line.end.y + yf };
				break;
			}
			default:
				break;
			}
		}
		Changed = true;
	}
}

void Geometry::ZoomByLocatin(D2D1_POINT_2F cent, float xper, float yper)
{

	auto calcZoomPos = [](D2D1_POINT_2F c, D2D1_POINT_2F p, float x, float y) -> D2D1_POINT_2F
		{
			float nx = p.x - c.x;
			float ny = p.y - c.y;
			nx = nx * x;
			ny = ny * y;
			return { nx + c.x,ny + c.y };
		};
	if (this->Lines.size() > 0)
	{
		Start = calcZoomPos(cent, Start, xper, yper);

		for (int i = 0; i < this->Lines.size(); i++)
		{
			auto& line = this->Lines[i];
			switch (line.Type)
			{
			case LineType::LINE:
			{
				line.end = calcZoomPos(cent, line.end, xper, yper);
				break;
			}
			case LineType::BEZIER:
			{
				line.c1 = calcZoomPos(cent, line.c1, xper, yper);
				line.c2 = calcZoomPos(cent, line.c2, xper, yper);
				line.end = calcZoomPos(cent, line.end, xper, yper);
				break;
			}
			case LineType::QBEZIER:
			{
				line.c1 = calcZoomPos(cent, line.c1, xper, yper);
				line.end = calcZoomPos(cent, line.end, xper, yper);
				break;
			}
			default:
				break;
			}
		}
		Changed = true;
	}
}

void Geometry::Rotate(D2D1_POINT_2F cent, float angle)
{
	const auto _rotate = [](D2D1_POINT_2F pt, D2D1_POINT_2F _cent, float angle)
		{
			angle = angle * 0.0174532925199433;
			float s = sin(angle);
			float c = cos(angle);
			pt.x -= _cent.x;
			pt.y -= _cent.y;
			float Ynew = pt.x * s + pt.y * c;
			pt.x = (pt.x * c - pt.y * s) + _cent.x;
			pt.y = Ynew + _cent.y;
			return pt;
		};
	if (this->Lines.size() > 0)
	{
		Start = _rotate(Start, cent, angle);
		for (int i = 0; i < this->Lines.size(); i++)
		{
			auto& line = this->Lines[i];
			switch (line.Type)
			{
			case LineType::LINE:
			{
				line.end = _rotate(line.end, cent, angle);
				break;
			}
			case LineType::BEZIER:
			{
				line.c1 = _rotate(line.c1, cent, angle);
				line.c2 = _rotate(line.c2, cent, angle);
				line.end = _rotate(line.end, cent, angle);
				break;
			}
			case LineType::QBEZIER:
			{
				line.c1 = _rotate(line.c1, cent, angle);
				line.end = _rotate(line.end, cent, angle);
				break;
			}
			default:
				break;
			}
		}
		Changed = true;
	}
}

void Geometry::Zoom(D2D1_POINT_2F cent, float xp, float yp)
{
	const auto _zoom = [](D2D1_POINT_2F pt, D2D1_POINT_2F _cent, float xp, float yp)
		{
			pt.x = ((pt.x - _cent.x) * xp) + _cent.x;
			pt.y = ((pt.y - _cent.y) * yp) + _cent.y;
			return pt;
		};
	if (this->Lines.size() > 0)
	{
		Start = _zoom(Start, cent, xp, yp);
		for (int i = 0; i < this->Lines.size(); i++)
		{
			auto& line = this->Lines[i];
			switch (line.Type)
			{
			case LineType::LINE:
			{
				line.end = _zoom(line.end, cent, xp, yp);
				break;
			}
			case LineType::BEZIER:
			{
				line.c1 = _zoom(line.c1, cent, xp, yp);
				line.c2 = _zoom(line.c2, cent, xp, yp);
				line.end = _zoom(line.end, cent, xp, yp);
				break;
			}
			case LineType::QBEZIER:
			{
				line.c1 = _zoom(line.c1, cent, xp, yp);
				line.end = _zoom(line.end, cent, xp, yp);
				break;
			}
			default:
				break;
			}
		}
		Changed = true;
	}
}

void Geometry::Flip(D2D1_POINT_2F cent, bool isHorizontal)
{
	const auto _flip = [](D2D1_POINT_2F pt, D2D1_POINT_2F _cent, bool isHorizontal)
		{
			if (isHorizontal)
			{
				pt.x = _cent.x - (pt.x - _cent.x);
			}
			else
			{
				pt.y = _cent.y - (pt.y - _cent.y);
			}
			return pt;
		};
	if (this->Lines.size() > 0)
	{
		Start = _flip(Start, cent, isHorizontal);
		for (int i = 0; i < this->Lines.size(); i++)
		{
			auto& line = this->Lines[i];
			switch (line.Type)
			{
			case LineType::LINE:
			{
				line.end = _flip(line.end, cent, isHorizontal);
				break;
			}
			case LineType::BEZIER:
			{
				line.c1 = _flip(line.c1, cent, isHorizontal);
				line.c2 = _flip(line.c2, cent, isHorizontal);
				line.end = _flip(line.end, cent, isHorizontal);
				break;
			}
			case LineType::QBEZIER:
			{
				line.c1 = _flip(line.c1, cent, isHorizontal);
				line.end = _flip(line.end, cent, isHorizontal);
				break;
			}
			default:
				break;
			}
		}
		Changed = true;
	}
}

Geometry* Geometry::Reversal()
{
	D2D1_POINT_2F start = this->Lines.back().end;
	std::vector<D2DLine> lines;
	for (int i = this->Lines.size() - 1; i >= 0; i--)
	{
		auto end = this->Start;
		if (i > 0)
		{
			end = this->Lines[i - 1].end;
		}
		if (this->Lines[i].Type == LINE)
		{
			lines.push_back(D2DLine(end));
		}
		else if (this->Lines[i].Type == QBEZIER)
		{
			lines.push_back(D2DLine(this->Lines[i].c1, end));
		}
		else if (this->Lines[i].Type == BEZIER)
		{
			lines.push_back(D2DLine(this->Lines[i].c2, this->Lines[i].c1, end));
		}
	}
	this->Lines.clear();
	this->Start = start;
	for (auto l : lines)
	{
		this->Lines.push_back(l);
	}
	this->Changed = true;
	return this;
}

Geometry* Geometry::Link(Geometry* target)
{
	Geometry* result = NULL;
	auto samepoint = [](D2D1_POINT_2F p1, D2D1_POINT_2F p2)
		{
			return p1.x == p2.x && p1.y == p2.y;
		};
	if (samepoint(this->Lines.back().end, target->Start))
	{
		result = new Geometry(this->Start);
		for (auto l : this->Lines)
		{
			result->Lines.push_back(l);
		}
		for (auto l : target->Lines)
		{
			result->Lines.push_back(l);
		}
	}
	else if (samepoint(target->Lines.back().end, this->Start))
	{
		result = new Geometry(target->Start);
		for (auto l : target->Lines)
		{
			result->Lines.push_back(l);
		}
		for (auto l : this->Lines)
		{
			result->Lines.push_back(l);
		}
	}
	else if (samepoint(this->Start, target->Start))
	{
		result = new Geometry(target->Start);
		for (auto l : target->Lines)
		{
			result->Lines.push_back(l);
		}
		result->Reversal();
		for (auto l : this->Lines)
		{
			result->Lines.push_back(l);
		}
	}
	else if (samepoint(this->Lines.back().end, target->Lines.back().end))
	{
		result = new Geometry(this->Start);
		for (auto l : this->Lines)
		{
			result->Lines.push_back(l);
		}
		auto tmp = new Geometry(target->Start);
		for (auto l : target->Lines)
		{
			tmp->Lines.push_back(l);
		}
		tmp->Reversal();
		for (auto l : tmp->Lines)
		{
			result->Lines.push_back(l);
		}
		delete tmp;
	}
	return result;
}

//从该图形复制一个新对象
Geometry* Geometry::Clone()
{
	Geometry* result = new Geometry(this->Start);
	result->Lines.reserve(this->Lines.size());
	result->Lines.resize(this->Lines.size());

	memcpy(result->Lines.data(), this->Lines.data(), this->Lines.size() * sizeof(D2DLine));
	return result;
}
//判断点是否包含在该图形内部
bool Geometry::Contains(D2D1_POINT_2F p)
{
	BOOL result = false;
	this->UpdateCache()->FillContainsPoint(p, NULL, 0.0f, &result);
	return result;
}
//根据指定误差判断点是否命中该图形的线
bool Geometry::StrokeHit(D2D1_POINT_2F p, float px)
{
	BOOL result = false;
	this->UpdateCache()->StrokeContainsPoint(p, px, 0, 0, &result);
	return result;
}
//该图形是否完全包含目标图形
bool Geometry::Contains(Geometry* p)
{
	D2D1_GEOMETRY_RELATION res = {};
	this->UpdateCache()->CompareWithGeometry(p->UpdateCache(), NULL, &res);
	return res == D2D1_GEOMETRY_RELATION_CONTAINS;
}
//该图形与目标图形是否存在任意一个完全包含另外一个的关系
bool Geometry::ArbitraryContains(Geometry* p)
{
	D2D1_GEOMETRY_RELATION res = {};
	this->UpdateCache()->CompareWithGeometry(p->UpdateCache(), NULL, &res);
	return res == D2D1_GEOMETRY_RELATION_IS_CONTAINED || res == D2D1_GEOMETRY_RELATION_CONTAINS;
}
//该图形是否完全包含目标图形
bool Geometry::Contains(Geometry& p)
{
	D2D1_GEOMETRY_RELATION res = {};
	this->UpdateCache()->CompareWithGeometry(p.UpdateCache(), NULL, &res);
	return res == D2D1_GEOMETRY_RELATION_CONTAINS;
}
//判断该图形与目标图形是否碰撞或重叠
bool Geometry::CollisionWhith(Geometry* p)
{
	D2D1_GEOMETRY_RELATION res = {};
	this->UpdateCache()->CompareWithGeometry(p->UpdateCache(), NULL, &res);
	return res > 1;
}
//判断该图形与目标图形是否碰撞或重叠
bool Geometry::CollisionWhith(Geometry& p)
{
	D2D1_GEOMETRY_RELATION res = {};
	this->UpdateCache()->CompareWithGeometry(p.UpdateCache(), NULL, &res);
	return res > 1;
}


SpecializedSink::SpecializedSink() : m_cRef(1)
{
}

ULONG SpecializedSink::AddRef(THIS)
{
	return InterlockedIncrement(reinterpret_cast<LONG volatile*>(&m_cRef));
}

ULONG SpecializedSink::Release(THIS)
{
	ULONG cRef = static_cast<ULONG>(
		InterlockedDecrement(reinterpret_cast<LONG volatile*>(&m_cRef)));

	if (0 == cRef)
	{
		delete this;
	}

	return cRef;
}

HRESULT SpecializedSink::QueryInterface(THIS_ REFIID iid, void** ppvObject)
{
	HRESULT hr = S_OK;

	if (__uuidof(IUnknown) == iid)
	{
		*ppvObject = static_cast<IUnknown*>(this);
		AddRef();
	}
	else if (__uuidof(ID2D1SimplifiedGeometrySink) == iid)
	{
		*ppvObject = static_cast<ID2D1SimplifiedGeometrySink*>(this);
		AddRef();
	}
	else
	{
		*ppvObject = NULL;
		hr = E_NOINTERFACE;
	}

	return hr;
}

void SpecializedSink::BeginFigure(D2D1_POINT_2F startPoint, D2D1_FIGURE_BEGIN figureBegin) {
	D2D1_POINT_2F newPoint = { startPoint.x + 0.5f, startPoint.y + 0.5f };
	this->skin = new Geometry(newPoint);
}

void SpecializedSink::AddBeziers(const D2D1_BEZIER_SEGMENT* beziers, UINT beziersCount)
{
	for (UINT i = 0; i < beziersCount; ++i)
	{
		this->skin->Add(beziers[i].point1, beziers[i].point2, beziers[i].point3);
	}
}

void SpecializedSink::AddLines(const D2D1_POINT_2F* points, UINT pointsCount)
{
	for (UINT i = 0; i < pointsCount; i++)
	{
		this->skin->Add(points[i]);
	}
}

void SpecializedSink::AddLine(D2D1_POINT_2F point)
{
	this->skin->Add(point);
}

void SpecializedSink::AddBezier(_In_ CONST D2D1_BEZIER_SEGMENT* bezier)
{
	this->skin->Add(bezier->point1, bezier->point2, bezier->point3);
}

void SpecializedSink::EndFigure(D2D1_FIGURE_END figureEnd)
{
	if (figureEnd == D2D1_FIGURE_END_CLOSED)
	{
		this->skin->Add(this->skin->Start);
	}
	this->Mulskin.push_back(this->skin);
	this->skin = NULL;
}

void SpecializedSink::SetFillMode(D2D1_FILL_MODE fillMode)
{
}

void SpecializedSink::SetSegmentFlags(D2D1_PATH_SEGMENT vertexFlags)
{
}

HRESULT SpecializedSink::Close()
{
	return S_OK;
}
