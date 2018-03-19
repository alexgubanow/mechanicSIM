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
            linearModel = new LinearModel(100, 0.0001, 5, 6, 1, 100, 10, 1);
            linearModel.applyLoad(100, 0.001);
            linearModel.calcMove();
            plotViewModel = new PlotModel();

            for (int j = 0; j < 5; j++)
            {
                var s1 = new LineSeries();
                s1.Title = "line" + j;
                s1.StrokeThickness = 1.2;
                //s1.RenderInLegend = false;
                for (int i = 0; i < 100; i++)
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