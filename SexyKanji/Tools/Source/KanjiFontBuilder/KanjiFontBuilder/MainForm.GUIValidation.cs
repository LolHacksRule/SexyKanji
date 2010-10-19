using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Drawing.Text;
using System.Collections;
using System.IO;
using System.Drawing.Imaging;
using System.Drawing.Drawing2D;
using System.Reflection;
using System.Xml;

namespace KanjiFontBuilder
{
    partial class MainForm
    {
        private void SetupOutputPaneDefaults()
        {
            //////////////////////////////////////////////////////////////////////////
            // Setup Output Pane
            //////////////////////////////////////////////////////////////////////////
            mImageFormatComboBox.SelectedIndex = 0;
            mDescriptorFormatComboBox.SelectedIndex = 0;
            mDescriptorEncodingComboBox.SelectedIndex = 1; // UTF-8
            mImageFileNameEditBox.Text = mFontComboBox.Text + mSizeComboBox.Text + ((mStyleComboBox.Text == "Regular") ? "" : mStyleComboBox.Text) + "_page%d";
            mDescriptorFileNameEditBox.Text = mFontComboBox.Text + mSizeComboBox.Text + ((mStyleComboBox.Text == "Regular") ? "" : mStyleComboBox.Text);

        }
        private void CreateToolTips()
        {
            mToolTips = new ToolTip();
            mToolTips.SetToolTip(mFontComboBox, "Select the Font To Draw.");
            mToolTips.SetToolTip(mStyleComboBox, "Set the Font Drawing Style.");
            mToolTips.SetToolTip(mSizeComboBox, "Set the Font Size.");
            mToolTips.SetToolTip(mFGColorButton, "Set the Glyph Color.\nDefault:White (Recommended)");
            mToolTips.SetToolTip(mBGColorButton, "Set the Background Color of the Texture.\nDefault: Transparent\nRecommended: {Black, Transparent}");
            mToolTips.SetToolTip(button2, "Set the Background Color to Transparent.");
            mToolTips.SetToolTip(mHintCombo, "Select Smoothing Algorithm of the Font.\n ClearType is NOT RECOMMENDED for Transparent Backgrounds.");
            mToolTips.SetToolTip(mPaddingControl, "Add Global spacing BETWEEN GLYPHS.\nDoes not affect Render Rect Size.\nTurn on Debug Rects to see affect.");
            mToolTips.SetToolTip(mDebugRects, "Toggle Drawing of Bounding Rectangles.\nFushia Rectangle is Render Rect for Font.\nYellow Rect is Bounding Rect for Glyph.");
            mToolTips.SetToolTip(groupBox3, "Add Extra Padding inside the Glyph Render Rect.\nWARNING: Padding does not affect the spacing when \nRendered In-Game!\nUse Kerning Instead.");
            mToolTips.SetToolTip(mCharMapButton, "Opens a Dialog to Select Common Character Sets.");
            mToolTips.SetToolTip(mImportButton, "Read Characters from a File.");
            mToolTips.SetToolTip(mExportButton, "Write Characters to a File.");
            mToolTips.SetToolTip(mCharacterEditBox, "These unique characters will \nbe sorted and drawn.");
            mToolTips.SetToolTip(tabControl2, "Kanji supports up to 4 Image Pages for Glyphs.\nEmpty Pages will not be saved.");
            mToolTips.SetToolTip(mTextureSizeCombo, "Set the size of the Image Pages \nfor holding Glyphs.");
            mToolTips.SetToolTip(mWriteImagesCheckbox, "Toggle the Writing of the \nGlyph Image Pages (Textures).");
            mToolTips.SetToolTip(mDirectoryPathText, "The output directory to save generated files.");
            mToolTips.SetToolTip(mDirBrowseButton, "Browse.");
            mToolTips.SetToolTip(mImageFormatComboBox, "Save Format of Images.\nWARNING: Only PNG supports Transparency.");
            mToolTips.SetToolTip(mImageFileNameEditBox, "File Name Format to Save Image Files.\n%d is token for Page Number.");
            mToolTips.SetToolTip(mWriteDescriptorCheckBox, "Toggle the Writing of the File Descriptor.");
            mToolTips.SetToolTip(mDescriptorFormatComboBox, "SexyKanji is XML Format\nKanji is Binary Format.");
            mToolTips.SetToolTip(mDescriptorEncodingComboBox, "Text Encoding for Descriptor File.\nRecommended: UTF-8.");
            mToolTips.SetToolTip(mDescriptorFileNameEditBox, "Output file name for Descriptor.");
            mToolTips.SetToolTip(mKerningSpin, "Adjust the Spacing between characters\nwhen Rendered In-Game.");
        }
        private void FillCharacterEditBox()
        {
            mCharacterEditBox.Text = mCharaterSet;
            mCharacterEditBox.TextChanged += new EventHandler(mCharacterEditBox_TextChanged);
            mCharacterEditBox.LostFocus += new EventHandler(mCharacterEditBox_LostFocus);
        }
        private void FillTextureSizeCombo()
        {
            ArrayList aTextureSizeList = new ArrayList();
            aTextureSizeList.Add(512);
            aTextureSizeList.Add(256);
            aTextureSizeList.Add(128);
			aTextureSizeList.Add(64);

            mTextureSizeCombo.DataSource = aTextureSizeList;
            mTextureSizeCombo.SelectedIndex = 0;
            mTextureSizeCombo.SelectedIndexChanged += new EventHandler(mTextureSizeCombo_SelectedIndexChanged);
        }

