using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using TRGameEditor.Scenes;

namespace TRGameEditor.Forms
{
    public class CCodeEditListBox : ListBox
    {
        ScriptCommand m_cmd;
        private int valueIndex;
        public int ValueIndex {
            get { return valueIndex; }
            set { valueIndex = value; }
        }
        public void InitData(ScriptCommand cmd,int valindex,ComboBox ccBox)
        {
            m_cmd = cmd;            
        }

        protected override void OnSelectedIndexChanged(System.EventArgs e)
        {
            ((CCodeEditDlg)Parent).listChange(this.SelectedIndex);
        }
    }
    public class CCodeEditCheckBox : CheckBox
    {

        // 属性
        public bool IsChange() { return m_IsChange; }
        public int GetParaData()
        {
            return 0;
        }
        public void SetChange() { m_IsChange = true; }
        public void ClearChange() { m_IsChange = false; }

        public void InitData(ScriptCommand cmd, int paraIndex, bool DefValue)
        {
            m_cmd = cmd;
            m_paraIndex = paraIndex;
            Checked = DefValue;
        }
        public void UpdateParaData()
        {
            m_cmd.values[m_paraIndex] = (ushort)(Checked ? 1 : 0);
        }

        private void ClearData()
        {
            m_IsChange = false;
        }


        private bool m_IsChange;

        ScriptCommand m_cmd;
        int m_paraIndex;
    };

    class CCodeEditComboBox : ComboBox
    {
        int n_paraIndex;
        public int N_paraIndex {
            get { return n_paraIndex; }
            set { n_paraIndex = value; }
        }
        // 属性
        public bool IsChange(){return m_IsChange;}
	    int GetParaData()
        {
            return 0;
        }

	    public void SetChange() {m_IsChange=true;}
	    public void ClearChange() {m_IsChange=false;}

        public void ReinitData(int index)
        {
            InitData(m_cmd, n_paraIndex + index, int.Parse(m_cmd.values[n_paraIndex + index].ToString()));
        }

        public void InitData(ScriptCommand cmd, int paraIndex, int DefValue)
        {
            m_cmd = cmd;
            m_paraIndex = paraIndex;            
            m_DefValue = DefValue;

            ParaTypeInfo TypeInfo;
            int Code = cmd.id;
            int ParaCode = cmd.keys[paraIndex];

            TypeInfo = CScenario.GetParaTypeInfo(ParaCode);
            Items.Clear();
            if (TypeInfo.IsUseCombo)
            {
                int t, i, it = -1, n = TypeInfo.NameNum;
                if ((Code != 0x6 && Code != 0x4a && Code != 0x46 && Code != 0x47) && (ParaCode == 2 || ParaCode == 0x38 || ParaCode == 0x39))
                {
                    int k = 0;
                    this.ResetText();//?
                    for (i = 0; i < n; i++)
                    {
                        t = CScenario.GetParaTypeNameIndex(ParaCode, i);
                        if (t < 0 || t >= 0x400 || m_DefValue == t || (t < 0x400 /*&& CScenario.gIsShowChar[t]*/))
                        {
                            
                            Items.Add(CScenario.GetParameterName_Int(ParaCode, t));
                            
                            //AddString(CScenario.GetParameterName_Int(ParaCode, t));
                            //SetItemData(k, t);
                            if (m_DefValue == t) it = k;
                            k++;
                        }
                    }
                }
                else
                {
                    for (i = 0; i < n; i++)
                    {
                        //if (!m_IsInit)
                        {
                            t = CScenario.GetParaTypeNameIndex(ParaCode, i);
                            Items.Add(CScenario.GetParameterName_Int(ParaCode, t));
                            //AddString(m_Scenario->GetParameterName_Int(ParaCode, t));
                            //SetItemData(i, t);
                        }
                        //else
                        //{
                            //t = GetItemData(i);
                        //}
                        if (m_DefValue == t) it = i;
                    }
                }
                if (m_DefValue >= 0 && m_DefValue < Items.Count)
                    SelectedIndex = m_DefValue;
                else if (it != -1)
                {
                    this.SelectedIndex = it;
                }
                else if (TypeInfo.DefIndex != 0x10000)
                {
                    this.SelectedIndex = TypeInfo.DefIndex;
                }
                else
                {
                    this.SelectedIndex = -1;
                }
            }
        }
        public bool UpdateParaData()
        {
            try
            {
                if (this.SelectedIndex != -1)
                    m_cmd.values[m_paraIndex] = SelectedIndex>=512?65535: SelectedIndex;
                return true;
            }
            catch(Exception ex) { throw ex; }
        }
	    void ClearData()
        {

        }


