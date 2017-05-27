using Newtonsoft.Json.Linq;
using TRGameEditor.Scenes;
using TRGameEditor.Tools;
using TRGameEditor.Datas;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Reflection;
using System.Text;
using System.Windows.Forms;
using System.Configuration;

namespace TRGameEditor.Forms
{
    public partial class FormMain : Form
    {
        public FormMain()
        {
            InitializeComponent();
        }

        private List<ScriptRoot> scriptRoot = new List<ScriptRoot>();
        private int m_cur_index = 0;
        private Configuration cfa = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);
        private int selectIndex = 5;

        private TreeNode copyNode = null;
        private TreeNode cutNodeParent = null;
        private bool isCut = false;


        private void FormMain_Load(object sender, EventArgs e)
        {
            ScriptConfig.Instance.loadConfig();

            string version = Assembly.GetExecutingAssembly().GetName().Version.ToString();
            this.Text = "同人游戏制作工具：" + version;
            if (cfa.AppSettings.Settings["pro_path"] != null)
            {
                this.toolStripTextBox1.Text = cfa.AppSettings.Settings["pro_path"].Value;
                GameData.Instance.loadData(this.toolStripTextBox1.Text);
            }
        }

        private void buildTreeView(ScriptRoot root)
        {

            treeViewRoot.BeginUpdate();

            treeViewRoot.Nodes.Clear();
            TreeNode tnRoot = new TreeNode(root.getNodeText());            
            tnRoot.Tag = root;

            foreach (ScriptScene item in root.SceneList)
            {
                TreeNode tnScene = new TreeNode(item.getNodeText());
                tnScene.Tag = item;

                foreach (ScriptSection section in item.SectionList)
                {
                    TreeNode tnSection = new TreeNode(section.getNodeText());
                    tnSection.Tag = section;

                    addScriptCommand(tnSection, section.CommandList);
                    tnSection.Expand();
                    tnScene.Nodes.Add(tnSection);
                }

                tnScene.Expand();
                tnRoot.Nodes.Add(tnScene);
            }

            tnRoot.Expand();
            treeViewRoot.Nodes.Add(tnRoot);
            treeViewRoot.EndUpdate();
            treeViewRoot.ExpandAll();

            foreach (TreeNode nodes in treeViewRoot.Nodes)
            {
                if (nodes.Nodes.Count > 0)
                    ChangeBgColor(nodes);
            }
            
        }

        private void ChangeBgColor(TreeNode node)
        {
            foreach (TreeNode eachNode in node.Nodes)
            {
                if (eachNode.Nodes.Count > 0)
                    ChangeBgColor(eachNode);

                if (eachNode.Level > 3)
                {
                    if (eachNode.Nodes.Count > 0)
                    {
                        eachNode.BackColor = Color.Aqua;                        
                    }
                }
                if (eachNode.Level > 4)
                {
                    if (((ScriptCommand)eachNode.Tag).id != 1)
                    {
                        eachNode.BackColor = Color.Aqua;
                    }
                }
            }
        }      

        private void addScriptCommand(TreeNode parent, List<ScriptCommand> commandList)
        {
            TreeNode curParent = parent;
            foreach (ScriptCommand cmd in commandList)
            {
                TreeNode tnCommand = new TreeNode(CScenario.getNodeText(cmd));
                tnCommand.Tag = cmd;
                if (((ScriptCommand)tnCommand.Tag).id == 1)
                {
                    tnCommand.ForeColor = Color.Red;
                }
                
                curParent.Nodes.Add(tnCommand);

                if (cmd.isExtValueAvailable())
                {
                    curParent = tnCommand;
                }
                else if (cmd.Id == (int)CommandId.CMD_ACTION_END)
                {
                    curParent = curParent.Parent;
                }
            }
        }

        private void FormMain_Paint(object sender, PaintEventArgs e)
        {
            e.Graphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;
        }

