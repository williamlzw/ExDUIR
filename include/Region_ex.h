#pragma once
#include "help_ex.h"
#include "ExDUIR_Func.h"


class SpecializedSink : public ID2D1SimplifiedGeometrySink
{
public:
    SpecializedSink(ExtractPathLinePROC proc1, ExtractPathCubicPROC proc2)
        : m_cRef(1), m_proc1(proc1), m_proc2(proc2)
    {
    }

    STDMETHOD_(ULONG, AddRef)(THIS)
    {
        return InterlockedIncrement(reinterpret_cast<LONG volatile*>(&m_cRef));
    }

    STDMETHOD_(ULONG, Release)(THIS)
    {
        ULONG cRef = static_cast<ULONG>(
            InterlockedDecrement(reinterpret_cast<LONG volatile*>(&m_cRef)));

        if (0 == cRef)
        {
            delete this;
        }

        return cRef;
    }

    STDMETHOD(QueryInterface)(THIS_ REFIID iid, void** ppvObject)
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

    STDMETHOD_(void, AddBeziers)(const D2D1_BEZIER_SEGMENT* beziers,
        UINT beziersCount)
    {
		if (m_proc2)
		{
			EX_BEZIER_SEGMENT* newSegments = new EX_BEZIER_SEGMENT[beziersCount];
			memmove(newSegments, beziers, sizeof(D2D1_BEZIER_SEGMENT) * beziersCount);
			m_proc2(newSegments, beziersCount);
			free(newSegments);
		}
    }

    STDMETHOD_(void, AddLines)(const D2D1_POINT_2F* points, UINT pointsCount)
    {
		if (m_proc1)
		{
			POINTF* newPoints = new POINTF[pointsCount];
			memmove(newPoints, points, sizeof(POINTF) * pointsCount);
			m_proc1(newPoints, pointsCount);
			free(newPoints);
		}
    }

    STDMETHOD_(void, BeginFigure)(D2D1_POINT_2F startPoint,
        D2D1_FIGURE_BEGIN figureBegin)
    {
    }

    STDMETHOD_(void, EndFigure)(D2D1_FIGURE_END figureEnd)
    {
    }

    STDMETHOD_(void, SetFillMode)(D2D1_FILL_MODE fillMode)
    {
    }

    STDMETHOD_(void, SetSegmentFlags)(D2D1_PATH_SEGMENT vertexFlags)
    {
    }

    STDMETHOD(Close)()
    {
        return S_OK;
    }

private:
	ExtractPathLinePROC m_proc1;
	ExtractPathCubicPROC m_proc2;
    UINT m_cRef;
};