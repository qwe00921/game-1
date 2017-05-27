using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using TRGameEditor.Scenes;

namespace TRGameEditor.Forms
{

    public partial class CCodeEditDlg : Form
    {
        static int m_MaxStrBox=1;
        static int m_MaxTextBox=12;
		static int m_MaxComboBox=12;
        static int m_MaxListBox=1;
        static int m_MaxCheckBox=5;
		static int m_MaxPosBox=2;
        static int m_MaxAreaBox=1;
        static int m_MaxCodeSelBox=1;

        int m_ComboBoxNum = 0;
        int m_TextBoxNum = 0;
        int m_StrBoxNum = 0;
        int m_CheckBoxNum = 0;
        int m_PosBoxNum = 0;
        int m_AreaBoxNum = 0;
        int m_CodeSelBoxNum = 0;
        int m_DefCharWidth=6;
	    int m_DefPadding=6;
        Label[] m_Static_1 = new Label[m_MaxTextBox]; //for textbox
        CCodeEditTextBox[] m_TextBox = new CCodeEditTextBox[m_MaxTextBox];
        Label[] m_Static_2 = new Label[m_MaxComboBox]; //for combobox
        CCodeEditComboBox[] m_ComboBox = new CCodeEditComboBox[m_MaxComboBox];
        CCodeEditCheckBox[] m_CheckBox = new CCodeEditCheckBox[m_MaxCheckBox];
        Label[] m_Static_3 = new Label[m_MaxStrBox]; //for strbox 
        CCodeEditStrBox[] m_StrBox = new CCodeEditStrBox[m_MaxStrBox];
        Label[] m_Static_4 = new Label[m_MaxPosBox];
        CCodeEditTextBox[] m_PosBox = new CCodeEditTextBox[m_MaxPosBox * 2];
        Label[] m_Static_5 = new Label[m_MaxAreaBox * 2];
        CCodeEditTextBox[] m_AreaBox = new CCodeEditTextBox[m_MaxAreaBox * 4];
        Label[] m_Static_6 = new Label[m_MaxCodeSelBox * 2];
        CCodeEditStrBox[] m_CodeSelBox = new CCodeEditStrBox[m_MaxCodeSelBox * 2];
        CCodeEditListBox[] m_ListBox = new CCodeEditListBox[m_MaxListBox];
        ScriptCommand cmd, cmd_backup;
        CDlgAlign m_DlgAlign = new CDlgAlign();

        public CCodeEditDlg(ScriptCommand obj)
        {
            InitializeComponent();
            for (int i = 0; i < m_TextBox.Length; i++)
            {
                m_TextBox[i] = new CCodeEditTextBox();
            }
            for (int i = 0; i < m_ComboBox.Length; i++)
            {
                m_ComboBox[i] = new CCodeEditComboBox();
            }
            for (int i = 0; i < m_CheckBox.Length; i++)
            {
                m_CheckBox[i] = new CCodeEditCheckBox();
            }
            for (int i = 0; i < m_StrBox.Length; i++)
            {
                m_StrBox[i] = new CCodeEditStrBox();
            }
            for (int i = 0; i < m_PosBox.Length; i++)
            {
                m_PosBox[i] = new CCodeEditTextBox();
            }
            for (int i = 0; i < m_AreaBox.Length; i++)
            {
                m_AreaBox[i] = new CCodeEditTextBox();
            }
            for (int i = 0; i < m_CodeSelBox.Length; i++)
            {
                m_CodeSelBox[i] = new CCodeEditStrBox();
            }
            for (int i = 0; i < m_ListBox.Length; i++)
            {
                m_ListBox[i] = new CCodeEditListBox();
            }
            for(int i=0;i<m_Static_1.Length;i++)
            {
                m_Static_1[i] = new Label();
            }
            for (int i = 0; i < m_Static_2.Length; i++)
            {
                m_Static_2[i] = new Label();
            }
            for (int i = 0; i < m_Static_3.Length; i++)
            {
                m_Static_3[i] = new Label();
            }
            for (int i = 0; i < m_Static_4.Length; i++)
            {
                m_Static_4[i] = new Label();
            }
            for (int i = 0; i < m_Static_5.Length; i++)
            {
                m_Static_5[i] = new Label();
            }
            for (int i = 0; i < m_Static_6.Length; i++)
            {
                m_Static_6[i] = new Label();
            }


            cmd = obj;
            cmd_backup = new ScriptCommand();
            cmd_backup.Clone(obj);
            m_DlgAlign.Reset(90,20,200,20);
            OnInitDialog();

        }
        int CalStrWidth(string str)
        {
            return 100;
        }
        void Create_Static(Label lpControl,string Caption,int ID = 0,long Flag = 0)
        {
	        int TextWidth=CalStrWidth(Caption)+m_DefPadding;
            Rectangle rect = m_DlgAlign.GetRect(TextWidth, m_DlgAlign.GetHeightDef(), 0, 3);
            lpControl.Text = Caption;
            
            lpControl.Location = new Point(rect.X, rect.Y);
            lpControl.Size = new Size(rect.Width, rect.Height);
            this.Controls.Add(lpControl);
	        m_DlgAlign.SetLastWidth(TextWidth);
	        m_DlgAlign.MoveToRight(false);
	        m_DlgAlign.SetLastHeight();
        }
        void Create_PictureBox(Label lpControl,int Width,int Height,int ID = 0,long Flag = 0)
        {
            Rectangle rect = m_DlgAlign.GetRect(Width,Height, 0, 3);
	        m_DlgAlign.SetLastWidth(Width);
	        m_DlgAlign.MoveToRight(false);
	        m_DlgAlign.SetLastHeight(Height);
        }
        void Create_Button(Button lpControl,string Caption,int ID = 0,long Flag = 0)
        {
	        int TextWidth=CalStrWidth(Caption)+m_DefPadding;
	        TextWidth+=12;
            Rectangle rect = m_DlgAlign.GetRect(TextWidth, m_DlgAlign.GetHeightDef());
            lpControl.Text = Caption;

            lpControl.Location = new Point(rect.X, rect.Y);
            lpControl.Size = new Size(rect.Width, rect.Height);
            this.Controls.Add(lpControl);
	        m_DlgAlign.SetLastWidth(TextWidth);
	        m_DlgAlign.MoveToRight();
	        m_DlgAlign.SetLastHeight();
        }
        int GetDigits(int Num)
        {
	        int t=1;
	        for (int i=1;i<10;++i) {
		        t*=10;
		        if (Num<t) return i;
	        }
	        return 10;
        }
        void Create_CheckBox(CheckBox lpControl, string Caption, int ID = 0, long Flag = 0)
        {
	        int TextWidth=CalStrWidth(Caption)+m_DefPadding;
	        TextWidth+=12;
            Rectangle rect = m_DlgAlign.GetRect(TextWidth, m_DlgAlign.GetHeightDef());
            lpControl.Text = Caption;

            lpControl.Location = new Point(rect.X, rect.Y);
            lpControl.Size = new Size(rect.Width, rect.Height);
            this.Controls.Add(lpControl);
	        m_DlgAlign.SetLastWidth(TextWidth);
	        m_DlgAlign.MoveToRight();
	        m_DlgAlign.SetLastHeight();
        }
        