        void mTextureSizeCombo_SelectedIndexChanged(object sender, EventArgs e)
        {
            switch (mTextureSizeCombo.SelectedIndex)
            {
                case 0:
                    mTextureSize = 512;
                    break;
                case 1:
                    mTextureSize = 256;
                    break;
                case 2:
                    mTextureSize = 128;
                    break;
				case 3:
					mTextureSize = 64;
					break;
            }

            InitImagePages();
            BuildImagePlanes();
        }

        void mCharacterEditBox_LostFocus(object sender, EventArgs e)
        {
            ArrayList aSortList = new ArrayList();
            foreach (char c in mCharacterEditBox.Text)
                aSortList.Add(c);

            aSortList.Sort();

            String aFinalString = "";
            foreach (char c in aSortList)
            {
                if (!aFinalString.Contains(c.ToString()))
                    aFinalString += c.ToString();
            }

            mCharacterEditBox.Text = aFinalString;
        }

        void mCharacterEditBox_TextChanged(object sender, EventArgs e)
        {
            mCharaterSet = mCharacterEditBox.Text;
            BuildImagePlanes();
        }

        /// <summary>
        /// Add cut and paste menu to character edit box
        /// </summary>
        /// <returns></returns>
        private void CreateEditBoxContextMenu()
        {
            ContextMenu aPopupMenu = new ContextMenu();
            aPopupMenu.MenuItems.Add("Cut", new EventHandler(aPopupMenu_CutClicked));
            aPopupMenu.MenuItems.Add("Copy", new EventHandler(aPopupMenu_CopyClicked));
            aPopupMenu.MenuItems.Add("Paste", new EventHandler(aPopupMenu_PasteClicked));
            aPopupMenu.MenuItems.Add("Select All", new EventHandler(aPopupMenu_SelectAllClicked));
            mCharacterEditBox.ContextMenu = aPopupMenu;
        }
        /// <summary>
        /// Cut, Copy, Paste and Select All Implementations for context menu
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        /// <returns></returns>
        void aPopupMenu_CutClicked(object sender, EventArgs e) { mCharacterEditBox.Cut(); }
        void aPopupMenu_CopyClicked(object sender, EventArgs e) { mCharacterEditBox.Copy(); }
        void aPopupMenu_PasteClicked(object sender, EventArgs e) { mCharacterEditBox.Paste(); }
        void aPopupMenu_SelectAllClicked(object sender, EventArgs e) { mCharacterEditBox.SelectAll(); }

        /// <summary>
        ///  Paint Handlers for Picture Boxes and Color control
        /// </summary>
        /// <returns></returns>
        private void RegisterPaintHandlers()
        {
            //////////////////////////////////////////////////////////////////////////
            // Picture Box Paint Handlers
            //////////////////////////////////////////////////////////////////////////
            mPictureBox1.Paint += new PaintEventHandler(mPictureBox1_Paint);
            mPictureBox2.Paint += new PaintEventHandler(mPictureBox2_Paint);
            mPictureBox3.Paint += new PaintEventHandler(mPictureBox3_Paint);
            mPictureBox4.Paint += new PaintEventHandler(mPictureBox4_Paint);
            mPictureBox5.Paint += new PaintEventHandler(mPictureBox5_Paint);
            mPictureBox6.Paint += new PaintEventHandler(mPictureBox6_Paint);
            mPictureBox7.Paint += new PaintEventHandler(mPictureBox7_Paint);
            mPictureBox8.Paint += new PaintEventHandler(mPictureBox8_Paint);
            // Color Buttons
            mBGColorButton.Paint += new PaintEventHandler(mBGColorButton_Paint);
            mFGColorButton.Paint += new PaintEventHandler(mFGColorButton_Paint);
        }

        void mPictureBox8_Paint(object sender, PaintEventArgs e)
        {
            e.Graphics.FillRectangle(new SolidBrush(Color.FromKnownColor(KnownColor.ControlDarkDark)),
                new Rectangle(0, 0, mPictureBox1.Width, mPictureBox1.Height)
            );
            e.Graphics.FillRectangle(
                new HatchBrush(
                    HatchStyle.LargeCheckerBoard,
                    Color.FromKnownColor(KnownColor.ControlDark),
                    Color.FromKnownColor(KnownColor.ControlDarkDark)),
                 new Rectangle(0, 0, mTextureSize, mTextureSize)
            );


            e.Graphics.DrawImage(mPictureBox8.Image, new Point(0, 0));
        }

