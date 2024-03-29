using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Collections;

namespace KanjiFontBuilder
{
    public partial class CharMap : Form
    {
        ArrayList mCurrentCharacterArray;

        String mCurrentCharacterString;
        ArrayList mStringSets;
        String mBasicLatinString = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
        String mLatin1Supplement = " ¡¢£¤¥¦§¨©ª«¬­®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõö÷øùúûüýþÿ";
        String mLatinExtendedA = "ĀāĂăĄąĆćĈĉĊċČčĎďĐđĒēĔĕĖėĘęĚěĜĝĞğĠġĢģĤĥĦħĨĩĪīĬĭĮįİıĲĳĴĵĶķĸĹĺĻļĽľĿŀŁłŃńŅņŇňŉŊŋŌōŎŏŐőŒœŔŕŖŗŘřŚśŜŝŞşŠšŢţŤťŦŧŨũŪūŬŭŮůŰűŲųŴŵŶŷŸŹźŻżŽžſ";
        String mLatinExtendedB = "ƀƁƂƃƄƅƆƇƈƉƊƋƌƍƎƏƐƑƒƓƔƕƖƗƘƙƚƛƜƝƞƟƠơƢƣƤƥƦƧƨƩƪƫƬƭƮƯưƱƲƳƴƵƶƷƸƹƺƻƼƽƾƿǀǁǂǃǄǅǆǇǈǉǊǋǌǍǎǏǐǑǒǓǔǕǖǗǘǙǚǛǜǝǞǟǠǡǢǣǤǥǦǧǨǩǪǫǬǭǮǯǰǱǲǳǴǵǺǻǼǽǾǿȀȁȂȃȄȅȆȇȈȉȊȋȌȍȎȏȐȑȒȓȔȕȖȗ";
        String mIPAExtensions = "ɐɑɒɓɔɕɖɗɘəɚɛɜɝɞɟɠɡɢɣɤɥɦɧɨɩɪɫɬɭɮɯɰɱɲɳɴɵɶɷɸɹɺɻɼɽɾɿʀʁʂʃʄʅʆʇʈʉʊʋʌʍʎʏʐʑʒʓʔʕʖʗʘʙʚʛʜʝʞʟʠʡʢʣʤʥʦʧʨ";
        String mLatinExtendedAdditional = "ḀḁḂḃḄḅḆḇḈḉḊḋḌḍḎḏḐḑḒḓḔḕḖḗḘḙḚḛḜḝḞḟḠḡḢḣḤḥḦḧḨḩḪḫḬḭḮḯḰḱḲḳḴḵḶḷḸḹḺḻḼḽḾḿṀṁṂṃṄṅṆṇṈṉṊṋṌṍṎṏṐṑṒṓṔṕṖṗṘṙṚṛṜṝṞṟṠṡṢṣṤṥṦṧṨṩṪṫṬṭṮṯṰṱṲṳṴṵṶṷṸṹṺṻṼṽṾṿẀẁẂẃẄẅẆẇẈẉẊẋẌẍẎẏẐẑẒẓẔẕẖẗẘẙẚẛẠạẢảẤấẦầẨẩẪẫẬậẮắẰằẲẳẴẵẶặẸẹẺẻẼẽẾếỀềỂểỄễỆệỈỉỊịỌọỎỏỐốỒồỔổỖỗỘộỚớỜờỞởỠỡỢợỤụỦủỨứỪừỬửỮữỰựỲỳỴỵỶỷỸỹ";
        String mGreek = "ʹ͵ͺ;΄΅Ά·ΈΉΊΌΎΏΐΑΒΓΔΕΖΗΘΙΚΛΜΝΞΟΠΡΣΤΥΦΧΨΩΪΫάέήίΰαβγδεζηθικλμνξοπρςστυφχψωϊϋόύώϐϑϒϓϔϕϖϚϜϞϠϢϣϤϥϦϧϨϩϪϫϬϭϮϯϰϱϲϳ";
        String mGreekExtended = "ἀἁἂἃἄἅἆἇἈἉἊἋἌἍἎἏἐἑἒἓἔἕἘἙἚἛἜἝἠἡἢἣἤἥἦἧἨἩἪἫἬἭἮἯἰἱἲἳἴἵἶἷἸἹἺἻἼἽἾἿὀὁὂὃὄὅὈὉὊὋὌὍὐὑὒὓὔὕὖὗὙὛὝὟὠὡὢὣὤὥὦὧὨὩὪὫὬὭὮὯὰάὲέὴήὶίὸόὺύὼώᾀᾁᾂᾃᾄᾅᾆᾇᾈᾉᾊᾋᾌᾍᾎᾏᾐᾑᾒᾓᾔᾕᾖᾗᾘᾙᾚᾛᾜᾝᾞᾟᾠᾡᾢᾣᾤᾥᾦᾧᾨᾩᾪᾫᾬᾭᾮᾯᾰᾱᾲᾳᾴᾶᾷᾸᾹᾺΆᾼ᾽ι᾿῀῁ῂῃῄῆῇῈΈῊΉῌ῍῎῏ῐῑῒΐῖῗῘῙῚΊ῝῞῟ῠῡῢΰῤῥῦῧῨῩῪΎῬ῭΅`ῲῳῴῶῷῸΌῺΏῼ´῾";
        String mCyrillic = "ЁЂЃЄЅІЇЈЉЊЋЌЎЏАБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдежзийклмнопрстуфхцчшщъыьэюяёђѓєѕіїјљњћќўџѠѡѢѣѤѥѦѧѨѩѪѫѬѭѮѯѰѱѲѳѴѵѶѷѸѹѺѻѼѽѾѿҀҁ҂҃҄҅҆ҐґҒғҔҕҖҗҘҙҚқҜҝҞҟҠҡҢңҤҥҦҧҨҩҪҫҬҭҮүҰұҲҳҴҵҶҷҸҹҺһҼҽҾҿӀӁӂӃӄӇӈӋӌӐӑӒӓӔӕӖӗӘәӚӛӜӝӞӟӠӡӢӣӤӥӦӧӨөӪӫӮӯӰӱӲӳӴӵӸӹ";
        String mArabic = "،؛؟ءآأؤإئابةتثجحخدذرزسشصضطظعغـفقكلمنهوىيًٌٍَُِّْ٠١٢٣٤٥٦٧٨٩٪٫٬٭ٰٱٲٳٴٵٶٷٸٹٺٻټٽپٿڀځڂڃڄڅچڇڈډڊڋڌڍڎڏڐڑڒړڔڕږڗژڙښڛڜڝڞڟڠڡڢڣڤڥڦڧڨکڪګڬڭڮگڰڱڲڳڴڵڶڷںڻڼڽھۀہۂۃۄۅۆۇۈۉۊۋیۍێېۑےۓ۔ەۖۗۘۙۚۛۜ۝۞ۣ۟۠ۡۢۤۥۦۧۨ۩۪ۭ۫۬۰۱۲۳۴۵۶۷۸۹";

