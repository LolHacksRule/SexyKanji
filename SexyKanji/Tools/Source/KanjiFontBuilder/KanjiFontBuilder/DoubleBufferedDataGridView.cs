using System;

namespace KanjiFontBuilder
{
    public class DoubleBufferedDataGridView : System.Windows.Forms.DataGridView
    {
        public DoubleBufferedDataGridView()
        {
            DoubleBuffered = true;
        }
    }
}