        void Create_TextBox(TextBox lpControl,int MaxVal,int ID = 0,long Flag = 0)
        {
            int TextWidth = GetDigits(MaxVal)*m_DefCharWidth+m_DefPadding*2;
            Rectangle rect = m_DlgAlign.GetRect(TextWidth, m_DlgAlign.GetHeightDef());

            lpControl.Location = new Point(rect.X, rect.Y);
            lpControl.Size = new Size(rect.Width, rect.Height);
            this.Controls.Add(lpControl);
	        m_DlgAlign.SetLastWidth(TextWidth);
	        m_DlgAlign.MoveToRight();
	        m_DlgAlign.SetLastHeight();
        }

        void Create_StrBox(TextBox lpControl, int LineCharNum, int LineNum, int ID = 0, long Flag = 0)
        {
	        //if (LineNum>1) Flag|=ES_MULTILINE;
            int TextWidth =  (LineCharNum) * m_DefCharWidth + m_DefPadding * 2;
            Rectangle rect = m_DlgAlign.GetRect(TextWidth, m_DlgAlign.GetHeightDef()+(LineNum - 1)*12);

            lpControl.Location = new Point(rect.X, rect.Y);
            lpControl.Multiline = true;
            lpControl.Size = new Size(rect.Width, rect.Height);
            this.Controls.Add(lpControl);
	        m_DlgAlign.SetLastWidth(TextWidth);
	        m_DlgAlign.MoveToRight();
	        m_DlgAlign.SetLastHeight(m_DlgAlign.GetHeightDef()+(LineNum-1)*12);
        }

        void Create_ComboBox(ComboBox lpControl,int MaxCharNum,int ID = 0,long Flag = 0)
        {
            int TextWidth = 100;//(MaxCharNum)?(static_cast<int>(MaxCharNum)*m_DefCharWidth+27):m_DlgAlign.GetWidthDef();

            Rectangle rect = m_DlgAlign.GetRect(TextWidth, m_DlgAlign.GetHeightDef()*20);

            lpControl.Location = new Point(rect.X, rect.Y);
            lpControl.Size = new Size(rect.Width, rect.Height);
            this.Controls.Add(lpControl);
  	        m_DlgAlign.SetLastWidth(TextWidth);
	        m_DlgAlign.MoveToRight();
	        m_DlgAlign.SetLastHeight();
        }
        void CreateCodeSelBox(int Index)
        {
	        string[] temp1 = new string[2];
            string temp;
            string[] lpCaption={("true变量 "),("false变量")};

	        int i,Num1,Num2;
            ushort[] lpData1 = (ushort[])cmd.Values[0];
            ushort[] lpData2 = (ushort[])cmd.Values[1];
            Num1=lpData1.Length;
            Num2=lpData2.Length;
 
	        if (Num1>0) {
		        temp1[0]=String.Format(("{0:G}"),lpData1[0]);
		        for (i=1;i<Num1;i++) {
			        temp = String.Format((",{0:G}"),lpData1[i]);
			        temp1[0]+=temp;
		        }
	        }
	        if (Num2>0) {
                temp1[1] = String.Format(("{0:G}"), lpData2[0]);
		        for (i=1;i<Num2;i++) {
                    temp = String.Format((",{0:G}"), lpData2[i]);
			        temp1[1]+=temp;
		        }
	        }

            
            int LineCharNum=52;
	        int LineNum=1;
            for (i = 0; i < 2; ++i)
            {
                Create_Static(m_Static_6[Index * 2 + i], lpCaption[i], 0, 0);
                m_CodeSelBox[Index * 2 + i].InitData(cmd, i, "");
                m_CodeSelBox[Index * 2 + i].Text = temp1[i];
                Create_StrBox(m_CodeSelBox[Index * 2 + i], LineCharNum, LineNum, 0, 0);
                //m_StrBoxNum++;
                m_DlgAlign.MoveToNextLine();
            }
        }
        void CreateStatic(Label lpStatic,int ParaIndex,string lpStr)
        {
	        string lpCaption;
	        if (lpStr==null) {
		        lpCaption=CScenario.GetParaName((int)cmd.Keys[ParaIndex],cmd.id,ParaIndex);
	        } else {
		        lpCaption=lpStr;
	        }
	        Create_Static(lpStatic,lpCaption);
        }

        void CreateCheckBox(int Index, int ParaIndex, string lpStr)
        {
            //m_CheckBox[Index].SetDefValue(CScenario.GetStepPara_Int(m_StepIndex,ParaIndex));
            //m_CheckBox[Index].SetDataInfo(m_Scenario,m_StepIndex,ParaIndex);
            bool Checked = (ushort)cmd.Values[ParaIndex] == 1 ? true : false;
            m_CheckBox[Index].InitData(cmd, ParaIndex, Checked);

            string lpCaption;
            if (lpStr == null)
            {
                lpCaption = CScenario.GetParaName((int)cmd.Keys[ParaIndex]);
            }
            else
            {
                lpCaption = lpStr;
            }
            Create_CheckBox(m_CheckBox[Index], lpCaption,0);
        }