        void mPictureBox7_Paint(object sender, PaintEventArgs e)
        {
            e.Graphics.FillRectangle(new SolidBrush(Color.FromKnownColor(KnownColor.ControlDarkDark)),
                new Rectangle(0, 0, mPictureBox1.Width, mPictureBox1.Height)
            );
            e.Graphics.FillRectangle(
                new HatchBrush(
                    HatchStyle.LargeCheckerBoard,
                    Color.FromKnownColor(KnownColor.ControlDark),
                    Color.FromKnownColor(KnownColor.ControlDarkDark)),
                 new Rectangle(0, 0, mTextureSize, mTextureSize)
            );


            e.Graphics.DrawImage(mPictureBox7.Image, new Point(0, 0));
        }

        void mPictureBox6_Paint(object sender, PaintEventArgs e)
        {
            e.Graphics.FillRectangle(new SolidBrush(Color.FromKnownColor(KnownColor.ControlDarkDark)),
                new Rectangle(0, 0, mPictureBox1.Width, mPictureBox1.Height)
            );
            e.Graphics.FillRectangle(
                new HatchBrush(
                    HatchStyle.LargeCheckerBoard,
                    Color.FromKnownColor(KnownColor.ControlDark),
                    Color.FromKnownColor(KnownColor.ControlDarkDark)),
                 new Rectangle(0, 0, mTextureSize, mTextureSize)
            );


            e.Graphics.DrawImage(mPictureBox6.Image, new Point(0, 0));
        }

        void mPictureBox5_Paint(object sender, PaintEventArgs e)
        {
            e.Graphics.FillRectangle(new SolidBrush(Color.FromKnownColor(KnownColor.ControlDarkDark)),
                new Rectangle(0, 0, mPictureBox1.Width, mPictureBox1.Height)
            );
            e.Graphics.FillRectangle(
                new HatchBrush(
                    HatchStyle.LargeCheckerBoard,
                    Color.FromKnownColor(KnownColor.ControlDark),
                    Color.FromKnownColor(KnownColor.ControlDarkDark)),
                 new Rectangle(0, 0, mTextureSize, mTextureSize)
            );


            e.Graphics.DrawImage(mPictureBox5.Image, new Point(0, 0));
        }
        /// <summary>
        /// {AntiAliasGridFit, AntiAlias, SingleBitPerPixel, SingleBitPerPixelGridFit, ClearTypeGridFit}
        /// </summary>
        /// <returns></returns>
        private void FillTextHintingDropDownCombo()
        {
            //////////////////////////////////////////////////////////////////////////
            // Text Hinting
            //////////////////////////////////////////////////////////////////////////
            ArrayList aHintArray = new ArrayList();
            aHintArray.Add(TextRenderingHint.AntiAliasGridFit.ToString());
            aHintArray.Add(TextRenderingHint.AntiAlias.ToString());
            aHintArray.Add(TextRenderingHint.SingleBitPerPixel.ToString());
            aHintArray.Add(TextRenderingHint.SingleBitPerPixelGridFit.ToString());
            aHintArray.Add(TextRenderingHint.ClearTypeGridFit.ToString());

            mHintCombo.DataSource = aHintArray;
            mHintCombo.SelectedIndex = 0;
            mHintCombo.SelectionChangeCommitted += new EventHandler(mHintCombo_SelectionChangeCommitted);

            mTextHint = TextRenderingHint.AntiAliasGridFit;
        }
        /// <summary>
        /// Size list = {8, 10, 12, 14, 16, 18, 20, 22, 24, 32, 48, 72}
        /// </summary>
        /// <returns></returns>
        private void FillFontSizeDropDownCombo()
        {
            //////////////////////////////////////////////////////////////////////////
            // Fill Size Combo Box
            //////////////////////////////////////////////////////////////////////////
            ArrayList aSizeList = new ArrayList();
            aSizeList.Add(8);
            aSizeList.Add(10);
            aSizeList.Add(12);
            aSizeList.Add(14);
            aSizeList.Add(16);
            aSizeList.Add(18);
            aSizeList.Add(20);
            aSizeList.Add(22);
            aSizeList.Add(24);
            aSizeList.Add(32);
            aSizeList.Add(48);
            aSizeList.Add(72);

            mSizeComboBox.TextChanged += new EventHandler(OnFontSizeComboTextChanged);
            mSizeComboBox.DataSource = aSizeList;
            mSizeComboBox.SelectedIndex = 8;
            mCurrentFontSize = 24;
        }
        /// <summary>
        /// Regular, Bold, Italics
        /// </summary>
        /// <returns></returns>
        private void FillFontStyleDropDownCombo()
        {
            //////////////////////////////////////////////////////////////////////////
            // Fill Font Styles.
            //////////////////////////////////////////////////////////////////////////
            mFontStyleList = new ArrayList();

            if (mCurrentFontFamily.IsStyleAvailable(FontStyle.Regular))
                mFontStyleList.Add("Regular");
            if (mCurrentFontFamily.IsStyleAvailable(FontStyle.Bold))
                mFontStyleList.Add("Bold");
            if (mCurrentFontFamily.IsStyleAvailable(FontStyle.Italic))
                mFontStyleList.Add("Italics");

            mStyleComboBox.DataSource = mFontStyleList;
            mStyleComboBox.SelectedIndexChanged += new EventHandler(OnFontStyleComboSelectedIndexChanged);
            mCurrentStyle = FontStyle.Regular;
        }