        protected override void OnSelectedIndexChanged(System.EventArgs e)
        {
            if(Parent!=null)
                ((CCodeEditDlg)Parent).OnSelChange(1);
        }


	    bool m_IsChange;
	    int m_DefValue;

        ScriptCommand m_cmd;
        int m_paraIndex;
    };

    public class CCodeEditStrBox :  TextBox
    {
	    string GetParaData()
        {
            return null;
        }

        public void InitData(ScriptCommand cmd, int paraIndex, string DefValue)
        {
            m_cmd = cmd;
            m_paraIndex = paraIndex;
            m_DefValue = DefValue;
            Text = DefValue;
        }
        ushort[] getUShort(string text)
        {

            string[] ss = text.Replace("，",",").Split(',');
            ushort[] dd = new ushort[ss.Length];
            for(int i = 0;i<dd.Length;i++)
            {
                try
                {
                    dd[i] = ushort.Parse(ss[i]);
                }
                catch {
                    continue;
                }
            }
            return dd;
        }
        public void UpdateParaData()
        {
            m_DefValue = Text;
            if (m_cmd.id == 5)
                m_cmd.values[m_paraIndex] = getUShort(Text);
            else
                m_cmd.values[m_paraIndex] = Text;
        }

	    void ClearData()
        {

        }


	    string m_DefValue;
        ScriptCommand m_cmd;
        int m_paraIndex;
    };

    class CCodeEditTextBox : TextBox
    {

	    long GetParaData()
        {
            return 0;
        }

	    public void SetIsAllowNULL(bool IsAllowNULL) 
        {
            m_IsAllowNULL=IsAllowNULL;
        }

        public void InitData(ScriptCommand cmd, int paraIndex, long DefValue, long LBound, long UBound)
        {
            m_cmd = cmd;
            m_paraIndex = paraIndex;
            m_DefValue = DefValue;
		    m_LBound=LBound;
		    m_UBound=UBound;

            if (m_UBound >= 0x10000)
            {
             //   SetLimitText(10);
            }
            else
            {
             //   SetLimitText(5);
            }

            long UBound1 = m_UBound;
            if (UBound1 > 0x10000) UBound1 -= 0x10000;

            if (m_DefValue >= m_LBound && (UBound1 == 0x10000 || m_DefValue <= UBound1) && (m_DefValue != 0xffff || m_IsAllowNULL == false))
            {
                Text = String.Format("{0}",m_DefValue);
            }
            else if (m_DefValue == 0xffff && m_IsAllowNULL)
            {
                Text = (("无"));
            }
            
	    }
        public int UpdateParaData() 
        {
            if (String.IsNullOrEmpty(Text))
                return -1;
            try
            {
                m_DefValue = Convert.ToInt32(Text);
                if (m_UBound >= 0x10000)
                {
                    m_cmd.values[m_paraIndex] = (int)m_DefValue;
                }
                else
                {
                    m_cmd.values[m_paraIndex] = (ushort)m_DefValue;
                }
                return 0;
            }
            catch (Exception ex) {
                throw ex;
            } 
        }
        void ClearData() { }


	    long m_DefValue;
	    long m_LBound,m_UBound;
	    bool m_IsAllowNULL;
        ScriptCommand m_cmd;
        int m_paraIndex;
    };
}