        void CreateComboBox(int Index,int ParaIndex,string lpStr,int MaxCharNum = 0)
        {
	        if (ParaIndex!=(-1)) {
		        ParaTypeInfo TypeInfo = CScenario.GetParaTypeInfo((int)cmd.Keys[ParaIndex]);
                //m_ComboBox[Index].SelectedIndex = (ushort)cmd.Values[ParaIndex];
		        //m_ComboBox[Index].SetDefValue(CScenario.GetStepPara_Int(m_StepIndex,ParaIndex));
		        //m_ComboBox[Index].SetDataInfo(m_Scenario,m_StepIndex,ParaIndex);
                m_ComboBox[Index].InitData(cmd, ParaIndex, int.Parse(cmd.Values[ParaIndex].ToString()));
                m_ComboBox[Index].N_paraIndex = ParaIndex;
		        MaxCharNum=TypeInfo.MaxNameLen;
	        }
	        MaxCharNum=(4>MaxCharNum?4:MaxCharNum);

	        CreateStatic(m_Static_2[Index],ParaIndex,lpStr);
	        int Left=m_DlgAlign.GetLastLeft();
	        Create_ComboBox(m_ComboBox[Index],MaxCharNum,0);
	        m_DlgAlign.SetLastLeft(Left);
        }

        void CreateTextBox(int Index,int ParaIndex,long LBound,long UBound,bool IsAllowNULL,string lpStr)
        {
	        //m_TextBox[Index].SetDataInfo(m_Scenario,m_StepIndex,ParaIndex);
            int defvalue;
	        if (UBound>=0x10000) {
                defvalue = (int)cmd.values[ParaIndex];
	        } else {
                defvalue = (ushort)cmd.values[ParaIndex];
	        }
            m_TextBox[Index].InitData(cmd,ParaIndex,defvalue,LBound,UBound);
	        m_TextBox[Index].SetIsAllowNULL(IsAllowNULL);

	        CreateStatic(m_Static_1[Index],ParaIndex,lpStr);
	        int Left=m_DlgAlign.GetLastLeft();
	        Create_TextBox(m_TextBox[Index],(UBound>=0x10000)?1000000000:65535,0);
	        m_DlgAlign.SetLastLeft(Left);
        }

        void CreateStrBox(int Index,int ParaIndex,string lpStr)
        {
	        const int LineCharNum=40;
	        const int LineNum=8;

	        CreateStatic(m_Static_3[Index],ParaIndex,lpStr);
	        string lpData=(string)cmd.values[ParaIndex];
            m_StrBox[Index].InitData(cmd, ParaIndex, lpData.Replace("\n", "\r\n"));
	        //m_StrBox[Index].SetDataInfo(m_Scenario,m_StepIndex,ParaIndex);
 	        Create_StrBox(m_StrBox[Index],LineCharNum,LineNum,0);
        }

        void CreateListBox(int Index,int ParaIndex,string lpStr,int MaxCharNum)
        {
	        int TextWidth=m_DlgAlign.GetWidthDef();
	        int LineNum=10;
            TextWidth = m_DefCharWidth * (4 > MaxCharNum ? 4 : MaxCharNum) + 27;
            //m_ListBox[Index].Items.Add();

            Rectangle rect = m_DlgAlign.GetRect(TextWidth, m_DlgAlign.GetHeightDef() * LineNum);
            ListBox lpControl = m_ListBox[Index];
            lpControl.Location = new Point(rect.X, rect.Y);
            lpControl.Size = new Size(rect.Width, rect.Height);
            //lpControl.SelectedIndexChanged += new EventHandler(CCodeEditListBox.OnSelectedIndexChanged);
            //newLabel.Click += new EventHandler(this.lblMain_Click);
            this.Controls.Add(lpControl);
            //m_ListBox
	        //m_ListBox[Index].CreateEx(WS_EX_CLIENTEDGE,("ListBox"),(""),WS_CHILD | WS_VISIBLE | WS_VSCROLL | WSABSTOP | LBS_NOTIFY | LBS_WANTKEYBOARDINPUT,m_DlgAlign.GetRect(TextWidth,m_DlgAlign.GetHeightDef()*LineNum),this,IDDef_ListBox+Index);

	       //SetDefFont(&m_ListBox[Index]);
	        m_DlgAlign.SetLastWidth(TextWidth);
	        m_DlgAlign.MoveToRight();
	        m_DlgAlign.SetLastHeight(m_DlgAlign.GetHeightDef()*LineNum);
        }
        void CreatePosBox(int Index,int ParaIndex,string lpStr)
        {
	        //long Pos[2];
	        //CScenario.GetStepPara_Pos(m_StepIndex,ParaIndex,Pos[0],Pos[1]);

	        CreateStatic(m_Static_4[Index],ParaIndex,lpStr);
	        int Left=m_DlgAlign.GetLastLeft();
	        for (int i=0;i<2;++i) {
// 		        m_PosBox[Index*2+i].SetDataInfo(m_Scenario,m_StepIndex,ParaIndex);
// 		        m_PosBox[Index*2+i].SetDefValue(Pos[i],0,0x10000);
// 		        m_PosBox[Index*2+i].SetIsAllowNULL(false);
 		        Create_TextBox(m_PosBox[Index*2+i],1000000000,0);
	        }
	        m_DlgAlign.SetLastLeft(Left);
        }