        /// <summary>
        /// Use System Loaded Fonts
        /// </summary>
        /// <returns></returns>
        private void FillFontDropDownCombo()
        {
            //////////////////////////////////////////////////////////////////////////
            // Fill Font Dropdown Box
            //////////////////////////////////////////////////////////////////////////
            mInstalledFonts = new PrivateFontCollection();

            DirectoryInfo di = new DirectoryInfo(Environment.ExpandEnvironmentVariables("%SystemRoot%") + "\\Fonts");
            foreach (FileInfo fi in di.GetFiles("*.ttf"))
            {
                string aFontFileName = fi.DirectoryName + "\\" + fi.Name;
                try
                {
                    mInstalledFonts.AddFontFile(aFontFileName);
                }
                catch (System.Exception)
                {
                }
            }

            ArrayList aFontList = new ArrayList();

            foreach (FontFamily f in mInstalledFonts.Families)
            {
                //if (f.IsStyleAvailable(FontStyle.Regular))
                    aFontList.Add(f.Name);
            }

            mFontComboBox.DataSource = aFontList;

            //////////////////////////////////////////////////////////////////////////
            // Select Starting Font
            //////////////////////////////////////////////////////////////////////////
            mCurrentFontFamily = mInstalledFonts.Families[0];
            foreach (FontFamily f in mInstalledFonts.Families)
            {
                if (f.IsStyleAvailable(FontStyle.Regular) && f.Name == "Arial")
                {
                    mCurrentFontFamily = f;
                    break;
                }
            }

            ///////////////////////////////////////////////////////////////////////////
            // Make default Selection
            //////////////////////////////////////////////////////////////////////////
            mFontComboBox.Text = mCurrentFontFamily.Name;
            mFontComboBox.SelectionChangeCommitted += new EventHandler(FontFace_SelectionChangeCommitted);
        }

        /// <summary>
        /// Paint the Checkerboard Hatch pattern so we can tell when the BG is set to Transparent
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        /// <returns></returns>
        void mPictureBox4_Paint(object sender, PaintEventArgs e)
        {
            e.Graphics.FillRectangle(new SolidBrush(Color.FromKnownColor(KnownColor.ControlDarkDark)),
                new Rectangle(0, 0, mPictureBox1.Width, mPictureBox1.Height));

            e.Graphics.FillRectangle(
                new HatchBrush(
                    HatchStyle.LargeCheckerBoard,
                    Color.FromKnownColor(KnownColor.ControlDark),
                    Color.FromKnownColor(KnownColor.ControlDarkDark)),
                new Rectangle(0, 0, mTextureSize, mTextureSize)
            );

            e.Graphics.DrawImage(mPictureBox4.Image, new Point(0, 0));
        }

        /// <summary>
        /// Paint the Checkerboard Hatch pattern so we can tell when the BG is set to Transparent
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        /// <returns></returns>
        void mPictureBox3_Paint(object sender, PaintEventArgs e)
        {
            e.Graphics.FillRectangle(new SolidBrush(Color.FromKnownColor(KnownColor.ControlDarkDark)),
                new Rectangle(0, 0, mPictureBox1.Width, mPictureBox1.Height));

            e.Graphics.FillRectangle(
                new HatchBrush(
                    HatchStyle.LargeCheckerBoard,
                    Color.FromKnownColor(KnownColor.ControlDark),
                    Color.FromKnownColor(KnownColor.ControlDarkDark)),
                new Rectangle(0, 0, mTextureSize, mTextureSize)
            );

            e.Graphics.DrawImage(mPictureBox3.Image, new Point(0, 0));
        }

        /// <summary>
        /// Paint the Checkerboard Hatch pattern so we can tell when the BG is set to Transparent
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        /// <returns></returns>
        void mPictureBox2_Paint(object sender, PaintEventArgs e)
        {
            e.Graphics.FillRectangle(new SolidBrush(Color.FromKnownColor(KnownColor.ControlDarkDark)),
                 new Rectangle(0, 0, mPictureBox1.Width, mPictureBox1.Height));

            e.Graphics.FillRectangle(
                new HatchBrush(
                    HatchStyle.LargeCheckerBoard,
                    Color.FromKnownColor(KnownColor.ControlDark),
                    Color.FromKnownColor(KnownColor.ControlDarkDark)),
                new Rectangle(0, 0, mTextureSize, mTextureSize)
            );

            e.Graphics.DrawImage(mPictureBox2.Image, new Point(0, 0));
        }

        /// <summary>
        /// Select the Text Hinting (AntiAlias, ClearType, SingleBit)
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        /// <returns></returns>
        void mHintCombo_SelectionChangeCommitted(object sender, EventArgs e)
        {
            switch (mHintCombo.SelectedIndex)
            {
                case 0:
                    mTextHint = TextRenderingHint.AntiAliasGridFit;
                    break;
                case 1:
                    mTextHint = TextRenderingHint.AntiAlias;
                    break;
                case 2:
                    mTextHint = TextRenderingHint.SingleBitPerPixel;
                    break;
                case 3:
                    mTextHint = TextRenderingHint.SingleBitPerPixelGridFit;
                    break;
                case 4:
                    mTextHint = TextRenderingHint.ClearTypeGridFit;
                    break;
                default:
                    mTextHint = TextRenderingHint.AntiAliasGridFit;
                    mHintCombo.SelectedIndex = 0;
                    break;
            }

            BuildImagePlanes();
        }

