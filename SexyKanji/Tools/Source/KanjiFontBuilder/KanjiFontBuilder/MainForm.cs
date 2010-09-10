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

    public partial class MainForm : Form
    {
        /************************************************************************/
        /* Loaded Font                                                          */
        /************************************************************************/
        PrivateFontCollection           mInstalledFonts;
        TextRenderingHint               mTextHint;
        FontFamily                      mCurrentFontFamily;
        ArrayList                       mFontStyleList;
        FontStyle                       mCurrentStyle;
        /************************************************************************/
        /* Metrics                                                              */
        /************************************************************************/
        int                             mCurrentFontSize;
        int                             emHeight;
        int                             mDescent;
        int                             mAscent;
        int                             mLineSpacing;
        /************************************************************************/
        /* Rendering                                                            */
        /************************************************************************/
        Color                           mTextColor;
        Color                           mBGColor;
        Color                           mTransparentColor;
        Image[]                         mImagePages;
        int                             mCharacterSpacing;
        int                             mTextureSize;
        int                             mTopPadding;
        int                             mBottomPadding;
        int                             mRightPadding;
        int                             mLeftPadding;
        float                           mGlobalKerning;
        const int KTEXT_MAXPAGES = 8; // defaults to 8 inside Kanji (KText.h)
        /************************************************************************/
        /* Draw String                                                          */
        /************************************************************************/
        String                          mCharaterSet;
        CharMap                         mCharMapDlg;

        ToolTip                         mToolTips;
        /************************************************************************/
        /*                                                                      */
        /************************************************************************/
        ProgressDlg                     mProgressDlg;

        public MainForm()
        {
            InitializeComponent();

            //////////////////////////////////////////////////////////////////////////
            // default character set
            //////////////////////////////////////////////////////////////////////////
            mCharaterSet = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

            //////////////////////////////////////////////////////////////////////////
            // Use System fonts to start with
            //////////////////////////////////////////////////////////////////////////
            FillFontDropDownCombo();
            
            //////////////////////////////////////////////////////////////////////////
            // Image planes we draw characters to
            //////////////////////////////////////////////////////////////////////////
            mTextureSize = 512;
            InitImagePages();
            
            //////////////////////////////////////////////////////////////////////////
            // Regular, Bold, Italics
            //////////////////////////////////////////////////////////////////////////
            FillFontStyleDropDownCombo();

            GetTextMetrics();

            FillFontSizeDropDownCombo();

            FillTextureSizeCombo();

            mTextColor = Color.White;
            mTransparentColor = Color.FromArgb(0, 255, 255 ,255);
            mBGColor = mTransparentColor;

            SetupOutputPaneDefaults();
            
            //////////////////////////////////////////////////////////////////////////
            // Paint Handlers for Picture Boxes and Color control
            //////////////////////////////////////////////////////////////////////////
            RegisterPaintHandlers();

            CreateEditBoxContextMenu();
            mCharMapDlg = new CharMap(); // persistent

            FillCharacterEditBox();

            mTopPadding = 0;
            mBottomPadding = 0;
            mLeftPadding = 0;
            mRightPadding = 0;
            mGlobalKerning = 0;

            FillTextHintingDropDownCombo();
            mCharacterSpacing = 2;

            BuildImagePlanes();

            CreateToolTips();

            mProgressDlg = new ProgressDlg();
            backgroundWorker1.DoWork += new DoWorkEventHandler(backgroundWorker1_DoWork);
            backgroundWorker1.RunWorkerCompleted += new RunWorkerCompletedEventHandler(backgroundWorker1_RunWorkerCompleted);
            backgroundWorker1.WorkerReportsProgress = true;
            backgroundWorker1.ProgressChanged += new ProgressChangedEventHandler(backgroundWorker1_ProgressChanged);
        }

        void backgroundWorker1_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            mProgressDlg.progressBar1.Value = e.ProgressPercentage;
        }

        void backgroundWorker1_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            mProgressDlg.Hide();
            MessageBox.Show("Font Generation Complete.", "Kanji Font Builder", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        void backgroundWorker1_DoWork(object sender, DoWorkEventArgs e)
        {
            /************************************************************************/
            /* Step 1: Ensure the Image planes are built without debug Rects        */
            /************************************************************************/
            //////////////////////////////////////////////////////////////////////////
            // This canvas is to help get metrics
            //////////////////////////////////////////////////////////////////////////
            Graphics canvas = Graphics.FromImage(mImagePages[0]);
            canvas.TextContrast = 0;
            Font aDrawFont = new Font(mFontComboBox.Text, mCurrentFontSize, mCurrentStyle, GraphicsUnit.Pixel);
            canvas.TextRenderingHint = mTextHint;

            //////////////////////////////////////////////////////////////////////////
            // Some padding to stay away from the edges of the textures
            float fX = 2;
            float fY = 2;
            // Measurements for placing characters
            int nTexWidth = 0;
            int nMaxRowHeight = 0;
            int nBaseline = mCurrentFontSize * mAscent / emHeight; // Ascent: http://msdn.microsoft.com/en-us/library/xwf9s90b.aspx
            int nDescent = mCurrentFontSize * mDescent / emHeight; // Descent: http://msdn.microsoft.com/en-us/library/xwf9s90b.aspx
            int nLineSpacing = mCurrentFontSize * mLineSpacing / emHeight; // Descent: http://msdn.microsoft.com/en-us/library/xwf9s90b.aspx
            int nChars = 0; // 
            int nTextures = 0; // current texture for placement
            float fMaxWidth = mTextureSize;  // of texture/image plane

            float CHARACTERSPACING = (float)mCharacterSpacing;
            

            //////////////////////////////////////////////////////////////////////////
            // For getting Kerning offsets using Graphics::MeasureCharacterRanges
            //////////////////////////////////////////////////////////////////////////
            StringFormat aStringFormat = new StringFormat(StringFormat.GenericTypographic);
            //aStringFormat.Alignment = StringAlignment.Center;
            aStringFormat.LineAlignment = StringAlignment.Center;
            aStringFormat.Trimming = StringTrimming.None;

            CharacterRange[] characterRanges = { new CharacterRange(0, 1) };
            aStringFormat.SetMeasurableCharacterRanges(characterRanges);
            //////////////////////////////////////////////////////////////////////////

            //////////////////////////////////////////////////////////////////////////
            // Test Image to scan for pixel boundaries
            //////////////////////////////////////////////////////////////////////////
            const int aGlyphTextureSize = 128;
            Bitmap aGlyphImage = new Bitmap(aGlyphTextureSize, aGlyphTextureSize, PixelFormat.Format32bppArgb);
            Graphics aGlyphGraphic = Graphics.FromImage(aGlyphImage);

            //////////////////////////////////////////////////////////////////////////
            // output array
            ArrayList emittedChars = new ArrayList();

            float aTotalCount = (float)mCharaterSet.Length * 3;
            float aCurrentCount = 0;

            foreach (Char c in mCharaterSet)
            {
                // Get an idea of character width/height 
                float aCharacterWidth = TextRenderer.MeasureText(c.ToString(), aDrawFont, new Size(0, 0), TextFormatFlags.VerticalCenter | TextFormatFlags.HorizontalCenter | TextFormatFlags.Internal | TextFormatFlags.NoPadding | TextFormatFlags.NoFullWidthCharacterBreak).Width;
                float aCharacterHeight = aDrawFont.Height + mTopPadding + mBottomPadding;

                // Get a more precise idea of the width
                RectangleF aTestRect2 = new RectangleF(0, 0, (float)aCharacterWidth, (float)aCharacterHeight);
                // Warning: space character is zero width from this measurement, use '-' instead.
                Region[] strRegions2 = canvas.MeasureCharacterRanges((c == ' ') ? "-" : c.ToString(), aDrawFont, aTestRect2, aStringFormat);
                if (strRegions2.Length > 0)
                {
                    RectangleF measureRect1 = strRegions2[0].GetBounds(canvas);
                    aCharacterWidth = measureRect1.Width + mRightPadding + mLeftPadding;
                }

                float advance = aCharacterWidth;
                //////////////////////////////////////////////////////////////////////////
                // This next section is very slow, but is necessary to get the best 
                // advancing posible.  Basically, it measures the exact pixel widths of
                // the glyph. (JPOAG)
                //////////////////////////////////////////////////////////////////////////
                {
                    Char aTestChar = (c == ' ') ? '-' : c;
                    aGlyphGraphic.Clear(Color.Black);
                    aGlyphGraphic.FillRectangle(new SolidBrush(Color.Black), 0, 0, aGlyphTextureSize, aGlyphTextureSize);
                    aGlyphGraphic.DrawString(aTestChar.ToString(), aDrawFont, new SolidBrush(Color.White), new RectangleF(0, 0, aGlyphTextureSize, aGlyphTextureSize), aStringFormat);

                    int aLastX = 0;
                    int aFirstX = aGlyphTextureSize;
                    Color aTestColor = Color.FromArgb(0xFF, 0, 0, 0);

                    for (int aX = 0; aX < aGlyphTextureSize; aX++)
                    {
                        for (int aY = 0; aY < aGlyphTextureSize; aY++)
                        {
                            if (aGlyphImage.GetPixel(aX, aY) != aTestColor)
                            {
                                if (aX > aLastX)
                                    aLastX = aX;
                                if (aX < aFirstX)
                                    aFirstX = aX;
                            }
                        }
                    }

                    if ((aLastX - aFirstX) > 0)
                    {
                        advance = (aLastX - aFirstX + 1 > 0) ? aLastX - aFirstX + 1 : 0;
                    }

                }

                if ((fX + aCharacterWidth) > fMaxWidth) // Row full, go to next line
                {
                    if (((long)fX) > nTexWidth)
                        nTexWidth = (int)fX;

                    fX = 2.0f;
                    fY += (float)nMaxRowHeight + CHARACTERSPACING;
                }

                if ((fY + nMaxRowHeight) > fMaxWidth) // Image Graphic full, go to next Page
                {
                    if (nTextures < (KTEXT_MAXPAGES - 1))
                    {
                        /* Create another texture */
                        nTextures++;
                        fX = 2;
                        fY = 2;
                        nMaxRowHeight = 0;
                    }
                    else
                    {
                        /* Out of texture pages; stop adding characters */
                        // K_LOG("KTrueText: %s: maximum graphics per font reached, some characters will be missing, WARNING.", _szFileName);
                        break;
                    }
                }

                KTrueTextChar lpChar = new KTrueTextChar();

                /* Store location */
                lpChar.c = c;
                lpChar.nGraphic = nTextures;
                lpChar.sx1 = fX;
                lpChar.sy1 = fY;
                lpChar.sx2 = fX + (float)aCharacterWidth;
                lpChar.sy2 = fY + (float)aCharacterHeight;
                lpChar.fXOffset = (float)-mLeftPadding;
                lpChar.fYOffset = (float)-mTopPadding;
                lpChar.fWidth = (float)advance; // advancing width
                nChars++;

                emittedChars.Add(lpChar);

                if (aCharacterHeight > nMaxRowHeight)
                    nMaxRowHeight = ((int)aCharacterHeight + (int)CHARACTERSPACING);

                // advance placement cursor
                fX += (float)(aCharacterWidth + CHARACTERSPACING);

                aCurrentCount++;
                backgroundWorker1.ReportProgress((int)(aCurrentCount / aTotalCount * 100.0f));
            }

            int nGeneratedPlanes = nTextures + 1;

            //////////////////////////////////////////////////////////////////////////
            // Clear unused image planes so they don't draw in picture boxes
            //////////////////////////////////////////////////////////////////////////
            for (int n = nTextures + 1; n < KTEXT_MAXPAGES; n++)
            {
                Graphics aTempCanvas = Graphics.FromImage(mImagePages[n]);
                aTempCanvas.Clear(Color.Transparent);
            }

            //////////////////////////////////////////////////////////////////////////
            // Prepare Image Planes
            //////////////////////////////////////////////////////////////////////////
            SetupGraphicPlanes();

            Graphics[] aGraphicArray = new Graphics[KTEXT_MAXPAGES];
            for (int i = 0; i < KTEXT_MAXPAGES; i++)
            {
                aGraphicArray[i] = Graphics.FromImage(mImagePages[i]);
                aGraphicArray[i].TextContrast = 0;
                aGraphicArray[i].TextRenderingHint = mTextHint;
            }

            /* Second pass: render characters to pixel buffer */
            foreach (KTrueTextChar lpChar in emittedChars)
            {
                if (lpChar.nGraphic >= KTEXT_MAXPAGES || lpChar.nGraphic < 0)
                    continue;

                RectangleF aBoundsRect = new RectangleF(lpChar.sx1, lpChar.sy1, (lpChar.sx2 - lpChar.sx1), (lpChar.sy2 - lpChar.sy1));
                RectangleF aDrawRect = new RectangleF(lpChar.sx1 + mLeftPadding, lpChar.sy1 + mTopPadding, (lpChar.sx2 - lpChar.sx1) - mLeftPadding - mRightPadding, (lpChar.sy2 - lpChar.sy1) - mTopPadding - mBottomPadding);
                aGraphicArray[lpChar.nGraphic].DrawString(lpChar.c.ToString(), aDrawFont, new SolidBrush(mTextColor), aDrawRect, aStringFormat);

                aCurrentCount++;
                backgroundWorker1.ReportProgress((int)(aCurrentCount / aTotalCount * 100.0f));
            }

            /************************************************************************/
            /* Part B: Save Image Planes to Files                                   */
            /************************************************************************/

            //////////////////////////////////////////////////////////////////////////
            // First: Validate Output Directory.
            //////////////////////////////////////////////////////////////////////////
            String exePath = Assembly.GetExecutingAssembly().Location;
            int lastSlash = exePath.LastIndexOf('\\');
            exePath = exePath.Remove(lastSlash + 1);

            String aSavePath = exePath;
            if (mDirectoryPathText.Text != "" && mDirectoryPathText.Text != ".\\")
                aSavePath = mDirectoryPathText.Text;

            String aFileExt = ".png";
            ImageFormat anImageFormat = ImageFormat.Png;

            switch (mImageFormatComboBox.SelectedIndex)
            {
                case 0:
                    aFileExt = ".png";
                    anImageFormat = ImageFormat.Png;
                    break;
                case 1:
                    aFileExt = ".bmp";
                    anImageFormat = ImageFormat.Bmp;
                    break;
                case 2:
                    aFileExt = ".jpg";
                    anImageFormat = ImageFormat.Jpeg;
                    break;
                case 3:
                    aFileExt = ".tiff";
                    anImageFormat = ImageFormat.Tiff;
                    break;
                case 4:
                    aFileExt = ".gif";
                    anImageFormat = ImageFormat.Gif;
                    break;
            }

            String anImageFileName = mImageFileNameEditBox.Text;
            if (!anImageFileName.Contains("%d"))
                anImageFileName += "_page%d";
            anImageFileName = anImageFileName.Replace("%d", "{0:d}");

            ArrayList anImageFileNameArray = new ArrayList();

            if (mWriteImagesCheckbox.Checked)
            {
                for (int i = 0; i < nGeneratedPlanes; i++)
                {
                    String anImageOutputFileName = String.Format("{0}{1}{2}", aSavePath, String.Format(anImageFileName, i), aFileExt);
                    anImageFileNameArray.Add(String.Format("{0}{1}", String.Format(anImageFileName, i), aFileExt));
                    FileStream anOutFile = new FileStream(anImageOutputFileName, FileMode.Create);
                    mImagePages[i].Save(anOutFile, anImageFormat);
                    anOutFile.Close();
                }
            }

            /************************************************************************/
            /* Step the Last: Save Descriptor                                       */
            /************************************************************************/
            if (mWriteDescriptorCheckBox.Checked)
            {
                Encoding aTextEncoding = Encoding.UTF8;
                switch (mDescriptorEncodingComboBox.SelectedIndex)
                {
                    case 0:
                        aTextEncoding = Encoding.ASCII;
                        break;
                    case 1:
                        aTextEncoding = Encoding.UTF8;
                        break;
                    case 2:
                        aTextEncoding = Encoding.Unicode;
                        break;
                    case 3:
                        aTextEncoding = Encoding.Unicode;
                        break;
                    case 4:
                        aTextEncoding = Encoding.BigEndianUnicode;
                        break;
                }

                // Sexy XML Format
                if(mDescriptorFormatComboBox.SelectedIndex == 0)
                {
                    String anXMLFileName = aSavePath + mDescriptorFileNameEditBox.Text + ".xml";
                    XmlTextWriter aWriter = new XmlTextWriter(anXMLFileName, aTextEncoding);
                    aWriter.IndentChar = '\t';
                    aWriter.Formatting = Formatting.Indented;

                    aWriter.WriteComment("Sexy-Kanji Font AutoGenerated by Kanji Font Builder (JPoag)");
                    aWriter.WriteStartElement("KanjiFont");
                    aWriter.WriteAttributeString("name", mCurrentFontFamily.Name);
                    aWriter.WriteAttributeString("style", mCurrentStyle.ToString());
                    aWriter.WriteAttributeString("size", mCurrentFontSize.ToString());
                    aWriter.WriteAttributeString("kerning", mGlobalKerning.ToString());
                    aWriter.WriteAttributeString("ascent", nBaseline.ToString());
                    aWriter.WriteAttributeString("descent", nDescent.ToString());
                    aWriter.WriteAttributeString("height", aDrawFont.Height.ToString());
                    aWriter.WriteAttributeString("line_spacing", nLineSpacing.ToString());

                    aWriter.WriteStartElement("ImagePlanes");
                    aWriter.WriteAttributeString("count", nGeneratedPlanes.ToString());

                    for (int i = 0; i < anImageFileNameArray.Count; i++)
                    {
                        aWriter.WriteStartElement("Plane");
                        aWriter.WriteAttributeString("id", i.ToString());
                        aWriter.WriteAttributeString("path", anImageFileNameArray[i].ToString());
                        aWriter.WriteEndElement(/*"Plane"*/);
                    }

                    aWriter.WriteEndElement(/*ImagePlanes*/);

                    if (emittedChars.Count > 0)
                    {
                        aWriter.WriteStartElement("FontTable");
                        foreach (KTrueTextChar lpChar in emittedChars)
                        {
                            aWriter.WriteStartElement("Char");
                            aWriter.WriteAttributeString("code", lpChar.c.ToString());
                            aWriter.WriteAttributeString("graphic_id", lpChar.nGraphic.ToString());
                            aWriter.WriteAttributeString("sx1", lpChar.sx1.ToString());
                            aWriter.WriteAttributeString("sy1", lpChar.sy1.ToString());
                            aWriter.WriteAttributeString("sx2", lpChar.sx2.ToString());
                            aWriter.WriteAttributeString("sy2", lpChar.sy2.ToString());
                            aWriter.WriteAttributeString("advance", lpChar.fWidth.ToString());
                            aWriter.WriteAttributeString("x_offset", lpChar.fXOffset.ToString());
                            aWriter.WriteAttributeString("y_offset", lpChar.fYOffset.ToString());
                            aWriter.WriteEndElement(/*Char*/);

                            aCurrentCount++;
                            backgroundWorker1.ReportProgress((int)(aCurrentCount / aTotalCount * 100.0f));
                        }
                        aWriter.WriteEndElement(/*FontTable*/);
                    }
                    aWriter.WriteEndElement(/*"KanjiFont"*/);

                    aWriter.Close();
                }
                // Kanji Binary format
                else if (mDescriptorFormatComboBox.SelectedIndex == 1)
                {
                    String aKFNTFileName = aSavePath + mDescriptorFileNameEditBox.Text + ".kfnt";
                    StreamWriter aWriter = new StreamWriter(aKFNTFileName, false, Encoding.ASCII);

                    aWriter.WriteLine("%ktext 1 {0:d} {1:d}", nGeneratedPlanes, emittedChars.Count);

                    foreach (KTrueTextChar lpChar in emittedChars)
                    {
                        aWriter.WriteLine("{0:d} {1:d} {2:d} {3:d} {4:d} {5:d} {6:d} {7:d} {8:d} {9:d}",
                            (ushort)lpChar.c, (long) lpChar.sx1, (long) lpChar.sy1, (long) lpChar.sx2, (long) lpChar.sy2,
                            (long) lpChar.fWidth, (long) lpChar.sy2 - (long) lpChar.sy1, lpChar.nGraphic,
                            (long) lpChar.fXOffset, (long) lpChar.fYOffset);
                        aCurrentCount++;
                        backgroundWorker1.ReportProgress((int)(aCurrentCount / aTotalCount * 100.0f));
                    }

                    aWriter.Close();
                }
            } // END: if (mWriteDescriptorCheckBox.Checked)

        }// END: backgroundWorker1_DoWork
       
        /// <summary>
        /// Ascent, Descent, emHeight, LineSpacing
        /// </summary>
        /// <returns></returns>
        private void GetTextMetrics()
        {
            //////////////////////////////////////////////////////////////////////////
            // Get Text Metrics
            //////////////////////////////////////////////////////////////////////////
            emHeight = mCurrentFontFamily.GetEmHeight(mCurrentStyle);
            mAscent = mCurrentFontFamily.GetCellAscent(mCurrentStyle);
            mDescent = mCurrentFontFamily.GetCellDescent(mCurrentStyle);
            mLineSpacing = mCurrentFontFamily.GetLineSpacing(mCurrentStyle);
        }

        /// <summary>
        /// Image Planes are the images we draw the font to.
        /// </summary>
        /// <returns></returns>
        private void InitImagePages()
        {
            //////////////////////////////////////////////////////////////////////////
            // Create Display Image
            //////////////////////////////////////////////////////////////////////////
            mImagePages = new Image[KTEXT_MAXPAGES];
            for (int i = 0; i < KTEXT_MAXPAGES; i++ )
                mImagePages[i] = new Bitmap(mTextureSize, mTextureSize, PixelFormat.Format32bppArgb);
        }
 
        /// <summary>
        /// This is to Wipe the slates clean
        /// </summary>
        /// <returns></returns>
        void SetupGraphicPlanes()
        {
            foreach (Image i in mImagePages)
            {
                Graphics aTempCanvas = Graphics.FromImage(i);
                aTempCanvas.Clear(mTextColor); // this is a trick!
                aTempCanvas.Clear(mBGColor);
                aTempCanvas.FillRectangle(new SolidBrush(mBGColor), new Rectangle(0, 0, mTextureSize, mTextureSize));
           }
        }

        /// <summary>
        /// Build the Image Planes: Render the Fonts to the Images.
        /// </summary>
        /// <returns></returns>
        private void BuildImagePlanes()
        {
            //////////////////////////////////////////////////////////////////////////
            // This canvas is to help get metrics
            //////////////////////////////////////////////////////////////////////////
            Graphics canvas = Graphics.FromImage(mImagePages[0]);
            canvas.TextContrast = 0;
            Font aDrawFont = new Font(mFontComboBox.Text, mCurrentFontSize, mCurrentStyle, GraphicsUnit.Pixel);
            canvas.TextRenderingHint = mTextHint;

            //////////////////////////////////////////////////////////////////////////
            // Some padding to stay away from the edges of the textures
            float fX = 2;
            float fY = 2;
            // Measurements for placing characters
            int nTexWidth = 0;
            int nMaxRowHeight = 0;
            int nBaseline = mCurrentFontSize * mAscent / emHeight; // mAscent: http://msdn.microsoft.com/en-us/library/xwf9s90b.aspx
            int nChars = 0; // 
            int nTextures = 0; // current texture for placement
            float fMaxWidth = mTextureSize;  // of texture/image plane

            float CHARACTERSPACING = (float)mCharacterSpacing;

            //////////////////////////////////////////////////////////////////////////
            // For getting Kerning offsets using Graphics::MeasureCharacterRanges
            //////////////////////////////////////////////////////////////////////////
            StringFormat aStringFormat = new StringFormat(StringFormat.GenericTypographic);
            //aStringFormat.Alignment = StringAlignment.Center;
            aStringFormat.LineAlignment = StringAlignment.Center;
            aStringFormat.Trimming = StringTrimming.None;

            CharacterRange[] characterRanges = { new CharacterRange(0, 1) };
            aStringFormat.SetMeasurableCharacterRanges(characterRanges);
            //////////////////////////////////////////////////////////////////////////

            // output array
            ArrayList emittedChars = new ArrayList();
            
            foreach (Char c in mCharaterSet)
            {
                // Get an idea of character width/height 
                float aCharacterWidth = TextRenderer.MeasureText(c.ToString(), aDrawFont, new Size(0, 0), TextFormatFlags.VerticalCenter | TextFormatFlags.HorizontalCenter | TextFormatFlags.Internal | TextFormatFlags.NoPadding | TextFormatFlags.NoFullWidthCharacterBreak).Width;
                float aCharacterHeight = aDrawFont.Height + mTopPadding + mBottomPadding;

                // Get a more precise idea of the width
                RectangleF aTestRect2 = new RectangleF(0, 0, (float)aCharacterWidth, (float)aCharacterHeight);
                // Warning: space character is zero width from this measurement, use '-' instead.
                Region[] strRegions2 = canvas.MeasureCharacterRanges((c == ' ') ? "-" : c.ToString(), aDrawFont, aTestRect2, aStringFormat);
                if (strRegions2.Length > 0)
                {
                    RectangleF measureRect1 = strRegions2[0].GetBounds(canvas);
                    aCharacterWidth = measureRect1.Width + mRightPadding + mLeftPadding;
                }

                if ((fX + aCharacterWidth) > fMaxWidth) // Row full, go to next line
                {
                    if (((long)fX) > nTexWidth)
                        nTexWidth = (int)fX;

                    fX = 2.0f;
                    fY += (float)nMaxRowHeight + CHARACTERSPACING;
                }

                if ((fY + nMaxRowHeight) > fMaxWidth) // Image Graphic full, go to next Page
                {
                    if (nTextures < (KTEXT_MAXPAGES - 1))
                    {
                        /* Create another texture */
                        nTextures++;
                        fX = 2;
                        fY = 2;
                        nMaxRowHeight = 0;
                    }
                    else
                    {
                        /* Out of texture pages; stop adding characters */
                       // K_LOG("KTrueText: %s: maximum graphics per font reached, some characters will be missing, WARNING.", _szFileName);
                        break;
                    }
                }

                KTrueTextChar lpChar = new KTrueTextChar();

                /* Store location */
                lpChar.c = c;
                lpChar.nGraphic = nTextures;
                lpChar.sx1 = fX;
                lpChar.sy1 = fY;
                lpChar.sx2 = fX + (float)aCharacterWidth;
                lpChar.sy2 = fY + (float)aCharacterHeight;
                lpChar.fXOffset = (float)-mLeftPadding;
                lpChar.fYOffset = (float)- mTopPadding;
                lpChar.fWidth = (float)aCharacterWidth - mLeftPadding - mRightPadding; // advancing width
                nChars++;

                emittedChars.Add(lpChar);

                if (aCharacterHeight > nMaxRowHeight)
                    nMaxRowHeight = ((int)aCharacterHeight + (int)CHARACTERSPACING);

                // advance placement cursor
                fX += (float)(aCharacterWidth + CHARACTERSPACING);

            }

            //////////////////////////////////////////////////////////////////////////
            // Clear unused image planes so they don't draw in picture boxes
            //////////////////////////////////////////////////////////////////////////
            for (int n = nTextures + 1; n < KTEXT_MAXPAGES; n++)
            {
                Graphics aTempCanvas = Graphics.FromImage(mImagePages[n]);
                aTempCanvas.Clear(Color.Transparent);
            }

            //////////////////////////////////////////////////////////////////////////
            // Prepare Image Planes
            //////////////////////////////////////////////////////////////////////////
            SetupGraphicPlanes();

            Graphics[] aGraphicArray = new Graphics[KTEXT_MAXPAGES];
            for (int i = 0; i < KTEXT_MAXPAGES; i++)
            {
                aGraphicArray[i] = Graphics.FromImage(mImagePages[i]);
                aGraphicArray[i].TextContrast = 0;
                aGraphicArray[i].TextRenderingHint = mTextHint;
            }

            /* Second pass: render characters to pixel buffer */
            foreach (KTrueTextChar lpChar in emittedChars)
            {
                if (lpChar.nGraphic >= KTEXT_MAXPAGES || lpChar.nGraphic < 0)
                    continue;

                RectangleF aBoundsRect = new RectangleF(lpChar.sx1, lpChar.sy1, (lpChar.sx2 - lpChar.sx1), (lpChar.sy2 - lpChar.sy1));
                RectangleF aDrawRect = new RectangleF(lpChar.sx1 + mLeftPadding, lpChar.sy1 + mTopPadding, (lpChar.sx2 - lpChar.sx1) - mLeftPadding - mRightPadding, (lpChar.sy2 - lpChar.sy1) - mTopPadding - mBottomPadding);
                aGraphicArray[lpChar.nGraphic].DrawString(lpChar.c.ToString(), aDrawFont, new SolidBrush(mTextColor), aDrawRect, aStringFormat);

                // Pink Rectangle: DrawString Bounds
                if(mDebugRects.Checked)
                    aGraphicArray[lpChar.nGraphic].DrawRectangle(Pens.Fuchsia, Rectangle.Round(aBoundsRect));
                
                Region[] strRegions = aGraphicArray[lpChar.nGraphic].MeasureCharacterRanges(lpChar.c.ToString(), aDrawFont, aDrawRect, aStringFormat);
                RectangleF measureRect1 = strRegions[0].GetBounds(canvas);

                // Yellow Rectangle: Tight Bounding box for character
                if (mDebugRects.Checked)
                    aGraphicArray[lpChar.nGraphic].DrawRectangle(Pens.Yellow, Rectangle.Round(measureRect1));
            }

            //////////////////////////////////////////////////////////////////////////
            // should cause redraw
            //////////////////////////////////////////////////////////////////////////
            mPictureBox1.Image = mImagePages[0];
            mPictureBox2.Image = mImagePages[1];
            mPictureBox3.Image = mImagePages[2];
            mPictureBox4.Image = mImagePages[3];
            mPictureBox5.Image = mImagePages[4];
            mPictureBox6.Image = mImagePages[5];
            mPictureBox7.Image = mImagePages[6];
            mPictureBox8.Image = mImagePages[7];
        }
 
        private void OnBuildFont(object sender, EventArgs e)
        {
            mProgressDlg.Show(this);
            mProgressDlg.progressBar1.Value = 0;
            backgroundWorker1.RunWorkerAsync();
        }
        
        /// <summary>
        /// Detects the byte order mark of a file and returns
        /// an appropriate encoding for the file.
        /// </summary>
        /// <param name="srcFile"></param>
        /// <returns></returns>
        public static Encoding GetFileEncoding(string srcFile)
        {
            // *** Use Default of Encoding.Default (Ansi CodePage)
            Encoding enc = Encoding.Default;

            // *** Detect byte order mark if any - otherwise assume default
            byte[] buffer = new byte[5];
            FileStream file = new FileStream(srcFile, FileMode.Open);
            file.Read(buffer, 0, 5);
            file.Close();

            if (buffer[0] == 0xef && buffer[1] == 0xbb && buffer[2] == 0xbf)
                enc = Encoding.UTF8;
            else if (buffer[0] == 0xfe && buffer[1] == 0xff)
                enc = Encoding.Unicode;
            else if (buffer[0] == 0xff && buffer[1] == 0xfe)
                enc = Encoding.BigEndianUnicode;
            else if (buffer[0] == 0 && buffer[1] == 0 && buffer[2] == 0xfe && buffer[3] == 0xff)
                enc = Encoding.UTF32;
            else if (buffer[0] == 0x2b && buffer[1] == 0x2f && buffer[2] == 0x76)
                enc = Encoding.UTF7;

            return enc;
        }

        /// <summary>
        /// Opens a stream reader with the appropriate text encoding applied.
        /// </summary>
        /// <param name="srcFile"></param>
        public static StreamReader OpenStreamReaderWithEncoding(string srcFile)
        {
            Encoding enc = GetFileEncoding(srcFile);
            return new StreamReader(srcFile, enc);
        }


    }
	
	    /** One entry in the character table - internal */
    class KTrueTextChar 
    {
       public char c;           /**< Character; 0 terminates the character table */
       public float sx1;                 /**< X coordinate of top, left of character bitmap in graphic */
       public float sy1;                 /**< Y coordinate of top, left of character bitmap in graphic */
       public float sx2;                 /**< X coordinate of bottom, right of character bitmap in graphic */
       public float sy2;                 /**< Y coordinate of bottom, right of character bitmap in graphic */
       public float fWidth;              /**< How much to increase screen X coordinate by, after drawing this character, in pixels */
       public long nGraphic;             /**< Index of graphic page that this character is on */
       public float fXOffset;            /**< Offset to be applied to screen X coordinate when drawing this character, in pixels */
       public float fYOffset;            /**< Offset to be applied to screen Y coordinate when drawing this character, in pixels */
    } ;

}