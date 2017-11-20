using System;
using System.Windows;
using System.Windows.Controls;
using System.Threading;
using System.Windows.Threading;
using System.IO;
using myMath;

namespace fexout
{
    /// <summary>
    /// Логика взаимодействия для MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        //double[][][] lstFex;
        //forcePoints[] myFPs;
        forceTypes forceType;
        forceCycleTypes forceCycle;
        forceApps forceApp;

        public MainWindow()
        {
            System.Threading.Thread.CurrentThread.CurrentUICulture = new System.Globalization.CultureInfo("en-US");
            System.Threading.Thread.CurrentThread.CurrentCulture = new System.Globalization.CultureInfo("en-US");
            InitializeComponent();
            this.Loaded += OnLoaded; 
        }

        private void OnLoaded(object sender, RoutedEventArgs routedEventArgs)
        {
            comboBox_forceapp.ItemsSource = Enum.GetValues(typeof(forceApps));
            comboBox_forceapp.SelectedIndex = 0;
            comboBox_forcetype.ItemsSource = Enum.GetValues(typeof(forceTypes));
            comboBox_forcetype.SelectedIndex = 0;
            comboBox_forcecycle.ItemsSource = Enum.GetValues(typeof(forceCycleTypes));
            comboBox_forcecycle.SelectedIndex = 0;
        }
        
        private void launchthr()
        {
            //lstFex = null;
            GC.Collect();
            ApplyEffect(this);
            double freq = Convert.ToSingle(textBox_freq.Text.Replace('.',',')) * (double)Math.Pow(10, Convert.ToInt32(textBox_freqpop.Text.Replace('.', ',')));
            double V0 = Convert.ToSingle(textBox_V0.Text.Replace('.', ',')) * (double)Math.Pow(10, Convert.ToInt32(textBox_V0pop.Text.Replace('.', ',')));
            double k = Convert.ToSingle(textBox_k.Text.Replace('.', ',')) * (double)Math.Pow(10, Convert.ToInt32(textBox_kpop.Text.Replace('.', ',')));
            int numot = Convert.ToInt32(Convert.ToInt32(textBox_numot.Text) * Math.Pow(10, Convert.ToInt32(textBox_numotpop.Text)));
            double deltat = Convert.ToDouble(Convert.ToDouble(textBox_deltat.Text) * Math.Pow(10, Convert.ToInt32(textBox_deltatpop.Text)));
            
            string deffname = "";
            switch (forceType)
            {
                case forceTypes.zeros:
                    deffname = "zeros";
                    break;
                case forceTypes.k_x:
                    deffname = "kx";
                    break;
                case forceTypes.sin_x:
                    deffname = "sin";
                    break;
                case forceTypes.cos_x:
                    deffname = "cos";
                    break;
                case forceTypes.mod_sin:
                    deffname = "mod_sin";
                    break;
                case forceTypes.rev_mod_sin:
                    deffname = "min_mod_sin";
                    break;
                default:
                    deffname = "zeros";
                    break;
            }
            forcePlaces myforcePlaces = new forcePlaces();
            if ((bool)radioButton_forceplx.IsChecked)
            {
                myforcePlaces.x = true;
                deffname = deffname + ".X";
            }
            if ((bool)radioButton_forceply.IsChecked)
            {
                myforcePlaces.y = true;
                deffname = deffname + ".Y";
            }
            if ((bool)radioButton_forceplz.IsChecked)
            {
                myforcePlaces.z = true;
                deffname = deffname + ".Z";
            }
            deffname = deffname + "_dt=" + deltat.ToString("e") + "_numCounts=" + numot.ToString("e");
            forcePoints[] myFPs = new forcePoints[1] {
                new forcePoints { nump = 0, Amp = V0, k = k, freq = freq, forceCycle = forceCycle, forceType = forceType, myforcePlaces = myforcePlaces, forceApp = forceApp }
            };
            Thread thrdraw = new Thread(delegate ()
            {
                getFex(deffname, numot, deltat, myFPs);
            });
            thrdraw.Start();
        }
        private void getFex(string flname, int numot, double dt, forcePoints[] fp)
        {
            //initArr._1d(numot, ref t);
            //calc.t(numot, dt, ref t);
            double[][][] lstFex = new double[numot][][];
            initArr._3d(numot, fp.Length, 3, ref lstFex);
            calc.ExtLoad(fp, numot, dt, ref lstFex);
            Microsoft.Win32.SaveFileDialog dlg = new Microsoft.Win32.SaveFileDialog();

            dlg.FileName = flname; // Default file name
            dlg.DefaultExt = ".txt"; // Default file extension
            dlg.AddExtension = true;
            //dlg.Filter = "force file|*.ini"; // Filter files by extension
            // Show save file dialog box
            Nullable<bool> result = dlg.ShowDialog();
            // Process save file dialog box results
            if (result == true)
            {
                // Save document
                //string filename = dlg.FileName;
                Thread thrsave = new Thread(delegate ()
                {
                    savetxt(dlg.FileName, numot, dt, fp, lstFex);
                });
                thrsave.Start();
            }
        }


        private void btn_calc_Click(object sender, RoutedEventArgs e)
        {
            launchthr();
        }       
               
        
        private void savetxt(string fname, int numot, double dt, forcePoints[] myFPs, double[][][] lstFex)
        {
            //MainFuncs mnfunc = new MainFuncs();
            //mnfunc.saveStrFile(fname, lstFex, t.Length, t[0]);

            //StringBuilder outputLines = new StringBuilder();
            //outputLines.Append(t.Length + "\n");
            //outputLines.Append(t[1] + "\n");
            //outputLines.Append("loadstart" + ':' + '0' + ':' + "force" + "\n");
            //for (int i = 0; i < lstFex.Length; i++)
            //{
            //    outputLines.Append(lstFex[i][0][0].ToString() + ":" + lstFex[i][0][1].ToString() + ":" + lstFex[i][0][2].ToString() + "\n");
            //}
            //outputLines.Append("loadend" + "\n");
            //System.IO.File.WriteAllText(fname, outputLines.ToString());
            using (StreamWriter sw = new StreamWriter(fname))
            {
                sw.WriteLine(numot);
                sw.WriteLine(dt);
                for (int p = 0; p < myFPs.Length; p++)
                {
                    sw.WriteLine("loadstart" + ':' + '0' + ':' + myFPs[p].forceApp);
                    for (int i = 0; i < lstFex.Length; i++)
                    {
                        sw.WriteLine(lstFex[i][0][0].ToString() + ":" + lstFex[i][0][1].ToString() + ":" + lstFex[i][0][2].ToString());
                    }
                    sw.WriteLine("loadend");
                }
            }

            ClearEffect(this);
        }
        
        private void comboBox_forcecycle_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            forceCycle = (forceCycleTypes)Enum.Parse(typeof(forceCycleTypes), e.AddedItems[0].ToString());
        }

        private void comboBox_forcetype_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            forceType = (forceTypes)Enum.Parse(typeof(forceTypes), e.AddedItems[0].ToString());
        }

        private void ApplyEffect(Window win)
        {
            Dispatcher.BeginInvoke(new Action(() => {
                System.Windows.Media.Effects.BlurEffect objBlur = new System.Windows.Media.Effects.BlurEffect();
                objBlur.Radius = 4;
                mngr.Visibility = Visibility.Collapsed;
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
                overlayrect.Visibility = Visibility.Collapsed;
                overlayring.Visibility = Visibility.Collapsed;
            }));
        }

        private void comboBox_forceapp_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            forceApp = (forceApps)Enum.Parse(typeof(forceApps), e.AddedItems[0].ToString());
        }
    }
}
