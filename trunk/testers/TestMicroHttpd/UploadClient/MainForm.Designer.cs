namespace UploadClient
{
	partial class MainForm
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
			this.label1 = new System.Windows.Forms.Label();
			this.txtPath = new System.Windows.Forms.TextBox();
			this.cmdOpen = new System.Windows.Forms.Button();
			this.cmdUploadWebClient = new System.Windows.Forms.Button();
			this.dlgOpenFile = new System.Windows.Forms.OpenFileDialog();
			this.label3 = new System.Windows.Forms.Label();
			this.txtUrl = new System.Windows.Forms.TextBox();
			this.cmdUploadWebRequest = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(12, 9);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(29, 12);
			this.label1.TabIndex = 0;
			this.label1.Text = "File";
			// 
			// txtPath
			// 
			this.txtPath.Location = new System.Drawing.Point(47, 6);
			this.txtPath.Name = "txtPath";
			this.txtPath.Size = new System.Drawing.Size(345, 21);
			this.txtPath.TabIndex = 1;
			// 
			// cmdOpen
			// 
			this.cmdOpen.Location = new System.Drawing.Point(393, 6);
			this.cmdOpen.Name = "cmdOpen";
			this.cmdOpen.Size = new System.Drawing.Size(63, 21);
			this.cmdOpen.TabIndex = 2;
			this.cmdOpen.Text = "&Open";
			this.cmdOpen.UseVisualStyleBackColor = true;
			this.cmdOpen.Click += new System.EventHandler(this.cmdOpen_Click);
			// 
			// cmdUploadWebClient
			// 
			this.cmdUploadWebClient.Location = new System.Drawing.Point(12, 73);
			this.cmdUploadWebClient.Name = "cmdUploadWebClient";
			this.cmdUploadWebClient.Size = new System.Drawing.Size(98, 44);
			this.cmdUploadWebClient.TabIndex = 5;
			this.cmdUploadWebClient.Text = "&Upload WebClient";
			this.cmdUploadWebClient.UseVisualStyleBackColor = true;
			this.cmdUploadWebClient.Click += new System.EventHandler(this.cmdUploadWebClient_Click);
			// 
			// dlgOpenFile
			// 
			this.dlgOpenFile.FileName = "openFileDialog1";
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(12, 39);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(23, 12);
			this.label3.TabIndex = 6;
			this.label3.Text = "URL";
			// 
			// txtUrl
			// 
			this.txtUrl.Location = new System.Drawing.Point(47, 36);
			this.txtUrl.Name = "txtUrl";
			this.txtUrl.Size = new System.Drawing.Size(345, 21);
			this.txtUrl.TabIndex = 7;
			this.txtUrl.Text = "http://127.0.0.1/";
			// 
			// cmdUploadWebRequest
			// 
			this.cmdUploadWebRequest.Location = new System.Drawing.Point(116, 73);
			this.cmdUploadWebRequest.Name = "cmdUploadWebRequest";
			this.cmdUploadWebRequest.Size = new System.Drawing.Size(98, 44);
			this.cmdUploadWebRequest.TabIndex = 8;
			this.cmdUploadWebRequest.Text = "&Upload WebRequest";
			this.cmdUploadWebRequest.UseVisualStyleBackColor = true;
			this.cmdUploadWebRequest.Click += new System.EventHandler(this.cmdUploadWebRequest_Click);
			// 
			// MainForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(468, 266);
			this.Controls.Add(this.cmdUploadWebRequest);
			this.Controls.Add(this.txtUrl);
			this.Controls.Add(this.label3);
			this.Controls.Add(this.cmdUploadWebClient);
			this.Controls.Add(this.cmdOpen);
			this.Controls.Add(this.txtPath);
			this.Controls.Add(this.label1);
			this.Name = "MainForm";
			this.Text = "Upload Client";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.TextBox txtPath;
		private System.Windows.Forms.Button cmdOpen;
		private System.Windows.Forms.Button cmdUploadWebClient;
		private System.Windows.Forms.OpenFileDialog dlgOpenFile;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.TextBox txtUrl;
		private System.Windows.Forms.Button cmdUploadWebRequest;
	}
}

