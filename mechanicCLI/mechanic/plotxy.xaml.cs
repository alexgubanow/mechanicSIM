using System;
using System.Collections.Generic;
using System.Windows;
using ZedGraph;
using System.Runtime.InteropServices;
using System.Drawing;
using System.ComponentModel;
using myMatch;
//using FFTWSharp;

namespace mechanic
{
    /// <summary>
    /// Interaction logic for plotxy.xaml
    /// </summary>
    public partial class plotxy : Window
    {
        allProps myAllProps;
        public double[] time;
        public double[][][] coords;
        public double dt;
        //public double expFreq = 3000;

        public double maxOnStart, maxOnEnd, minOnStart, minOnEnd;

        //IntPtr pin, pout;
        //float[] fin, fout;
        //public IntPtr fplan1;

        [DllImport("shlwapi.dll")]
        static extern int ColorHLSToRGB(int H, int L, int S);
        //public ObservableCollection<CheckedListItem<Customer>> Customers { get; set; }
        public plotxy()
        {
            InitializeComponent();
            //Customers = new ObservableCollection<CheckedListItem<Customer>>();
            myAllProps = new allProps() {
                rangeSliderMinValue = 0,
                rangeSliderCurrentMinValue = 0,
                rangeSliderMaxValue = 100,
                rangeSliderCurrentMaxValue = 100
            };
            DataContext = myAllProps;

            //mainPlot.GraphPane.Legend.IsVisible = false;
            mainPlot.IsShowPointValues = true;
            mainPlot.PointValueEvent += new ZedGraphControl.PointValueHandler(plotter_PointValueEvent);

        }

        public class allProps : INotifyPropertyChanged
        {
            private double
                _rangeSliderMinValue = 0,
                _rangeSliderMaxValue = 0,
                _rangeSliderCurrentMinValue = 0,
                _rangeSliderCurrentMaxValue = 0;

            public event PropertyChangedEventHandler PropertyChanged;

            public void RaisePropertyChanged(string propertyName)
            {
                if (PropertyChanged != null)
                    PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
            }

            public double rangeSliderMinValue
            {
                get { return _rangeSliderMinValue; }
                set
                { _rangeSliderMinValue = value; RaisePropertyChanged("rangeSliderMinValue"); }
            }

            public double rangeSliderMaxValue
            {
                get { return _rangeSliderMaxValue; }
                set
                { _rangeSliderMaxValue = value; RaisePropertyChanged("rangeSliderMaxValue"); }
            }
            public double rangeSliderCurrentMinValue
            {
                get { return _rangeSliderCurrentMinValue; }
                set
                { _rangeSliderCurrentMinValue = value; RaisePropertyChanged("rangeSliderCurrentMinValue"); }
            }
            public double rangeSliderCurrentMaxValue
            {
                get { return _rangeSliderCurrentMaxValue; }
                set { _rangeSliderCurrentMaxValue = value; RaisePropertyChanged("rangeSliderCurrentMaxValue"); }
            }
        }