        private void MenuItem_setProjPath_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog fbd = new FolderBrowserDialog();
            fbd.Description = "设置项目路径";
            if (fbd.ShowDialog() != System.Windows.Forms.DialogResult.OK)
            {
                return;
            }
            this.toolStripTextBox1.Text = fbd.SelectedPath;
            GameData.Instance.loadData(this.toolStripTextBox1.Text);
            cfa.AppSettings.Settings.Add("pro_path", this.toolStripTextBox1.Text);
            cfa.Save();
        }

        private void treeViewRoot_NodeMouseDoubleClick(object sender, TreeNodeMouseClickEventArgs e)
        {
            if (treeViewRoot.SelectedNode.Level > 2 )
            {
                //treeViewRoot.l
                ScriptCommand cmd = (ScriptCommand)treeViewRoot.SelectedNode.Tag;
                if (cmd.Keys.Length > 0)
                {
                    CCodeEditDlg dlg = new CCodeEditDlg(cmd);
                    if (dlg.ShowDialog() == DialogResult.OK)
                    {
                        treeViewRoot.SelectedNode.Text = CScenario.getNodeText(cmd);
                    }
                }
            }
        }

        private void treeViewRoot_NodeMouseClick(object sender, TreeNodeMouseClickEventArgs e)
        {

            TreeView tn = (TreeView)sender;
            if (e.Button == MouseButtons.Right)
            {
                treeViewRoot.SelectedNode = e.Node;
            } 

        }

        private void 新建NToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ScriptRoot sr = new ScriptRoot(null);
            sr.addScene(0);
            scriptRoot.Add(sr);
            buildTreeView(sr);
            TabPage Page = new TabPage();
            Page.Name = "Page1";
            Page.Text = sr.getNodeText();
            tabControl1.Controls.Add(Page);
            tabControl1.SelectedIndex = tabControl1.Controls.Count - 1;
            
        }

        private void 打开OToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Title = "打开剧本文件";
            ofd.Filter = "eex new剧本文件(*.eex_new)|*.eex_new";
            ofd.Multiselect = true;
            if (ofd.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                ScriptRoot sr = null;

                for (int i = 0; i < ofd.FileNames.Length; i++)
                {
                    bool isExist = false;
                    for (int j = 0; j < scriptRoot.Count; j++)
                    {
                        if (scriptRoot[j].m_path == ofd.FileNames[i])
                        {
                            isExist = true;
                            sr = scriptRoot[j];
                            m_cur_index = j;
                            tabControl1.SelectTab(m_cur_index);
                            break;
                        }
                    }

                    if (!isExist)
                    {
                        try
                        {
                            TabPage Page = new TabPage();
                            Page.Name = "Page1";
                            Page.Text = ofd.FileNames[i].Substring(ofd.FileNames[i].LastIndexOf("\\") + 1, ofd.FileNames[i].Length - 1 - ofd.FileNames[i].LastIndexOf("\\"));
                            Page.BackColor = Color.White;
                            tabControl1.Controls.Add(Page);
                            sr = EexFile.loadFile(ofd.FileNames[i]);
                            scriptRoot.Add(sr);
                            tabControl1.SelectedIndex = tabControl1.Controls.Count - 1;
                        }
                        catch (EexReaderException exception)
                        {
                            MessageBox.Show(this, String.Format("文件{0}，解析指令0x{1:x}出错，内部信息为:{2}", ofd.FileName[i], exception.CommandId, exception.Message));
                        }  
                    }
                }
                buildTreeView(sr);
            }
        }

        private void 保存SToolStripMenuItem_Click(object sender, EventArgs e)
        {

            ScriptRoot sr = scriptRoot[m_cur_index];
            if (sr == null)
            {
                MessageBox.Show(this, "未加载，无法保存");
                return;
            }
            if(sr.m_path==null)
            {
                SaveFileDialog sfd = new SaveFileDialog();
                sfd.Title = "保存剧本文件";
                sfd.Filter = "eex new剧本文件(*.eex_new)|*.eex_new|旧版本eex剧本文件(*.eex)|*.eex|新版本eex剧本文件(*.eex)|*.eex|eex json剧本文件(*.eexjs)|*.eexjs";
                if (sfd.ShowDialog() == System.Windows.Forms.DialogResult.OK)
                {

                    EexFile.saveFile(sr, sfd.FileName);
                }
            }
            else
                EexFile.saveFile(sr, sr.m_path);
         
        }

        private void 另存为AToolStripMenuItem_Click(object sender, EventArgs e)
        {
                ScriptRoot sr = scriptRoot[m_cur_index];
                if (sr == null)
                {
                    MessageBox.Show(this, "未加载，无法保存");
                    return;
                }
                SaveFileDialog sfd = new SaveFileDialog();
                sfd.Title = "保存剧本文件";
                sfd.Filter = "eex new剧本文件(*.eex_new)|*.eex_new";
                if (sfd.ShowDialog() == System.Windows.Forms.DialogResult.OK)
                {

                    EexFile.saveFile(sr, sfd.FileName);
                }
        }

        private void 退出XToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //调用关闭全部

            Close();
        }

        private void 关于AToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Formhelp form = new Formhelp();
            form.Show();
        }

        private void tabControl1_SelectedIndexChanged(object sender, EventArgs e)
        {
            m_cur_index = tabControl1.SelectedIndex;
            buildTreeView(scriptRoot[m_cur_index]);
        }

        private void 修改ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (treeViewRoot.SelectedNode.Level > 2)
            {
                ScriptCommand cmd = (ScriptCommand)treeViewRoot.SelectedNode.Tag;
                if (cmd.Keys.Length > 0)
                {
                    CCodeEditDlg dlg = new CCodeEditDlg(cmd);
                    if (dlg.ShowDialog() == DialogResult.OK)
                    {
                        treeViewRoot.SelectedNode.Text = CScenario.getNodeText(cmd);
                    }
                }
            }
        }

        private void 添加ToolStripMenuItem_Click(object sender, EventArgs e)
        {

            if (treeViewRoot.SelectedNode.Level == 1)
            {

                scriptRoot[m_cur_index].addScene((ScriptScene)treeViewRoot.SelectedNode.Tag);

            }
            else if (treeViewRoot.SelectedNode.Level == 2)
            {
                ScriptScene scene = (ScriptScene)treeViewRoot.SelectedNode.Parent.Tag;
                scene.addSection((ScriptSection)treeViewRoot.SelectedNode.Tag);
            }
            else if (treeViewRoot.SelectedNode.Level == 3) {
                MessageBox.Show("不能在此处添加", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            else if (treeViewRoot.SelectedNode.Level > 3)
            {
                FormAddCode addform = new FormAddCode(0, selectIndex);
                if (addform.ShowDialog() == DialogResult.OK)
                {
                    selectIndex = addform.m_select + 1;
                    if (addform.m_select < 4) {
                        MessageBox.Show("code 0,1,2,3不能被手动添加","提示",MessageBoxButtons.OK,MessageBoxIcon.Error);
                        return;
                    }
                    ScriptSection sec = (ScriptSection)getLeve2Nodes(treeViewRoot.SelectedNode).Tag;
                    sec.addCode(addform.m_select, (ScriptCommand)treeViewRoot.SelectedNode.Parent.Tag);
                }

            }
            buildTreeView(scriptRoot[m_cur_index]);
        }

        private void 添加子事件ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var node = treeViewRoot.SelectedNode;
            if (node.Level < 4) {
                MessageBox.Show("不能在这里添加子事件!","提示",MessageBoxButtons.OK,MessageBoxIcon.Error);
                return;
            }
            

            FormAddCode addform = new FormAddCode();
            if (addform.ShowDialog() == DialogResult.OK)
            {
                if (addform.m_select < 4)
                {
                    MessageBox.Show("code 0,1,2,3不能被手动添加", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }

                ScriptSection sec = (ScriptSection)getLeve2Nodes(node).Tag;
                ScriptCommand newCmd = (ScriptCommand)node.Parent.Tag;
                newCmd.extValue = 2;
                sec.addCode(0, newCmd);
                sec.addCode(addform.m_select, newCmd, true);
                sec.addCode(1, newCmd);
                buildTreeView(scriptRoot[m_cur_index]);
            }
        }

        protected TreeNode getLeve2Nodes(TreeNode node)
        {
            if (node.Parent.Level > 2)
                return getLeve2Nodes(node.Parent);
            else
                return node.Parent;
        }

        private void 删除ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ScriptSection sec = null;
            ScriptCommand n_cmd = null;
            var node = treeViewRoot.SelectedNode;
            DialogResult dr;

            if (node.Level <= 2)
            {
                if (node.PrevNode == null && node.NextNode == null)
                {
                    MessageBox.Show("唯一的Scetion或Scene不能删除", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }
                else
                {
                    dr= MessageBox.Show("确定要删除这条数据吗？", "提示", MessageBoxButtons.OKCancel, MessageBoxIcon.Question);
                    if (dr == DialogResult.OK)
                    {
                        if (node.Level == 1)
                        {
                            scriptRoot[m_cur_index].removeScene((ScriptScene)node.Tag);  
                        }
                        else
                        {
                            ScriptScene scene = (ScriptScene)node.Parent.Tag;
                            scene.removeSection((ScriptSection)node.Tag);
                        }
                        treeViewRoot.Nodes.Remove(node);
                    }
                    return;
                }
            }
            else if (node.Level == 3)
            {
                MessageBox.Show("不能被删除", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            else {
                sec = (ScriptSection)getLeve2Nodes(node).Tag;
                n_cmd = (ScriptCommand)node.Tag;
            }
            dr = MessageBox.Show("确定要删除这条数据吗？", "提示", MessageBoxButtons.OKCancel, MessageBoxIcon.Question);
            if (dr == DialogResult.OK)
            {
                if (node.Nodes.Count < 1)
                    sec.removeCode(n_cmd);
                else
                {
                    RemoveCode(sec, node);
                }
                node.Remove();
            }
        }

        protected void RemoveCode(ScriptSection sec,TreeNode nodes)
        {
            foreach (TreeNode node in nodes.Nodes)
            {
                if (node.Nodes.Count > 0)
                    RemoveCode(sec, node);
                sec.removeCode((ScriptCommand)node.Tag);
            }
            sec.removeCode((ScriptCommand)nodes.Tag);
        }

        private void 剪切ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ScriptSection sec = null;
            ScriptCommand n_cmd = null;
            var node = treeViewRoot.SelectedNode;

            if (node.Level <= 2)
            {
                if (node.PrevNode == null && node.NextNode == null)
                {
                    MessageBox.Show("唯一的Scetion或Scene不能删除", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }
                else
                {                                   
                    if (node.Level == 1)
                    {
                        scriptRoot[m_cur_index].removeScene((ScriptScene)node.Tag);
                    }
                    else
                    {
                        ScriptScene scene = (ScriptScene)node.Parent.Tag;
                        scene.removeSection((ScriptSection)node.Tag);
                    }
                }
            }
            else if (node.Level == 3)
            {
                MessageBox.Show("不能被删除", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            else
            {
                sec = (ScriptSection)node.Parent.Parent.Tag;
                n_cmd = (ScriptCommand)node.Tag;
                sec.removeCode(n_cmd);                
            }

            copyNode = new TreeNode();
            copyNode.Text = node.Text;
            copyNode.Tag = node.Tag;
            copyNode = CopyTreeNode(copyNode, node);
            cutNodeParent = node.Parent;
            粘帖ToolStripMenuItem.Enabled = true;
            node.Remove();
            isCut = true;
        }

        TreeNode CopyTreeNode(TreeNode tree1, TreeNode tree2)
        {
            TreeNode tempNode;
            foreach (TreeNode n in tree2.Nodes)
            {
                tempNode = (TreeNode)n.Clone();
                tempNode.Tag = n.Tag;
                tempNode.Nodes.Clear();
                if (n.Nodes.Count > 0)
                {
                    CopyTreeNode(tempNode, n);
                }
                tree1.Nodes.Add(tempNode);
            }

            return tree1;
        }

        private void 复制ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            copyNode = treeViewRoot.SelectedNode;
            粘帖ToolStripMenuItem.Enabled = true;
            isCut = false;
        }

        private void 粘帖ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (copyNode != null && treeViewRoot.SelectedNode != null)
            {
                if (copyNode.Level != treeViewRoot.SelectedNode.Level)
                {
                    if (cutNodeParent != null && cutNodeParent.Level != treeViewRoot.SelectedNode.Parent.Level)
                    {
                        MessageBox.Show("粘贴目录不匹配", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        return;
                    }
                }

                if (isCut)
                {
                    cutNodeParent.Nodes.Add(copyNode);
                }

                if (treeViewRoot.SelectedNode.Level == 1)
                {
                    ScriptScene scene = scriptRoot[m_cur_index].addScene((ScriptScene)copyNode.Tag);
                    scene.SectionList = ((ScriptScene)copyNode.Tag).SectionList;
                }
                else if (treeViewRoot.SelectedNode.Level == 2)
                {
                    ScriptScene scene = (ScriptScene)(copyNode.Parent != null ? copyNode.Parent.Tag : cutNodeParent.Tag);
                    ScriptSection section = scene.addSection((ScriptSection)copyNode.Tag);
                    section.CommandList = ((ScriptSection)copyNode.Tag).CommandList;
                }
                else if (treeViewRoot.SelectedNode.Level == 3)
                {
                    MessageBox.Show("不能在此处复制", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }
                else if (treeViewRoot.SelectedNode.Level > 3)
                {
                    ScriptCommand copy_cmd = (ScriptCommand)copyNode.Tag;
                    if (copy_cmd.id != 0)
                    {
                        ScriptSection sec = (ScriptSection)getLeve2Nodes(copyNode).Tag;
                        if (copyNode.Nodes.Count > 0)
                        {
                            //sec.addCode(copy_cmd.id, (ScriptCommand)copyNode.Parent.Tag, true);
                            CopyNode(sec, copyNode);
                        }
                        else
                            sec.addCode(copy_cmd.id, (ScriptCommand)copyNode.Tag);


                        //int copy_index = sec.FindCommand((ScriptCommand)copyNode.Parent.Tag);
                        //int index = sec.addCode(copy_cmd.id, (ScriptCommand)copyNode.Parent.Tag, copy_cmd.isExtValueAvailable());
                        //sec.CommandList[index] = sec.CommandList[copy_index];
                    }
                    else
                    {
                        MessageBox.Show("code 0,1,2,3不能被手动添加", "提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        return;
                    }
                }
                buildTreeView(scriptRoot[m_cur_index]);

            }
        }

        void CopyNode(ScriptSection sec, TreeNode node, int level = 1)
        {
            ScriptCommand copy_cmd = new ScriptCommand();
            foreach (TreeNode eachNode in node.Nodes)
            {
                if (eachNode.Index == 0)
                {
                    copy_cmd = (ScriptCommand)eachNode.Parent.Tag;
                    sec.addCode(copy_cmd.id, (ScriptCommand)eachNode.Parent.Tag, true);
                    sec.addCode(0, copy_cmd);
                }
                else
                {
                    copy_cmd = (ScriptCommand)eachNode.Tag;
                    sec.addCode(copy_cmd.id, (ScriptCommand)eachNode.Parent.Tag);
                }
                if (eachNode.Nodes.Count > 0)
                    CopyNode(sec, eachNode);
            }
        }

        private void 全部展开ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            treeViewRoot.BeginUpdate();
            treeViewRoot.Nodes[0].ExpandAll();
            treeViewRoot.EndUpdate();
        }

        private void contextMenuStrip1_Opening(object sender, CancelEventArgs e)
        {
            if (treeViewRoot.Nodes.Count == 0 || treeViewRoot.SelectedNode == null)
            {
                contextMenuStrip1.Visible = false;
            }
            else
            {
                if (treeViewRoot.SelectedNode.Level == 0)
                {
                    for (int i = 0; i < contextMenuStrip1.Items.Count; i++)
                    {
                        if (i != contextMenuStrip1.Items.Count - 1)
                        {
                            contextMenuStrip1.Items[i].Enabled = false;
                        }
                    }
                }
                else if (treeViewRoot.SelectedNode.Level == 3)
                {
                    复制ToolStripMenuItem.Enabled = false;
                }
                else
                {
                    foreach (ToolStripItem item in contextMenuStrip1.Items)
                    {
                        if (item == 粘帖ToolStripMenuItem && copyNode == null)
                            continue;
                        item.Enabled = true;                        
                    }
                }
            }
        }
    }
}