        /// <summary>
        /// Paint the color of the Text on the button
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        /// <returns></returns>
        void mFGColorButton_Paint(object sender, PaintEventArgs e)
        {
            e.Graphics.Clear(Color.Transparent);
            int aWidth = mFGColorButton.Width;
            int aHeight = mFGColorButton.Height;

            if (mTextColor == mTransparentColor)
            {
                e.Graphics.FillRectangle(
                    new HatchBrush(
                        HatchStyle.LargeCheckerBoard,
                            Color.FromKnownColor(KnownColor.ControlDark),
                            Color.FromKnownColor(KnownColor.ControlDarkDark)),
                    new Rectangle(0, 0, aWidth, aHeight)
                );
            }
            else
            {
                e.Graphics.FillRectangle(new SolidBrush(mTextColor), 0, 0, aWidth, aHeight);
            }
        }

        /// <summary>
        /// Paint the Checkerboard Hatch pattern so we can tell when the BG is set to Transparent
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        /// <returns></returns>
        void mPictureBox1_Paint(object sender, PaintEventArgs e)
        {
            e.Graphics.FillRectangle(new SolidBrush(Color.FromKnownColor(KnownColor.ControlDarkDark)),
                new Rectangle(0, 0, mPictureBox1.Width, mPictureBox1.Height)
            );
            e.Graphics.FillRectangle(
                new HatchBrush(
                    HatchStyle.LargeCheckerBoard,
                    Color.FromKnownColor(KnownColor.ControlDark),
                    Color.FromKnownColor(KnownColor.ControlDarkDark)),
                 new Rectangle(0, 0, mTextureSize, mTextureSize)
            );


            e.Graphics.DrawImage(mPictureBox1.Image, new Point(0, 0));
        }

        /// <summary>
        /// Paint the Checkerboard Hatch pattern so we can tell when the BG is set to Transparent
        /// Otherwise paint the BG color.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        /// <returns></returns>
        void mBGColorButton_Paint(object sender, PaintEventArgs e)
        {
            e.Graphics.Clear(Color.Transparent);
            int aWidth = mBGColorButton.Width;
            int aHeight = mBGColorButton.Height;

            if (mBGColor == mTransparentColor && mBGColor != Color.White)
            {
                e.Graphics.FillRectangle(
                    new HatchBrush(
                        HatchStyle.LargeCheckerBoard,
                            Color.FromKnownColor(KnownColor.ControlDark),
                            Color.FromKnownColor(KnownColor.ControlDarkDark)),
                    new Rectangle(0, 0, aWidth, aHeight)
                );
            }
            else
            {
                e.Graphics.FillRectangle(new SolidBrush(mBGColor), 0, 0, aWidth, aHeight);
            }
        }


        /// <summary>
        /// Rehup the Font Size
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        /// <returns></returns>
        void OnFontSizeComboTextChanged(object sender, EventArgs e)
        {
            int aBackup = mCurrentFontSize;
            if (!Int32.TryParse(mSizeComboBox.Text.ToString(), out mCurrentFontSize))
            {
                mCurrentFontSize = aBackup;
                mSizeComboBox.Text = mCurrentFontSize.ToString();
            }
            else if (mCurrentFontSize < 1 || mCurrentFontSize > 100)
            {
                mCurrentFontSize = aBackup;
                mSizeComboBox.Text = mCurrentFontSize.ToString();
            }

            // rehup text metrics
            GetTextMetrics();

            BuildImagePlanes();

            mImageFileNameEditBox.Text = mFontComboBox.Text + mSizeComboBox.Text + ((mStyleComboBox.Text == "Regular") ? "" : mStyleComboBox.Text) + "_page%d";
            mDescriptorFileNameEditBox.Text = mFontComboBox.Text + mSizeComboBox.Text + ((mStyleComboBox.Text == "Regular") ? "" : mStyleComboBox.Text);

        }