        public void draw2d(axistype xaxis, axistype yaxis, int numP, int step,
            bool xzeroline, bool yzeroline, string title, string xtitle, string ytitle, bool isAdd, System.Drawing.Color clrCurve)
        {
            GraphPane pane = mainPlot.GraphPane;
            pane.Title.Text = title;
            pane.XAxis.Title.Text = xtitle;
            pane.YAxis.Title.Text = ytitle;
            pane.XAxis.MajorGrid.IsZeroLine = xzeroline;
            pane.YAxis.MajorGrid.IsZeroLine = yzeroline;
            pane.IsFontsScaled = false;
            if (!isAdd)
            {
                pane.CurveList.Clear();
            }
            for (int np = 0; np < numP; np += step)
            {
                double[][] lstpp = new double[2][];
                switch (xaxis)
                {
                    case axistype.t:
                        double[] yt = new double[time.Length];
                        for (int i = 0; i < time.Length; i++)
                        {
                            switch (yaxis)
                            {
                                case axistype.x:
                                    yt[i] = coords[i][np][0];
                                    break;
                                case axistype.y:
                                    yt[i] = coords[i][np][1];
                                    break;
                            }
                        }
                        lstpp = new double[2][] { time, yt };
                        break;
                    case axistype.x:
                        double[] xx = new double[time.Length];
                        double[] xy = new double[time.Length];
                        for (int i = 0; i < time.Length; i++)
                        {
                            xx[i] = coords[i][np][0];
                            xy[i] = coords[i][np][1];
                        }
                        //lstpp = new PointPairList();
                        lstpp = new double[2][] { xx, xy };
                        //ACurve = pane.AddCurve("p" + np.ToString(), xx,xy, System.Drawing.Color.Red, SymbolType.None);
                        //FCurve.Line.Width = 2f;
                        break;
                    case axistype.y:
                        double[] yx = new double[time.Length];
                        double[] yy = new double[time.Length];
                        for (int i = 0; i < time.Length; i++)
                        {
                            yx[i] = coords[i][np][1];
                            yy[i] = coords[i][np][0];
                        }
                        //lstpp = new PointPairList();
                        lstpp = new double[2][] { yx, yy };
                        //ACurve = pane.AddCurve("p" + np.ToString(), yx,yy, System.Drawing.Color.Red, SymbolType.None);
                        //FCurve.Line.Width = 2f;
                        //MainWindow.crvlst.Add(new tempcurve() { label = "p" + np.ToString(), lstpoints = lstpp, color = System.Drawing.Color.Red, symbolType = SymbolType.None });
                        break;
                }
                //FilteredPointList filteredList = new FilteredPointList(lstpp[0], lstpp[1]);

                //double filteredXMin = 0;
                //double filteredXMax = 0;

                //if (lstpp[0].Min() > lstpp[1].Min())
                //{ filteredXMin = lstpp[1].Min(); }
                //else
                //{ filteredXMin = lstpp[0].Min(); }

                //if (lstpp[0].Max() < lstpp[1].Max())
                //{ filteredXMax = lstpp[1].Max(); }
                //else
                //{ filteredXMax = lstpp[0].Max(); }

                //int filteredCount = 50000;

                //filteredList.SetBounds(filteredXMin, filteredXMax, filteredCount);

                //pane.AddCurve("p" + np.ToString(), filteredList, HLSToColor(0, 255, 255), SymbolType.None);
                pane.AddCurve(title, lstpp[0], lstpp[1], clrCurve, SymbolType.None);
                //crvlst.Add(new tempcurve() { label = "p" + np.ToString(), lstpoints = null, xpoints = lstpp[0], ypoints = lstpp[1], color = HLSToColor(0, 255, 255), symbolType = SymbolType.None });

                //Customers.Add(new CheckedListItem<Customer>(new Customer() { Name = "p" + np.ToString() }));
                //chkboxStack.Children.Add(new CheckBox
                //{ Name = "p" + np.ToString() , IsChecked = true, Content = "p" + np.ToString()
                //});
                //FCurve.Line.Width = 2f;
                //MainWindow.crvlst.Add(new tempcurve() { label = "p" + np.ToString(), lstpoints = lstpp, color = System.Drawing.Color.Red, symbolType = SymbolType.None });
            }
            mainPlot.AxisChange();
            mainPlot.Invalidate();
        }

