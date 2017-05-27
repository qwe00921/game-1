using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

namespace TRGameEditor.Forms
{
    class CDlgAlign
    {
        int m_LeftDef,m_TopDef,m_Left,m_Top,m_LeftOld,m_TopOld;
	    int m_WidthDef,m_HeightDef;
	    int m_LastWidth,m_LastHeight;
	    int m_HeightMax,m_WidthMax;
	    int m_MarginV,m_MarginH;
        public CDlgAlign()
        {
	        m_LeftDef=6;
	        m_TopDef=6;
	        m_MarginV=3;
	        m_MarginH=6;
        }

        public void Reset(int WidthDef,int HeightDef,int WidthMax,int HeightMax)
        {
	        m_HeightDef=HeightDef;
	        m_WidthDef=WidthDef;
	        m_HeightMax=HeightMax;
	        m_WidthMax=WidthMax;
	        m_LeftOld=m_Left=m_LeftDef;
	        m_TopOld=m_Top=m_TopDef;
	        m_LastHeight=m_LastWidth=0;
        }

        public void MoveToNextLine(bool isUseMargin = true, bool isLeft= true)
        {
	        m_Top+=m_LastHeight;
	        if (isLeft) {
		        m_LeftOld=m_Left=m_LeftDef;
	        } else {
		        m_Left=m_LeftOld;
	        }
	        if (isUseMargin)
		        m_Top+=m_MarginV;
	        if (m_Top>=m_HeightMax)
		        m_HeightMax=m_Top;
	        m_LastHeight=0;
	        if (isLeft) {
		        m_TopOld=m_Top;
		        m_LastWidth=0;
	        }
        }

        public void MoveToRight(bool isUseMargin = true)
        {
	        m_LeftOld=m_Left;
	        m_Left+=m_LastWidth;
	        if (isUseMargin) m_Left+=m_MarginH;
	        if (m_Left>=m_WidthMax) m_WidthMax=m_Left;
        }

        public void SetLastHeight(int Height = 0)
        {
	        int t=(Height>0)?Height:m_HeightDef;
	        if (t>m_LastHeight) m_LastHeight=t;
        }

        public Rectangle GetRect(int Width, int Height, int LeftOffset = 0 , int TopOffset = 0)
        {
	        return new Rectangle(m_Left+LeftOffset,m_Top+TopOffset,Width,Height);
        }
        public void MoveLeftBack() { m_Left = m_LeftOld; }
        public void MoveTopBack() { m_Top = m_TopOld; }

        public void SetLastWidth(int Width) { m_LastWidth = Width; }

        public int GetLastLeft() { return m_LeftOld; }
        public void SetLastLeft(int Left) { m_LeftOld = Left; }

        public int GetMarginV() { return m_MarginV; }
        public int GetMarginH() { return m_MarginH; }
        public int GetLeftDef() { return m_LeftDef; }
        public int GetTopDef() { return m_TopDef; }
        public int GetWidthDef() { return m_WidthDef; }
        public int GetHeightDef() { return m_HeightDef; }
        public int GetWidthMax() { return m_WidthMax; }
        public int GetHeightMax() { return m_HeightMax; }
        public int GetLastWidth() { return m_LastWidth; }
        public int GetLeft() { return m_Left; }
        public int GetTop() { return m_Top; }
    }
}