        /// <summary>
        /// Select the Regular, Bold, Italics style
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        /// <returns></returns>
        void OnFontStyleComboSelectedIndexChanged(object sender, EventArgs e)
        {
            if (mStyleComboBox.Text == "") return;

            mCurrentStyle = FontStyle.Regular;
            if (mStyleComboBox.Text == "Italics")
                mCurrentStyle = FontStyle.Italic;
            if (mStyleComboBox.Text == "Bold")
                mCurrentStyle = FontStyle.Bold;

            // rehup text metrics
            GetTextMetrics();

            BuildImagePlanes();

            mImageFileNameEditBox.Text = mFontComboBox.Text + mSizeComboBox.Text + ((mStyleComboBox.Text == "Regular") ? "" : mStyleComboBox.Text) + "_page%d";
            mDescriptorFileNameEditBox.Text = mFontComboBox.Text + mSizeComboBox.Text + ((mStyleComboBox.Text == "Regular") ? "" : mStyleComboBox.Text);

        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        /// <returns></returns>
        void FontFace_SelectionChangeCommitted(object sender, EventArgs e)
        {
            mFontStyleList.Clear();

            foreach (FontFamily F in mInstalledFonts.Families)
            {
                if (F.Name == mFontComboBox.Text)
                {
                    if (F.IsStyleAvailable(FontStyle.Regular))
                        mFontStyleList.Add("Regular");
                    if (F.IsStyleAvailable(FontStyle.Bold))
                        mFontStyleList.Add("Bold");
                    if (F.IsStyleAvailable(FontStyle.Italic))
                        mFontStyleList.Add("Italics");

                    mCurrentFontFamily = F;
                    mCurrentStyle = (F.IsStyleAvailable(FontStyle.Regular)) ? FontStyle.Regular : 
                        ((F.IsStyleAvailable(FontStyle.Bold) ? FontStyle.Bold : (F.IsStyleAvailable(FontStyle.Italic) ? FontStyle.Italic : FontStyle.Regular)));

                    mStyleComboBox.DataSource = null;
                    mStyleComboBox.Items.Clear();
                    mStyleComboBox.DataSource = mFontStyleList;
                    mStyleComboBox.SelectedIndex = 0;
                    mStyleComboBox.Invalidate();


                    // rehup text metrics
                    GetTextMetrics();
                    mImageFileNameEditBox.Text = mFontComboBox.Text + mSizeComboBox.Text + ((mStyleComboBox.Text == "Regular") ? "" : mStyleComboBox.Text) + "_page%d";
                    mDescriptorFileNameEditBox.Text = mFontComboBox.Text + mSizeComboBox.Text + ((mStyleComboBox.Text == "Regular") ? "" : mStyleComboBox.Text);
                    BuildImagePlanes();
                    return;
                }
            }

            mImageFileNameEditBox.Text = mFontComboBox.Text + mSizeComboBox.Text + ((mStyleComboBox.Text == "Regular") ? "" : mStyleComboBox.Text) + "_page%d";
            mDescriptorFileNameEditBox.Text = mFontComboBox.Text + mSizeComboBox.Text + ((mStyleComboBox.Text == "Regular") ? "" : mStyleComboBox.Text);

            BuildImagePlanes();

        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }


        private void mFGColorButton_Click(object sender, EventArgs e)
        {
            mColorDialog.Color = mTextColor;
            if (mColorDialog.ShowDialog() == DialogResult.OK)
            {
                mTextColor = mColorDialog.Color;
                BuildImagePlanes();
            }
            mFGColorButton.Invalidate();
        }

        private void mBGColorButton_Click(object sender, EventArgs e)
        {
            if (mBGColor != mTransparentColor)
                mColorDialog.Color = mBGColor;
            if (mColorDialog.ShowDialog() == DialogResult.OK)
            {
                mBGColor = mColorDialog.Color;
                BuildImagePlanes();
            }

            mBGColorButton.Invalidate();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            mBGColor = mTransparentColor;
            mBGColorButton.Invalidate();
            BuildImagePlanes();
        }

        private void mPaddingControl_ValueChanged(object sender, EventArgs e)
        {
            mCharacterSpacing = (int)mPaddingControl.Value;
            BuildImagePlanes();
        }

        private void mDebugRects_CheckedChanged(object sender, EventArgs e)
        {
            BuildImagePlanes();
        }

        private void mCharMapButton_Click(object sender, EventArgs e)
        {
            if (mCharMapDlg.ShowDialog() == DialogResult.OK)
            {
                mCharacterEditBox.Text += mCharMapDlg.ReturnString;

                ArrayList aSortList = new ArrayList();
                foreach (char c in mCharacterEditBox.Text)
                    aSortList.Add(c);

                aSortList.Sort();

                String aFinalString = "";
                foreach (char c in aSortList)
                {
                    if (!aFinalString.Contains(c.ToString()))
                        aFinalString += c.ToString();
                }

                mCharacterEditBox.Text = aFinalString;
            }
        }

        private void mRightPaddingSpin_ValueChanged(object sender, EventArgs e)
        {
            mRightPadding = (int)mRightPaddingSpin.Value;
            BuildImagePlanes();
        }

        private void mLeftPaddingSpin_ValueChanged(object sender, EventArgs e)
        {
            mLeftPadding = (int)mLeftPaddingSpin.Value;
            BuildImagePlanes();
        }

        private void mTopPaddingSpin_ValueChanged(object sender, EventArgs e)
        {
            mTopPadding = (int)mTopPaddingSpin.Value;
            BuildImagePlanes();
        }

        private void mBottomPaddingSpin_ValueChanged(object sender, EventArgs e)
        {
            mBottomPadding = (int)mBottomPaddingSpin.Value;
            BuildImagePlanes();
        }

        private void mOpenTTF_Click(object sender, EventArgs e)
        {
            mOpenFileDialog.Title = "Open Font File.";
            mOpenFileDialog.Multiselect = false;
            mOpenFileDialog.DefaultExt = "ttf";
            mOpenFileDialog.Filter = "Font files (*.ttf)|*.ttf|All files (*.*)|*.*";
            if (mOpenFileDialog.ShowDialog() == DialogResult.OK)
            {
                FileInfo fi = new FileInfo(mOpenFileDialog.FileName);
                if(fi.Extension.ToLower() == ".ttf")
                    mInstalledFonts.AddFontFile(mOpenFileDialog.FileName);
                else if (MessageBox.Show(mOpenFileDialog.FileName + " will probably crash the program, try anyways?", "Warning", MessageBoxButtons.OKCancel) == DialogResult.OK)
                {
                    mInstalledFonts.AddFontFile(mOpenFileDialog.FileName);
                }

                PrivateFontCollection aTempPFC = new PrivateFontCollection();
                aTempPFC.AddFontFile(mOpenFileDialog.FileName);

                FontFamily aFamily = aTempPFC.Families[0];


                ArrayList aFontList = new ArrayList();
                foreach (FontFamily f in mInstalledFonts.Families)
                {
                    //if (f.IsStyleAvailable(FontStyle.Regular))
                        aFontList.Add(f.Name);
                }

                mFontComboBox.DataSource = aFontList;


                mCurrentFontFamily = mInstalledFonts.Families[0];
                int index = 0;
                foreach (FontFamily f in mInstalledFonts.Families)
                {
                    if (f.Name == aFamily.Name)
                    {
                        mCurrentFontFamily = f;
                        mFontComboBox.SelectedIndex = index;
                        break;
                    }
                    index++;
                }

                ///////////////////////////////////////////////////////////////////////////
                // Make default Selection
                //////////////////////////////////////////////////////////////////////////
                mFontComboBox.Text = mCurrentFontFamily.Name;


                mFontStyleList.Clear();

                foreach (FontFamily F in mInstalledFonts.Families)
                {
                    if (F.Name == mFontComboBox.Text)
                    {
                        if (F.IsStyleAvailable(FontStyle.Regular))
                            mFontStyleList.Add("Regular");
                        if (F.IsStyleAvailable(FontStyle.Bold))
                            mFontStyleList.Add("Bold");
                        if (F.IsStyleAvailable(FontStyle.Italic))
                            mFontStyleList.Add("Italics");

                        mCurrentFontFamily = F;
                        mCurrentStyle = (F.IsStyleAvailable(FontStyle.Regular)) ? FontStyle.Regular :
                            ((F.IsStyleAvailable(FontStyle.Bold) ? FontStyle.Bold : (F.IsStyleAvailable(FontStyle.Italic) ? FontStyle.Italic : FontStyle.Regular)));

                        mStyleComboBox.DataSource = null;
                        mStyleComboBox.Items.Clear();
                        mStyleComboBox.DataSource = mFontStyleList;
                        mStyleComboBox.SelectedIndex = 0;
                        mStyleComboBox.Invalidate();

                        // rehup text metrics
                        GetTextMetrics();
                        mImageFileNameEditBox.Text = mFontComboBox.Text + mSizeComboBox.Text + ((mStyleComboBox.Text == "Regular") ? "" : mStyleComboBox.Text) + "_page%d";
                        mDescriptorFileNameEditBox.Text = mFontComboBox.Text + mSizeComboBox.Text + ((mStyleComboBox.Text == "Regular") ? "" : mStyleComboBox.Text);
                        BuildImagePlanes();

                        return;
                    }
                }

                mImageFileNameEditBox.Text = mFontComboBox.Text + mSizeComboBox.Text + ((mStyleComboBox.Text == "Regular") ? "" : mStyleComboBox.Text) + "_page%d";
                mDescriptorFileNameEditBox.Text = mFontComboBox.Text + mSizeComboBox.Text + ((mStyleComboBox.Text == "Regular") ? "" : mStyleComboBox.Text);
                BuildImagePlanes();


             }

        } 

        private void mImportButton_Click(object sender, EventArgs e)
        {
            mOpenFileDialog.Title = "Import Characters from Text File.";
            mOpenFileDialog.Multiselect = false;
            mOpenFileDialog.DefaultExt = "txt";
            mOpenFileDialog.Filter = "Text files (*.txt)|*.txt|All files (*.*)|*.*";
            if (mOpenFileDialog.ShowDialog() == DialogResult.OK)
            {
                TextReader aTextReader = OpenStreamReaderWithEncoding(mOpenFileDialog.FileName);
                String aSortString = aTextReader.ReadToEnd() + mCharacterEditBox.Text;

                ArrayList aSortArray = new ArrayList();
                foreach (char c in aSortString)
                    if (c >= 0x0020)
                        aSortArray.Add(c);

                aSortArray.Sort();

                String aFinalString = "";
                foreach (char c in aSortArray)
                {
                    if (!aFinalString.Contains(c.ToString()))
                        aFinalString += c.ToString();
                }

                mCharacterEditBox.Text = aFinalString;
            }
        }


        private void mExportButton_Click(object sender, EventArgs e)
        {
            mSaveFileDialog.Title = "Export Characters to UTF8 text file.";
            mSaveFileDialog.CreatePrompt = false;
            mSaveFileDialog.DefaultExt = "txt";
            mSaveFileDialog.OverwritePrompt = true;
            mSaveFileDialog.Filter = "Text files (*.txt)|*.txt|All files (*.*)|*.*";
            if (mSaveFileDialog.ShowDialog() == DialogResult.OK)
            {
                TextWriter aTextWriter = new StreamWriter(mSaveFileDialog.FileName, false, Encoding.UTF8);
                aTextWriter.Write(mCharaterSet);
                aTextWriter.Close();
            }
        }

        private void comboBox2_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (mDescriptorFormatComboBox.SelectedIndex == 0)
            {
                mDescriptorEncodingComboBox.SelectedIndex = 1;
                mLoadingCodeTextBox.Text = "SexyFont* mFont = new SexyFont();\r\nif(!mFont.OpenFontFile(\"" + mDescriptorFileNameEditBox.Text + ".xml\"))\r\n     Fail();";
            }
            else if (mDescriptorEncodingComboBox.SelectedIndex == 1)
            {
                mDescriptorEncodingComboBox.SelectedIndex = 0;

                String aPageName = mImageFileNameEditBox.Text;
                aPageName = aPageName.Replace("%d", "0.") + mImageFormatComboBox.Text.ToLower();
                mLoadingCodeTextBox.Text = "KText* mFont = new KText();\r\nif(!mFont.loadFontTableAndBitmap(\"" + mDescriptorFileNameEditBox.Text + ".kfnt\", \"" + aPageName +"\"))\r\n     Fail();";
            }
        }