        void CreateAreaBox(int Index,int ParaIndex,string lpStr)
        {
	        //long Pos[4];
	        //CScenario.GetStepPara_Area(m_StepIndex,ParaIndex,Pos[0],Pos[1],Pos[2],Pos[3]);
	        CreateStatic(m_Static_5[Index*2],ParaIndex,lpStr);
	        int Left=m_DlgAlign.GetLastLeft();
	        for (int i=0;i<4;++i) {
		        if (i==2) {
			        CreateStatic(m_Static_5[Index*2+1],ParaIndex,("-"));
		        }
// 		        m_AreaBox[Index*4+i].SetDataInfo(m_Scenario,m_StepIndex,ParaIndex);
// 		        m_AreaBox[Index*4+i].SetDefValue(Pos[i],0,0x10000);
// 		        m_AreaBox[Index*4+i].SetIsAllowNULL(false);
		        Create_TextBox(m_AreaBox[Index*4+i],1000000000,0);
	        }
	        m_DlgAlign.SetLastLeft(Left);
        }
        int CreateControl_Def(int ParaCode,int i,string lpStr = null)
        {
	        int ControlNum=0;
	        switch (ParaCode)
	        {
	        case 4:
	        case 0x102:
		        CreateTextBox(m_TextBoxNum++,i,0,0x10000,false,lpStr);
		        ControlNum=1;
		        break;
	        case 5:
		        m_DlgAlign.MoveToNextLine();
		        CreateStrBox(m_StrBoxNum++,i,lpStr);
		        m_DlgAlign.MoveToNextLine();
		        break;
	        case 0x26:
		        if (lpStr==null) lpStr=CScenario.GetParameterName_Int(0x26,1,cmd.id,i);
		        CreateCheckBox(m_CheckBoxNum++,i,lpStr);
		        ControlNum=1;
		        break;
	        case 0x100:
		        CreatePosBox(m_PosBoxNum++,i,lpStr);
		        ControlNum=2;
		        break;
	        case 0x101:
		        CreateAreaBox(m_AreaBoxNum++,i,lpStr);
		        ControlNum=4;
		        break;
	        default:
		        {
                    ParaTypeInfo TypeInfo = CScenario.GetParaTypeInfo(ParaCode);
			        if (TypeInfo.IsUseCombo) {
				        CreateComboBox(m_ComboBoxNum++,i,lpStr);
			        } else {
				        CreateTextBox(m_TextBoxNum++,i,0,0xffff,true,lpStr);
			        }
			        ControlNum=1;
		        }
                break;
	        }
	        return ControlNum;
        }
        void ShowComboBox(int Index,bool isShow)
        {
	        if (Index>=m_ComboBoxNum) return;
	        m_ComboBox[Index].Visible = isShow;//.ShowWindow(GetShowCmd(isShow));
            m_Static_2[Index].Visible = isShow;//.ShowWindow(GetShowCmd(isShow));
        }
        void ShowCheckBox(int Index,bool isShow)
        {
	        if (Index>=m_CheckBoxNum) return;
            m_CheckBox[Index].Visible = isShow;//.ShowWindow(GetShowCmd(isShow));
        }
        void ShowTextBox(int Index,bool isShow)
        {
	        if (Index>=m_TextBoxNum) return;
            m_TextBox[Index].Visible = isShow;//.ShowWindow(GetShowCmd(isShow));
            m_Static_1[Index].Visible = isShow;//.ShowWindow(GetShowCmd(isShow));
        }
        void ShowStrBox(int Index,bool isShow)
        {
	        if (Index>=m_StrBoxNum) return;
            m_StrBox[Index].Visible = isShow;//.ShowWindow(GetShowCmd(isShow));
            m_Static_3[Index].Visible = isShow;//.ShowWindow(GetShowCmd(isShow));
        }
        void ShowAreaBox(int Index,bool isShow)
        {
	        if (Index>=m_AreaBoxNum) return;
            m_AreaBox[Index * 4].Visible = isShow;//.ShowWindow(GetShowCmd(isShow));
            m_AreaBox[Index * 4 + 1].Visible = isShow;//.ShowWindow(GetShowCmd(isShow));
            m_AreaBox[Index * 4 + 2].Visible = isShow;//.ShowWindow(GetShowCmd(isShow));
            m_AreaBox[Index * 4 + 3].Visible = isShow;//.ShowWindow(GetShowCmd(isShow));
            m_Static_5[Index * 2].Visible = isShow;//.ShowWindow(GetShowCmd(isShow));
            m_Static_5[Index * 2 + 1].Visible = isShow;//.ShowWindow(GetShowCmd(isShow));
        }
        void ShowPosBox(int Index,bool isShow)
        {
	        if (Index>=m_PosBoxNum) return;
            m_PosBox[Index * 2].Visible = isShow;//.ShowWindow(GetShowCmd(isShow));
            m_PosBox[Index * 2 + 1].Visible = isShow;//.ShowWindow(GetShowCmd(isShow));
            m_Static_4[Index].Visible = isShow;//.ShowWindow(GetShowCmd(isShow));
        }

        public void listChange(int selectindex)
        {
            if (m_ComboBox[0].Items.Count > 0)
                m_ComboBox[0].ReinitData(selectindex);
            //if (m_ComboBox[0].Items.Count > 0)
            //{
            //    if (cmd.values[selectindex + 2].ToString() == "65535")
            //        m_ComboBox[0].Text = "无";
            //    else
            //        m_ComboBox[0].SelectedIndex = (int)cmd.values[selectindex + 2];
                
            //}
        }

