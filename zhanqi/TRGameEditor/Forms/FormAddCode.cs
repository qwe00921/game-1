using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using TRGameEditor.Tools;

namespace TRGameEditor.Forms
{
    
    public partial class FormAddCode : Form
    {
        private int m_type = 0;
        private int m_index = 0;
        public int m_select = 0;
        public FormAddCode(int type = 0, int index = 0)
        {
            m_type = type;
            m_index = index;
            InitializeComponent();
        }

        private void FormAddCode_Load(object sender, EventArgs e)
        {
            for (int i = 0; i < ScriptConfig.Instance.getCommandNum(); i++)
            {
                comboBox1.Items.Add(String.Format("{0:x0}: {1}", i,ScriptConfig.Instance.getCommandConfig(i).Name));
            }
            if (m_index == 0)
                comboBox1.SelectedIndex = 5;
            else if (m_index >= comboBox1.Items.Count)
                comboBox1.SelectedIndex = comboBox1.Items.Count - 1;
            else
                comboBox1.SelectedIndex = m_index;
        }

        private void 确定_Click(object sender, EventArgs e)
        {
            m_select = comboBox1.SelectedIndex;           
        }
    }
}