        private void mDescriptorFileNameEditBox_TextChanged(object sender, EventArgs e)
        {
            string trim = mDescriptorFileNameEditBox.Text.Replace(" ", "");
            trim = trim.Replace("\r", "");
            trim = trim.Replace("\n", "");
            trim = trim.Replace("\t", "");
            trim = trim.Replace(".", "");
            mDescriptorFileNameEditBox.Text = trim;

            if (mDescriptorFormatComboBox.SelectedIndex == 0)
            {
                mLoadingCodeTextBox.Text = "SexyFont* mFont = new SexyFont();\r\nif(!mFont.OpenFontFile(\"" + mDescriptorFileNameEditBox.Text + ".xml\"))\r\n     Fail();";
            }
            else if (mDescriptorEncodingComboBox.SelectedIndex == 1)
            {
                String aPageName = mImageFileNameEditBox.Text;
                aPageName = aPageName.Replace("%d", "0.") + mImageFormatComboBox.Text.ToLower();
                mLoadingCodeTextBox.Text = "KText* mFont = new KText();\r\nif(!mFont.loadFontTableAndBitmap(\"" + mDescriptorFileNameEditBox.Text + ".kfnt\", \"" + aPageName + "\"))\r\n     Fail();";
            }
        }

        private void mImageFileNameEditBox_TextChanged(object sender, EventArgs e)
        {
            string trim = mImageFileNameEditBox.Text.Replace(" ", "");
            trim = trim.Replace("\r", "");
            trim = trim.Replace("\n", "");
            trim = trim.Replace("\t", "");
            trim = trim.Replace(".", "");
            mImageFileNameEditBox.Text = trim;

            if (mDescriptorFormatComboBox.SelectedIndex == 0)
            {
                mLoadingCodeTextBox.Text = "SexyFont* mFont = new SexyFont();\r\nif(!mFont.OpenFontFile(\"" + mDescriptorFileNameEditBox.Text + ".xml\"))\r\n     Fail();";
            }
            else if (mDescriptorEncodingComboBox.SelectedIndex == 1)
            {
                String aPageName = mImageFileNameEditBox.Text;
                aPageName = aPageName.Replace("%d", "0.") + mImageFormatComboBox.Text.ToLower();
                mLoadingCodeTextBox.Text = "KText* mFont = new KText();\r\nif(!mFont.loadFontTableAndBitmap(\"" + mDescriptorFileNameEditBox.Text + ".kfnt\", \"" + aPageName + "\"))\r\n     Fail();";
            }
        }

        private void mKerningSpin_ValueChanged(object sender, EventArgs e)
        {
            mGlobalKerning = (float)mKerningSpin.Value;
        }

        private void mDirBrowseButton_Click(object sender, EventArgs e)
        {
            if (mFolderBrowserDialog.ShowDialog() == DialogResult.OK)
            {
                mDirectoryPathText.Text = mFolderBrowserDialog.SelectedPath + "\\";
            }
        }

        private void mImageFormatComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (mBGColor == mTransparentColor && mImageFormatComboBox.SelectedIndex != 0)
            {
                MessageBox.Show("Only PNG Supports Transparent Background Color.", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
        }

        private void mDescriptorEncodingComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (mDescriptorEncodingComboBox.SelectedIndex == 0)
            {
                foreach (char c in mCharaterSet)
                {
                    if (c > 0x00FF && mDescriptorFormatComboBox.SelectedIndex == 0)
                    {
                        MessageBox.Show("Your Font includes characters that can't be saved in ANSI formatting", "WARNING", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                        return;
                    }
                }
            }
        }
    }
}