        /*void UpdateCodeData()
        {
            int i;
            for (i = 0; i < m_ComboBoxNum; i++)
            {
                if (m_ComboBox[i].IsChange())
                {
                    if (m_ComboBox[i].UpdateParaData()) m_ComboBox[i].SetChange() ;
                    m_ComboBox[i].ClearChange();
                }
            }
            for (i = 0; i < m_CheckBoxNum; i++)
            {
                if (m_CheckBox[i].IsChange())
                {
                    if (m_CheckBox[i].UpdateParaData()) m_IsChange = true;
                    m_CheckBox[i].ClearChange();
                }
            }
            for (i = 0; i < m_TextBoxNum; i++)
            {
                if (m_TextBox[i].GetModify())
                {
                    if (m_TextBox[i].UpdateParaData()) m_IsChange = true;
                    m_TextBox[i].SetModify(FALSE);
                }
            }
            for (i = 0; i < m_StrBoxNum; i++)
            {
                if (m_StrBox[i].GetModify())
                {
                    if (m_StrBox[i].UpdateParaData()) m_IsChange = true;
                    m_StrBox[i].SetModify(FALSE);
                }
            }
            for (i = 0; i < m_PosBoxNum; i++)
            {
                if (m_PosBox[i * 2].GetModify() || m_PosBox[i * 2 + 1].GetModify())
                {
                    if (m_Scenario->SetStepPara_Pos(m_StepIndex, m_PosBox[i * 2].GetParaIndex(),
                        m_PosBox[i * 2].GetParaData(), m_PosBox[i * 2 + 1].GetParaData()))
                    {
                        m_IsChange = true;
                    }
                    m_PosBox[i * 2].SetModify(FALSE);
                    m_PosBox[i * 2 + 1].SetModify(FALSE);
                }
            }
            for (i = 0; i < m_AreaBoxNum; i++)
            {
                if (m_AreaBox[i * 4].GetModify() || m_AreaBox[i * 4 + 1].GetModify() || m_AreaBox[i * 4 + 2].GetModify() || m_AreaBox[i * 4 + 3].GetModify())
                {
                    if (m_Scenario->SetStepPara_Area(m_StepIndex, m_AreaBox[i * 4].GetParaIndex(),
                        m_AreaBox[i * 4].GetParaData(), m_AreaBox[i * 4 + 1].GetParaData(),
                        m_AreaBox[i * 4 + 2].GetParaData(), m_AreaBox[i * 4 + 3].GetParaData()))
                    {
                        m_IsChange = true;
                    }
                    m_AreaBox[i * 4].SetModify(FALSE);
                    m_AreaBox[i * 4 + 1].SetModify(FALSE);
                    m_AreaBox[i * 4 + 2].SetModify(FALSE);
                    m_AreaBox[i * 4 + 3].SetModify(FALSE);
                }
            }
            for (i = 0; i < m_CodeSelBoxNum; i++)
            {
                if (m_CodeSelBox[i * 2].GetModify() || m_CodeSelBox[i * 2 + 1].GetModify())
                {
                    vector<int> lpData1, lpData2;
                    CString str1, str2;
                    LPCTSTR t1, t2;
                    TCHAR* temp;
                    long t;
                    m_CodeSelBox[i * 2].SetModify(FALSE);
                    m_CodeSelBox[i * 2 + 1].SetModify(FALSE);
                    str1 = m_CodeSelBox[i * 2].GetParaData();
                    str2 = m_CodeSelBox[i * 2 + 1].GetParaData();
                    t1 = str1;
                    t2 = str2;
                    t = _tcstol(t1, &temp, 10);
                    if (t1 != temp && t < 0x10000) lpData1.push_back(t);
                    while (t1 != temp && *temp == _T(','))
                    {
                        t1 = temp + 1;
                        if (*t1 == 0) break;
                        t = _tcstol(t1, &temp, 10);
                        if (t1 != temp && t < 0x10000) lpData1.push_back(t);
                    }
                    if (*temp) continue;
                    t = _tcstol(t2, &temp, 10);
                    if (t2 != temp && t < 0x10000) lpData2.push_back(t);
                    while (*temp == _T(','))
                    {
                        t2 = temp + 1;
                        if (*t2 == 0) break;
                        t = _tcstol(t2, &temp, 10);
                        if (t2 != temp && t < 0x10000) lpData2.push_back(t);
                    }
                    if (*temp) continue;

                    if (m_Scenario->SetStepPara_CodeSel(m_StepIndex, lpData1, lpData2))
                    {
                        m_IsChange = true;
                    }
                }
            }
        }*/

        static int CAOCAO_CHARLIMIT = 5;

