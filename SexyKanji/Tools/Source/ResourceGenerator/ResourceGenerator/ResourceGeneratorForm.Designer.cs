namespace ResourceGenerator
{
    partial class ResourceGeneratorForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.generateButton = new System.Windows.Forms.Button();
            this.mruComboBox = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.newProjectButton = new System.Windows.Forms.Button();
            this.deleteProjectButton = new System.Windows.Forms.Button();
            this.projectNameTextBox = new System.Windows.Forms.TextBox();
            this.resourceXMLTextBox = new System.Windows.Forms.TextBox();
            this.outputDirTextBox = new System.Windows.Forms.TextBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.outputFileTextBox = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.browseResourceXMLButton = new System.Windows.Forms.Button();
            this.browseOutDirButton = new System.Windows.Forms.Button();
            this.consoleOutListBox = new System.Windows.Forms.ListBox();
            this.folderBrowserDialog1 = new System.Windows.Forms.FolderBrowserDialog();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.outnameLabel = new System.Windows.Forms.Label();
            this.functionPrefixTextBox = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
            this.label7 = new System.Windows.Forms.Label();
            this.baseResPathTextBox = new System.Windows.Forms.TextBox();
            this.reportUnusedFilesCheckBox = new System.Windows.Forms.CheckBox();
            this.browseBaseResPathButton = new System.Windows.Forms.Button();
            this.writeDelBatchCheckBox = new System.Windows.Forms.CheckBox();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // generateButton
            // 
            this.generateButton.Location = new System.Drawing.Point(356, 277);
            this.generateButton.Name = "generateButton";
            this.generateButton.Size = new System.Drawing.Size(75, 23);
            this.generateButton.TabIndex = 12;
            this.generateButton.Text = "Generate";
            this.toolTip1.SetToolTip(this.generateButton, "Parse the XML Resource file, generate C++ source files,\r\nreport unused files, sav" +
                    "e project information.\r\n");
            this.generateButton.UseVisualStyleBackColor = true;
            this.generateButton.Click += new System.EventHandler(this.generate_Click);
            // 
            // mruComboBox
            // 
            this.mruComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.mruComboBox.FormattingEnabled = true;
            this.mruComboBox.Location = new System.Drawing.Point(297, 19);
            this.mruComboBox.Name = "mruComboBox";
            this.mruComboBox.Size = new System.Drawing.Size(121, 21);
            this.mruComboBox.TabIndex = 15;
            this.toolTip1.SetToolTip(this.mruComboBox, "List and choose a most recently used project.");
            this.mruComboBox.SelectedIndexChanged += new System.EventHandler(this.mruComboBox_SelectedIndexChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(246, 22);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(54, 13);
            this.label1.TabIndex = 17;
            this.label1.Text = "MRU List:";
            // 
            // newProjectButton
            // 
            this.newProjectButton.Location = new System.Drawing.Point(27, 19);
            this.newProjectButton.Name = "newProjectButton";
            this.newProjectButton.Size = new System.Drawing.Size(75, 23);
            this.newProjectButton.TabIndex = 13;
            this.newProjectButton.Text = "New";
            this.toolTip1.SetToolTip(this.newProjectButton, "Create a new resource project.");
            this.newProjectButton.UseVisualStyleBackColor = true;
            this.newProjectButton.Click += new System.EventHandler(this.newProjectButton_Click);
            // 
            // deleteProjectButton
            // 
            this.deleteProjectButton.Location = new System.Drawing.Point(108, 19);
            this.deleteProjectButton.Name = "deleteProjectButton";
            this.deleteProjectButton.Size = new System.Drawing.Size(75, 23);
            this.deleteProjectButton.TabIndex = 14;
            this.deleteProjectButton.Text = "Delete";
            this.toolTip1.SetToolTip(this.deleteProjectButton, "Delete the current resource project.");
            this.deleteProjectButton.UseVisualStyleBackColor = true;
            this.deleteProjectButton.Click += new System.EventHandler(this.deleteProjectButton_Click);
            // 
            // projectNameTextBox
            // 
            this.projectNameTextBox.Location = new System.Drawing.Point(105, 76);
            this.projectNameTextBox.Name = "projectNameTextBox";
            this.projectNameTextBox.Size = new System.Drawing.Size(246, 20);
            this.projectNameTextBox.TabIndex = 1;
            this.toolTip1.SetToolTip(this.projectNameTextBox, "Enter your project name here.");
            this.projectNameTextBox.LostFocus += new System.EventHandler(this.projectNameTextBox_LostFocus);
            // 
            // resourceXMLTextBox
            // 
            this.resourceXMLTextBox.Location = new System.Drawing.Point(105, 105);
            this.resourceXMLTextBox.Name = "resourceXMLTextBox";
            this.resourceXMLTextBox.Size = new System.Drawing.Size(246, 20);
            this.resourceXMLTextBox.TabIndex = 2;
            this.toolTip1.SetToolTip(this.resourceXMLTextBox, "Enter your resource .XML file here.");
            // 
            // outputDirTextBox
            // 
            this.outputDirTextBox.Enabled = false;
            this.outputDirTextBox.Location = new System.Drawing.Point(105, 134);
            this.outputDirTextBox.Name = "outputDirTextBox";
            this.outputDirTextBox.Size = new System.Drawing.Size(246, 20);
            this.outputDirTextBox.TabIndex = 4;
            this.toolTip1.SetToolTip(this.outputDirTextBox, "Enter the directory for the generated C++ source files.");
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.newProjectButton);
            this.groupBox1.Controls.Add(this.deleteProjectButton);
            this.groupBox1.Controls.Add(this.mruComboBox);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Location = new System.Drawing.Point(13, 13);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(424, 52);
            this.groupBox1.TabIndex = 30;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Project";
            // 
            // outputFileTextBox
            // 
            this.outputFileTextBox.Location = new System.Drawing.Point(105, 163);
            this.outputFileTextBox.Name = "outputFileTextBox";
            this.outputFileTextBox.Size = new System.Drawing.Size(246, 20);
            this.outputFileTextBox.TabIndex = 6;
            this.toolTip1.SetToolTip(this.outputFileTextBox, "Enter name of generated source files here.");
            this.outputFileTextBox.TextChanged += new System.EventHandler(this.outputFileTextBox_TextChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(28, 80);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(71, 13);
            this.label2.TabIndex = 18;
            this.label2.Text = "Project Name";
            this.toolTip1.SetToolTip(this.label2, "The project name.");
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(21, 109);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(78, 13);
            this.label3.TabIndex = 19;
            this.label3.Text = "Resource.XML";
            this.toolTip1.SetToolTip(this.label3, "Your XML file which defines the used resources.");
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(15, 138);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(84, 13);
            this.label4.TabIndex = 20;
            this.label4.Text = "Output Directory";
            this.toolTip1.SetToolTip(this.label4, "The generated C++ source files will be written to this directory.");
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(10, 166);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(89, 13);
            this.label5.TabIndex = 21;
            this.label5.Text = "Output File Name";
            this.toolTip1.SetToolTip(this.label5, "The name of the generated C++ source files.");
            // 
            // browseResourceXMLButton
            // 
            this.browseResourceXMLButton.Location = new System.Drawing.Point(357, 104);
            this.browseResourceXMLButton.Name = "browseResourceXMLButton";
            this.browseResourceXMLButton.Size = new System.Drawing.Size(33, 20);
            this.browseResourceXMLButton.TabIndex = 3;
            this.browseResourceXMLButton.Text = "...";
            this.toolTip1.SetToolTip(this.browseResourceXMLButton, "Choose your resource .XML file.");
            this.browseResourceXMLButton.UseVisualStyleBackColor = true;
            this.browseResourceXMLButton.Click += new System.EventHandler(this.browseResXML_Click);
            // 
            // browseOutDirButton
            // 
            this.browseOutDirButton.Location = new System.Drawing.Point(357, 133);
            this.browseOutDirButton.Name = "browseOutDirButton";
            this.browseOutDirButton.Size = new System.Drawing.Size(33, 20);
            this.browseOutDirButton.TabIndex = 5;
            this.browseOutDirButton.Text = "...";
            this.toolTip1.SetToolTip(this.browseOutDirButton, "Choose your destination path for the generated C++ source files.");
            this.browseOutDirButton.UseVisualStyleBackColor = true;
            this.browseOutDirButton.Click += new System.EventHandler(this.browseOutDirButton_Click);
            // 
            // consoleOutListBox
            // 
            this.consoleOutListBox.BackColor = System.Drawing.SystemColors.Control;
            this.consoleOutListBox.FormattingEnabled = true;
            this.consoleOutListBox.IntegralHeight = false;
            this.consoleOutListBox.Location = new System.Drawing.Point(13, 316);
            this.consoleOutListBox.Name = "consoleOutListBox";
            this.consoleOutListBox.SelectionMode = System.Windows.Forms.SelectionMode.None;
            this.consoleOutListBox.Size = new System.Drawing.Size(427, 134);
            this.consoleOutListBox.TabIndex = 16;
            // 
            // outnameLabel
            // 
            this.outnameLabel.Location = new System.Drawing.Point(102, 277);
            this.outnameLabel.Name = "outnameLabel";
            this.outnameLabel.Size = new System.Drawing.Size(245, 23);
            this.outnameLabel.TabIndex = 24;
            this.outnameLabel.Text = "label6";
            this.toolTip1.SetToolTip(this.outnameLabel, "The name of the generated C++ source files.");
            // 
            // functionPrefixTextBox
            // 
            this.functionPrefixTextBox.Location = new System.Drawing.Point(105, 190);
            this.functionPrefixTextBox.Name = "functionPrefixTextBox";
            this.functionPrefixTextBox.Size = new System.Drawing.Size(245, 20);
            this.functionPrefixTextBox.TabIndex = 7;
            this.toolTip1.SetToolTip(this.functionPrefixTextBox, "Enter function prefix here.");
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(22, 193);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(77, 13);
            this.label6.TabIndex = 22;
            this.label6.Text = "Function Prefix";
            this.toolTip1.SetToolTip(this.label6, "Prefix for the generated functions, i.e. when the prefix is \"Bob\",\r\nthe function " +
                    "name is BobLoadImageById() instead of LoadImageById()");
            // 
            // toolTip1
            // 
            this.toolTip1.ToolTipTitle = "Hint:";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(18, 245);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(81, 13);
            this.label7.TabIndex = 23;
            this.label7.Text = "Base Res. Path";
            this.toolTip1.SetToolTip(this.label7, "The path which contains the subdirectories with resources.");
            // 
            // baseResPathTextBox
            // 
            this.baseResPathTextBox.Enabled = false;
            this.baseResPathTextBox.Location = new System.Drawing.Point(105, 241);
            this.baseResPathTextBox.Name = "baseResPathTextBox";
            this.baseResPathTextBox.Size = new System.Drawing.Size(245, 20);
            this.baseResPathTextBox.TabIndex = 10;
            this.toolTip1.SetToolTip(this.baseResPathTextBox, "Enter the path that contains the directories with the resources.");
            // 
            // reportUnusedFilesCheckBox
            // 
            this.reportUnusedFilesCheckBox.AutoSize = true;
            this.reportUnusedFilesCheckBox.Location = new System.Drawing.Point(105, 218);
            this.reportUnusedFilesCheckBox.Name = "reportUnusedFilesCheckBox";
            this.reportUnusedFilesCheckBox.Size = new System.Drawing.Size(122, 17);
            this.reportUnusedFilesCheckBox.TabIndex = 8;
            this.reportUnusedFilesCheckBox.Text = "Report Unused Files";
            this.toolTip1.SetToolTip(this.reportUnusedFilesCheckBox, "Check this, if you want to report unused resources.");
            this.reportUnusedFilesCheckBox.UseVisualStyleBackColor = true;
            this.reportUnusedFilesCheckBox.CheckedChanged += new System.EventHandler(this.reportUnusedFilesCheckBox_CheckedChanged);
            // 
            // browseBaseResPathButton
            // 
            this.browseBaseResPathButton.Enabled = false;
            this.browseBaseResPathButton.Location = new System.Drawing.Point(357, 241);
            this.browseBaseResPathButton.Name = "browseBaseResPathButton";
            this.browseBaseResPathButton.Size = new System.Drawing.Size(33, 20);
            this.browseBaseResPathButton.TabIndex = 11;
            this.browseBaseResPathButton.Text = "...";
            this.toolTip1.SetToolTip(this.browseBaseResPathButton, "Choose the base resource path.");
            this.browseBaseResPathButton.UseVisualStyleBackColor = true;
            this.browseBaseResPathButton.Click += new System.EventHandler(this.browseBaseResPathButton_Click);
            // 
            // writeDelBatchCheckBox
            // 
            this.writeDelBatchCheckBox.AutoSize = true;
            this.writeDelBatchCheckBox.Enabled = false;
            this.writeDelBatchCheckBox.Location = new System.Drawing.Point(233, 218);
            this.writeDelBatchCheckBox.Name = "writeDelBatchCheckBox";
            this.writeDelBatchCheckBox.Size = new System.Drawing.Size(135, 17);
            this.writeDelBatchCheckBox.TabIndex = 9;
            this.writeDelBatchCheckBox.Text = "Write Delete Batch File";
            this.toolTip1.SetToolTip(this.writeDelBatchCheckBox, "Check this, if you want to generate a batch file to delete the unused resources.");
            this.writeDelBatchCheckBox.UseVisualStyleBackColor = true;
            // 
            // ResourceGeneratorForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(452, 462);
            this.Controls.Add(this.writeDelBatchCheckBox);
            this.Controls.Add(this.browseBaseResPathButton);
            this.Controls.Add(this.baseResPathTextBox);
            this.Controls.Add(this.reportUnusedFilesCheckBox);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.functionPrefixTextBox);
            this.Controls.Add(this.outnameLabel);
            this.Controls.Add(this.consoleOutListBox);
            this.Controls.Add(this.browseOutDirButton);
            this.Controls.Add(this.browseResourceXMLButton);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.outputFileTextBox);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.outputDirTextBox);
            this.Controls.Add(this.resourceXMLTextBox);
            this.Controls.Add(this.projectNameTextBox);
            this.Controls.Add(this.generateButton);
            this.MaximizeBox = false;
            this.Name = "ResourceGeneratorForm";
            this.ShowIcon = false;
            this.Text = "Sexy Kanji Resource Generator";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button generateButton;
        private System.Windows.Forms.ComboBox mruComboBox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button newProjectButton;
        private System.Windows.Forms.Button deleteProjectButton;
        private System.Windows.Forms.TextBox projectNameTextBox;
        private System.Windows.Forms.TextBox resourceXMLTextBox;
        private System.Windows.Forms.TextBox outputDirTextBox;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.TextBox outputFileTextBox;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Button browseResourceXMLButton;
        private System.Windows.Forms.Button browseOutDirButton;
        private System.Windows.Forms.ListBox consoleOutListBox;
        private System.Windows.Forms.FolderBrowserDialog folderBrowserDialog1;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.Label outnameLabel;
        private System.Windows.Forms.TextBox functionPrefixTextBox;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.ToolTip toolTip1;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.CheckBox reportUnusedFilesCheckBox;
        private System.Windows.Forms.TextBox baseResPathTextBox;
        private System.Windows.Forms.Button browseBaseResPathButton;
        private System.Windows.Forms.CheckBox writeDelBatchCheckBox;
    }
}