        public String ReturnString;

        public CharMap()
        {
            InitializeComponent();

            mCurrentCharacterString = mBasicLatinString;
            FillCharacterSetArray();

            mDataGridView.RowTemplate.Height = 40;
            mDataGridView.ColumnCount = 12;
            mDataGridView.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.Fill;
            mDataGridView.Rows.Add();
            mDataGridView.Rows.AddCopies(0, mCurrentCharacterArray.Count / 12);
            mDataGridView.CellValueNeeded += new DataGridViewCellValueEventHandler(mDataGridView_CellValueNeeded);
            mDataGridView.MouseMove += new MouseEventHandler(mDataGridView_MouseMove);
            mDataGridView.ClearSelection();

            mStringSets = new ArrayList();

            mListBox.SelectionMode = SelectionMode.One;
            mListBox.Items.Add("Basic Latin"); mStringSets.Add(mBasicLatinString);
            mListBox.SetItemChecked(0, true);
            mListBox.Items.Add("Latin-1 Supplement"); mStringSets.Add(mLatin1Supplement);
            mListBox.Items.Add("Latin Extended-A"); mStringSets.Add(mLatinExtendedA);
            mListBox.Items.Add("Latin Extended-B"); mStringSets.Add(mLatinExtendedB);
            mListBox.Items.Add("IPA Extensions"); mStringSets.Add(mIPAExtensions);
            mListBox.Items.Add("Latin Extended Additional"); mStringSets.Add(mLatinExtendedAdditional);
            mListBox.Items.Add("Greek"); mStringSets.Add(mGreek);
            mListBox.Items.Add("Greek Extended"); mStringSets.Add(mGreekExtended);
            mListBox.Items.Add("Cyrillic"); mStringSets.Add(mCyrillic);
            mListBox.Items.Add("Arabic"); mStringSets.Add(mArabic);
            mListBox.SelectedIndexChanged += new EventHandler(mListBox_SelectedIndexChanged);
            mListBox.ItemCheck += new ItemCheckEventHandler(mListBox_ItemCheck);

            this.FormClosing += new FormClosingEventHandler(CharMap_FormClosing);
        }

