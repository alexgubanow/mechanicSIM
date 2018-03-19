using GalaSoft.MvvmLight;
using System;
using System.Collections.ObjectModel;
using myMatch;
using OxyPlot.Wpf;
using System.Collections.Generic;
using OxyPlot;

namespace linearElem.ViewModel
{
    /// <summary>
    /// This class contains properties that the main View can data bind to.
    /// <para>
    /// Use the <strong>mvvminpc</strong> snippet to add bindable properties to this ViewModel.
    /// </para>
    /// <para>
    /// You can also use Blend to data bind with the tool's support.
    /// </para>
    /// <para>
    /// See http://www.galasoft.ch/mvvm
    /// </para>
    /// </summary>
    public class MainViewModel : ViewModelBase
    {
        public MainViewModel()
        {
            linearModel = new LinearModel();
            linearModel.initTime(100000, 0.000001);
            linearModel.initTimeMoments(100000, 5, 6);
            linearModel.applyLoad();
            linearModel.calcMove();

            //PlotModel model = new PlotModel();
            //LineSeries s1 = new LineSeries();
            //s1.
            //AddPoints(s1.Points, 100000);
            //model.Series.Add(s1);

            //this.Measurements = new Collection<Measurement>();
            //for (int i = 0; i < linearModel.timeMoments.Length; i++)
            //{
            //    for (int j = 0; j < linearModel.timeMoments[i].Nodes.Length; j++)
            //    {
            //        this.Measurements.Add(new Measurement
            //        {
            //            Time = i,
            //            Value = (double)linearModel.timeMoments[i].Nodes[j].force[0],
            //            Maximum = 0,
            //            Minimum = 0
            //        });
            //    }
            //}

        }
        public PlotModel Model { get; set; }
        
        private static IList<DataPoint> GetPoints(int n)
        {
            var points = new List<DataPoint>();
            AddPoints(points, n);
            return points;
        }

        private static void AddPoints(IList<DataPoint> points, int n)
        {
            for (int i = 0; i < n; i++)
            {
                double x = Math.PI * 10 * i / (n - 1);
                //points.Add(new DataPoint(x * Math.Cos(x), x * Math.Sin(x)));
            }
        }
        public LinearModel linearModel { get; private set; }

        public Collection<LineSeries> myLineSeries { get; private set; }

        public Collection<DataPoint> Measurements { get; private set; }

        public class Measurement
        {
            public double Time { get; set; }
            public double Value { get; set; }
            public double Minimum { get; set; }
            public double Maximum { get; set; }

            public override string ToString()
            {
                return String.Format("{0:#0.0} {1:##0.0}", Time, Value);
            }
        }
    }
}