        public void initControls()
        {
            rangeSlider.MinRangeWidth = time[1];
            rangeSlider.SmallChange = rangeSlider.MinRangeWidth;
            rangeSlider.LargeChange = rangeSlider.SmallChange * 100;
            rangeSlider.TickFrequency = rangeSlider.LargeChange;
            myAllProps.rangeSliderMaxValue = time[time.Length - 1];
            myAllProps.rangeSliderCurrentMaxValue = myAllProps.rangeSliderMaxValue;
            myAllProps.RaisePropertyChanged("MaxValue");
            myAllProps.RaisePropertyChanged("CurrentMaxValue");

            int startP = 0, endP = 0;
            for (int i = 0; i < time.Length - 1; i++)
            {
                if (time[i] == myAllProps.rangeSliderCurrentMinValue)
                {
                    startP = i;
                }
                else if (time[i + 1] == myAllProps.rangeSliderCurrentMinValue)
                {
                    startP = i + 1;
                }
                else if (time[i] < myAllProps.rangeSliderCurrentMinValue && time[i + 1] > myAllProps.rangeSliderCurrentMinValue)
                {
                    startP = i;
                }
            }
            for (int i = 0; i < time.Length - 1; i++)
            {
                if (time[i] == myAllProps.rangeSliderCurrentMaxValue)
                {
                    endP = i;
                }
                else if (time[i + 1] == myAllProps.rangeSliderCurrentMaxValue)
                {
                    endP = i + 1;
                }
                else if (time[i] < myAllProps.rangeSliderCurrentMaxValue && time[i + 1] > myAllProps.rangeSliderCurrentMaxValue)
                {
                    endP = i + 1;
                }
            }
            refreshInfo(time, coords, dt, startP, endP);
        }
        private void refreshInfo(double[] time, double[][][] coords, double dt, int startP, int endP)
        {
            //drawFFT(time, coords, dt, startP, endP);
            drawSetTime(time, coords, dt, startP, endP);
            expFreqTextbox.Text = 3000.ToString("e5");
            List<int> Mins = new List<int>();
            List<int> Maxs = new List<int>();
            findMins(time, coords, dt, startP, endP, ref Mins);
            findMaxs(time, coords, dt, startP, endP, ref Maxs);
            if (Mins.Count > 0 && Maxs.Count > 0)
            {
                //drawPoint(time, coords, Mins.ToArray(), "firstMin");
                //drawPoint(time, coords, Maxs.ToArray(), "firstMax");
                drawPoint(time, coords, Mins[0], "firstMin");
                drawPoint(time, coords, Mins[Mins.Count - 1], "lastMin");
                drawPoint(time, coords, Maxs[0], "firstMax");
                drawPoint(time, coords, Maxs[Maxs.Count - 1], "lastMax");
                maxOnStartTextbox.Text = coords[Maxs[0]][0][0].ToString("e5");
                minOnStartTextbox.Text = coords[Mins[0]][0][0].ToString("e5");
                maxOnEndTextbox.Text = coords[Maxs[Maxs.Count - 1]][0][0].ToString("e5");
                minOnEndTextbox.Text = coords[Mins[Mins.Count - 1]][0][0].ToString("e5");

                if (coords[Mins[0]][0][0] == coords[Mins[Mins.Count - 1]][0][0])
                {
                    onStEndMinlabel.Content = '=';
                }
                else
                if (coords[Mins[0]][0][0] > coords[Mins[Mins.Count - 1]][0][0])
                {
                    onStEndMinlabel.Content = '>';
                }
                else
                if (coords[Mins[0]][0][0] < coords[Mins[Mins.Count - 1]][0][0])
                {
                    onStEndMinlabel.Content = '<';
                }

                if (coords[Maxs[0]][0][0] == coords[Maxs[Maxs.Count - 1]][0][0])
                {
                    onStEndMaxlabel.Content = '=';
                }
                else
                if (coords[Maxs[0]][0][0] > coords[Maxs[Maxs.Count - 1]][0][0])
                {
                    onStEndMaxlabel.Content = '>';
                }
                else
                if (coords[Maxs[0]][0][0] < coords[Maxs[Maxs.Count - 1]][0][0])
                {
                    onStEndMaxlabel.Content = '<';
                }

                try
                {
                    double periodExp = ((double)1 / (double)3000);
                    periodExpTextbox.Text = periodExp.ToString("e5");
                    double periodCalc = time[Mins[Mins.Count - 1]] - time[Mins[Mins.Count - 2]];
                    periodCalcTextbox.Text = periodCalc.ToString("e5");
                    calcFreqTextbox.Text = (1 / periodCalc).ToString("e5");
                    if (periodExp == periodCalc)
                    {
                        TexpTcalclabel.Content = '=';
                    }
                    else
                    if (periodExp > periodCalc)
                    {
                        TexpTcalclabel.Content = '>';
                    }
                    else
                    if (periodExp < periodCalc)
                    {
                        TexpTcalclabel.Content = '<';
                    }
                }
                catch (Exception)
                {
                    MessageBox.Show("Selected time range too small");
                    periodCalcTextbox.Text = "";
                }
                try
                {
                    double periodExp5 = ((double)1 / (double)3000) * Convert.ToDouble(multiplierTTextbox.Text);
                    periodExp5Textbox.Text = periodExp5.ToString("e5");
                    double periodCalc5 = time[Mins[Mins.Count - 1]] - time[Mins[Mins.Count - 6]];
                    periodCalc5Textbox.Text = periodCalc5.ToString("e5");
                    if (periodExp5 == periodCalc5)
                    {
                        Texp5Tcalc5label.Content = '=';
                    }
                    else
                    if (periodExp5 > periodCalc5)
                    {
                        Texp5Tcalc5label.Content = '>';
                    }
                    else
                    if (periodExp5 < periodCalc5)
                    {
                        Texp5Tcalc5label.Content = '<';
                    }
                }
                catch (Exception)
                {
                    MessageBox.Show("Selected time range too small");
                    periodCalc5Textbox.Text = "";
                }
            }
        }

