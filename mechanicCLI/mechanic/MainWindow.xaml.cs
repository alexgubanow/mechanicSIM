using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Threading;
using System.Windows.Threading;
using System.Collections.ObjectModel;
using System.Collections.Specialized;
using ZedGraph;
using System.Drawing;
using Microsoft.Win32;
using System.Runtime.InteropServices;
using System.IO;
using myMatch;
using static myMatch.MainFuncs;

namespace mechanic
{
    /// <summary>
    /// Логика взаимодействия для MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        [DllImport("Kernel32")]
        public static extern void AllocConsole();

        [DllImport("Kernel32")]
        public static extern void FreeConsole();
        [DllImport("Kernel32.dll")]
        public static extern bool AttachConsole(int processId);

        public static List<plotxy> plotxys = new List<plotxy>();

        bool IsConsoleOut = true;
        Models Modeltype = Models.liner;
        Retypes Retype = Retypes.stat;
        IntegrSchems IntegShematype = IntegrSchems.euler;
        AllowSinCos AllowSinCostype = AllowSinCos.no;
        MaterialModels MaterialModeltype = MaterialModels.normal;
        CalcTypes CalcType = CalcTypes.dynamical;

        //double[] time;
        //double[][][] lstF;
        //double[][][] lstFem1;
        //double[][][] lstFep1;
        //double[][][] lsta;
        //double[][][] lstv;
        //double[][][] lstvAN;
        //double[][][] lstdispla;
        //double[][][] lstcoords;
        //double[][][] lstb;
        //MainFuncs.point[] points;

        Thread thrdraw;
        enum MaterialsDB
        {
            blank,
            metal,
            chorda,
            artChorda,
            glassPart
        }

        public MainWindow()
        {
            System.Threading.Thread.CurrentThread.CurrentUICulture = new System.Globalization.CultureInfo("en-US");
            System.Threading.Thread.CurrentThread.CurrentCulture = new System.Globalization.CultureInfo("en-US");

            InitializeComponent();
            this.Loaded += OnLoaded; 
        }

        private void OnLoaded(object sender, RoutedEventArgs routedEventArgs)
        {
            comboBox_integr.ItemsSource = Enum.GetValues(typeof(IntegrSchems));
            comboBox_integr.SelectedIndex = 0;
            comboBox_models.ItemsSource = Enum.GetValues(typeof(Models));
            comboBox_models.SelectedIndex = 0;
            comboBox_Re.ItemsSource = Enum.GetValues(typeof(Retypes));
            comboBox_Re.SelectedIndex = 0;
            comboBox_mater.ItemsSource = Enum.GetValues(typeof(MaterialsDB));
            comboBox_mater.SelectedIndex = 0;
            checkBoxIsconsole.IsChecked = IsConsoleOut;
        }

        private void analytic(double Dp, double qp, double Viscos, double pp)
        {

        }