        void CharMap_FormClosing(object sender, FormClosingEventArgs e)
        {
            ReturnString = "";

            for (int i = 0; i < mListBox.CheckedIndices.Count; i++ )
            {
                int aCheckedIndex = mListBox.CheckedIndices[i];
                if (aCheckedIndex >= 0 && aCheckedIndex < mStringSets.Count)
                {
                    ReturnString += mStringSets[aCheckedIndex];
                }
            }
        }

        void mDataGridView_MouseMove(object sender, MouseEventArgs e)
        {
            DataGridView.HitTestInfo aHitTest = mDataGridView.HitTest(e.X, e.Y);

            int aCell = aHitTest.RowIndex * 12 + aHitTest.ColumnIndex;
            if (aHitTest.Type == DataGridViewHitTestType.Cell && aCell >= 0 && aCell < mCurrentCharacterArray.Count)
            {
                byte[] int32Bytes;
                char aChar = mCurrentCharacterArray[aHitTest.RowIndex * 12 + aHitTest.ColumnIndex].ToString()[0];
                int32Bytes = BitConverter.GetBytes(aChar);
                mMouseOverChar.Text = "Code:" + "U+" + String.Format("{0}{1}", padString(int32Bytes[1].ToString("X")),padString(int32Bytes[0].ToString("X")));
            }
        }

        private string padString(string s)
        {
            while (s.Length < 2) s = "0" + s;
            return s;
        }

        void mListBox_ItemCheck(object sender, ItemCheckEventArgs e)
        {
            if (e.NewValue == CheckState.Checked)
                mDataGridView.SelectAll();
            else if(e.NewValue == CheckState.Unchecked)
                mDataGridView.ClearSelection();
        }

        void mListBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (mListBox.SelectedIndex >= 0 && mListBox.SelectedIndex < mStringSets.Count)
                mCurrentCharacterString = mStringSets[mListBox.SelectedIndex] as String;

            FillCharacterSetArray();

            mDataGridView.Rows.Clear();
            mDataGridView.Rows.Add((mCurrentCharacterArray.Count / 12) + ((mCurrentCharacterArray.Count % 12 > 0) ? 1 : 0));
            if (mListBox.GetItemCheckState(mListBox.SelectedIndex) == CheckState.Checked)
                mDataGridView.SelectAll();
            else if(mListBox.GetItemCheckState(mListBox.SelectedIndex) == CheckState.Unchecked)
                 mDataGridView.ClearSelection();
       }

        private void FillCharacterSetArray()
        {
            mCurrentCharacterArray = new ArrayList();
            foreach (char c in mCurrentCharacterString)
                mCurrentCharacterArray.Add(c);
        }
        void mDataGridView_CellValueNeeded(object sender, DataGridViewCellValueEventArgs e)
        {
            e.Value = " ";
            int anIndex = e.RowIndex * 12 + e.ColumnIndex;
            if (anIndex >= 0 && anIndex < mCurrentCharacterArray.Count)
            {
                e.Value = mCurrentCharacterArray[anIndex].ToString();
            }
        }
    }
}