        private void findMins(double[] time, double[][][] coords, double dt, int startP, int endP, ref List<int> Mins)
        {
            int indexMin = startP;
            for (int i = startP + 1; i < endP; i++)
            {
                if (coords[indexMin][0][0] > coords[i][0][0] && coords[i][0][0] < coords[i + 1][0][0])
                {
                    indexMin = i;
                    Mins.Add(indexMin);
                }
                if (coords[indexMin][0][0] < coords[i + 1][0][0] && indexMin != startP)
                {
                    indexMin = i + 1;
                }
            }
        }
        private void findMaxs(double[] time, double[][][] coords, double dt, int startP, int endP, ref List<int> Maxs)
        {
            int indexMax = startP;
            for (int i = startP + 1; i < endP; i++)
            {
                if (coords[indexMax][0][0] < coords[i][0][0] && coords[i][0][0] > coords[i + 1][0][0])
                {
                    indexMax = i;
                    Maxs.Add(indexMax);
                }
                if (coords[indexMax][0][0] > coords[i + 1][0][0] && indexMax != startP)
                {
                    indexMax = i + 1;
                }
            }
        }
        private void drawSetTime(double[] time, double[][][] coords, double dt, int startP, int endP)
        {
            double[] XArr = new double[2];
            double[] YArr = new double[2];
            XArr[0] = time[startP];
            XArr[1] = time[endP];

            YArr[0] = coords[startP][0][0];
            YArr[1] = coords[endP][0][0];


            // Создадим список точек
            PointPairList list = new PointPairList();
            list.Add(XArr[0], YArr[0]);
            list.Add(PointPairBase.Missing, PointPairBase.Missing);
            list.Add(XArr[1], YArr[1]);
            GraphPane pane = mainPlot.GraphPane;
            pane.CurveList.Remove(pane.CurveList["SetTimecurve"]);
            pane.AddCurve("SetTimecurve", list, HLSToColor(0, 0, 255), SymbolType.Star);
            mainPlot.AxisChange();
            mainPlot.Invalidate();
        }

        private void drawFFT(double[] time, double[][][] coords, double dt, int startP, int endP)
        {
            //int n = endP - startP;
            ////int n = time.Length;
            //// create two unmanaged arrays, properly aligned
            //pin = fftwf.malloc(n * 8);
            //pout = fftwf.malloc(n * 8);

            //// create two managed arrays, possibly misalinged
            //// n*2 because we are dealing with complex numbers
            //fin = new float[n * 2];
            //fout = new float[n * 2];
            //int k = startP;
            //for (int i = 0; i < n; i++)
            //{
            //    fin[i * 2] = (float)coords[k][0][0];
            //    fin[i * 2 + 1] = 0;
            //    k++;
            //}
            //// copy managed arrays to unmanaged arrays
            //Marshal.Copy(fin, 0, pin, n * 2);
            //Marshal.Copy(fout, 0, pout, n * 2);

            //fplan1 = fftwf.dft_1d(n, pin, pout, fftw_direction.Forward, fftw_flags.Estimate);
            //fftwf.execute(fplan1);

            //Marshal.Copy(pout, fout, 0, n * 2);

            ////if (!File.Exists("wisdom.wsd"))
            ////{
            ////    fftwf.export_wisdom_to_filename("wisdom.wsd");
            ////}
            //// it is essential that you call these after finishing
            //// that's why they're in the destructor. See Also: RAII
            //fftwf.free(pin);
            //fftwf.free(pout);
            //fftwf.destroy_plan(fplan1);
            //double[] freqXArr = new double[fout.Length / 2];
            //double[] freqYArr = new double[fout.Length / 2];

            //for (int i = 1; i < fout.Length / 2; i++)
            //{
            //    freqXArr[i] = freqXArr[i - 1] + ((1 / dt) / 2) / (freqYArr.Length - 1);
            //    freqYArr[i] = Math.Abs(fout[i]);
            //}
            //double maxf = 0;
            //int maxfi = 0;
            //for (int i = 1; i < freqYArr.Length; i++)
            //{
            //    if (freqYArr[i] > maxf)
            //    {
            //        maxf = freqYArr[i];
            //        maxfi = i;
            //    }
            //}
            //calcFreqTextbox.Text = freqXArr[maxfi].ToString();


            //GraphPane paneFFT = plotFFT.GraphPane;
            //paneFFT.Title.IsVisible = false;
            //paneFFT.Legend.IsVisible = false;
            //paneFFT.XAxis.Title.IsVisible = false;
            //paneFFT.YAxis.Title.IsVisible = false;
            //paneFFT.YAxis.MajorGrid.IsZeroLine = false;
            //paneFFT.IsFontsScaled = false;
            //paneFFT.CurveList.Clear();
            //paneFFT.AddCurve("DFTcurve", freqXArr, freqYArr, HLSToColor(0, 255, 255), SymbolType.None);
            //plotFFT.AxisChange();
            //plotFFT.Invalidate();
        }

