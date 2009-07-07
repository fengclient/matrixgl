using System;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace MatrixGLConfig
{
    public partial class frmConfig : Form
    {
        public string path = Environment.GetEnvironmentVariable("SystemRoot") + "\\matrixgl_config";
        public frmConfig()
        {
            InitializeComponent();
            // if file already exists, set current options, otherwise set default.
            // using ascii values for 0-3 (48-51) because
            byte[] b = {48,48,49};
            if (File.Exists(path))
            {
                using (FileStream fs = File.OpenRead(path))
                {
                    ASCIIEncoding temp = new ASCIIEncoding();
                    while (fs.Read(b, 0, b.Length) > 0)
                    {
                        Console.WriteLine(temp.GetString(b));
                    }
                    fs.Close();
                }
            }
            if (b[0] == 48 | b[0] > 49 | b[0] < 48) { btnStyle3D.Select(); }
            else if (b[0] == 49) { btnStyleStatic.Select(); }

            if (b[1] == 48 | b[1] > 49 | b[1] < 48) { btnCreditsNo.Select(); }
            else if (b[1] == 49) { btnCreditsYes.Select(); }

            if (b[2] == 49 | b[2] < 49 | b[2] > 51) { btnColorGreen.Select(); }
            else if (b[2] == 50) { btnColorRed.Select(); }
            else if (b[2] == 51) { btnColorBlue.Select(); }
        }
        
        private void btnSave_Click(object sender, EventArgs e)
        {
            // set defaults if nothing happens to be clicked
            byte style = 48, credit = 48, color = 49;
            
            if (btnStyle3D.Checked) { style = 48; }
            else if(btnStyleStatic.Checked) { style = 49; }

            if (btnCreditsNo.Checked) { credit = 48; }
            else if (btnCreditsYes.Checked) { credit = 49; }

            if (btnColorGreen.Checked) { color = 49; }
            else if (btnColorRed.Checked) { color = 50; }
            else if (btnColorBlue.Checked) { color = 51; }

            if (File.Exists(path))
            {
                File.Delete(path);
            }
            using (FileStream fs = File.Create(path))
            {
                fs.WriteByte(style);
                fs.WriteByte(credit);
                fs.WriteByte(color);
                fs.Close();
            }
        }

        private void btnExit_Click(object sender, EventArgs e)
        {
            Close();
        }

    }
}
