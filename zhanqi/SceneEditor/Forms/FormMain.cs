using Newtonsoft.Json.Linq;
using SceneEditor.Datas;
using SceneEditor.Tools;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Reflection;
using System.Text;
using System.Windows.Forms;

namespace SceneEditor.Forms
{
    public partial class FormMain : Form
    {
        public FormMain()
        {
            InitializeComponent();
        }

        private ScriptRoot scriptRoot = null;
        private Dictionary<TreeNode, string> treeContent = new Dictionary<TreeNode, string>();

        private void btnLoad_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Title = "打开剧本文件";
            ofd.Filter = "旧版本eex剧本文件(*.eex)|*.eex|新版本eex剧本文件(*.eex)|*.eex|eex new剧本文件(*.eex_new)|*.eex_new|eex json剧本文件(*.eexjs)|*.eexjs";
            ofd.Multiselect = false;
            if (ofd.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                try
                {
                    int group = 0;
                    if (ofd.FilterIndex == 2)
                    {
                        group = 1;
                    }
                    ScriptRoot sr = EexFile.loadFile(ofd.FileName, group);
                    buildTreeView(sr);
                    String readString = sr.toJsonObject().ToString(Newtonsoft.Json.Formatting.Indented);
                    tbContent.Text = readString;
                    scriptRoot = sr;
                }
                catch (EexReaderException exception)
                {
                    MessageBox.Show(this, String.Format("文件{0}，解析指令0x{1:x}出错，内部信息为:{2}", ofd.FileName, exception.CommandId, exception.Message));
                }
               
            }
        }

        private void btnSave_Click(object sender, EventArgs e)
        {
            ScriptRoot sr = scriptRoot;
            if (sr == null)
            {
                MessageBox.Show(this, "json未加载，无法保存");
                return;
            }
            /*
            try
            {
                sr = EexFile.jsonText2ScriptRoot(tbContent.Text);
            }
            catch (Exception)
            {
                MessageBox.Show(this, "json格式错误，无法保存");
                return;
            }
             * */

            buildTreeView(sr);
            String readString = sr.toJsonObject().ToString(Newtonsoft.Json.Formatting.Indented);
            tbContent.Text = readString;
            SaveFileDialog sfd = new SaveFileDialog();
            sfd.Title = "保存剧本文件";
            sfd.Filter = "旧版本eex剧本文件(*.eex)|*.eex|新版本eex剧本文件(*.eex)|*.eex|eex new剧本文件(*.eex_new)|*.eex_new|eex json剧本文件(*.eexjs)|*.eexjs";
            if (sfd.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                int group = 0;
                if (sfd.FilterIndex <= 2)
                {
                    group = sfd.FilterIndex - 1;
                }
                EexFile.saveFile(sr, sfd.FileName, group);
            }
        }

        private void FormMain_Load(object sender, EventArgs e)
        {
            ScriptConfig.Instance.loadConfig();

            string version = Assembly.GetExecutingAssembly().GetName().Version.ToString();
            this.Text = "SceneEditor剧本编辑器：" + version;

            //ScriptRoot sr = EexFile.loadEexFile(@"D:\projects\gametools\SceneEditor\datafile\R_02.eex");
            //buildTreeView(sr);
            //tbContent.Text = EexFile.scriptRoot2JsonText(sr);
            //EexFile.saveEexFile(sr, @"D:\projects\gametools\SceneEditor\datafile\R_02_x_11.eex");
            //EexFile.saveJsonFile(sr, @"D:\projects\gametools\SceneEditor\datafile\R_02_x.eexjs");
            //tbContent.Text = EexFile.scriptRoot2JsonText(sr);
            //String readString = EexFile.loadEexFile(@"D:\projects\gametools\SceneEditor\datafile\R_02_x.eex").toJsonObject().ToString(Newtonsoft.Json.Formatting.Indented);
            //tbResult.Text = readString;
            //ScriptRoot sr = EexFile.loadJsonText(readString);
        }

        private void buildTreeView(ScriptRoot root)
        {
            treeContent.Clear();

            treeViewRoot.BeginUpdate();

            treeViewRoot.Nodes.Clear();
            TreeNode tnRoot = new TreeNode(root.getNodeText());            
            tnRoot.Tag = root;
            treeContent[tnRoot] = root.toJsonObject().ToString(Newtonsoft.Json.Formatting.Indented);

            foreach (ScriptScene item in root.SceneList)
            {
                TreeNode tnScene = new TreeNode(item.getNodeText());
                tnScene.Tag = item;
                treeContent[tnScene] = item.toJsonObject().ToString(Newtonsoft.Json.Formatting.Indented);

                foreach (ScriptSection section in item.SectionList)
                {
                    TreeNode tnSection = new TreeNode(section.getNodeText());
                    tnSection.Tag = section;
                    treeContent[tnSection] = section.toJsonObject().ToString(Newtonsoft.Json.Formatting.Indented);

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
        }

        private void addScriptCommand(TreeNode parent, List<ScriptCommand> commandList)
        {
            TreeNode curParent = parent;
            foreach (ScriptCommand cmd in commandList)
            {
                TreeNode tnCommand = new TreeNode(cmd.getNodeText());
                tnCommand.Tag = cmd;
                treeContent[tnCommand] = cmd.toJsonObject().ToString(Newtonsoft.Json.Formatting.Indented);

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

        private void btnBatch_Click(object sender, EventArgs e)
        {
            FormBatchConvert fbc = new FormBatchConvert();
            fbc.ShowDialog();
        }

        private void treeViewRoot_AfterSelect(object sender, TreeViewEventArgs e)
        {
            string content = "";
            if (treeContent.ContainsKey(e.Node))
            {
                content = treeContent[e.Node];
            }
            tbContent.Text = content;
        }
    }
}