        public void OnSelChange(int nID)
        {
            m_ComboBox[0].UpdateParaData();
            //if (nID >= IDDef_ListBox && nID < IDDef_ListBox + m_MaxListBox)
            //{
            //    int Index = 0;// nID - IDDef_ListBox;
            //    int t = m_ListBox[Index].SelectedIndex;
            //    if (t == -1) return;
            //    switch (cmd.id)
            //    {
            //        case 0x6:
            //        case 0x4a:
            //            if (Index == 0)
            //            {
            //                //UpdateCodeData();
            //                //ChangeControlParaIndex_Def(0, 0, ((m_Code == 0x6) ? 2 : 1) + t);
            //            }
            //            break;
            //        case 0x46:
            //        case 0x47:
            //            if (Index == 0)
            //            {
            //                //UpdateCodeData();
            //                int i = (m_Code == 0x46) ? 9 : 10;
            //                i *= t;
            //                ChangeControlParaIndex_Def(0, 0, i++);
            //                ChangeControlParaIndex_Def(1, 0, i++);
            //                if (m_Code == 0x47) ChangeControlParaIndex_Def(1, 1, i++);
            //                ChangeControlParaIndex_Def(4, 0, i++);
            //                ChangeControlParaIndex_Def(0, 1, i++);
            //                ChangeControlParaIndex_Def(0, 2, i++);
            //                ChangeControlParaIndex_Def(0, 3, i++);
            //                ChangeControlParaIndex_Def(0, 4, i++);
            //                ChangeControlParaIndex_Def(0, 5, i++);
            //                ChangeControlParaIndex_Def(4, 1, i++);
            //                t = m_ComboBox[4].GetCurSel();
            //                ShowComboBox(5, (t == 3 || t == 5));
            //                ShowPosBox(1, (t == 4 || t == 6));
            //            }
            //            break;
            //    }
            //}
            //if (nID >= IDDef_ComboBox && nID < IDDef_ComboBox + m_MaxComboBox)
            {
            //    int Index = 0;// nID - IDDef_ComboBox;
            //    int t = m_ComboBox[Index].SelectedIndex;
            //    if (t == -1) return;
            //    switch (cmd.id)
            //    {
            //        case 0x27:
            //            if (Index == 0)
            //            {
            //                for (size_t i = 1; i < m_ComboBoxNum; ++i)
            //                {
            //                    ShowComboBox(i, (i == static_cast<size_t>(t) + 1));
            //                }
            //            }
            //            break;
            //        case 0x31:
            //        case 0x4e:
            //        case 0x53:
            //            if (Index == 0)
            //            {
            //                ShowComboBox(1, (t == 0));
            //                ShowAreaBox(0, (t == 1));
            //                ShowComboBox(2, (t == 1));
            //            }
            //            if (m_Code == 0x4e && Index == 3)
            //            {
            //                ShowComboBox(4, (t == 3 || t == 5));
            //                ShowPosBox(0, (t == 4 || t == 6));
            //            }
            //            break;
            //        case 0x32:
            //        case 0x4c:
            //        case 0x55:
            //        case 0x6d:
            //            if (Index == 0)
            //            {
            //                ShowComboBox(1, (t == 0));
            //                ShowTextBox(0, (t == 1));
            //            }
            //            break;
            //        case 0x41:
            //            if (Index == 2) ShowAreaBox(0, t == 1);
            //            break;
            //        case 0x46:
            //        case 0x47:
            //            if (Index == 0)
            //            {
            //                int t1 = m_ListBox[0].GetCurSel();
            //                if (t1 != -1)
            //                {
            //                    m_ListBox[0].InsertString(t1 + 1, m_Scenario->GetParameterName_Int(2, m_ComboBox[Index].GetItemData(t)));
            //                    m_ListBox[0].DeleteString(t1);
            //                    m_ListBox[0].SetItemData(t1, t1);
            //                    m_ListBox[0].SetCurSel(t1);
            //                }
            //            }
            //            else if (Index == 4)
            //            {
            //                ShowComboBox(5, (t == 3 || t == 5));
            //                ShowPosBox(1, (t == 4 || t == 6));
            //            }
            //            break;
            //        case 0x4d:
            //            if (Index == 0)
            //            {
            //                ShowComboBox(1, (t == 0));
            //                ShowTextBox(0, (t == 1));
            //                ShowAreaBox(0, (t == 2));
            //                ShowComboBox(2, (t == 2));
            //            }
            //            break;
            //    }
            //    m_ComboBox[Index].SetChange();
            }
        }
        void OnInitDialog() 
        {
            int i = 0;
            int t;
            int CharLimit = CAOCAO_CHARLIMIT;
            int m_Code = cmd.Id;
            int Num = cmd.Keys.Length;
            int ParaCode;
            switch (m_Code)
            {
                case 5:
                    CreateCodeSelBox(m_CodeSelBoxNum++);
                    break;
                case 6:
                case 0x4a:
                	{
                		if (m_Code==6) 
                            CreateCheckBox(m_CheckBoxNum++,i++,("出场限制"));
                		CreateTextBox(m_TextBoxNum++,i++,0,0x10000+15,false,("人数限制"));
                		m_DlgAlign.MoveToNextLine();
                		t=i;
                		CreateListBox(0,(-1),("出场限制"),10);
                		m_DlgAlign.MoveToNextLine(false,false);
                		m_DlgAlign.MoveTopBack();
                		m_DlgAlign.MoveToRight();
                		int ListBoxWidth=m_DlgAlign.GetLastWidth();
                		for (i=0;i<CharLimit+CharLimit;i++)
                		{
                            m_ListBox[0].Items.Add((i < CharLimit) ? ("强制出场") : ("强制不出场"));
                			//m_ListBox[0].SetItemData(i,i);
                		}
                   
                        m_ListBox[0].SelectedIndex = 0;
                		m_DlgAlign.MoveToNextLine();
                		m_DlgAlign.MoveTopBack();
                
                		m_DlgAlign.SetLastWidth(ListBoxWidth);
                		m_DlgAlign.MoveToRight();
                		ParaCode=cmd.Keys[t];
                		CreateControl_Def(ParaCode,t,("武将"));
                	}
                	break;
                case 0x32:
                case 0x4c:
                case 0x55:
                case 0x6d:
                	CreateControl_Def(0x40,i++);
                	t=m_ComboBox[m_ComboBoxNum-1].SelectedIndex;
                	CreateControl_Def(0x2,i++);
                	m_DlgAlign.MoveLeftBack();
                	ShowComboBox(m_ComboBoxNum-1,(t==0));
                	CreateTextBox(m_TextBoxNum++,i++,0,0x10000+114,false,("BFIndex"));
                	m_DlgAlign.MoveLeftBack();
                	ShowTextBox(m_TextBoxNum-1,(t==1));
                	if (m_Code==0x32 || m_Code==0x55)
                	{
                		m_DlgAlign.MoveToNextLine();
                		CreateControl_Def(0x100,i++);
                		CreateControl_Def(0x2b,i++);
                	}
                	if (m_Code==0x6d) {
                		m_DlgAlign.MoveToNextLine();
                		for (;i<Num;i++)
                		{
                			if (i==5) m_DlgAlign.MoveToNextLine();
                			ParaCode=cmd.Keys[i];
                			CreateControl_Def(ParaCode,i);
                		}
                	}
                	break;
                case 0x46:
                	{
                        ParaTypeInfo TypeInfo = CScenario.GetParaTypeInfo(2);
                		CreateListBox(0,(-1),("友军出场人物"),TypeInfo.MaxNameLen);
                		m_DlgAlign.MoveToNextLine(false,false);
                		m_DlgAlign.MoveTopBack();
                		m_DlgAlign.MoveToRight();
                		int ListBoxWidth=m_DlgAlign.GetLastWidth();
                		for (i=0;i<20;i++)
                		{
                			//t=CScenario.GetStepPara_Int(m_StepIndex,i*9);
                            t = (int)cmd.ArrayValues[i][0];
                			//m_ListBox[0].AddString(CScenario.GetParameterName_Int(0x2,t));
                			//m_ListBox[0].SetItemData(i,i);
                		}
                		m_ListBox[0].SelectedIndex = 0;
                		m_DlgAlign.MoveToNextLine();
                		m_DlgAlign.MoveTopBack();
                		t=0; //in case "potentially uninitialized local variable 't' used"
                		for (i=0;i<Num/20;i++)
                		{
                			m_DlgAlign.SetLastWidth(ListBoxWidth);
                			if (i!=8) m_DlgAlign.MoveToRight();
                			ParaCode=cmd.Keys[i];
                			CreateControl_Def(ParaCode,i);
                			if (i!=7) m_DlgAlign.MoveToNextLine();
                			//if (i==2 || i==5) 
                				//m_DlgAlign.MoveToNextLine();
                			if (i==6) {
                				t=m_ComboBox[m_ComboBoxNum-1].SelectedIndex;
                			} else if (i==7) {
                				m_DlgAlign.MoveLeftBack();
                				ShowComboBox(m_ComboBoxNum-1,(t==3 || t==5));
                			} else if (i==8) {
                				//m_DlgAlign.MoveLeftBack();
                				ShowPosBox(m_PosBoxNum-1,(t==4 || t==6));
                			}
                		}
                	}
                	break;
                	case 0x47:
                		{
                            ParaTypeInfo TypeInfo = CScenario.GetParaTypeInfo(2);
                			CreateListBox(0,(-1),("敌军出场人物"),TypeInfo.MaxNameLen);
                			m_DlgAlign.MoveToNextLine(false,false);
                			m_DlgAlign.MoveTopBack();
                			m_DlgAlign.MoveToRight();
                			int ListBoxWidth=m_DlgAlign.GetLastWidth();
                			for (i=0;i<80;i++)
                			{
                				//t=CScenario.GetStepPara_Int(m_StepIndex,i*10);
                                t = (int)cmd.ArrayValues[i][0];
                                m_ListBox[0].Items.Add(CScenario.GetParameterName_Int(0x2, t));
                				//m_ListBox[0].AddString(CScenario.GetParameterName_Int(0x2,t));
                				//m_ListBox[0].SetItemData(i,i);
                			}
                			m_ListBox[0].SelectedIndex = (0);
                			m_DlgAlign.MoveToNextLine();
                			m_DlgAlign.MoveTopBack();
                			t=0; //in case "potentially uninitialized local variable 't' used"
                			for (i=0;i<Num/80;i++)
                			{
                				m_DlgAlign.SetLastWidth(ListBoxWidth);
                				if (i!=2 && i!=9) m_DlgAlign.MoveToRight();
                				ParaCode=cmd.Keys[i];
                				CreateControl_Def(ParaCode,i);
                				if (i!=1 && i!=8) m_DlgAlign.MoveToNextLine();
                				if (i==7) {
                					t=m_ComboBox[m_ComboBoxNum-1].SelectedIndex;
                				} else if (i==8) {
                					m_DlgAlign.MoveLeftBack();
                					ShowComboBox(m_ComboBoxNum-1,(t==3 || t==5));
                				}
                				else if (i==9) {
                					ShowPosBox(m_PosBoxNum-1,(t==4 || t==6));
                				}
                			}
                		}
                		break;
                	case 0x4b:
                		CreateTextBox(m_TextBoxNum++,i++,0,0x10000+14,false,("BFIndex"));
                		for (;i<Num;i++)
                		{
                			ParaCode=cmd.Keys[i];
                			CreateControl_Def(ParaCode,i);
                		}
                		break;
                case 0x4d:
               	    CreateControl_Def(0x41,i++);
               	    t=m_ComboBox[m_ComboBoxNum-1].SelectedIndex;
               	    m_DlgAlign.MoveToNextLine();
               	    CreateControl_Def(0x2,i++);
               	    m_DlgAlign.MoveLeftBack();
               	    ShowComboBox(m_ComboBoxNum-1,(t==0));
               	    CreateTextBox(m_TextBoxNum++,i++,0,0x10000+114,false,("BFIndex"));
               	    m_DlgAlign.MoveLeftBack();
               	    ShowTextBox(m_TextBoxNum-1,(t==1));
               	    CreateControl_Def(0x101,i++);
               	    CreateControl_Def(0x3,i++);
               	    m_DlgAlign.MoveLeftBack();
               	    ShowAreaBox(m_AreaBoxNum-1,(t==2));
               	    ShowComboBox(m_ComboBoxNum-1,(t==2));
               	    m_DlgAlign.MoveToNextLine();
               	    CreateControl_Def(0x2f,i++);
               	    CreateControl_Def(0x18,i++);
               	    CreateControl_Def(0x30,i++);
               	    m_DlgAlign.MoveToNextLine();
               	    CreateControl_Def(0x4,i++,("-HP"));
               	    CreateControl_Def(0x4,i++,("-MP"));
               	    break;
                case 0x100:
                	{
                        ParaTypeInfo TypeInfo = CScenario.GetParaTypeInfo(0x100);
                		CreateComboBox(0,(-1),("代码"),TypeInfo.MaxNameLen);
                		//m_ComboBox[0].SetScrollRange()
                		for (i=0;i<TypeInfo.NameNum;i++)
                		{
                			//t=CScenario.GetParaTypeNameIndex(0x100,i);
                			//m_ComboBox[0].AddString(CScenario.GetParameterName_Int(0x100,t));
                			//m_ComboBox[0].SetItemData(i,t);
                			//if (int(m_CodeSelect)==t || (m_CodeSelect==-1 && t==5)) m_ComboBox[0].SetCurSel(i);
                		}
                	}
                	break;
                default:
                {
                	int ControlNum=0;
                	t=0; //in case "potentially uninitialized local variable 't' used"
                	for (i=0;i<Num;i++)
                	{
                		ParaCode=cmd.Keys[i];
                		ControlNum+=CreateControl_Def(ParaCode,i);
                		switch (m_Code)
                		{
                		case 0x27:
                			if (i==0) 
                			{
                				m_DlgAlign.MoveToNextLine();
                				t=m_ComboBox[m_ComboBoxNum-1].SelectedIndex;
                			}
                			else
                			{
                				m_DlgAlign.MoveLeftBack();
                				ShowComboBox(m_ComboBoxNum-1,(i==t+1));
                			}
                			break;
                		case 0x31:
                		case 0x4e:
                		case 0x53:
                			//0x31 i==0 || i==1
                			switch (i)
                			{
                				case 0:
                					m_DlgAlign.MoveToNextLine();
                					t=m_ComboBox[m_ComboBoxNum-1].SelectedIndex;
                					break;
                				case 1:
                					ShowComboBox(m_ComboBoxNum-1,(t==0));
                					m_DlgAlign.MoveLeftBack();
                					break;
                				case 2:
                					ShowAreaBox(m_AreaBoxNum-1,(t==1));
                					break;
                				case 3:
                					m_DlgAlign.MoveToNextLine();
                					ShowComboBox(m_ComboBoxNum-1,(t==1));
                					break;
                				case 4:
                					if (m_Code==0x4e)
                						t=m_ComboBox[m_ComboBoxNum-1].SelectedIndex;
                					break;
                				case 5:
                					if (m_Code==0x4e)
                					{
                						m_DlgAlign.MoveLeftBack();
                						ShowComboBox(m_ComboBoxNum-1,(t==3 || t==5));
                					}
                					break;
                				case 6:
                					if (m_Code==0x4e)
                					{
                						m_DlgAlign.MoveLeftBack();
                						ShowPosBox(m_PosBoxNum-1,(t==4 || t==6));
                					}
                					break;
                			}
                			break;
                		case 0x3d:case 0x4f:
                			if (i==2) m_DlgAlign.MoveToNextLine();
                			break;
                		case 0x3e:case 0x48:
                			if (i==0 || i==2 || i==4) m_DlgAlign.MoveToNextLine();
                			break;
                		case 0x41:
                			if (i==2) m_DlgAlign.MoveToNextLine();
                			else if (i==3)
                				t=m_ComboBox[m_ComboBoxNum-1].SelectedIndex;
                			else if (i==4)
                			{
                				ShowAreaBox(m_AreaBoxNum-1,(t==1));
                			}
                			break;
                		case 0x45:
                			if (i==1 || i==3 || i==7) m_DlgAlign.MoveToNextLine();
                			break;
                		case 0x50:
                			if (i==1) m_DlgAlign.MoveToNextLine();
                			break;
                		case 0x58:
                			if (i==3) m_DlgAlign.MoveToNextLine();
                			break;
                		case 0x59:
                			if (i==0 || i==2 || i==4 || i==6) m_DlgAlign.MoveToNextLine();
                			break;
                		case 0x67:
                			if (i==0) m_DlgAlign.MoveToNextLine();
                			break;
                		default:
                			if (ControlNum>=5)
                			{
                				m_DlgAlign.MoveToNextLine();
                				ControlNum=0;
                			}
                            break;
                		}
                	}
                }
                break;
            }
            m_DlgAlign.MoveToNextLine();
            if (m_DlgAlign.GetWidthMax() < 200)
                this.Width = m_DlgAlign.GetWidthMax();
            else
                this.Width = m_DlgAlign.GetWidthMax() + 30;
            this.Height = m_DlgAlign.GetHeightMax() + 80;



            //PlaceOKCancelButton();

            string WinCaption=CScenario.GetCodeName(m_Code);
            
            if (m_Code<0x100) {
                this.Text = ("修改-") + WinCaption;
            } else if (m_Code>=0x100 && m_Code<=0x100) {
                this.Text = "选择代码";
            }
        }
        void PlaceOKCancelButton(/*UINT *IDList,int IDNum,bool isMenu*/)
        {
// 	        int DefButtonW=75;
// 	        LONG SpanW=15;
// 	        int w=0;
// 	        for (int i=0;i<IDNum;++i) {
// 		        CString Caption;
// 		        GetDlgItem(IDList[i])->GetWindowText(Caption);
// 		        w=max(CalStrWidth(Caption),w);
// 	        }
// 	        LONG ButtonW=max(DefButtonW,w+m_DefPadding+12);
// 	        LONG ButtonH=m_DlgAlign.GetHeightDef()+3;
// 	        LONG ButtonT=m_DlgAlign.GetHeightMax()+m_DlgAlign.GetMarginV();
// 	        int WinHeight=ButtonT+ButtonH+m_DlgAlign.GetTopDef();
// 	        int WinWidth=m_DlgAlign.GetWidthMax()+m_DlgAlign.GetLeftDef();
// 	        if (IDNum) {
// 		        LONG ButtonL=(WinWidth-static_cast<int>(ButtonW*IDNum+SpanW*(IDNum-1)))/2;
// 
// 		        CWnd *button;
// 		        for (int i=0;i<IDNum;++i) {
// 			        button=GetDlgItem(IDList[i]);
// 			        SetDefFont(button);
// 			        button->SetWindowPos(NULL,ButtonL+(ButtonW+SpanW)*static_cast<int>(i),ButtonT,ButtonW,ButtonH,SWP_NOZORDER);
// 		        }
// 	        }
// 
// 	        CRect rc(0,0,WinWidth,WinHeight);
// 	        ::AdjustWindowRect(rc,GetStyle(),isMenu);
// 
// 	        SetWindowPos(NULL,0,0,rc.Width(),rc.Height(),SWP_NOMOVE | SWP_NOZORDER);
        }