        private void getAngular()
        { }

        
        private void launchthr()
        {
            foreach (plotxy pl in plotxys)
            { pl.Close(); }
            
            point[] points;
            GC.Collect();
            double ro = get_val_pop(textBox_ro.Text, textBox_ropop.Text);
            double L = get_val_pop(textBox_L.Text, textBox_Lpop.Text);
            double h = get_val_pop(textBox_h.Text, textBox_hpop.Text);
            double b = get_val_pop(textBox_b.Text, textBox_bpop.Text);
            double D = get_val_pop(textBox_D.Text, textBox_Dpop.Text);
            double v0 = get_val_pop(textBox_v0.Text, textBox_v0pop.Text);
            double vamp = get_val_pop(textBox_vamp.Text, textBox_vamppop.Text);
            int numP = Convert.ToInt32(textBox_numP.Text);
            double elastic = get_val_pop(textBox_elas.Text, textBox_elaspop.Text);
            double Re = get_val_pop(textBox_Renum.Text, textBox_Renumpop.Text);
            string flname = textBox_file.Text;
            ApplyEffect(this);
            thrdraw = new Thread(delegate ()
            {
                MainFuncs myfuncs = new MainFuncs();
                if (flname != "")
                {
                    //string[] strLoads = new string[1];
                    //int res = myfuncs.getExtStr(flname, ref strLoads);
                    //if (res != 0)
                    //{
                    //    MessageBox.Show("Fail, Err#" + res);
                    //}
                    int[] findIndex = new int[1];
                    //res = myfuncs.getnumExtLoad(strLoads, ref findIndex);
                    //if (res != 0)
                    //{
                    //    MessageBox.Show("Fail, Err#" + res);
                    //}

                    if (findIndex.Length > 0)
                    {
                        //int numP_ext = Convert.ToInt32(strLoads[findIndex[findIndex.Length - 1]].Split(':')[1]);
                        //if (numP < numP_ext)
                        //{
                        //    MessageBoxResult messboxres_numP = MessageBox.Show("Выбрано меньше точек чем в файле эксперимента\r\n\r\nВзять число точек из файла?", "numP < countp", MessageBoxButton.YesNo);
                        //    switch (messboxres_numP)
                        //    {
                        //        case MessageBoxResult.No:
                        //            return;
                        //        case MessageBoxResult.Yes:
                        //            numP = numP_ext;
                        //            break;
                        //    }
                        //}
                        FileStream fs = new FileStream(flname, FileMode.Open);
                        StreamReader sr = new StreamReader(fs);
                        string strRead = sr.ReadLine();
                        int countsExt = Convert.ToInt32(strRead);
                        double[] time = new double[countsExt];
                        double[][][] lstF = new double[countsExt][][];
                        double[][][] lstFem1 = new double[countsExt][][];
                        double[][][] lstFep1 = new double[countsExt][][];
                        double[][][] lsta = new double[countsExt][][];
                        double[][][] lstv = new double[countsExt][][];
                        double[][][] lstvAN = new double[countsExt][][];
                        double[][][] lstdispla = new double[countsExt][][];
                        double[][][] lstcoords = new double[countsExt][][];
                        double[][][] lstb = new double[countsExt][][];
                        strRead = sr.ReadLine();
                        double dtExt = Convert.ToDouble(strRead);
                        string[] strload = sr.ReadLine().Split(':');
                        points = new point[numP];
                        for (int np = 0; np < numP; np++)
                        {
                            points[np] = new point();
                            points[np].ExtLoad = ExtLoadType.none;
                        }
                        points[Convert.ToInt32(strload[1])].ExtLoad = myfuncs.get_pinfo(strload[2]);
                        sr.Close();
                        fs.Close();
                        calc.t(countsExt, dtExt, ref time);                        
                        initarr._1d(countsExt, numP, 3, ref lstF);
                        myfuncs.initarr(countsExt, numP, 3, ref lstdispla);
                        myfuncs.initarr(countsExt, numP, 3, ref lstcoords);
                        myfuncs.initCoordsArr(L / numP, points, ref lstcoords);
                        myfuncs.initarr(countsExt, numP, 3, ref lstFem1);
                        myfuncs.initarr(countsExt, numP, 3, ref lstFep1);
                        myfuncs.initarr(countsExt, numP, 3, ref lsta);
                        myfuncs.initarr(countsExt, numP, 3, ref lstv);
                        myfuncs.initarr(countsExt, numP, 3, ref lstvAN);
                        myfuncs.initarr(countsExt, numP, 3, ref lstb);
                        
                        if (CalcType == MainFuncs.CalcTypes.statical)
                        {
                            //myfuncs.calcMovementStatic(flname, points, Modeltype, IntegShematype, CalcType, MaterialModeltype,
                            //    L, A, Iz, ro, numP, countsExt, dtExt, elastic, v0,
                            //    ref lstF, ref lstFep1, ref lstFem1, ref lsta, ref lstv, ref lstdispla, ref lstcoords);
                        }
                        else
                        {
                            myfuncs.calcMovement(flname, points, Modeltype, IntegShematype, CalcType, MaterialModeltype, Retype, IsConsoleOut,
                                L, b, h, ro, numP, countsExt, elastic, v0, vamp, D, Re, time,
                                ref lstF, ref lstFep1, ref lstFem1, ref lsta, ref lstb, ref lstv, ref lstdispla, ref lstcoords, ref lstvAN);
                        }
                        
                        Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            plotxy plot = new plotxy();
                            //plot.Show();
                            //plot.dt = dtExt;
                            //plot.time = time;
                            //plot.coords = lstdispla;
                            //plot.draw2d(MainFuncs.axistype.t, MainFuncs.axistype.x, numP - 1, 1, false, false, "x/t(displ)", "Time", "X", false, System.Drawing.Color.Red);
                            //plot.initControls();
                            //plotxys.Add(plot);
                            //plot = new plotxy();
                            //plot.Show();
                            //plot.dt = dtExt;
                            //plot.time = time;
                            //plot.coords = lstcoords;
                            //plot.draw2d(MainFuncs.axistype.t, MainFuncs.axistype.x, numP - 1, 1, false, false, "x/t(coords)", "Time", "Y", false, System.Drawing.Color.Red);
                            //plot.initControls();
                            //plotxys.Add(plot);

                            //plot = new plotxy();
                            //plot.Show();
                            //plot.draw2d(MainFuncs.axistype.t, MainFuncs.axistype.x, lsta, time, numP - 1, 1, false, false, "a(x)", "t", "a");
                            //plotxys.Add(plot);

                            //plot = new plotxy();
                            //plot.Show();
                            //plot.dt = dtExt;
                            //plot.time = time;
                            //plot.coords = lstv;
                            //plot.draw2d(MainFuncs.axistype.t, MainFuncs.axistype.x, numP - 1, 1, false, false, "v(x)", "t", "v", false, System.Drawing.Color.Red);
                            //plot.initControls();
                            //plotxys.Add(plot);
                            if (Modeltype == MainFuncs.Models.particle)
                            {
                                //plot = new plotxy();
                                //plot.Show();
                                //plot.dt = dtExt;
                                //plot.time = time;
                                //plot.coords = lstvAN;
                                //plot.draw2d(MainFuncs.axistype.t, MainFuncs.axistype.x, numP - 1, 1, false, false, "vAN(x)", "t", "vAN", false, System.Drawing.Color.Red);
                                //plot.initControls();
                                //plotxys.Add(plot);

                                plot = new plotxy();
                                plot.Show();
                                plot.dt = dtExt;
                                plot.time = time;
                                plot.coords = lstvAN;
                                plot.draw2d(MainFuncs.axistype.t, MainFuncs.axistype.x, numP - 1, 1, false, false, "vAN(x)", "t", "vAN", false, System.Drawing.Color.Red);
                                plot.initControls();
                                plot.coords = lstv;
                                plot.draw2d(MainFuncs.axistype.t, MainFuncs.axistype.x, numP - 1, 1, false, false, "v(x)", "t", "v", true, System.Drawing.Color.Blue);
                                plot.initControls();
                                plotxys.Add(plot);
                            }
                            //plot = new plotxy();
                            //plot.Show();
                            //plot.draw2d(MainFuncs.axistype.t, MainFuncs.axistype.x, lstF, time, numP, 1, false, false, "Ftot(x)", "t", "N");
                            //plotxys.Add(plot);
                            //plot = new plotxy();
                            //plot.Show();
                            //plot.draw2d(MainFuncs.axistype.t, MainFuncs.axistype.x, lstFep1, time, numP, 1, false, false, "Fep1(x)", "t", "N");
                            //plotxys.Add(plot);
                        }));
                        ClearEffect(this);
                        //GC.Collect();
                    }
                }
                else
                {
                    MessageBox.Show("В файле нет нагруженных точек");
                }
            });
            thrdraw.Start();
        }


        private double get_val_pop(string val, string pop)
        {
            return Convert.ToDouble(Convert.ToDouble(val) * Math.Pow(10, Convert.ToInt32(pop)));
        }
        //private Int64 get_val_pop64(string val, string pop)
        //{
        //    return Convert.ToInt64(Convert.ToDouble(val) * Math.Pow(10, Convert.ToInt32(pop)));
        //}
        private void btn_calc_Click(object sender, RoutedEventArgs e)
        {
            launchthr();
        }
            
        private void ApplyEffect(Window win)
        {
            Dispatcher.BeginInvoke(new Action(() => {
                System.Windows.Media.Effects.BlurEffect objBlur = new System.Windows.Media.Effects.BlurEffect();
                objBlur.Radius = 4;
                mngr.Visibility = Visibility.Collapsed;
                btn_abrt.Visibility = Visibility.Visible;
                overlayrect.Visibility = Visibility.Visible;
                overlayring.Visibility = Visibility.Visible;
                win.Effect = objBlur;
            }));
        }
        private void ClearEffect(Window win)
        {
            Dispatcher.BeginInvoke(new Action(() => {
                win.Effect = null;
                mngr.Visibility = Visibility.Visible;
                btn_abrt.Visibility = Visibility.Collapsed;
                overlayrect.Visibility = Visibility.Collapsed;
                overlayring.Visibility = Visibility.Collapsed;
            }));
        }

        private void exitMenuItem_Click(object sender, RoutedEventArgs e)
        {
            Application.Current.Shutdown();
        }

        private void btn_open_Click(object sender, RoutedEventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Multiselect = false;
            if (openFileDialog.ShowDialog() == true)
            {
                textBox_file.Text = openFileDialog.FileName;
                FileStream fs = new FileStream(openFileDialog.FileName, FileMode.Open);
                StreamReader sr = new StreamReader(fs);
                int countsExt = Convert.ToInt32(sr.ReadLine());
                double dtExt = Convert.ToDouble(sr.ReadLine());
                fileInfo.Text = string.Format(
                    "File info:" + "\r\n"
                    + "deltat: " + dtExt.ToString("e") + "\r\n"
                    + "counts: " + countsExt.ToString("e"));
                sr.Close();
                fs.Close();
            }
        }
               

        private void radioButton_metal_Checked(object sender, RoutedEventArgs e)
        {
            textBox_numP.Text = 5.ToString();
            textBox_ro.Text = 77.ToString();
            textBox_ropop.Text = 2.ToString();
            textBox_L.Text = 135.ToString();
            textBox_Lpop.Text = (-4).ToString();
            textBox_h.Text = 0.ToString();
            textBox_hpop.Text = 0.ToString();
            textBox_b.Text = 0.ToString();
            textBox_bpop.Text = 0.ToString();
            textBox_D.Text = 71.ToString();
            textBox_Dpop.Text = (-5).ToString();
            textBox_elas.Text = 215.ToString();
            textBox_elaspop.Text = 9.ToString();
        }

        private void radioButton_chorda_Checked(object sender, RoutedEventArgs e)
        {
            textBox_numP.Text = 3.ToString();
            textBox_ro.Text = 104.ToString();
            textBox_ropop.Text = 1.ToString();
            textBox_L.Text = 135.ToString();
            textBox_Lpop.Text = (-4).ToString();
            textBox_h.Text = 0.ToString();
            textBox_hpop.Text = 0.ToString();
            textBox_b.Text = 0.ToString();
            textBox_bpop.Text = 0.ToString();
            textBox_D.Text = 71.ToString();
            textBox_Dpop.Text = (-5).ToString();
            textBox_elas.Text = 5.ToString();
            textBox_elaspop.Text = 8.ToString();
        }

        private void radioButton_artchorda_Checked(object sender, RoutedEventArgs e)
        {
            textBox_numP.Text = 5.ToString();
            textBox_ro.Text = 220.ToString();
            textBox_ropop.Text = 1.ToString();
            textBox_L.Text = 135.ToString();
            textBox_Lpop.Text = (-4).ToString();
            textBox_h.Text = 0.ToString();
            textBox_hpop.Text = 0.ToString();
            textBox_b.Text = 0.ToString();
            textBox_bpop.Text = 0.ToString();
            textBox_D.Text = 15.ToString();
            textBox_Dpop.Text = (-5).ToString();
            textBox_elas.Text = 75.ToString();
            textBox_elaspop.Text = 8.ToString();
        }
        

        private void radioButton_sinyes_Checked(object sender, RoutedEventArgs e)
        {
            AllowSinCostype = AllowSinCos.yes;
        }

        private void radioButton_sinno_Checked(object sender, RoutedEventArgs e)
        {
            AllowSinCostype = AllowSinCos.no;
        }
        
        private void MenuItem_Click(object sender, RoutedEventArgs e)
        {
            //SaveFileDialog saveFileDialog = new SaveFileDialog();
            //saveFileDialog.FileName = @"outcoords";
            //saveFileDialog.DefaultExt = "txt";
            //saveFileDialog.Filter = "Text file |*.txt";
            //if (saveFileDialog.ShowDialog() == true)
            //{
            //    using (System.IO.StreamWriter file =
            //   new System.IO.StreamWriter(saveFileDialog.FileName, false))
            //    {
            //        file.WriteLine("i" + "\t" + "np" + "\t" + "t" + "\t" + "a" + "\t" + "v"+ "\t" + "Ux" + "\t" + "Fex" + "\t" + "Fem1" + "\t" + "Fep1" + "\t" + "Ftot" );
            //        for (int i = 0; i < time.Length / 4; i++)
            //        {
            //            for (int np = 0; np < 3; np++)
            //            {
            //                file.WriteLine(i + "\t" + np + "\t" + time[i] + "\t" + lsta[i][np][0] + "\t" + lstv[i][np][0] + "\t" + lstdispla[i][np][0] + "\t" + 0 + "\t" + lstFem1[i][np][0] + "\t" + lstFep1[i][np][0] + "\t" + lstF[i][np][0]);
            //            }
            //        }
            //    }
            //}
        }

        private void radioButton_normalmod_Checked(object sender, RoutedEventArgs e)
        {
            MaterialModeltype = MainFuncs.MaterialModels.normal;
        }

        private void radioButton_n0mod_Checked(object sender, RoutedEventArgs e)
        {
            MaterialModeltype = MainFuncs.MaterialModels.n0;
        }
        
        private void btn_abrt_Click(object sender, RoutedEventArgs e)
        {
            thrdraw.Abort();
            GC.Collect();
            ClearEffect(this);
        }

        private void radioButton_dynamic_Checked(object sender, RoutedEventArgs e)
        {
            CalcType = MainFuncs.CalcTypes.dynamical;
        }

        private void radioButton_static_Checked(object sender, RoutedEventArgs e)
        {
            CalcType = MainFuncs.CalcTypes.statical;
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            foreach (plotxy pl in plotxys)
            { pl.Close(); }
        }
        
        private void comboBox_integr_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            IntegShematype = (MainFuncs.IntegrSchems)Enum.Parse(typeof(MainFuncs.IntegrSchems), e.AddedItems[0].ToString());
        }
        
        private void comboBox_models_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            Modeltype = (MainFuncs.Models)Enum.Parse(typeof(MainFuncs.Models), e.AddedItems[0].ToString());
            if (Modeltype == Models.particle)
            {
                comboRe.Visibility = Visibility.Visible;
            }
            else
            {
                comboRe.Visibility = Visibility.Collapsed;
            }
        }

        private void comboBox_mater_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            switch (e.AddedItems[0])
            {
                case MaterialsDB.blank:
                    textBox_numP.Text = 0.ToString();
                    textBox_ro.Text = 0.ToString();
                    textBox_ropop.Text = 0.ToString();
                    textBox_L.Text = 0.ToString();
                    textBox_Lpop.Text = 0.ToString();
                    textBox_h.Text = 0.ToString();
                    textBox_hpop.Text = 0.ToString();
                    textBox_b.Text = 0.ToString();
                    textBox_bpop.Text = 0.ToString();
                    textBox_D.Text = 0.ToString();
                    textBox_Dpop.Text = 0.ToString();
                    textBox_elas.Text = 0.ToString();
                    textBox_elaspop.Text = 0.ToString();
                    break;
                case MaterialsDB.metal:
                    textBox_numP.Text = 5.ToString();
                    textBox_ro.Text = 77.ToString();
                    textBox_ropop.Text = 2.ToString();
                    textBox_L.Text = 135.ToString();
                    textBox_Lpop.Text = (-4).ToString();
                    textBox_h.Text = 0.ToString();
                    textBox_hpop.Text = 0.ToString();
                    textBox_b.Text = 0.ToString();
                    textBox_bpop.Text = 0.ToString();
                    textBox_D.Text = 71.ToString();
                    textBox_Dpop.Text = (-5).ToString();
                    textBox_elas.Text = 215.ToString();
                    textBox_elaspop.Text = 9.ToString();
                    break;
                case MaterialsDB.chorda:
                    textBox_numP.Text = 3.ToString();
                    textBox_ro.Text = 104.ToString();
                    textBox_ropop.Text = 1.ToString();
                    textBox_L.Text = 135.ToString();
                    textBox_Lpop.Text = (-4).ToString();
                    textBox_h.Text = 0.ToString();
                    textBox_hpop.Text = 0.ToString();
                    textBox_b.Text = 0.ToString();
                    textBox_bpop.Text = 0.ToString();
                    textBox_D.Text = 71.ToString();
                    textBox_Dpop.Text = (-5).ToString();
                    textBox_elas.Text = 5.ToString();
                    textBox_elaspop.Text = 8.ToString();
                    break;
                case MaterialsDB.artChorda:
                    textBox_numP.Text = 5.ToString();
                    textBox_ro.Text = 220.ToString();
                    textBox_ropop.Text = 1.ToString();
                    textBox_L.Text = 135.ToString();
                    textBox_Lpop.Text = (-4).ToString();
                    textBox_h.Text = 0.ToString();
                    textBox_hpop.Text = 0.ToString();
                    textBox_b.Text = 0.ToString();
                    textBox_bpop.Text = 0.ToString();
                    textBox_D.Text = 15.ToString();
                    textBox_Dpop.Text = (-5).ToString();
                    textBox_elas.Text = 75.ToString();
                    textBox_elaspop.Text = 8.ToString();
                    break;
                case MaterialsDB.glassPart:
                    textBox_numP.Text = 2.ToString();
                    textBox_ro.Text = 24.ToString();
                    textBox_ropop.Text = 2.ToString();
                    textBox_L.Text = 8.ToString();
                    textBox_Lpop.Text = (-6).ToString();
                    textBox_h.Text = 0.ToString();
                    textBox_hpop.Text = 0.ToString();
                    textBox_b.Text = 0.ToString();
                    textBox_bpop.Text = 0.ToString();
                    textBox_D.Text = 8.ToString();
                    textBox_Dpop.Text = (-6).ToString();
                    textBox_elas.Text = 0.ToString();
                    textBox_elaspop.Text = 0.ToString();
                    textBox_vamp.Text = (0.44).ToString();
                    break;
            }
        }

        private void checkBoxIscos_Checked(object sender, RoutedEventArgs e)
        {
            AllowSinCostype = (bool)checkBoxIscos.IsChecked ? AllowSinCos.yes : AllowSinCos.no;
        }

        private void radioButtonFile_Checked(object sender, RoutedEventArgs e)
        {

        }

        private void radioButtonFormula_Checked(object sender, RoutedEventArgs e)
        {

        }

        private void checkBoxIsconsole_Checked(object sender, RoutedEventArgs e)
        {
            IsConsoleOut = (bool)checkBoxIsconsole.IsChecked;
        }

        private void comboBox_Re_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            Retype = (Retypes)Enum.Parse(typeof(Retypes), e.AddedItems[0].ToString());
            if (Retype == Retypes.dyn)
            { numberRe.Visibility = Visibility.Collapsed; }
            else
            { numberRe.Visibility = Visibility.Visible; }
        }
    }
}
