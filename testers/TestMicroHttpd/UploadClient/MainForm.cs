using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Net;

namespace UploadClient
{
	public partial class MainForm : Form
	{
		public MainForm()
		{
			InitializeComponent();
		}

		static class Program
		{
			/// <summary>
			/// The main entry point for the application.
			/// </summary>
			[STAThread]
			static void Main()
			{
				Application.EnableVisualStyles();
				Application.SetCompatibleTextRenderingDefault(false);
				Application.Run(new MainForm());
			}
		}

		private void cmdOpen_Click(object sender, EventArgs e)
		{
			if (dlgOpenFile.ShowDialog() != System.Windows.Forms.DialogResult.OK)
				return;

			string path = dlgOpenFile.FileName;
			txtPath.Text = path;
		}

		private bool CheckArgs(out string path, out string url)
		{
			path = txtPath.Text;
			url = txtUrl.Text;

			if (File.Exists(path))
				return true;

			MessageBox.Show("File not found.", this.Text, MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
			return false;
		}

		private void cmdUploadWebClient_Click(object sender, EventArgs e)
		{
			string path, url;
			if (!CheckArgs(out path, out url))
				return;

			try
			{
				WebClient wc = new WebClient();
				wc.UploadFile(url, path);
				MessageBox.Show("Upload completed.", this.Text, MessageBoxButtons.OK, MessageBoxIcon.Information);
			}
			catch (Exception ex)
			{
				MessageBox.Show(ex.Message, this.Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
			}
		}

		private void cmdUploadWebRequest_Click(object sender, EventArgs e)
		{
			string path, url;
			if (!CheckArgs(out path, out url))
				return;

			try
			{
				Stream fs = new FileStream(path, FileMode.Open, FileAccess.Read, FileShare.Read);
				HttpWebRequest request = (HttpWebRequest)HttpWebRequest.Create(url);
				request.Method = "POST";
				request.ContentType = "application/x-www-form-urlencoded";
				request.ContentLength = fs.Length;
				System.IO.Stream reqStream = request.GetRequestStream();

				byte[] buffer = new byte[65536];
				fs.Seek(0, SeekOrigin.Begin);
				while (true)
				{
					int cbRead = fs.Read(buffer, 0, buffer.Length);
					if (cbRead == 0)
						break;
					reqStream.Write(buffer, 0, cbRead);
				}

				reqStream.Close();
				fs.Close();

				HttpWebResponse response = (HttpWebResponse)request.GetResponse();
				TextReader tr = new StreamReader(response.GetResponseStream(), Encoding.UTF8);
				string respText = tr.ReadToEnd();
				tr.Close();

				// Response 和 ResponseStream 两者只需要关闭其中一个即可
				response.Close();
			}
			catch (Exception ex)
			{
				MessageBox.Show(ex.Message, this.Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
			}
		}
	}
}