        private void Cancel_Click(object sender, EventArgs e)
        {
            cmd.Clone(cmd_backup);
        }

        private void Accept_Click(object sender, EventArgs e)
        {
            int i;
            for (i = 0; i < m_ComboBoxNum; i++)
            {
                m_ComboBox[i].UpdateParaData();
            }
            for (i = 0; i < m_CheckBoxNum; i++)
            {
                m_CheckBox[i].UpdateParaData();
            }
            for (i = 0; i < m_TextBoxNum; i++)
            {
                m_TextBox[i].UpdateParaData();
            }
            for (i = 0; i < m_StrBoxNum; i++)
            {
                m_StrBox[i].UpdateParaData();
            }
            //             for (i = 0; i < m_PosBoxNum; i++)
            //             {
            //                 if (m_PosBox[i * 2].GetModify() || m_PosBox[i * 2 + 1].GetModify())
            //                 {
            //                     if (m_Scenario->SetStepPara_Pos(m_StepIndex, m_PosBox[i * 2].GetParaIndex(),
            //                         m_PosBox[i * 2].GetParaData(), m_PosBox[i * 2 + 1].GetParaData()))
            //                     {
            //                         m_IsChange = true;
            //                     }
            //                     m_PosBox[i * 2].SetModify(FALSE);
            //                     m_PosBox[i * 2 + 1].SetModify(FALSE);
            //                 }
            //             }
            //             for (i = 0; i < m_AreaBoxNum; i++)
            //             {
            //                 if (m_AreaBox[i * 4].GetModify() || m_AreaBox[i * 4 + 1].GetModify() || m_AreaBox[i * 4 + 2].GetModify() || m_AreaBox[i * 4 + 3].GetModify())
            //                 {
            //                     if (m_Scenario->SetStepPara_Area(m_StepIndex, m_AreaBox[i * 4].GetParaIndex(),
            //                         m_AreaBox[i * 4].GetParaData(), m_AreaBox[i * 4 + 1].GetParaData(),
            //                         m_AreaBox[i * 4 + 2].GetParaData(), m_AreaBox[i * 4 + 3].GetParaData()))
            //                     {
            //                         m_IsChange = true;
            //                     }
            //                     m_AreaBox[i * 4].SetModify(FALSE);
            //                     m_AreaBox[i * 4 + 1].SetModify(FALSE);
            //                     m_AreaBox[i * 4 + 2].SetModify(FALSE);
            //                     m_AreaBox[i * 4 + 3].SetModify(FALSE);
            //                 }
            //             }
            for (i = 0; i < m_CodeSelBoxNum; i++)
            {
                m_CodeSelBox[2 * i].UpdateParaData();
                m_CodeSelBox[2 * i+1].UpdateParaData();
            }
        }

    }
}
