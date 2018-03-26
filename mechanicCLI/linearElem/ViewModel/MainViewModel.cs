using GalaSoft.MvvmLight;
using myMatch;
using OxyPlot;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Data;

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
            int counts = 500;
            double dt = 0.00001;
            int elements = 5;
            //int points = elements * 2;
            int nodes = elements + 1;
            double Length = 180;
            double l = Length / elements * Math.Pow(10, -3);
            double b = 10 * Math.Pow(10, -3);
            double h = 0.1 * Math.Pow(10, -3);
            double massa = 0.1;
            linearModel = new LinearModel(counts, dt, nodes, elements, massa, l, b, h);
            linearModel.applyLoad(100, 0.000000001);
            linearModel.calcMove();
            MyList = new ObservableCollection<deriv>();
            for (int i = 0; i < counts; i++)
            {
                MyList.Add(new deriv() { force = linearModel.timeMoments[i].Nodes[1].derivatives.force[0],
                    accl = linearModel.timeMoments[i].Nodes[1].derivatives.accl[0],
                    velos = linearModel.timeMoments[i].Nodes[1].derivatives.velos[0],
                    displ = linearModel.timeMoments[i].Nodes[1].derivatives.displ[0]
                });
            }
        }
        ObservableCollection<deriv> _MyList;
        public ObservableCollection<deriv> MyList { get { return _MyList; } set { _MyList = value; RaisePropertyChanged("MyList"); }  }

        public class deriv : INotifyPropertyChanged
        {
            double _force;
            public double force { get => _force; set { _force = value; RaisePropertyChanged("force");} }
            double _accl;
            public double accl { get => _accl; set { _accl = value; RaisePropertyChanged("accl"); } }
            double _velos;
            public double velos { get => _velos; set { _velos = value; RaisePropertyChanged("velos"); } }
            double _displ;
            public double displ { get => _displ; set { _displ = value; RaisePropertyChanged("displ"); } }



            public event PropertyChangedEventHandler PropertyChanged;

            public void RaisePropertyChanged(string propertyName)
            {
                if (PropertyChanged != null)
                    PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
            }
        }

        public LinearModel linearModel { get; private set; }

        public event PropertyChangedEventHandler PropertyChanged;

        public void RaisePropertyChanged(string propertyName)
        {
            if (PropertyChanged != null)
                PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}