        private void drawPoint(double[] time, double[][][] coords, int indexPoint, string curveName)
        {
            PointPairList list = new PointPairList();
            list.Add(time[indexPoint], coords[indexPoint][0][0]);
            GraphPane pane = mainPlot.GraphPane;
            pane.CurveList.Remove(pane.CurveList[curveName]);
            LineItem myCurve = pane.AddCurve(curveName, list, HLSToColor(0, 0, 255), SymbolType.Circle);
            myCurve.Symbol.Fill.Type = FillType.Solid;
            myCurve.Line.IsVisible = false;
            myCurve.Symbol.Size = 7;
            mainPlot.AxisChange();
            mainPlot.Invalidate();
        }
        private void drawPoint(double[] time, double[][][] coords, int[] indexPoint, string curveName)
        {
            PointPairList list = new PointPairList();
            for (int i = 0; i < indexPoint.Length; i++)
            {
                list.Add(time[indexPoint[i]], coords[indexPoint[i]][0][0]);
            }
            GraphPane pane = mainPlot.GraphPane;
            pane.CurveList.Remove(pane.CurveList[curveName]);
            LineItem myCurve = pane.AddCurve(curveName, list, HLSToColor(0, 0, 255), SymbolType.Circle);
            myCurve.Symbol.Fill.Type = FillType.Solid;
            myCurve.Line.IsVisible = false;
            myCurve.Symbol.Size = 7;
            mainPlot.AxisChange();
            mainPlot.Invalidate();
        }


        public System.Drawing.Color get_h_fromForce(double currForce, double maxForce, double minForce)
        {
            return ColorTranslator.FromWin32(ColorHLSToRGB(255, 255, 255));
        }

        static public System.Drawing.Color HLSToColor(int H, int L, int S)
        {
            //
            // Convert Hue, Luminance, and Saturation values to System.Drawing.Color structure.
            // H, L, and S are in the range of 0-240.
            // ColorHLSToRGB returns a Win32 RGB value (0x00BBGGRR).  To convert to System.Drawing.Color
            // structure, use ColorTranslator.FromWin32.
            //
            return ColorTranslator.FromWin32(ColorHLSToRGB(H, L, S));
        }
        string plotter_PointValueEvent(ZedGraphControl sender, GraphPane pane, CurveItem curve, int iPt)
        {
            PointPair point = curve[iPt];
            string result = string.Format("X: {0:F8}\nY: {1:F8}", point.X, point.Y);
            return result;
        }

        private void plotFFT_DoubleClick(object sender, EventArgs e)
        {
            //if (CurveBox.Visibility == Visibility.Collapsed)
            //{
            //    CurveBox.Visibility = Visibility.Visible;
            //    Grid.SetColumnSpan(infoStack, 1);
            //    Grid.SetRowSpan(infoStack, 1);
            //    FFTbox.Height = FFTbox.MinHeight;
            //}
            //else
            //{
            //    FFTbox.Height = CurveBox.Height;
            //    CurveBox.Visibility = Visibility.Collapsed;
            //    Grid.SetColumnSpan(infoStack, 2);
            //    Grid.SetRowSpan(infoStack, 2);
            //}
        }

        private void rangeSlider_LowerValueChanged(object sender, MahApps.Metro.Controls.RangeParameterChangedEventArgs e)
        {
            setTimeTextbox.Text = myAllProps.rangeSliderCurrentMinValue.ToString("e5") + ":" + myAllProps.rangeSliderCurrentMaxValue.ToString("e5");
        }

        private int findTimeP(double timeToFind)
        {
            int p = 0;
            for (int i = 0; i < time.Length - 1; i++)
            {
                if (time[i] == timeToFind)
                {
                    p = i;
                }
                else if (time[i + 1] == timeToFind)
                {
                    p = i + 1;
                }
                else if (time[i] < timeToFind && time[i + 1] > timeToFind)
                {
                    p = i;
                }
            }
            return p;
        }
        private void rangeSlider_LowerThumbDragCompleted(object sender, System.Windows.Controls.Primitives.DragCompletedEventArgs e)
        {
            int startP = findTimeP(myAllProps.rangeSliderCurrentMinValue);
            int endP = findTimeP(myAllProps.rangeSliderCurrentMaxValue);
            refreshInfo(time, coords, dt, startP, endP);
        }

    }
}
