using GalaSoft.MvvmLight;
using System;
using System.Collections.ObjectModel;
using myMatch;

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
            muArr newMyClass = new muArr();
            newMyClass.Test1(10);
            linearModel = new LinearModel();
            linearModel.initTimeMoments(10, 5, 6);
            linearModel.calcMove();
            this.Measurements = new Collection<Measurement>();
            for (int i = 0; i < linearModel.timeMoments.Length; i++)
            {
                for (int j = 0; j < linearModel.timeMoments[i].Nodes.Length; j++)
                {
                    this.Measurements.Add(new Measurement
                    {
                        Time = (double)linearModel.timeMoments[i].Nodes[j].force[0],
                        Value = 0,
                        Maximum = 0,
                        Minimum = 0
                    });
                }
            }
            

            //const int N = 50000;
            ////this.Subtitle = "N = " + N;

            //var r = new Random(385);
            //double dy = 0;
            //double y = 0;
            //for (int i = 0; i < N; i++)
            //{
            //    dy += (r.NextDouble() * 2) - 1;
            //    y += dy;
            //    this.Measurements.Add(new Measurement
            //    {
            //        Time = 2.5 * i / (N - 1),
            //        Value = y / (N - 1),
            //        Maximum = (y / (N - 1)) + 5,
            //        Minimum = (y / (N - 1)) - 5
            //    });
            //}
        }

        public LinearModel linearModel { get; private set; }

        public Collection<timeMoment> allCalcs { get; private set; }

        public Collection<Measurement> Measurements { get; private set; }

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