using GalaSoft.MvvmLight;
using InteractiveDataDisplay.WPF;
using OxyPlot;
using OxyPlot.Series;

//using myMatch;
using System;
using System.Collections.ObjectModel;
using System.ComponentModel;

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
            MainWin = new MainWin();
        }

        public MainWin MainWin { get; set; }
    }

    public class MainWin : INotifyPropertyChanged
    {
        public MainWin()
        {
            freq = 100;
            amp = 1;
            reCalc();
            //MyList = new ObservableCollection<deriv>();
            //for (int i = 0; i < counts; i++)
            //{
            //    MyList.Add(new deriv()
            //    {
            //        force = linearModel.timeMoments[i].Points[1].derivatives.force[0],
            //        accl = linearModel.timeMoments[i].Points[1].derivatives.accl[0],
            //        velos = linearModel.timeMoments[i].Points[1].derivatives.velos[0],
            //        displ = linearModel.timeMoments[i].Points[1].derivatives.displ[0]
            //    });
            //}
        }

        private void reCalc()
        {
            int counts = 9000;
            double dt = 0.000003;
            int elements = 5;
            //int points = elements * 2;
            int nodes = elements + 1;
            double Length = 180;
            double l = Length / elements * Math.Pow(10, -3);
            double b = 50 * Math.Pow(10, -3);
            double h = 0.1 * Math.Pow(10, -3);
            double massa = 0.1;
            linearModel = new LinearModel(counts, dt, nodes, elements, massa, l, b, h);
            linearModel.applyLoad(freq, (amp * Math.Pow(10, -2)));
            linearModel.calcMove();
            //lg = new LineGraph();
            //for (int i = 0; i < linearModel.timeMoments[0].Nodes.Length; i++)
            //{
            //    lg.Stroke = new SolidColorBrush(Color.FromRgb(255, 0, 0));
            //    lg.Description = String.Format("Node {0}", i + 1);
            //    lg.StrokeThickness = 1;
            //    double[] iojhno = new double[linearModel.timeMoments.Length];
            //    for (int j = 0; j < linearModel.timeMoments.Length; j++)
            //    {
            //        iojhno[j] = (linearModel.timeMoments[j].Nodes[i].derivatives.displ[0] + i * 0.036);
            //    }
            //    lg.Plot(linearModel.time, iojhno);
            //}
            plotViewModel = new PlotModel();
            for (int j = 0; j < linearModel.timeMoments[0].Nodes.Length; j++)
            {
                var s1 = new LineSeries();
                s1.Title = "node" + j;
                s1.StrokeThickness = 1.2;
                s1.LineStyle = LineStyle.Solid;
                s1.Color = OxyColor.FromRgb(255, 0, 0);
                //s1.Color = OxyColor.FromRgb(41, 177, 255);
                //s1.RenderInLegend = false;
                for (int i = 0; i < counts; i++)
                {
                    s1.Points.Add(new DataPoint(linearModel.time[i], linearModel.timeMoments[i].Nodes[j].derivatives.displ[0]));
                }
                plotViewModel.Series.Add(s1);
            }
        }

        private PlotModel _plotViewModel;

        public PlotModel plotViewModel { get => _plotViewModel; set { _plotViewModel = value; RaisePropertyChanged("plotViewModel"); } }

        private LineGraph _lg;
        public LineGraph lg { get => _lg; set { _lg = value; RaisePropertyChanged("lg"); } }

        private int _freq;
        public int freq
        {
            get { return _freq; }
            set {
                _freq = value;
                RaisePropertyChanged("freq");
                reCalc();
            }
        }

        private int _amp;
        public int amp { get => _amp; set { _amp = value; RaisePropertyChanged("amp"); } }

        private ObservableCollection<deriv> _MyList;
        public ObservableCollection<deriv> MyList { get { return _MyList; } set { _MyList = value; RaisePropertyChanged("MyList"); } }

        public class deriv : INotifyPropertyChanged
        {
            private double _force;
            public double force { get => _force; set { _force = value; RaisePropertyChanged("force"); } }
            private double _accl;
            public double accl { get => _accl; set { _accl = value; RaisePropertyChanged("accl"); } }
            private double _velos;
            public double velos { get => _velos; set { _velos = value; RaisePropertyChanged("velos"); } }
            private double _displ;
            public double displ { get => _displ; set { _displ = value; RaisePropertyChanged("displ"); } }

            public event PropertyChangedEventHandler PropertyChanged;

            public void RaisePropertyChanged(string propertyName)
            {
                if (PropertyChanged != null)
                    PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
            }
        }

        public LinearModel linearModel { get; private set; }

        public enum nodeFreedoms { x, y, z, xy, xz, yz, xyz };

        //derivatives
        public class Derivatives
        {
            public double[] coord;
            public double[] force;
            public double[] displ;
            public double[] velos;
            public double[] accl;
            public double[] jerk;

            public Derivatives(int demesia)
            {
                coord = new double[demesia];
                force = new double[demesia];
                displ = new double[demesia];
                velos = new double[demesia];
                accl = new double[demesia];
                jerk = new double[demesia];
            }
        };

        public class Point
        {
            public Derivatives derivatives;

            public Point(int demesia)
            {
                derivatives = new Derivatives(demesia);
            }
        };

        public class Element
        {
            public int point1;
            public int point2;

            public Element(int point1Index, int point2Index, int demesia)
            {
                point1 = point1Index;
                point2 = point2Index;
            }
        };

        public class Node
        {
            public nodeFreedoms nodeFreedom;
            public int[] ListOfLinks;
            public int[] ListOfPoints;
            public Derivatives derivatives;

            public Node(nodeFreedoms newnodeFreedom, int[] newListOfLinks, int[] newListOfPoints, int demesia)
            {
                nodeFreedom = newnodeFreedom;
                ListOfLinks = newListOfLinks;
                ListOfPoints = newListOfPoints;
                derivatives = new Derivatives(demesia);
            }
        };

        public class timeMoment
        {
            public Node[] Nodes;
            public Element[] Elements;
            public Point[] Points;

            public timeMoment(int numNodes, int numElements, int demesia)
            {
                Nodes = new Node[numNodes];
                Elements = new Element[numElements];
                Points = new Point[numElements * 2];
                for (int i = 0, j = 0; i < numElements; i++, j += 2)
                {
                    Elements[i] = new Element(j, j + 1, demesia);
                    Points[j] = new Point(3);
                    Points[j + 1] = new Point(3);
                }
                Nodes[0] = new Node(nodeFreedoms.x, new int[1] { 0 }, new int[1] { 0 }, demesia);
                for (int i = 1, j = 1; i < numNodes - 1; i++, j += 2)
                {
                    Nodes[i] = new Node(nodeFreedoms.x, new int[2] { i - 1, i }, new int[2] { j, j + 1 }, demesia);
                }
                Nodes[numNodes - 1] = new Node(nodeFreedoms.x, new int[1] { numNodes - 2 }, new int[1] { numNodes }, demesia);
            }
        };

        public class LinearModel
        {
            private void calcOneMove(int momentNow, int prevMoment)
            {
                double A = _b * _h;
                double _elastic = 215 * Math.Pow(10, 9);
                //overview links of curr node
                for (int i = 0; i < timeMoments[momentNow].Elements.Length; i++)
                {
                    double ro = 7.8 * Math.Pow(10, 3);
                    //double ro = 6.12 * Math.Pow(10, 4);
                    double massa = ro * A * _l;
                    /*calc each link*/
                    double forcePrev1 = timeMoments[prevMoment].Points[timeMoments[prevMoment].Elements[i].point1].derivatives.force[0];
                    double acclNow1 = timeMoments[momentNow].Points[timeMoments[momentNow].Elements[i].point1].derivatives.accl[0];
                    double velosPrev1 = timeMoments[prevMoment].Points[timeMoments[prevMoment].Elements[i].point1].derivatives.velos[0];
                    double displPrev1 = timeMoments[prevMoment].Points[timeMoments[prevMoment].Elements[i].point1].derivatives.displ[0];
                    double velosNow1 = timeMoments[momentNow].Points[timeMoments[momentNow].Elements[i].point1].derivatives.velos[0];
                    double displNow1 = timeMoments[momentNow].Points[timeMoments[momentNow].Elements[i].point1].derivatives.displ[0];

                    double forcePrev2 = timeMoments[prevMoment].Points[timeMoments[prevMoment].Elements[i].point2].derivatives.force[0];
                    double acclNow2 = timeMoments[momentNow].Points[timeMoments[momentNow].Elements[i].point2].derivatives.accl[0];
                    double velosPrev2 = timeMoments[prevMoment].Points[timeMoments[prevMoment].Elements[i].point2].derivatives.velos[0];
                    double displPrev2 = timeMoments[prevMoment].Points[timeMoments[prevMoment].Elements[i].point2].derivatives.displ[0];
                    double velosNow2 = timeMoments[momentNow].Points[timeMoments[momentNow].Elements[i].point2].derivatives.velos[0];
                    double displNow2 = timeMoments[momentNow].Points[timeMoments[momentNow].Elements[i].point2].derivatives.displ[0];

                    forcePrev1 = ((_elastic * A / _l) * (displPrev2 - displPrev1));
                    acclNow1 = forcePrev1 / massa;
                    velosNow1 = acclNow1 * time[1];
                    displNow1 = velosNow1 * time[1];

                    forcePrev2 = -((_elastic * A / _l) * (displPrev2 - displPrev1));
                    acclNow2 = forcePrev2 / massa;
                    velosNow2 = acclNow2 * time[1];
                    displNow2 = velosNow2 * time[1];

                    timeMoments[prevMoment].Points[timeMoments[prevMoment].Elements[i].point1].derivatives.force[0] += forcePrev1;
                    timeMoments[momentNow].Points[timeMoments[momentNow].Elements[i].point1].derivatives.accl[0] += acclNow1;
                    timeMoments[momentNow].Points[timeMoments[momentNow].Elements[i].point1].derivatives.displ[0] += displNow1;
                    timeMoments[momentNow].Points[timeMoments[momentNow].Elements[i].point1].derivatives.velos[0] += velosNow1;

                    timeMoments[prevMoment].Points[timeMoments[prevMoment].Elements[i].point2].derivatives.force[0] += forcePrev2;
                    timeMoments[momentNow].Points[timeMoments[momentNow].Elements[i].point2].derivatives.accl[0] += acclNow2;
                    timeMoments[momentNow].Points[timeMoments[momentNow].Elements[i].point2].derivatives.velos[0] += velosNow2;
                    timeMoments[momentNow].Points[timeMoments[momentNow].Elements[i].point2].derivatives.displ[0] += displNow2;
                }
                for (int i = 0; i < timeMoments[momentNow].Nodes.Length - 1; i++)
                {
                    //sum all
                    for (int j = 0; j < timeMoments[momentNow].Nodes[i].ListOfPoints.Length; j++)
                    {
                        timeMoments[momentNow].Nodes[i].derivatives.force[0] += timeMoments[momentNow].Points[timeMoments[momentNow].Nodes[i].ListOfPoints[j]].derivatives.force[0];
                        timeMoments[momentNow].Nodes[i].derivatives.accl[0] += timeMoments[momentNow].Points[timeMoments[momentNow].Nodes[i].ListOfPoints[j]].derivatives.accl[0];
                        timeMoments[momentNow].Nodes[i].derivatives.velos[0] += timeMoments[momentNow].Points[timeMoments[momentNow].Nodes[i].ListOfPoints[j]].derivatives.velos[0];
                        timeMoments[momentNow].Nodes[i].derivatives.displ[0] += timeMoments[momentNow].Points[timeMoments[momentNow].Nodes[i].ListOfPoints[j]].derivatives.displ[0];
                        timeMoments[momentNow].Nodes[i].derivatives.coord[0] += timeMoments[momentNow].Points[timeMoments[momentNow].Nodes[i].ListOfPoints[j]].derivatives.coord[0];
                    }
                    //update final value of points inside node
                    for (int j = 0; j < timeMoments[momentNow].Nodes[i].ListOfPoints.Length; j++)
                    {
                        timeMoments[momentNow].Points[timeMoments[momentNow].Nodes[i].ListOfPoints[j]].derivatives.force[0] = timeMoments[momentNow].Nodes[i].derivatives.force[0];
                        timeMoments[momentNow].Points[timeMoments[momentNow].Nodes[i].ListOfPoints[j]].derivatives.accl[0] = timeMoments[momentNow].Nodes[i].derivatives.accl[0];
                        timeMoments[momentNow].Points[timeMoments[momentNow].Nodes[i].ListOfPoints[j]].derivatives.velos[0] = timeMoments[momentNow].Nodes[i].derivatives.velos[0];
                        timeMoments[momentNow].Points[timeMoments[momentNow].Nodes[i].ListOfPoints[j]].derivatives.displ[0] = timeMoments[momentNow].Nodes[i].derivatives.displ[0];
                        timeMoments[momentNow].Points[timeMoments[momentNow].Nodes[i].ListOfPoints[j]].derivatives.coord[0] = timeMoments[momentNow].Nodes[i].derivatives.coord[0];
                    }
                }
            }

            public void euler(Derivatives[] prevMoment, Derivatives[] moment, double deltat, double massa, double l)
            {
                //if (moment.force[0] != 0)
                //{
                //    moment.accl[0] = prevMoment.accl[0] + moment.force[0] / massa;
                //    moment.velos[0] = prevMoment.velos[0] + moment.accl[0] * deltat;
                //    moment.displ[0] = prevMoment.displ[0] + moment.velos[0] * deltat;
                //    moment.coord[0] = prevMoment.coord[0] + moment.displ[0];
                //}
                //ap[2] = F[2] / massa * l * l * 0.5;
            }

            public timeMoment[] timeMoments;
            public double[] time;
            private double _m;
            private double _l;
            private double _b;
            private double _h;
            private int _counts;
            private double _dt;

            public LinearModel(int counts, double dt, int numNodes, int numElements, double massa, double lenght, double b, double h)
            {
                _m = massa;
                _l = lenght;
                _b = b;
                _h = h;
                _counts = counts;
                _dt = dt;
                initTime();
                initTimeMoments(numNodes, numElements);
            }

            public void initTime()
            {
                time = new double[_counts];
                for (int i = 1; i < _counts; i++)
                {
                    time[i] = time[i - 1] + _dt;
                }
            }

            public void initTimeMoments(int numNodes, int numElements)
            {
                timeMoments = new timeMoment[_counts];
                for (int i = 0; i < _counts; i++)
                {
                    timeMoments[i] = new timeMoment(numNodes, numElements, 3);
                }
            }

            public void applyLoad(int freq, double amp)
            {
                for (int i = 0; i < timeMoments.Length; i++)
                {
                    timeMoments[i].Nodes[0].derivatives.displ[0] = amp * Math.Sin(time[i] * 2 * Math.PI * freq);
                    for (int j = 0; j < timeMoments[i].Nodes[0].ListOfPoints.Length; j++)
                    {
                        timeMoments[i].Points[timeMoments[i].Nodes[0].ListOfPoints[j]].derivatives.displ[0] = timeMoments[i].Nodes[0].derivatives.displ[0];
                    }
                }
            }

            public void calcMove()
            {
                for (int i = 1; i < timeMoments.Length; i++)
                {
                    calcOneMove(i, i - 1);
                    if (timeMoments[i].Points[timeMoments[i].Points.Length - 1].derivatives.displ[0] > 0.001 || timeMoments[i].Points[timeMoments[i].Points.Length - 1].derivatives.displ[0] < -0.001)
                    { break; }
                }
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;

        public void RaisePropertyChanged(string propertyName)
        {
            if (PropertyChanged != null)
                PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}