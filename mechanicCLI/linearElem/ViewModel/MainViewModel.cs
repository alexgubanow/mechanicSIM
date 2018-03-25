using GalaSoft.MvvmLight;
using myMatch;
using OxyPlot;
using OxyPlot.Series;

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
            int counts = 1000;
            double dt = 0.00001;
            int elements = 6;
            int points = elements * 2;
            int nodes = elements + 1;
            double l = 0.05;
            double b = 1;
            double h = 0.01;
            linearModel = new LinearModel(counts, dt, nodes, points, 0.01, l, b, h);
            linearModel.applyLoad(100, 0.05);
            linearModel.calcMove();
            plotViewModel = new PlotModel();

            for (int j = 0; j < nodes; j++)
            {
                var s1 = new LineSeries();
                s1.Title = "line" + j;
                s1.StrokeThickness = 1.2;
                s1.LineStyle = LineStyle.Solid;
                s1.Color = OxyColor.FromRgb(255, 0, 0);
                //s1.Color = OxyColor.FromRgb(41, 177, 255);
                //s1.RenderInLegend = false;
                for (int i = 0; i < counts; i++)
                {
                    s1.Points.Add(new DataPoint(linearModel.time[i], linearModel.timeMoments[i].Nodes[j].displ[0]));
                }
                plotViewModel.Series.Add(s1);
            }
        }

        private PlotModel _plotModel;

        public PlotModel plotViewModel
        {
            get
            {
                return _plotModel;
            }

            set
            {
                if (_plotModel == value)
                {
                    return;
                }

                _plotModel = value;
                RaisePropertyChanged("plotViewModel");
            }
        }

        public LinearModel linearModel { get; private set; }
    }
}